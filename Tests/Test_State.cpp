#include "Test_State.h"
#include "AES/State.h"

#include <QVector>

void Test_State::Test_AddRoundKey()
{
    const unsigned char Input[16] = {
        0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34
    };
    QByteArray InputBytes = QByteArray::fromRawData((char*) Input, 16);

    const unsigned char Output[16] = {
        0x19, 0x3d, 0xe3, 0xbe, 0xa0, 0xf4, 0xe2, 0x2b, 0x9a, 0xc6, 0x8d, 0x2a, 0xe9, 0xf8, 0x48, 0x08
    };
    QByteArray OutputBytes = QByteArray::fromRawData((char*) Output, 16);

    QVector<Word> KeySchedule;
    KeySchedule.append(Word { 0x2b, 0x7e, 0x15, 0x16 });
    KeySchedule.append(Word { 0x28, 0xae, 0xd2, 0xa6 });
    KeySchedule.append(Word { 0xab, 0xf7, 0x15, 0x88 });
    KeySchedule.append(Word { 0x09, 0xcf, 0x4f, 0x3c });


    State State(InputBytes, 0);
    State.AddRoundKey(KeySchedule, 0);
    QCOMPARE(OutputBytes, State.ToByteArray());
}

void Test_State::Test_SubstituteBytes()
{
    const unsigned char Input[16] = {
        0x19, 0x3d, 0xe3, 0xbe, 0xa0, 0xf4, 0xe2, 0x2b, 0x9a, 0xc6, 0x8d, 0x2a, 0xe9, 0xf8, 0x48, 0x08
    };
    QByteArray InputBytes = QByteArray::fromRawData((char*) Input, 16);

    const unsigned char Output[16] = {
        0xd4, 0x27, 0x11, 0xae, 0xe0, 0xbf, 0x98, 0xf1, 0xb8, 0xb4, 0x5d, 0xe5, 0x1e, 0x41, 0x52, 0x30
    };
    QByteArray OutputBytes = QByteArray::fromRawData((char*) Output, 16);


    State State(InputBytes, 0);
    State.SubstituteBytes();
    QCOMPARE(OutputBytes, State.ToByteArray());

    State.InvSubstituteBytes();
    QCOMPARE(InputBytes, State.ToByteArray());

}

void Test_State::Test_ShiftRows()
{
    const unsigned char Input[16] = {
        0xd4, 0x27, 0x11, 0xae, 0xe0, 0xbf, 0x98, 0xf1, 0xb8, 0xb4, 0x5d, 0xe5, 0x1e, 0x41, 0x52, 0x30
    };
    QByteArray InputBytes = QByteArray::fromRawData((char*) Input, 16);

    const unsigned char Output[16] = {
        0xd4, 0xbf, 0x5d, 0x30, 0xe0, 0xb4, 0x52, 0xae, 0xb8, 0x41, 0x11, 0xf1, 0x1e, 0x27, 0x98, 0xe5
    };
    QByteArray OutputBytes = QByteArray::fromRawData((char*) Output, 16);


    State State(InputBytes, 0);
    State.ShiftRows();
    QCOMPARE(OutputBytes, State.ToByteArray());

    State.InvShiftRows();
    QCOMPARE(InputBytes, State.ToByteArray());
}

void Test_State::Test_MixColumns()
{
    const unsigned char Input[16] = {
        0xd4, 0xbf, 0x5d, 0x30, 0xe0, 0xb4, 0x52, 0xae, 0xb8, 0x41, 0x11, 0xf1, 0x1e, 0x27, 0x98, 0xe5
    };
    QByteArray InputBytes = QByteArray::fromRawData((char*) Input, 16);

    const unsigned char Output[16] = {
        0x04, 0x66, 0x81, 0xe5, 0xe0, 0xcb, 0x19, 0x9a, 0x48, 0xf8, 0xd3, 0x7a, 0x28, 0x06, 0x26, 0x4c
    };
    QByteArray OutputBytes = QByteArray::fromRawData((char*) Output, 16);


    State State(InputBytes, 0);
    State.MixColumns();
    QCOMPARE(OutputBytes, State.ToByteArray());

    State.InvMixColumns();
    QCOMPARE(InputBytes, State.ToByteArray());
}
