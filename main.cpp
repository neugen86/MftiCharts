#include <QApplication>
#include <QQmlApplicationEngine>

#include <SeriesModel.h>
#include <PointHandle.h>
#include <PointGenerator.h>

void RegisterTypes()
{
    qmlRegisterType<SeriesModel>("MyTypes", 1, 0, "SeriesModel");
    qmlRegisterType<PointGenerator>("MyTypes", 1, 0, "PointGenerator");
    qmlRegisterUncreatableType<PointHandle>("MyTypes", 1, 0, "PointHandle", "Use SeriesModel to get object");
}

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    RegisterTypes();

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
