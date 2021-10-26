#include "AES/State.h"
#include "Utility/AESUtility.h"

#include <algorithm>
#include <QVector>

State::State(const QByteArray& InputBytes, int32_t StartIndex)
{
    assert(InputBytes.size() % State::StateBytesNum == 0);

    for (int32_t j = 0; j < State::ColumnNum; ++j)
    {
        for (int32_t i = 0; i < State::RowNum; ++i)
        {
            Bytes[i][j] = InputBytes.at(StartIndex + State::ColumnNum * j + i);
        }
    }
}

State::~State()
{
    for (int32_t i = 0; i < State::RowNum; ++i)
    {
        for (int32_t j = 0; j < State::ColumnNum; ++j)
        {
            Bytes[i][j] = 0x0;
        }
    }
}

QByteArray State::ToByteArray() const
{
    QByteArray OutputBytes;

    for (int32_t j = 0; j < State::ColumnNum; ++j)
    {
        for (int32_t i = 0; i < State::RowNum; ++i)
        {
            OutputBytes.append(Bytes[i][j]);
        }
    }

    return OutputBytes;
}

void State::AddRoundKey(const QVector<Word>& KeySchedule, int32_t Round)
{
    for (int32_t j = 0; j < State::ColumnNum; ++j)
    {
        for (int32_t i = 0; i < State::RowNum; ++i)
        {
            Bytes[i][j] ^= KeySchedule[Round + j].Bytes[i];
        }
    }
}

void State::SubstituteBytes()
{
    for (int32_t i = 0; i < State::RowNum; ++i)
    {
        for (int32_t j = 0; j < State::ColumnNum; ++j)
        {
            Bytes[i][j] = AESUtility::GetSubByte(Bytes[i][j]);
        }
    }
}

void State::InvSubstituteBytes()
{
    for (int32_t i = 0; i < State::RowNum; ++i)
    {
        for (int32_t j = 0; j < State::ColumnNum; ++j)
        {
            Bytes[i][j] = AESUtility::GetInvSubByte(Bytes[i][j]);
        }
    }
}

void State::ShiftRows()
{
    for (int32_t i = 1; i < State::RowNum; ++i)
    {
        std::rotate(Bytes[i], Bytes[i] + i, Bytes[i] + State::ColumnNum);
    }
}

void State::InvShiftRows()
{
    for (int32_t i = 1; i < State::RowNum; ++i)
    {
        std::rotate(Bytes[i], Bytes[i] + (ColumnNum - i) % ColumnNum, Bytes[i] + ColumnNum);
    }
}

void State::MixColumns()
{
    static const uint8_t MixMatrix[State::RowNum][State::ColumnNum] = {
        { 0x02, 0x03, 0x01, 0x01 },
        { 0x01, 0x02, 0x03, 0x01 },
        { 0x01, 0x01, 0x02, 0x03 },
        { 0x03, 0x01, 0x01, 0x02 }
    };

    CommonMixColumns(MixMatrix);
}

void State::InvMixColumns()
{
    static const uint8_t InvMixMatrix[State::RowNum][State::ColumnNum] = {
        { 0x0e, 0x0b, 0x0d, 0x09 },
        { 0x09, 0x0e, 0x0b, 0x0d },
        { 0x0d, 0x09, 0x0e, 0x0b },
        { 0x0b, 0x0d, 0x09, 0x0e }
    };

    CommonMixColumns(InvMixMatrix);
}

void State::CommonMixColumns(const uint8_t (&Matrix)[State::RowNum][State::ColumnNum])
{
    for (int32_t j = 0; j < State::ColumnNum; ++j)
    {
        uint8_t StateColumn[State::RowNum] = { 0x0, 0x0, 0x0, 0x0 };
        for (int32_t i = 0; i < State::RowNum; ++i)
        {
            StateColumn[i] = Bytes[i][j];
        }

        for (int32_t i = 0; i < State::RowNum; ++i)
        {
            Bytes[i][j] = AESUtility::DotProduct(StateColumn, Matrix[i], State::ColumnNum);
        }
    }
}
