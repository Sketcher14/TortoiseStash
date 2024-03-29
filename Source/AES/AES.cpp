#include "AES/AES.h"

#include <QVector>
#include <QCryptographicHash>
#include <QFile>

QByteArray AES::Encrypt(const QString& Input, const QString& CipherKey, AES::Type Type)
{
    QByteArray KeyHash;
    AES::Params Params;
    PrepareKeyAndParams(CipherKey, Type, KeyHash, Params);

    return Encrypt(Input, KeyHash, Params);
}

QString AES::Decrypt(const QByteArray& InputBytes, const QString& CipherKey, AES::Type Type)
{
    QByteArray KeyHash;
    AES::Params Params;
    PrepareKeyAndParams(CipherKey, Type, KeyHash, Params);

    return Decrypt(InputBytes, KeyHash, Params);
}

void AES::PrepareKeyAndParams(const QString& CipherKey, AES::Type Type, QByteArray& KeyHash, AES::Params& Params)
{
    switch (Type)
    {
        case AES::Type::AES128:
        {
            Params.Nk = 4;
            Params.Nr = 10;
            KeyHash = QCryptographicHash::hash(CipherKey.toUtf8(), QCryptographicHash::Algorithm::Md5);
            break;
        }
        case AES::Type::AES192:
        {
            Params.Nk = 6;
            Params.Nr = 12;
            KeyHash = QCryptographicHash::hash(CipherKey.toUtf8(), QCryptographicHash::Algorithm::Sha384).left(24);
            break;
        }
        case AES::Type::AES256:
        {
            Params.Nk = 8;
            Params.Nr = 14;
            KeyHash = QCryptographicHash::hash(CipherKey.toUtf8(), QCryptographicHash::Algorithm::Sha256);
            break;
        }
        default:
        {
            assert(false);
        }
    }
}

QByteArray AES::Encrypt(const QString& Input, const QByteArray& KeyHash, const AES::Params& Params)
{
    QByteArray InputBytes = Input.toUtf8();
    AddPKCS7Padding(InputBytes);

    QVector<State> States;
    SplitInputByStates(States, InputBytes);

    QVector<Word> KeySchedule(Nb * (Params.Nr + 1));
    GenerateKeyExpansion(KeySchedule, KeyHash, Params);

    for (State& State : States)
        EncryptState(State, KeySchedule, Params);

    QByteArray OutputBytes;
    UnionStates(States, OutputBytes);

    return OutputBytes;
}

QString AES::Decrypt(const QByteArray& InputBytes, const QByteArray& KeyHash, const AES::Params& Params)
{
    QVector<State> States;
    SplitInputByStates(States, InputBytes);

    QVector<Word> KeySchedule(Nb * (Params.Nr + 1));
    GenerateKeyExpansion(KeySchedule, KeyHash, Params);

    for (State& State : States)
        DecryptState(State, KeySchedule, Params);

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

void AES::GenerateKeyExpansion(QVector<Word>& KeySchedule, const QByteArray& KeyHash, const AES::Params& Params)
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

    assert(KeyHash.length() == Params.Nk * Word::ByteNum);

    int32_t i = 0;
    while (i < Params.Nk)
    {
        for (int32_t j = 0; j < Word::ByteNum; ++j)
        {
            KeySchedule[i].Bytes[j] = KeyHash.at(Word::ByteNum * i + j);
        }
        ++i;
    }

    assert(i == Params.Nk);

    while (i < KeySchedule.length())
    {
        Word Temp = KeySchedule[i - 1];
        if (i % Params.Nk == 0)
        {
            Temp.Rotate().Substitute();
            Temp ^= Rcon[i / Params.Nk];
        }
        else if (Params.Nk > 6 && (i % Params.Nk == 4))
        {
            Temp.Substitute();
        }
        KeySchedule[i] = KeySchedule[i - Params.Nk] ^ Temp;
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

void AES::EncryptState(State& State, const QVector<Word>& KeySchedule, const AES::Params& Params)
{
    int32_t Round = 0;
    State.AddRoundKey(KeySchedule, Round);

    for (Round = 1; Round < Params.Nr; ++Round)
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

void AES::DecryptState(State& State, const QVector<Word>& KeySchedule, const AES::Params& Params)
{
    int32_t Round = Params.Nr;
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
