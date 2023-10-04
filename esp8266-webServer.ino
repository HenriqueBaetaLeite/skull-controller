#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

#include "PageIndex.h"

#define ServoPort D5

#ifndef STASSID
#define STASSID "Baeta"
#define STAPSK "senha123456789"
#endif

const char *ssid = STASSID;
const char *password = STAPSK;

Servo myservo;

ESP8266WebServer server(80);

void openClose(int delayTime = 30)
{
  for (int servoPosition = 0; servoPosition <= 180; servoPosition += 1)
  {
    myservo.write(servoPosition);
    delay(delayTime);
  }
  for (int servoPosition = 180; servoPosition >= 0; servoPosition -= 1)
  {
    myservo.write(servoPosition);
    delay(delayTime);
  }
}

void skullLaughing()
{
  for (int servoPosition = 50; servoPosition <= 130; servoPosition += 1)
  {
    myservo.write(servoPosition);
    delay(5);
  }
  for (int servoPosition = 130; servoPosition >= 50; servoPosition -= 1)
  {
    myservo.write(servoPosition);
    delay(5);
  }
}

void handleRoot()
{
  String main = MAIN_page;
  server.send(200, "text/html", main);
}

void handleServo()
{
  String servoRangePosition = server.arg("servoRange");
  int servoPosition = servoRangePosition.toInt();
  myservo.write(servoPosition);
  delay(15);
  server.send(200, "text/plain", "");
}

void controlServo()
{
  String chosenFunction = server.arg("setFunction");
  Serial.println(chosenFunction);
  if (chosenFunction == "1")
  {
    openClose();
  }
  else if (chosenFunction == "2")
  {
    myservo.write(0);
  }
  else if (chosenFunction == "3")
  {
    myservo.write(90);
  }
  else if (chosenFunction == "4")
  {
    myservo.write(180);
  }
  else if (chosenFunction == "5")
  {
    int superSlow = 100;
    openClose(superSlow);
  }
  server.send(200, "text/plain", "");
}

void setup()
{
  Serial.begin(115200);
  delay(500);

  myservo.attach(ServoPort, 500, 2400);

  Serial.print("Trying to connect on: ");
  Serial.print(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

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