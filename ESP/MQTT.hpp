#ifndef MQTT_H
#define MQTT_H

#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <WiFi.h>

class MQTT_manager {
public:
    MQTT_manager(WiFiClient& client, const char* server, uint16_t port, const char* topic_base);
    bool connect();
    void send_to_broker(const String& message);

private:
    Adafruit_MQTT_Client mqttClient;
    Adafruit_MQTT_Publish topicPub;
};

#endif // MQTT_H
