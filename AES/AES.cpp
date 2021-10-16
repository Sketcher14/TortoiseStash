#include "AES.h"
#include "Utility/AESUtility.h"
#include "Utility/GFUtility.h"

#include <algorithm>
#include <numeric>
#include <QVector>
#include <QDebug>

AES::Word AES::Word::operator^(const AES::Word &OtherWord)
{
    Word Result { 0x00, 0x00, 0x00, 0x00 };
    for (ssize_t i = 0; i < AES::Word::ByteNum; ++i)
    {
        Result.Bytes[i] = Bytes[i] ^ OtherWord.Bytes[i];
    }

    return Result;
}

AES::Word& AES::Word::operator^=(const uint8_t (&Array)[4])
{
    for (ssize_t i = 0; i < AES::Word::ByteNum; ++i)
    {
        Bytes[i] ^= Array[i];
    }

    return *this;
}

AES::Word& AES::Word::Substitute()
{
    for (ssize_t i = 0; i < AES::Word::ByteNum; ++i)
    {
        Bytes[i] = AESUtility::GetSubByte(Bytes[i]);
    }

    return *this;
}

AES::Word& AES::Word::Rotate()
{
    std::rotate(Bytes, Bytes + 1, Bytes + AES::Word::ByteNum);

    return *this;
}

void AES::Word::Print(int32_t i)
{
    QString Result = QString::number(i) + ": ";
    for (ssize_t i = 0; i < AES::Word::ByteNum; ++i)
    {
        Result += QString::number(Bytes[i], 16) + ' ';
    }
    qDebug().noquote() << Result;
}

void AES::State::Print(int32_t Round, QString Operation)
{
    QString Result = "Round: " + QString::number(Round) + ", Operation: " + Operation + '\n';

    for (ssize_t i = 0; i < AES::State::RowNum; ++i)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            Result += QString::number(Bytes[i][j], 16) + ' ';
        }
        Result += '\n';
    }
    qDebug().noquote() << Result;
}

AES::AES(AESType Type)
{
    switch (Type)
    {
        case AESType::AES128:
        {
            Nk = 4;
            Nr = 10;
            HashAlgo = QCryptographicHash::Algorithm::Md5;
            break;
        }
        case AESType::AES192:
        {
            Nk = 6;
            Nr = 12;
            break;
        }
        case AESType::AES256:
        {
            Nk = 8;
            Nr = 14;
            break;
        }
    }
}

QByteArray AES::Encrypt(const QString& Input, const QString& CipherKey)
{
    QByteArray InputBytes = Input.toUtf8();
    AddPKCS7Padding(InputBytes);

    QVector<State> States;
    SplitInputByStates(States, InputBytes);

    QVector<Word> KeySchedule(Nb * (Nr + 1));
    GenerateKeyExpansion(KeySchedule, CipherKey);

    for (State& State : States)
        EncryptState(State, KeySchedule);

    QByteArray OutputBytes;
    UnionStates(States, OutputBytes);

    return OutputBytes;
}

QString AES::Decrypt(const QByteArray& InputBytes, const QString& CipherKey)
{
    QVector<State> States;
    SplitInputByStates(States, InputBytes);

    QVector<Word> KeySchedule(Nb * (Nr + 1));
    GenerateKeyExpansion(KeySchedule, CipherKey);

    for (State& State : States)
        DecryptState(State, KeySchedule);

    QByteArray OutputBytes;
    UnionStates(States, OutputBytes);
    RemovePKCS7Padding(OutputBytes);

    return OutputBytes;
}

void AES::AddPKCS7Padding(QByteArray& InputBytes)
{
    const uint8_t PaddingByte = AES::State::StateBytesNum - (InputBytes.size() % AES::State::StateBytesNum);
    if (PaddingByte < AES::State::StateBytesNum)
    {
        InputBytes.append(PaddingByte, PaddingByte);
    }

    assert(InputBytes.size() % AES::State::StateBytesNum == 0);
}

void AES::RemovePKCS7Padding(QByteArray& OutputBytes)
{
    const uint8_t LastByte = OutputBytes.back();
    if (LastByte < AES::State::StateBytesNum)
    {
        OutputBytes.chop(LastByte);
    }
}

void AES::SplitInputByStates(QVector<AES::State>& States, const QByteArray& InputBytes)
{
    States.resize(InputBytes.size() / AES::State::StateBytesNum);

    ssize_t s = 0;
    ssize_t r = 0;
    ssize_t c = 0;
    for (ssize_t i = 0; i < InputBytes.size(); ++i)
    {
        States[s].Bytes[r][c] = InputBytes.at(i);
        if (++r >= AES::State::RowNum)
        {
            if (++c >= Nb)
            {
                s++;
                c = 0;
            }
            r = 0;
        }
    }
}

