#ifndef AESUTILITY_H
#define AESUTILITY_H

#include <stdint.h>

namespace AESUtility {
    uint8_t GetSubByte(uint8_t Byte);
    uint8_t GetInvSubByte(uint8_t Byte);

    uint8_t DotProduct(const uint8_t* A, const uint8_t* B, int32_t N);
}

#endif // AESUTILITY_H
