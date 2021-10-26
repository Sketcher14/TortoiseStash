#pragma once

#include "AES/Word.h"

#include <stdint.h>
#include <QString>


class State {
public:
    static constexpr int32_t ColumnNum = 4;
    static constexpr int32_t RowNum = 4;
    static constexpr int32_t StateBytesNum = RowNum * ColumnNum;

    State(const QByteArray& InputBytes, int32_t StartIndex);
    ~State();

    QByteArray ToByteArray() const;

    void AddRoundKey(const QVector<Word>& KeySchedule, int32_t Round);

    void SubstituteBytes();
    void InvSubstituteBytes();

    void ShiftRows();
    void InvShiftRows();

    void MixColumns();
    void InvMixColumns();

private:
    uint8_t Bytes[RowNum][ColumnNum] = {
        { 0x0, 0x0, 0x0, 0x0 },
        { 0x0, 0x0, 0x0, 0x0 },
        { 0x0, 0x0, 0x0, 0x0 },
        { 0x0, 0x0, 0x0, 0x0 }
    };

    void CommonMixColumns(const uint8_t (&Matrix)[RowNum][ColumnNum]);
};
