#ifndef AES_H
#define AES_H

#include <QString>
#include <stdint.h>

enum class AESType {
    AES128,
    AES192,
    AES256
};


class AES
{
public:
    AES(AESType Type);

    void Encrypt(QString FilePath, QString HashPassword);
    void Decrypt(QString FilePath, QString HashPassword);

private:
    static const int32_t Nb = 4;

    struct State {
        uint8_t Word0[Nb] { 0x0, 0x0, 0x0, 0x0 };
        uint8_t Word1[Nb] { 0x0, 0x0, 0x0, 0x0 };
        uint8_t Word2[Nb] { 0x0, 0x0, 0x0, 0x0 };
        uint8_t Word3[Nb] { 0x0, 0x0, 0x0, 0x0 };
    };

    int32_t Nk = 0;
    int32_t Nr = 0;

    void calculateKeyExpansion(QString HashPassword);

    void AddRoundKey();
    void ApplySubBytes();
    void ApplyShiftRows();
    void ApplyMixColumns();
};

#endif // AES_H
