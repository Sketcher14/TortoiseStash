#include "Test_Word.h"
#include "AES/Word.h"

void Test_Word::Test_Rotate()
{
    Word Input { 0x09, 0xcf, 0x4f, 0x3c };
    Word Output { 0xcf, 0x4f, 0x3c, 0x09 };

    Input.Rotate();
    QCOMPARE(Output, Input);
}

void Test_Word::Test_Substitute()
{
    Word Input { 0xcf, 0x4f, 0x3c, 0x09 };
    Word Output { 0x8a, 0x84, 0xeb, 0x01 };

    Input.Substitute();
    QCOMPARE(Output, Input);
}
