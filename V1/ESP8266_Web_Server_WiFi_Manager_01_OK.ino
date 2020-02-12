#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#define USESERIAL
#define USELED

#include "Config.h"
#include "FirmwareReset.h"
#include "AdminPage.h"

ESP8266WebServer server(80); //Server on port 80

// declaration of a variable
String v1, v2, v3, i, p, ah, kwh, t;

//Redirect to our html web page
void handleRoot() {
  server.sendHeader("Location", "/index.html", true);
  server.send(302, "text/plain", "");
}

void handleWebRequests() {
  if (loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}

void PackVoltageV1() {
  server.send(200, "text/plain", v1);
}
void PackVoltageV2() {
  server.send(200, "text/plain", v2);
}
void PackVoltageV3() {
  server.send(200, "text/plain", v3);
}
void Power() {
  server.send(200, "text/plain", i);
}
void Current() {
  server.send(200, "text/plain", p);
}
void AmpereHours() {
  server.send(200, "text/plain", ah);
}
void KilowattHours() {
  server.send(200, "text/plain", kwh);
}
void Temperature() {
  server.send(200, "text/plain", t);
}
void SaveConfig() {
  server.send(200, "text/plain", "No Yet Implemented");
}

void setup(void) {
  Serial.begin(19200);

  Serial.println("MCU Rebooted");

  //Initialize File System
  SPIFFS.begin();

  InitConfig();

  //Start the wifi with the required username and password
  WiFi.mode(WIFI_AP);

  LoadConfig();
  PrintConfig();

  //Check to see if the flag is still set from the previous boot
  if (checkResetFlag()) {
    //Do the firmware reset here
    Serial.printf("Reset Firmware\n");

    //Set the ssid to default value and turn off the password
    WiFi.softAP("APConfig", "", 1, false, 1);
  }
  else {
    WiFi.softAP(config.ssid, config.pass, 1, false, 1);
  }

  //Initialize Webserver
  server.on("/", handleRoot);
  server.on("/admin", std::bind(serveAdmin, &server));
  server.on("/PackVoltageV1", PackVoltageV1);
  server.on("/PackVoltageV2", PackVoltageV2);
  server.on("/PackVoltageV3", PackVoltageV3);
  server.on("/Current", Current);
  server.on("/Power", Power);
  server.on("/AmpereHours", AmpereHours);
  server.on("/KilowattHours", KilowattHours);
  server.on("/Temperature", Temperature);
  server.on("/SaveConfig", SaveConfig);
  server.onNotFound(handleWebRequests);
  server.begin();
  Serial.println("HTTP server started");
}

bool loadFromSpiffs(String path) {
  String dataType = "text/plain";
  if (path.endsWith("/")) path += "index.htm";
  if (path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html")) dataType = "text/html";
  else if (path.endsWith(".htm")) dataType = "text/html";
  else if (path.endsWith(".css")) dataType = "text/css";
  else if (path.endsWith(".js")) dataType = "application/javascript";
  else if (path.endsWith(".png")) dataType = "image/png";
  else if (path.endsWith(".gif")) dataType = "image/gif";
  else if (path.endsWith(".jpg")) dataType = "image/jpeg";
  else if (path.endsWith(".ico")) dataType = "image/x-icon";
  else if (path.endsWith(".xml")) dataType = "text/xml";
  else if (path.endsWith(".pdf")) dataType = "application/pdf";
  else if (path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {
  }
  dataFile.close();
  return true;
}

void loop(void) {
  //Handle client requests
  server.handleClient();

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
