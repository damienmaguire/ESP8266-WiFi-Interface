#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>

#include "Config.h"
#include "FirmwareReset.h"
#include "AdminPage.h"

#define USESERIAL

ESP8266WebServer server(80); //Server on port 80

const int MAX_VALUES = 10;
String keys[MAX_VALUES];
String values[MAX_VALUES];

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

void Value() {
  String key = server.arg(0);
  for(int i; i < MAX_VALUES; i++) {
    if (keys[i] == key) {
        server.send(200, "text/plain", values[i]);
    }
  }

  server.send(200, "text/plain", "Prefix not in input stream");

}

void SaveChartConfig() {
   String frequencies[MAX_VALUES];
   String mins[MAX_VALUES];
   String maxs[MAX_VALUES];
   String titles[MAX_VALUES];
   String prefixes[MAX_VALUES];
   String sufixes[MAX_VALUES];

   uint8_t highestIndex = 0;
   for (uint8_t i = 0; i < server.args(); i++) {
    uint8_t index = server.argName(i).substring(server.argName(i).indexOf('[') + 1, server.argName(i).indexOf(']')).toInt();
    if (index > highestIndex) {
      highestIndex = index;
    }
    String key = server.argName(i).substring(0, server.argName(i).indexOf('['));
    if (key == "min") {
        mins[index] = server.arg(i);
    } else if (key == "max") {
        maxs[index] = server.arg(i);
    } else if (key == "frequency") {
        frequencies[index] = server.arg(i);
    } else if (key == "title") {
        titles[index] = server.arg(i);
    } else if (key == "suffix") {
        sufixes[index] = server.arg(i);
    } else if (key == "prefix") {
        prefixes[index] = server.arg(i);
    }
   }

   String json = "[";

   for (uint8_t i = 0; i <= highestIndex; i++) {
    if (i > 0) {
      json += ",";
    }
    json += "{";
    json += "\"min\": " + mins[i] + ",";
    json += "\"max\": " + maxs[i] + ",";
    json += "\"updateFrequency\": " + frequencies[i] + ",";
    json += "\"title\": \"" + titles[i] + "\",";
    json += "\"prefix\": \"" + prefixes[i] + "\",";
    json += "\"valueSuffix\": \"" + sufixes[i] + "\",";
    json += "\"initialValue\": 0";


    json += "}";

   }

   json += "]";
   Serial.println(json);
   SPIFFS.remove("/data.json");
   File file = SPIFFS.open("/data.json", "w");
   file.print(json);
   file.close();
   server.sendHeader("Location", "/index.html", true);
   server.send(302, "text/plain", "");
}

void ChartConfig() {
  File file = SPIFFS.open("/data.json", "r");
  if (file && file.size()) {
    String content;
    int i;
    for(i=0;i<file.size();i++) //Read upto complete file size
     {
       content =  content + (char)file.read();
     }

     content = "var chartConfig = " + content;
     file.close();  //Close file
     server.send(200, "text/plain", content);

  } else {

       server.send(200, "text/plain", "No File");

  }

}


  void SetupAP() {
    Serial.printf("Starting: '%s'\n", config.ssid);
    //Start the wifi with the required username and password
    WiFi.mode(WIFI_AP);

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
  }

  void JoinWifi() {
    // Connect to WiFi
    Serial.printf("Joining: '%s'\n", config.ssid);
    unsigned int tries = 0;

    WiFi.mode(WIFI_STA);
    WiFi.begin(config.ssid, config.pass);
    while (WiFi.status() != WL_CONNECTED && tries < 20) {
      tries++;
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi join failed");
      SetupAP();
    } else {
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println(WiFi.localIP());
    }

  }

void setup(void) {
  Serial.begin(115200);

  Serial.println("MCU Rebooted");

  //Initialize File System
  SPIFFS.begin();

  InitConfig();
  LoadConfig();
  PrintConfig();

  String wifimode = String(config.wifimode);
  wifimode.trim();
  if (wifimode == "JOIN") {
    JoinWifi();
  } else {
    SetupAP();
  }
  //Initialize Webserver
  server.on("/", handleRoot);
  server.on("/admin", std::bind(serveAdmin, &server));
  server.on("/data", Value);
  server.on("/SaveConfig", SaveChartConfig);
  server.on("/ChartConfig", ChartConfig);

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
  String data = Serial.readStringUntil('\n');

  if (data != "") {
    String part;
    int terminate = false;
    int index = 0;
    do {
      part = data.substring(0, data.indexOf(','));
      data = data.substring(data.indexOf(',') + 1);
      terminate  = part.indexOf('*');
      if (terminate != -1) {
        part = part.substring(0, part.length() -1);
      }
      String key = part.substring(0, part.indexOf(':'));
      String value = part.substring(part.indexOf(':') + 1);
      Serial.println("adding: " + key + " " + value);

      keys[index] = key;
      values[index] = value;

      index++;

    } while (terminate == -1 && index < MAX_VALUES);
  }

  delay(1);

}
