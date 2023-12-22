#include "MQTT.hpp"

MQTT_manager::MQTT_manager(WiFiClient& client, const char* server, uint16_t port, const char* topic_base)
    : mqttClient(&client, server, port), topicPub(&mqttClient, topic_base) {}

// To connect to MQTT server
bool MQTT_manager::connect() {
    if (mqttClient.connected()) {
        return true;
    }

    Serial.print("Connecting to MQTT SERVER... ");
    for (int attempt = 0; attempt < 3; attempt++) {
        if (this->mqttClient.connect() == 0) {
          Serial.println("Unable to connect to MQTT. System will restart.");
          while(1); // Restart the system if connection fails
        }
        Serial.println("Retrying...");
        delay(1000);
    }
    Serial.println("MQTT server connected succesfully!");
}

// To send to MQTT server
void MQTT_manager::send_to_broker(const String& message) {
    if (!mqttClient.connected()) {
        Serial.println("MQTT not connected.");
        return;
    }

    if (topicPub.publish(message.c_str())) {
        Serial.println("Message sent successfully.");
    } else {
        Serial.println("Failed to send message.");
    }
}
