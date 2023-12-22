#ifndef WIFI_CONFIG_H
#define WIFI_CONFIG_H

#include <WiFi.h>

class WiFi_manager {
public:
    WiFi_manager(const char* ssid, const char* password);
    WiFi_manager(const char* ssid, const char* eapAnonId, const char* eapIdentity, const char* eapUsername, const char* eapPassword); 
    void init_WiFi_conexion();
    void print_ip_rssi();

private:
    const char* ssid;
    const char* password;

    bool isEnterprise;

    const char* eapAnonId;
    const char* eapIdentity;
    const char* eapUsername;
    const char* eapPassword;
};

#endif // WIFI_CONFIG_H
