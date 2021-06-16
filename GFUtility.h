#ifndef GFUTILITY_H
#define GFUTILITY_H

#include <stdint.h>

namespace GF256
{
    uint8_t Add(uint8_t a, uint8_t b);
    uint8_t Multiply(uint8_t a, uint8_t b);
    uint8_t Divide(uint8_t a, uint8_t b);

    void GenerateTables();
}

#endif // GFUTILITY_H
