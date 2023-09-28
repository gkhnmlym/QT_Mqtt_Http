#include "iot.h"

void onMqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic) {
    qDebug() << "MQTT'den gelen veri:" << message << "Konu (Topic):" << topic.name();
    // Burada gelen verileri işleyebilirsiniz.
}

void setupMqtt(QMqttClient &client) {
    // MQTT sunucusuna bağlan
    client.setHostname("mqttadress"); // MQTT sunucusunun adresi
    client.setPort(1883); // MQTT sunucusunun portu (genellikle 1883 veya 8883)
    client.connectToHost();

    // Bağlandığında çalışacak olan bir lambda işlevi tanımlayın
    QObject::connect(&client, &QMqttClient::connected, [&client]() {
        // Veriyi gönder
        QTimer *timer = new QTimer(&client);
        QObject::connect(timer, &QTimer::timeout, [&client]() {
            QByteArray message = "ruthere";
            QMqttTopicName topic("cig/machines/device1"); // MQTT konusu
            client.publish(topic, message);
        });
        timer->start(30000); // 30 saniyede bir çalışacak (30000 milisaniye)

        QMqttTopicFilter filter("crystal/machines/smach85"); // Dinlenecek konu
        client.subscribe(filter);

        // Gelen MQTT mesajlarını işlemek için bir işlevi bağlayın
        QObject::connect(&client, &QMqttClient::messageReceived, &onMqttMessageReceived);
    });
}

void sendHttpPostRequest() {
    QNetworkRequest request(QUrl("http://172.16.149.127:3001/api/mqtt-data"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonObject json;
    json.insert("message", "value1");

    QNetworkAccessManager nam;
    QNetworkReply *reply = nam.post(request, QJsonDocument(json).toJson());

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            qDebug() << "Başarıyla POST isteği gönderildi ve yanıt alındı.";
            qDebug() << "Yanıt verisi:" << reply->readAll();
        } else {
            qDebug() << "POST isteği sırasında bir hata oluştu:" << reply->errorString();
        }

        reply->deleteLater();
        QCoreApplication::quit();
    });
}
