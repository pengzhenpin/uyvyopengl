#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QQmlContext>
#include "Player.h"
int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGLRhi);
    QQmlApplicationEngine engine;
    // @uri demo
    const char *uri = "demo";
    qmlRegisterType<Player>(uri, 1, 0, "Player");
    const QUrl url(u"qrc:/qml/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
