#include "Test_AES.h"
#include "AES/AES.h"

void Test_AES::Test_AES128()
{
    const AES::Type Type = AES::Type::AES128;
    QString CipherKey("VeryDifficultPassword");

    // Message lenght is less then number of state bytes
    QString Message1("HelloWorld!");
    QString DecryptedMessage1 = AES::Decrypt(AES::Encrypt(Message1, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message1, DecryptedMessage1);

    // Message lenght is equal to number of state bytes
    QString Message2("HelloWorld!12345");
    QString DecryptedMessage2 = AES::Decrypt(AES::Encrypt(Message2, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message2, DecryptedMessage2);

    // Message lenght is greater then number of state bytes
    QString Message3("HelloWorld!_HelloWorld!_HelloWorld!");
    QString DecryptedMessage3 = AES::Decrypt(AES::Encrypt(Message3, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message3, DecryptedMessage3);
}

void Test_AES::Test_AES192()
{
    const AES::Type Type = AES::Type::AES192;
    QString CipherKey("VeryDifficultPassword");

    // Message lenght is less then number of state bytes
    QString Message1("HelloWorld!");
    QString DecryptedMessage1 = AES::Decrypt(AES::Encrypt(Message1, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message1, DecryptedMessage1);

    // Message lenght is equal to number of state bytes
    QString Message2("HelloWorld!12345");
    QString DecryptedMessage2 = AES::Decrypt(AES::Encrypt(Message2, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message2, DecryptedMessage2);

    // Message lenght is greater then number of state bytes
    QString Message3("HelloWorld!_HelloWorld!_HelloWorld!");
    QString DecryptedMessage3 = AES::Decrypt(AES::Encrypt(Message3, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message3, DecryptedMessage3);
}

void Test_AES::Test_AES256()
{
    const AES::Type Type = AES::Type::AES256;
    QString CipherKey("VeryDifficultPassword");

    // Message lenght is less then number of state bytes
    QString Message1("HelloWorld!");
    QString DecryptedMessage1 = AES::Decrypt(AES::Encrypt(Message1, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message1, DecryptedMessage1);

    // Message lenght is equal to number of state bytes
    QString Message2("HelloWorld!12345");
    QString DecryptedMessage2 = AES::Decrypt(AES::Encrypt(Message2, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message2, DecryptedMessage2);

    // Message lenght is greater then number of state bytes
    QString Message3("HelloWorld!_HelloWorld!_HelloWorld!");
    QString DecryptedMessage3 = AES::Decrypt(AES::Encrypt(Message3, CipherKey, Type), CipherKey, Type);
    QCOMPARE(Message3, DecryptedMessage3);
}
