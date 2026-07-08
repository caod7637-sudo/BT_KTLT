#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gameengine.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    GameEngine *gameEngineObject = new GameEngine(&app);
    engine.rootContext()->setContextProperty("gameEngine", gameEngineObject);

    engine.loadFromModule("CarGame", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
