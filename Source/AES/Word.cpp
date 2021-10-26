#include "AES/Word.h"
#include "Utility/AESUtility.h"

#include <algorithm>

Word Word::operator^(const Word& OtherWord)
{
    Word Result { 0x00, 0x00, 0x00, 0x00 };

    for (int32_t i = 0; i < Word::ByteNum; ++i)
    {
        Result.Bytes[i] = Bytes[i] ^ OtherWord.Bytes[i];
    }

    return Result;
}

Word& Word::operator^=(const uint8_t (&Array)[ByteNum])
{
    for (int32_t i = 0; i < Word::ByteNum; ++i)
    {
        Bytes[i] ^= Array[i];
    }

    return *this;
}

Word& Word::Substitute()
{
    for (int32_t i = 0; i < Word::ByteNum; ++i)
    {
        Bytes[i] = AESUtility::GetSubByte(Bytes[i]);
    }

    return *this;
}

Word& Word::Rotate()
{
    std::rotate(Bytes, Bytes + 1, Bytes + Word::ByteNum);

    return *this;
}
