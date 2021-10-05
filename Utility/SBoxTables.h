#ifndef SBOXTABLES_H
#define SBOXTABLES_H

#include <stdint.h>

namespace SBOX_TABLE {
    uint8_t GetSubByte(uint8_t Byte);
    uint8_t GetInvSubByte(uint8_t Byte);
}

#endif // SBOXTABLES_H
