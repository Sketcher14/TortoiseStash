#pragma once

#include <stdint.h>

struct Word {
    static constexpr int32_t ByteNum = 4;

    uint8_t Bytes[ByteNum] = { 0x0, 0x0, 0x0, 0x0 };

    Word operator^ (const Word& OtherWord);
    Word& operator^= (const uint8_t (&Array)[ByteNum]);

    Word& Substitute();
    Word& Rotate();
};
