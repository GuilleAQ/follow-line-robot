#include "WiFi_config.hpp"

WiFi_manager::WiFi_manager(const char* ssid, const char* password)
    : ssid(ssid), password(password) {}

WiFi_manager::WiFi_manager(const char* ssid, const char* eapAnonId, const char* eapIdentity, const char* eapUsername, const char* eapPassword)
    : ssid(ssid), eapAnonId(eapAnonId), eapIdentity(eapIdentity), eapUsername(eapUsername), eapPassword(eapPassword), isEnterprise(true) {}


void WiFi_manager::init_WiFi_conexion() {
    WiFi.mode(WIFI_STA);
    if (!isEnterprise) {
        WiFi.begin(ssid, password);
    } else {
      WiFi.begin(ssid, WPA2_AUTH_PEAP, eapIdentity, eapUsername, eapPassword); 
    }

    Serial.print(F("Connecting to network: "));
    Serial.println(ssid);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print('.');
        delay(500);
    }
    Serial.println(F("WiFi is connected!"));
}

void WiFi_manager::print_ip_rssi() {
    IPAddress ip = WiFi.localIP();
    Serial.println(F("IP address set: "));
    Serial.println(ip);
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
}