void AES::GenerateKeyExpansion(QVector<Word>& KeySchedule, const QString& CipherKey)
{
    static const uint8_t Rcon[11][4] = {
        { 0x00, 0x00, 0x00, 0x00 },
        { 0x01, 0x00, 0x00, 0x00 },
        { 0x02, 0x00, 0x00, 0x00 },
        { 0x04, 0x00, 0x00, 0x00 },
        { 0x08, 0x00, 0x00, 0x00 },
        { 0x10, 0x00, 0x00, 0x00 },
        { 0x20, 0x00, 0x00, 0x00 },
        { 0x40, 0x00, 0x00, 0x00 },
        { 0x80, 0x00, 0x00, 0x00 },
        { 0x1b, 0x00, 0x00, 0x00 },
        { 0x36, 0x00, 0x00, 0x00 },
    };

    QByteArray KeyHash = QCryptographicHash::hash(CipherKey.toUtf8(), HashAlgo);

    assert(KeyHash.length() == Nk * AES::Word::ByteNum);

    ssize_t i = 0;
    while (i < Nk)
    {
        for (ssize_t j = 0; j < AES::Word::ByteNum; ++j)
        {
            KeySchedule[i].Bytes[j] = KeyHash.at(AES::Word::ByteNum * i + j);
        }
        ++i;
    }

    assert(i == Nk);

    while (i < KeySchedule.length())
    {
        Word Temp = KeySchedule[i - 1];
        if (i % Nk == 0)
        {
            Temp.Rotate().Substitute();
            Temp ^= Rcon[i / Nk];
        }
        else if (Nk > 6 && (i % Nk == 4))
        {
            Temp.Substitute();
        }
        KeySchedule[i] = KeySchedule[i - Nk] ^ Temp;
        ++i;
    }
}

void AES::UnionStates(const QVector<AES::State>& States, QByteArray& OutputBytes)
{
    for (ssize_t s = 0; s < States.length(); ++s)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            for (ssize_t i = 0; i < AES::State::RowNum; ++i)
            {
                OutputBytes.append(States[s].Bytes[i][j]);
            }
        }
    }
}

void AES::EncryptState(AES::State &State, const QVector<Word>& KeySchedule)
{
    int32_t Round = 0;
    AddRoundKey(State, KeySchedule, Round);

    for (Round = 1; Round < Nr; ++Round)
    {
        SubstituteBytes(State);
        ShiftRows(State);
        MixColumns(State);
        AddRoundKey(State, KeySchedule, Round * Nb);
    }

    SubstituteBytes(State);
    ShiftRows(State);
    AddRoundKey(State, KeySchedule, Round * Nb);
}

void AES::DecryptState(AES::State &State, const QVector<Word>& KeySchedule)
{
    int32_t Round = Nr;
    AddRoundKey(State, KeySchedule, Round * Nb);

    for (Round = Round - 1; Round > 0; --Round)
    {
        InvShiftRows(State);
        InvSubstituteBytes(State);
        AddRoundKey(State, KeySchedule, Round * Nb);
        InvMixColumns(State);
    }

    InvShiftRows(State);
    InvSubstituteBytes(State);
    AddRoundKey(State, KeySchedule, Round);
}

void AES::AddRoundKey(AES::State& State, const QVector<AES::Word>& KeySchedule, int32_t Round)
{
    for (ssize_t i = 0; i < AES::State::RowNum; ++i)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            State.Bytes[i][j] ^= KeySchedule[Round + j].Bytes[i];
        }
    }
}

void AES::SubstituteBytes(AES::State& State)
{
    for (ssize_t i = 0; i < AES::State::RowNum; ++i)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            State.Bytes[i][j] = AESUtility::GetSubByte(State.Bytes[i][j]);
        }
    }
}

void AES::InvSubstituteBytes(AES::State &State)
{
    for (ssize_t i = 0; i < AES::State::RowNum; ++i)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            State.Bytes[i][j] = AESUtility::GetInvSubByte(State.Bytes[i][j]);
        }
    }
}

void AES::ShiftRows(State& State)
{
    for (ssize_t i = 1; i < AES::State::RowNum; ++i)
    {
        std::rotate(State.Bytes[i], State.Bytes[i] + i, State.Bytes[i] + Nb);
    }
}

void AES::InvShiftRows(AES::State &State)
{
    for (ssize_t i = 1; i < AES::State::RowNum; ++i)
    {
        std::rotate(State.Bytes[i], State.Bytes[i] + (Nb - i) % Nb, State.Bytes[i] + Nb);
    }
}

void AES::CommonMixColumns(AES::State& State, const uint8_t (&Matrix)[4][4])
{
    for (ssize_t j = 0; j < Nb; ++j)
    {
        uint8_t StateColumn[AES::State::RowNum] = { 0x0, 0x0, 0x0, 0x0 };
        for (ssize_t i = 0; i < AES::State::RowNum; ++i)
        {
            StateColumn[i] = State.Bytes[i][j];
        }

        for (ssize_t i = 0; i < AES::State::RowNum; ++i)
        {
            State.Bytes[i][j] = AESUtility::DotProduct(StateColumn, Matrix[i], Nb);
        }
    }
}

void AES::MixColumns(AES::State& State)
{
    static const uint8_t MixMatrix[4][4] = {
        { 0x02, 0x03, 0x01, 0x01 },
        { 0x01, 0x02, 0x03, 0x01 },
        { 0x01, 0x01, 0x02, 0x03 },
        { 0x03, 0x01, 0x01, 0x02 }
    };

    CommonMixColumns(State, MixMatrix);
}

void AES::InvMixColumns(AES::State &State)
{
    static const uint8_t InvMixMatrix[4][4] = {
        { 0x0e, 0x0b, 0x0d, 0x09 },
        { 0x09, 0x0e, 0x0b, 0x0d },
        { 0x0d, 0x09, 0x0e, 0x0b },
        { 0x0b, 0x0d, 0x09, 0x0e }
    };

    CommonMixColumns(State, InvMixMatrix);
}
