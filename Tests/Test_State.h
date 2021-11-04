#pragma once

#include <QtTest>

class Test_State : public QObject
{
    Q_OBJECT

private slots:
    void Test_AddRoundKey();
    void Test_SubstituteBytes();
    void Test_ShiftRows();
    void Test_MixColumns();

};
