#include "CryptographicService.h"
#include "AES/AES.h"

#include <QFile>
#include <QDir>

namespace Extension
{
    static const char* TTS = "tts";
    static const char* DotTTS = ".tts";
    static const char* DotTXT = ".txt";
}

CryptographicService &CryptographicService::GetInstance()
{
    static CryptographicService Service;
    return Service;
}

void CryptographicService::EncryptFile(const QString& FilePath, const QString& Password, CryptographicService::Algo Algo)
{
    QString HandledFilePath = QDir::fromNativeSeparators(FilePath);

    QFile InputFile(HandledFilePath);
    if (!InputFile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QFileInfo InputFileInfo(InputFile);
    QString OutputFilePath =  InputFileInfo.path() + QDir::separator() + InputFileInfo.completeBaseName() + Extension::DotTTS;

    QFile OutputFile(OutputFilePath);
    if (!OutputFile.open(QIODevice::WriteOnly))
    {
        InputFile.close();
        return;
    }

    QString InputText = InputFile.readAll();
    QByteArray EncryptedInput;

    switch (Algo)
    {
        case CryptographicService::Algo::AES128:
        {
            EncryptedInput = AES::Encrypt(InputText, Password, AES::Type::AES128);
            break;
        }
        case CryptographicService::Algo::AES192:
        {
            EncryptedInput = AES::Encrypt(InputText, Password, AES::Type::AES192);
            break;
        }
        case CryptographicService::Algo::AES256:
        {
            EncryptedInput = AES::Encrypt(InputText, Password, AES::Type::AES256);
            break;
        }
    }

    OutputFile.write(EncryptedInput);

    InputFile.close();
    OutputFile.close();
}

void CryptographicService::DecryptFile(const QString& FilePath, const QString& Password, CryptographicService::Algo Algo)
{
    QString HandledFilePath = QDir::fromNativeSeparators(FilePath);

    QFile InputFile(HandledFilePath);
    if (!InputFile.open(QIODevice::ReadOnly))
        return;

    QFileInfo InputFileInfo(InputFile);
    QString InputFileExtension = InputFileInfo.suffix();

    if (InputFileExtension != Extension::TTS)
    {
        InputFile.close();
        return;
    }

    QString OutputFilePath = HandledFilePath + Extension::DotTXT;

    QFile OutputFile(OutputFilePath);
    if (!OutputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        InputFile.close();
        return;
    }

    QByteArray InputBytes = InputFile.readAll();

    QString DecryptedInput;

    switch (Algo)
    {
        case CryptographicService::Algo::AES128:
        {
            DecryptedInput = AES::Decrypt(InputBytes, Password, AES::Type::AES128);
            break;
        }
        case CryptographicService::Algo::AES192:
        {
            DecryptedInput = AES::Decrypt(InputBytes, Password, AES::Type::AES192);
            break;
        }
        case CryptographicService::Algo::AES256:
        {
            DecryptedInput = AES::Decrypt(InputBytes, Password, AES::Type::AES256);
            break;
        }
    }

    OutputFile.write(DecryptedInput.toUtf8());

    InputFile.close();
    OutputFile.close();
}
