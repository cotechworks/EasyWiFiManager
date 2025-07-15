#ifdef ARDUINO_ESP8266_WEMOS_D1MINI
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>
#endif

// Web page (Port 80)
ESP8266WebServer server(8080);
static const char* hostAddress = "sample"

// http://sample.local:8080/cmd
static const char* cpResponse200 = "<HTML>"
                                   "<BODY style='background-color:#ffffde;font-family:sans-serif;font-size:40px;'>"
                                   "CONTROL WEB<br/><br/>"
                                   "<a href=/cmd?CMD=dummy>DUMMY</a><br/>"
                                   "</BODY></HTML>\r\n";

WiFiManager wifiManager;

void setup() {
  Serial.begin(115200);
  Serial.println("");

  //WiFiManager
  WiFiManager wifiManager;
  wifiManager.setBreakAfterConfig(true);
  if (!wifiManager.autoConnect("AutoConnectAP", "password")) {
    Serial.println("failed to connect, we should reset as see if it connects");
    delay(3000);
    ESP.reset();
    delay(5000);
  }

  Serial.println("");
  Serial.println("WiFi Connected.");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");
  Serial.println("");

  // Set up mDNS responder:
  if (!MDNS.begin("sample")) {  // sample.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  Serial.println("");

  // WebServer
  server.on("/cmd", WebCommand);
  server.begin();

  // Add service to MDNS-SD
  MDNS.addService("http", "tcp", 8080);
}

// http://sample.local:8080/cmd?CMD=XXXXX で操作
void WebCommand() {
  String cmd = server.arg("CMD");
  if (cmd == "dummy") {
    // hogehoge...
  } else if (cmd == "reset") {  //　resetコマンドでWiFi再設定
    WiFiManager wifiManager;
    delay(1000);
    Serial.println("");
    Serial.println("WiFi Reset !");
    Serial.println("");
    wifiManager.resetSettings();
    delay(3000);
    ESP.reset();
  }
  server.send(200, "text/html", cpResponse200);
}

void loop() {
  MDNS.update();
  server.handleClient();
}