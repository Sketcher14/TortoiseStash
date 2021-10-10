#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <QString>

enum class AESType {
    AES128,
    AES192,
    AES256
};


class AES
{
public:
    AES(AESType Type);

    void Encrypt(QString Input, QString HashPassword);
    void Decrypt(QString Input, QString HashPassword);

private:
    inline static const int32_t Nb = 4;

    struct State {
        inline static const int32_t RowNum = 4;

        uint8_t Bytes[RowNum][Nb] = {
            { 0x0, 0x0, 0x0, 0x0 },
            { 0x0, 0x0, 0x0, 0x0 },
            { 0x0, 0x0, 0x0, 0x0 },
            { 0x0, 0x0, 0x0, 0x0 }
        };
    };

    int32_t Nk = 0;
    int32_t Nr = 0;

    void calculateKeyExpansion(QString HashPassword);

    void EncryptState(State& State);
    void DecryptState(State& State);

    void AddRoundKey();

    void ApplySubBytes(State& State);
    void AppplyInvSubBytes(State& State);

    void ApplyShiftRows(State& State);
    void ApplyInvShiftRows(State& State);

    void CommonMixColumns(AES::State& State, const uint8_t (&Matrix)[4][4]);
    void ApplyMixColumns(State& State);
    void ApplyInvMixColumns(State& State);
};

#endif // AES_H
