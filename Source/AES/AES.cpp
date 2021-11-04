#include "AES/AES.h"

#include <QVector>
#include <QCryptographicHash>

AES::AES(AESType Type)
{
    switch (Type)
    {
        case AESType::AES128:
        {
            Nk = 4;
            Nr = 10;

            auto Hash128 = [](const QString& CipherKey) -> QByteArray
            {
                return QCryptographicHash::hash(CipherKey.toUtf8(), QCryptographicHash::Algorithm::Md5);
            };

            HashFunction = Hash128;
            break;
        }
        case AESType::AES192:
        {
            Nk = 6;
            Nr = 12;

            auto Hash192 = [](const QString& CipherKey) -> QByteArray
            {
                return QCryptographicHash::hash(CipherKey.toUtf8(), QCryptographicHash::Algorithm::Sha384).left(24);
            };

            HashFunction = Hash192;
            break;
        }
        case AESType::AES256:
        {
            Nk = 8;
            Nr = 14;

            auto Hash256 = [](const QString& CipherKey) -> QByteArray
            {
                return QCryptographicHash::hash(CipherKey.toUtf8(), QCryptographicHash::Algorithm::Sha256);
            };

            HashFunction = Hash256;
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
    const uint8_t PaddingByte = State::StateBytesNum - (InputBytes.size() % State::StateBytesNum);
    if (PaddingByte < State::StateBytesNum)
    {
        InputBytes.append(PaddingByte, PaddingByte);
    }

    assert(InputBytes.size() % State::StateBytesNum == 0);
}

void AES::RemovePKCS7Padding(QByteArray& OutputBytes)
{
    const uint8_t LastByte = OutputBytes.back();
    if (LastByte < State::StateBytesNum)
    {
        OutputBytes.chop(LastByte);
    }
}

void AES::SplitInputByStates(QVector<State>& States, const QByteArray& InputBytes)
{
    int32_t StartIndex = 0;
    while (StartIndex < InputBytes.size())
    {
        States.emplaceBack(InputBytes, StartIndex);
        StartIndex += State::StateBytesNum;
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

    QByteArray KeyHash = HashFunction(CipherKey);

    assert(KeyHash.length() == Nk * Word::ByteNum);

    int32_t i = 0;
    while (i < Nk)
    {
        for (int32_t j = 0; j < Word::ByteNum; ++j)
        {
            KeySchedule[i].Bytes[j] = KeyHash.at(Word::ByteNum * i + j);
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

void AES::UnionStates(const QVector<State>& States, QByteArray& OutputBytes)
{
    for (const State& State : States)
    {
        OutputBytes.append(State.ToByteArray());
    }
}

void AES::EncryptState(State& State, const QVector<Word>& KeySchedule)
{
    int32_t Round = 0;
    State.AddRoundKey(KeySchedule, Round);

    for (Round = 1; Round < Nr; ++Round)
    {
        State.SubstituteBytes();
        State.ShiftRows();
        State.MixColumns();
        State.AddRoundKey(KeySchedule, Round * Nb);
    }

    State.SubstituteBytes();
    State.ShiftRows();
    State.AddRoundKey(KeySchedule, Round * Nb);
}

void AES::DecryptState(State& State, const QVector<Word>& KeySchedule)
{
    int32_t Round = Nr;
    State.AddRoundKey(KeySchedule, Round * Nb);

    for (Round = Round - 1; Round > 0; --Round)
    {
        State.InvShiftRows();
        State.InvSubstituteBytes();
        State.AddRoundKey(KeySchedule, Round * Nb);
        State.InvMixColumns();
    }

    State.InvShiftRows();
    State.InvSubstituteBytes();
    State.AddRoundKey(KeySchedule, Round);
}
