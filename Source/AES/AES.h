#pragma once

#include "AES/Word.h"
#include "AES/State.h"

#include <stdint.h>
#include <QString>

class AES
{
public:
    enum Type {
        AES128,
        AES192,
        AES256
    };

    static QByteArray Encrypt(const QString& Input, const QString& CipherKey, AES::Type Type);
    static QString Decrypt(const QByteArray& InputBytes, const QString& CipherKey, AES::Type Type);

private:
    static constexpr int32_t Nb = 4;

    struct Params {
        int32_t Nk = 0;
        int32_t Nr = 0;
    };

    static void PrepareKeyAndParams(const QString& CipherKey, AES::Type Type, QByteArray& KeyHash, AES::Params& Params);

    static QByteArray Encrypt(const QString& Input, const QByteArray& KeyHash, const AES::Params& Params);
    static QString Decrypt(const QByteArray& InputBytes, const QByteArray& KeyHash, const AES::Params& Params);

    static void AddPKCS7Padding(QByteArray& InputBytes);
    static void RemovePKCS7Padding(QByteArray& OutputBytes);

    static void SplitInputByStates(QVector<State>& States, const QByteArray& InputBytes);
    static void GenerateKeyExpansion(QVector<Word>& KeySchedule, const QByteArray& KeyHash, const AES::Params& Params);
    static void UnionStates(const QVector<State>& States, QByteArray& OutputBytes);

    static void EncryptState(State& State, const QVector<Word>& KeySchedule, const AES::Params& Params);
    static void DecryptState(State& State, const QVector<Word>& KeySchedule, const AES::Params& Params);
};
