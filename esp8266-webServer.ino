#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

#include "PageIndex.h";

#define ServoPort D5

#ifndef STASSID
#define STASSID "Baeta"
#define STAPSK "senha123456789"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

Servo myservo;

ESP8266WebServer server(80);

String servoState = "OFF";

// This routine is executed when you open NodeMCU ESP8266 IP Address in browser

void handleRoot()
{
  String main = MAIN_page;
  server.send(200, "text/html", main);
}

// Procedure for handling servo control

void handleServo()
{
  String servoRespPosition = server.arg("servoRange");
  int servoPosition = servoRespPosition.toInt();
  myservo.write(servoPosition);
  delay(15);
  Serial.print("Servo Angle:");
  Serial.println(servoPosition);
  server.send(200, "text/plain", "");
}

void controlServo()
{
  String t_state = server.arg("servoState");
  Serial.println(t_state);
  int pos;
  if (t_state == "1")
  {
    servoState = pos;

    for (pos = 0; pos <= 180; pos += 1)
    {
      myservo.write(pos);
      delay(15);
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {
      myservo.write(pos);
      delay(15);
    }
  }
  else if (t_state == "2")
  {
    servoState = pos;
    myservo.write(0);
  }
  else if (t_state == "3")
  {
    servoState = pos;
    myservo.write(90);
  }
  else if (t_state == "4")
  {
    servoState = pos;
    myservo.write(180);
  }
  else
  {
    servoState = "OFF";
  }
  server.send(200, "text/plain", servoState);
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  myservo.attach(ServoPort, 500, 2400);

  Serial.print("Trying to connect on: ");
  Serial.print(ssid);
  Serial.print("...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266"))
  {
    Serial.println("MDNS responder started");
  }

  // Initialize Webserver
  server.on("/", handleRoot);
  server.on("/setRange", handleServo);
  server.on("/setServoFunction", controlServo);
  server.begin();
  Serial.println("HTTP server started");
}

void loop()
{
  server.handleClient();
}