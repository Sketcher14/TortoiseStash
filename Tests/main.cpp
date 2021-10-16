#include "Test_AES128.h"
#include "Test_AES192.h"
#include "Test_AES256.h"

#include <QtTest>

int main(int argc, char** argv)
{
    int Status = 0;

    Status |= QTest::qExec(new Test_AES128, argc, argv);
    Status |= QTest::qExec(new Test_AES192, argc, argv);
    Status |= QTest::qExec(new Test_AES256, argc, argv);

   return Status;
}
