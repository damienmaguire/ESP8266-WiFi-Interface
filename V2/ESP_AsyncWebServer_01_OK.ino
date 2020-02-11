// Import required libraries
#ifdef ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#else
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#endif
#include <Wire.h>

// declaration of a variable
String v1, v2, v3, i, p, ah, kwh, t;

// Replace with your network credentials
const char* ssid = "NEWSSID001";
const char* password = "1234567890";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String PackVoltageV1() {
  return v1;
}
String PackVoltageV2() {
  return v2;
}
String PackVoltageV3() {
  return v3;
}
String Power() {
  return i;
}
String Current() {
  return p;
}
String AmpereHours() {
  return ah;
}
String KilowattHours() {
  return kwh;
}
String Temperature() {
  return t;
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(19200);

  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  //Start WiFi AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  // Route for root / web pages
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html");
  });
  server.on("/highcharts.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/highcharts.js", "text/javascript");
  });
  server.on("/highcharts-more.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/highcharts-more.js", "text/javascript");
  });
  server.on("/solid-gauge.js", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/solid-gauge.js", "text/javascript");
  });
  server.on("/PackVoltageV1", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", PackVoltageV1().c_str());
  });
  server.on("/PackVoltageV2", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", PackVoltageV2().c_str());
  });
  server.on("/PackVoltageV3", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", PackVoltageV3().c_str());
  });
  server.on("/Current", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", Current().c_str());
  });
  server.on("/Power", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", Power().c_str());
  });
  server.on("/AmpereHours", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", AmpereHours().c_str());
  });
  server.on("/KilowattHours", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", KilowattHours().c_str());
  });
  server.on("/Temperature", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/plain", Temperature().c_str());
  });

  // Start Async Web server
  server.begin();
}

void loop() {

  String justRates = Serial.readStringUntil('\n');

  //split justrate variable from begining to first "," charactor
  int x1stSpaceIndex = justRates.indexOf(",");
  int x2ndSpaceIndex = justRates.indexOf(",", x1stSpaceIndex + 1);
  int x3rdSpaceIndex = justRates.indexOf(",", x2ndSpaceIndex + 1);
  int x4thSpaceIndex = justRates.indexOf(",", x3rdSpaceIndex + 1);
  int x5thSpaceIndex = justRates.indexOf(",", x4thSpaceIndex + 1);
  int x6thSpaceIndex = justRates.indexOf(",", x5thSpaceIndex + 1);
  int x7thSpaceIndex = justRates.indexOf(",", x6thSpaceIndex + 1);

  v1 = (justRates.substring(1, x1stSpaceIndex)).toInt();
  v2 = (justRates.substring(x1stSpaceIndex + 2, x2ndSpaceIndex));
  v3 = (justRates.substring(x2ndSpaceIndex + 2, x3rdSpaceIndex));
  i = (justRates.substring(x3rdSpaceIndex + 2, x4thSpaceIndex));
  p = (justRates.substring(x4thSpaceIndex + 2, x5thSpaceIndex));
  ah = (justRates.substring(x5thSpaceIndex + 3, x6thSpaceIndex));
  kwh = (justRates.substring(x6thSpaceIndex + 4, x7thSpaceIndex));
  t = (justRates.substring(x7thSpaceIndex + 2)).toInt();

  Serial.println();
  Serial.print("Serial input - ");
  Serial.println(justRates);
  Serial.print("Split values - ");
  Serial.println("v1 : " + String(v1) + " v2 : " + String(v2) + " v3 : " + (v3) + " i : " + String(i) + " p : " + String(p) + " ah : " + String(ah) + " kwh " + String(kwh) + " t : " + String(t));
}
