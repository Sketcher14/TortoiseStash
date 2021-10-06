#include "AES.h"
#include "Utility/SBoxTables.h"

#include <QVector>
#include <algorithm>

namespace AESLocal {
    constexpr int32_t MixMatrixSize = 4;
    uint8_t MixMatrix[MixMatrixSize][MixMatrixSize] = {
        { 0x02, 0x03, 0x01, 0x01 },
        { 0x01, 0x02, 0x03, 0x01 },
        { 0x01, 0x01, 0x02, 0x03 },
        { 0x03, 0x01, 0x01, 0x02 }
    };
}

AES::AES(AESType Type)
{
    switch (Type)
    {
        case AESType::AES128:
        {
            Nk = 4;
            Nr = 10;
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

void AES::Encrypt(QString Input, QString HashPassword)
{
    QByteArray InputBytes = Input.toUtf8();

    const int32_t StateBytesCount = State::RowNum * Nb;
    const int32_t AmountStates = (InputBytes.size() + StateBytesCount - 1) / StateBytesCount;
    QVector<State> States(AmountStates);

    ssize_t s = 0;
    ssize_t r = 0;
    ssize_t c = 0;
    for (ssize_t i = 0; i < InputBytes.size(); ++i)
    {
        States[s].Bytes[r][c] = InputBytes.at(i);
        if (++r >= State::RowNum)
        {
            if (++c >= Nb)
            {
                s++;
                c = 0;
            }
            r = 0;
        }
    }

    for (State& State : States)
        EncryptState(State);

    //union states and write to file
}

void AES::Decrypt(QString Input, QString HashPassword)
{

}

void AES::calculateKeyExpansion(QString HashPassword)
{

}

void AES::EncryptState(AES::State &State)
{

}

void AES::DecryptState(AES::State &State)
{

}

void AES::AddRoundKey()
{

}

void AES::ApplySubBytes(State& State)
{
    for (ssize_t i = 0; i < State::RowNum; ++i)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            State.Bytes[i][j] = SBOX_TABLE::GetSubByte(State.Bytes[i][j]);
        }
    }
}

void AES::AppplyInvSubBytes(AES::State &State)
{
    for (ssize_t i = 0; i < State::RowNum; ++i)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            State.Bytes[i][j] = SBOX_TABLE::GetInvSubByte(State.Bytes[i][j]);
        }
    }
}

void AES::ApplyShiftRows(State& State)
{
    for (ssize_t i = 1; i < State::RowNum; ++i)
    {
        std::rotate(State.Bytes[i], State.Bytes[i] + i, State.Bytes[i] + Nb);
    }
}

void AES::ApplyInvShiftRows(AES::State &State)
{
    for (ssize_t i = 1; i < State::RowNum; ++i)
    {
        std::rotate(State.Bytes[i], State.Bytes[i] + (Nb - i) % Nb, State.Bytes[i] + Nb);
    }
}

void AES::ApplyMixColumns(State& State)
{

}

void AES::ApplyInvMixColumns(AES::State &State)
{

}
