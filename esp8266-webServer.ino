#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

#include "index.h"

#define ServoPort D5

#ifndef STASSID
#define STASSID "Baeta"
#define STAPSK "senha123456789"
#endif

int opened = 60;
int closed = 0;

const char *ssid = STASSID;
const char *password = STAPSK;

Servo myservo;

ESP8266WebServer server(80);

int getServoPosition()
{
  return myservo.read();
}

void setMouthClosed()
{
  int position = getServoPosition();
  int delayTime = 60;
  for (position; position >= 0; position -= 1)
  {
    myservo.write(position);
    delay(delayTime);
  }
}

void openClose(int delayTime = 30)
{
  setMouthClosed();
  for (int servoPosition = 0; servoPosition <= 50; servoPosition += 1)
  {
    myservo.write(servoPosition);
    delay(delayTime);
  }
  for (int servoPosition = 50; servoPosition >= 0; servoPosition -= 1)
  {
    myservo.write(servoPosition);
    delay(delayTime);
  }
}

void skullTalking()
{
  setMouthClosed();
  for (int index = 0; index < 5; index += 1)
  {

    for (int servoPosition = 10; servoPosition <= 50; servoPosition += 1)
    {
      myservo.write(servoPosition);
      delay(1);
    }
    for (int servoPosition = 50; servoPosition >= 25; servoPosition -= 1)
    {
      myservo.write(servoPosition);
      delay(1);
    }
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
    myservo.write(closed);
  }
  else if (chosenFunction == "3")
  {
    skullTalking();
  }
  else if (chosenFunction == "4")
  {
    myservo.write(opened);
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