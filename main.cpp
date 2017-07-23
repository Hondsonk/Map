#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "map.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        qDebug() << "root object is empty";

    Map myMap(&engine);
    myMap.setWindow(engine.rootObjects().at(0));

    QObject::connect(myMap.getWindow(), SIGNAL(clicked()),&myMap, SLOT(onClicked()));
    QObject::connect(myMap.getWindow(), SIGNAL(doubleClicked()),&myMap, SLOT(onDoubleClicked()));
    QObject::connect(myMap.getWindow(), SIGNAL(windowSizeChanged(int, int)), &myMap, SLOT(onWindowSizeChanged(int, int)));

    return app.exec();
}
