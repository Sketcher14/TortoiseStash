#include "TortoiseStashApp.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    TortoiseStashApp App(argc, argv);

    return App.Run();
}
