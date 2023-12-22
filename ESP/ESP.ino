#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>

#include "WiFi_config.hpp"
#include "MQTT.hpp"

#define EAP_ANONYMOUS_IDENTITY "20220719anonymous@urjc.es" // leave as it is
#define EAP_IDENTITY "g.alcocer.2020@alumnos.urjc.es"    // Use your URJC email
#define EAP_PASSWORD "XXXXXXXXXXXXX"            // User your URJC password
#define EAP_USERNAME "g.alcocer.2020@alumnos.urjc.es"  // Use your URJC email


// Json messages
#define START_LAP "0"
#define END_LAP "1"
#define OBSTACLE_DETECTED "2"
#define LINE_LOST "3"
#define PING "4"
#define INIT_LINE_SEARCH "5"
#define STOP_LINE_SEARCH "6"
#define LINE_FOUND "7"
#define VISIBLE_LINE "8"

// serial conexion config
#define RXD2 33
#define TXD2 4

//SSID NAME
const char* ssid = "eduroam"; // eduroam SSID

WiFi_manager wifiManager(ssid, EAP_ANONYMOUS_IDENTITY, EAP_IDENTITY, EAP_USERNAME, EAP_PASSWORD);
// WiFi_manager wifiManager("Galaxy XXXX", "XXXXXX");
// WiFi_manager wifiManager("MPOCO XXXX", "XXXXXX");
WiFiClient client;
MQTT_manager mqtt_manager(client, "garceta.tsc.urjc.es", 21883, "/SETR/2023/12/");

// msgs buffer
String output_buffer; // ESP to MQTT broker
String input_buffer; // Arduino to ESP

// timers
long begin_millis;
long last_millis = 0;

void send_json_msgs() {
  // if (input_buffer == START_LAP) {
  //   output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"START_LAP\"\n}";
  //   Serial.println("Sending START_LAP");
  // } 
  // else if 
  if (input_buffer == END_LAP) {
    String time_lap = Serial2.readStringUntil('\n');
    long time_lap_long = strtol(time_lap.c_str(), NULL, 10);
    Serial.println("Sending END_LAP");
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"END_LAP\",\n\t\"time\": " + String(time_lap_long) + "\n}";
  } 
  else if (input_buffer == OBSTACLE_DETECTED) {
    String distance = Serial2.readStringUntil('\n');
    int distance_int = distance.toInt();
    Serial.println("Sending OBSTACLE_DETECTED");
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"OBSTACLE_DETECTED\",\n\t\"distance\": \"" + String(distance_int) + "\"\n}";
  } 
  else if (input_buffer == LINE_LOST) {
    Serial.println("Sending LINE_LOST");
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"LINE_LOST\"\n}";
  } 
  else if (input_buffer == PING){
    String time_ping = Serial2.readStringUntil('\n');
    long time_ping_long = strtol(time_ping.c_str(), NULL, 10);
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"PING\",\n\t\"time\": " + String(time_ping_long) + "\n}";
  } 
  else if (input_buffer == INIT_LINE_SEARCH) {
    Serial.println("Sending INIT_LINE_SEARCH");
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"INIT_LINE_SEARCH\"\n}";
  } 
  else if (input_buffer == STOP_LINE_SEARCH) {
    Serial.println("Sending STOP_LINE_SEARCH");
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"STOP_LINE_SEARCH\"\n}";
  } 
  else if (input_buffer == LINE_FOUND) {
    Serial.println("Sending LINE_FOUND");
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"LINE_FOUND\"\n}";
  } 
  else if (input_buffer == VISIBLE_LINE) {
    String line_percent = Serial2.readStringUntil('\n');
    float line_percent_float = line_percent.toFloat();
    Serial.println("Sending VISIBLE_LINE");
    String value = "0.00";
    output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"VISIBLE_LINE\",\n\t\"value\": \"" + String(line_percent_float) + "\"\n}";
  }
  mqtt_manager.send_to_broker(output_buffer);
}

void setup() {
  Serial.begin(9600);

  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);

  wifiManager.init_WiFi_conexion();
  wifiManager.print_ip_rssi();

  while (!mqtt_manager.connect()) {
    mqtt_manager.connect();
  }

  // send to the robot it is connected succesfully
  Serial2.write(START_LAP);
  output_buffer = "\n{\n\t\"team_name\": \"els boquerunets\",\n\t\"id\": \"12\",\n\t\"action\": \"START_LAP\"\n}";
  mqtt_manager.send_to_broker(output_buffer); // Envía el contenido del archivo JSON  

  // time from ESP start
  begin_millis = millis(); 
  last_millis = millis();
}

void loop() {
    if (!mqtt_manager.connect()) {
      mqtt_manager.connect();
    }

    if (Serial2.available()) {
      char read_buffer = Serial2.read();
      input_buffer += read_buffer;

      send_json_msgs();
    }
    input_buffer = "";
 }