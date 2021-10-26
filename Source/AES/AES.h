#pragma once

#include "AES/Word.h"
#include "AES/State.h"

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
    static constexpr int32_t Nb = 4;

    int32_t Nk = 0;
    int32_t Nr = 0;
    QCryptographicHash::Algorithm HashAlgo = QCryptographicHash::Algorithm::Md5;

    void AddPKCS7Padding(QByteArray& InputBytes);
    void RemovePKCS7Padding(QByteArray& OutputBytes);

    void SplitInputByStates(QVector<State>& States, const QByteArray& InputBytes);
    void GenerateKeyExpansion(QVector<Word>& KeySchedule, const QString& CipherKey);
    void UnionStates(const QVector<State>& States, QByteArray& OutputBytes);

    void EncryptState(State& State, const QVector<Word>& KeySchedule);
    void DecryptState(State& State, const QVector<Word>& KeySchedule);
};
