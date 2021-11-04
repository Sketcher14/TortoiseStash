#include "Test_State.h"
#include "Test_AES.h"
#include "Test_Word.h"

#include <QtTest>

int main(int argc, char** argv)
{
    int Status = 0;

    Status |= QTest::qExec(new Test_Word, argc, argv);
    Status |= QTest::qExec(new Test_State, argc, argv);
    Status |= QTest::qExec(new Test_AES, argc, argv);

   return Status;
}
