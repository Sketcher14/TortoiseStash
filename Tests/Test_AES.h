#pragma once

#include <QtTest>

class Test_AES : public QObject
{
    Q_OBJECT

private slots:
    void Test_AES128();
    void Test_AES192();
    void Test_AES256();

};
