#ifndef AES_H
#define AES_H

#include <stdint.h>
#include <QString>
#include <QCryptographicHash>

enum class AESType {
    AES128,
    AES192,
    AES256
};


class AES
{
public:
    AES(AESType Type);

    QByteArray Encrypt(const QString& Input, const QString& CipherKey);
    QString Decrypt(const QByteArray& InputBytes, const QString& CipherKey);

private:
    inline static const int32_t Nb = 4;

    struct Word {
        inline static const int32_t ByteNum = 4;

        uint8_t Bytes[Nb] = { 0x0, 0x0, 0x0, 0x0 };

        Word operator^ (const Word& OtherWord);
        Word& operator^= (const uint8_t (&Array)[4]);

        Word& Substitute();
        Word& Rotate();
        void Print(int32_t i);
    };

    struct State {
        inline static const int32_t RowNum = 4;

        uint8_t Bytes[RowNum][Nb] = {
            { 0x0, 0x0, 0x0, 0x0 },
            { 0x0, 0x0, 0x0, 0x0 },
            { 0x0, 0x0, 0x0, 0x0 },
            { 0x0, 0x0, 0x0, 0x0 }
        };

        void Print(int32_t Round, QString Operation);
    };

    int32_t Nk = 0;
    int32_t Nr = 0;
    QCryptographicHash::Algorithm HashAlgo = QCryptographicHash::Algorithm::Md5;

    void SplitInputByStates(QVector<State>& States, const QByteArray& InputBytes);
    void GenerateKeyExpansion(QVector<Word>& KeySchedule, const QString& CipherKey);
    QByteArray UnionStates(QVector<State>& States);

    void EncryptState(State& State, const QVector<Word>& KeySchedule);
    void DecryptState(State& State, const QVector<Word>& KeySchedule);

    void AddRoundKey(State& State, const QVector<Word>& KeySchedule, int32_t Round);

    void SubstituteBytes(State& State);
    void InvSubstituteBytes(State& State);

    void ShiftRows(State& State);
    void InvShiftRows(State& State);

    void CommonMixColumns(State& State, const uint8_t (&Matrix)[4][4]);
    void MixColumns(State& State);
    void InvMixColumns(State& State);
};

#endif // AES_H
