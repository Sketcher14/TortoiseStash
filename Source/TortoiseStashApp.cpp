#include "TortoiseStashApp.h"

TortoiseStashApp::TortoiseStashApp(int argc, char *argv[]) : QGuiApplication(argc, argv)
{
}

int TortoiseStashApp::Run()
{
    const QUrl Url(QStringLiteral("qrc:/main.qml"));

    QObject::connect(&Engine, &QQmlApplicationEngine::objectCreated, this, [Url](QObject *Obj, const QUrl &ObjUrl)
    {
        if (!Obj && Url == ObjUrl)
            QCoreApplication::exit(-1);

    }, Qt::QueuedConnection);

    Engine.load(Url);

    return exec();
}


