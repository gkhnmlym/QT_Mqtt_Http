#ifndef IOT_H
#define IOT_H

#include <QtMqtt/QMqttClient>
#include <QtNetwork>

void onMqttMessageReceived(const QByteArray &message, const QMqttTopicName &topic);
void setupMqtt(QMqttClient &client);
void sendHttpPostRequest();

#endif // IOT_H
