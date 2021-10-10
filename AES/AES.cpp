#include "AES.h"
#include "Utility/AESUtility.h"

#include <algorithm>
#include <numeric>
#include <QVector>

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
            State.Bytes[i][j] = AESUtility::GetSubByte(State.Bytes[i][j]);
        }
    }
}

void AES::AppplyInvSubBytes(AES::State &State)
{
    for (ssize_t i = 0; i < State::RowNum; ++i)
    {
        for (ssize_t j = 0; j < Nb; ++j)
        {
            State.Bytes[i][j] = AESUtility::GetInvSubByte(State.Bytes[i][j]);
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

void AES::CommonMixColumns(AES::State& State, const uint8_t (&Matrix)[4][4])
{
    for (ssize_t j = 0; j < Nb; ++j)
    {
        uint8_t StateColumn[State::RowNum] = { 0x0, 0x0, 0x0, 0x0 };
        for (ssize_t i = 0; i < State::RowNum; ++i)
        {
            StateColumn[i] = State.Bytes[i][j];
        }

        for (ssize_t i = 0; i < State::RowNum; ++j)
        {
            State.Bytes[i][j] = AESUtility::DotProduct(StateColumn, Matrix[i], Nb);
        }
    }
}

void AES::ApplyMixColumns(State& State)
{
    static const uint8_t MixMatrix[4][4] = {
        { 0x02, 0x03, 0x01, 0x01 },
        { 0x01, 0x02, 0x03, 0x01 },
        { 0x01, 0x01, 0x02, 0x03 },
        { 0x03, 0x01, 0x01, 0x02 }
    };

    CommonMixColumns(State, MixMatrix);
}

void AES::ApplyInvMixColumns(AES::State &State)
{
    static const uint8_t InvMixMatrix[4][4] = {
        { 0x0e, 0x0b, 0x0d, 0x09 },
        { 0x09, 0x0e, 0x0b, 0x0d },
        { 0x0d, 0x09, 0x0e, 0x0b },
        { 0x0b, 0x0d, 0x09, 0x0e }
    };

    CommonMixColumns(State, InvMixMatrix);
}
