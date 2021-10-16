#ifndef TORTOISESTASHAPP_H
#define TORTOISESTASHAPP_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>


class TortoiseStashApp : public QGuiApplication
{
public:
    TortoiseStashApp(int argc, char *argv[]);

    int Run();

private:
    QQmlApplicationEngine Engine;
};

#endif // TORTOISESTASHAPP_H
