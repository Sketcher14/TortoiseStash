#pragma once

#include <QString>

class CryptographicService
{
public:
    enum Algo
    {
        AES128,
        AES192,
        AES256
    };

    CryptographicService(const CryptographicService&) = delete;
    CryptographicService& operator=(const CryptographicService&) = delete;

    static CryptographicService& GetInstance();

    void EncryptFile(const QString& FilePath, const QString& Password, CryptographicService::Algo Algo);
    void DecryptFile(const QString& FilePath, const QString& Password, CryptographicService::Algo Algo);

private:
    CryptographicService() = default;
};
