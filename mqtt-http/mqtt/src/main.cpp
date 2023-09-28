#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "app_environment.h"
#include "import_qml_components_plugins.h"
#include "import_qml_plugins.h"
#include <QtNetwork>
#include <QCoreApplication>
#include <QtMqtt/QMqttClient>
#include <QtMqtt/QMqttTopicName>
#include <QTimer>
#include "iot.h"

int main(int argc, char *argv[])
{
    set_qt_environment();

    QGuiApplication app(argc, argv);

    // MQTT istemcisi oluşturun
    QMqttClient client;

    // MQTT ayarlarını yapmak için ayrı bir fonksiyon çağırın
    setupMqtt(client);

    // HTTP POST isteği göndermek için işlevi çağırın
    sendHttpPostRequest();

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:Main/main.qml"_qs);
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
