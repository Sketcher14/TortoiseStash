#include "AES.h"

AES::AES(AESType Type)
{
    switch (Type)
    {
        case AESType::AES128:
        {
            Nk = 4;
            Nr = 10;
            break;
        }
        case AESType::AES192:
        {
            Nk = 6;
            Nr = 12;
            break;
        }
        case AESType::AES256:
        {
            Nk = 8;
            Nr = 14;
            break;
        }
    }
}

void AES::Encrypt(QString FilePath, QString HashPassword)
{

}

void AES::Decrypt(QString FilePath, QString HashPassword)
{

}

void AES::calculateKeyExpansion(QString HashPassword)
{

}

void AES::AddRoundKey()
{

}

void AES::ApplySubBytes()
{

}

void AES::ApplyShiftRows()
{

}

void AES::ApplyMixColumns()
{

}
