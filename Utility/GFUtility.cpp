#include "GFUtility.h"

#include <QDebug>

constexpr uint16_t GFSize = 256;
constexpr uint8_t IrreduciblePolynomial = 0x1b; // x^8 + x^4 + x^3 + x + 1

namespace GFUtilitylocal
{
    uint8_t Mul2(uint8_t A)
    {
        if (A < GFSize / 2)
            return A << 1;

        return A << 1 ^ IrreduciblePolynomial;
    }

    uint8_t Mul2XTimes(uint8_t A, uint32_t X)
    {
        if (X <= 1)
            return Mul2(A);

        return Mul2XTimes(Mul2(A), X - 1);
    }
}


uint8_t GF256::Add(uint8_t A, uint8_t B)
{
    return A ^ B;
}

uint8_t GF256::Multiply(uint8_t A, uint8_t B)
{
    if (A == 0 || B == 0)
        return 0;

    uint8_t Res = 0;
    uint16_t i = 0;

    while (B != 0)
    {
        if (B % 2 != 0)
        {
            Res = Add(Res, i == 0 ? A : GFUtilitylocal::Mul2XTimes(A, i));
        }

        i++;
        B /= 2;
    }

    return Res;
}
