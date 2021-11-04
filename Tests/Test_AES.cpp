#include "Test_AES.h"
#include "AES/AES.h"

void Test_AES::Test_AES128()
{
    AES Algo(AESType::AES128);
    QString CipherKey("VeryDifficultPassword");

    // Message lenght is less then number of state bytes
    QString Message1("HelloWorld!");
    QString DecryptedMessage1 = Algo.Decrypt(Algo.Encrypt(Message1, CipherKey), CipherKey);
    QCOMPARE(Message1, DecryptedMessage1);

    // Message lenght is equal to number of state bytes
    QString Message2("HelloWorld!12345");
    QString DecryptedMessage2 = Algo.Decrypt(Algo.Encrypt(Message2, CipherKey), CipherKey);
    QCOMPARE(Message2, DecryptedMessage2);

    // Message lenght is greater then number of state bytes
    QString Message3("HelloWorld!_HelloWorld!_HelloWorld!");
    QString DecryptedMessage3 = Algo.Decrypt(Algo.Encrypt(Message3, CipherKey), CipherKey);
    QCOMPARE(Message3, DecryptedMessage3);
}

void Test_AES::Test_AES192()
{
    AES Algo(AESType::AES192);
    QString CipherKey("VeryDifficultPassword");

    // Message lenght is less then number of state bytes
    QString Message1("HelloWorld!");
    QString DecryptedMessage1 = Algo.Decrypt(Algo.Encrypt(Message1, CipherKey), CipherKey);
    QCOMPARE(Message1, DecryptedMessage1);

    // Message lenght is equal to number of state bytes
    QString Message2("HelloWorld!12345");
    QString DecryptedMessage2 = Algo.Decrypt(Algo.Encrypt(Message2, CipherKey), CipherKey);
    QCOMPARE(Message2, DecryptedMessage2);

    // Message lenght is greater then number of state bytes
    QString Message3("HelloWorld!_HelloWorld!_HelloWorld!");
    QString DecryptedMessage3 = Algo.Decrypt(Algo.Encrypt(Message3, CipherKey), CipherKey);
    QCOMPARE(Message3, DecryptedMessage3);
}

void Test_AES::Test_AES256()
{
    AES Algo(AESType::AES256);
    QString CipherKey("VeryDifficultPassword");

    // Message lenght is less then number of state bytes
    QString Message1("HelloWorld!");
    QString DecryptedMessage1 = Algo.Decrypt(Algo.Encrypt(Message1, CipherKey), CipherKey);
    QCOMPARE(Message1, DecryptedMessage1);

    // Message lenght is equal to number of state bytes
    QString Message2("HelloWorld!12345");
    QString DecryptedMessage2 = Algo.Decrypt(Algo.Encrypt(Message2, CipherKey), CipherKey);
    QCOMPARE(Message2, DecryptedMessage2);

    // Message lenght is greater then number of state bytes
    QString Message3("HelloWorld!_HelloWorld!_HelloWorld!");
    QString DecryptedMessage3 = Algo.Decrypt(Algo.Encrypt(Message3, CipherKey), CipherKey);
    QCOMPARE(Message3, DecryptedMessage3);
}
