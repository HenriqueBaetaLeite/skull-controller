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

void functionOpenClose(servoPosition: int)
{
  for (servoPosition = 0; servoPosition <= 180; servoPosition += 1)
  {
    myservo.write(servoPosition);
    delay(30);
  }
  for (servoPosition = 180; servoPosition >= 0; servoPosition -= 1)
  {
    myservo.write(servoPosition);
    delay(30);
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
  int servoPosition;
  if (chosenFunction == "1")
  {
    servoState = servoPosition;
    functionOpenClose(servoPosition);
  }
  else if (chosenFunction == "2")
  {
    servoState = servoPosition;
    myservo.write(0);
  }
  else if (chosenFunction == "3")
  {
    servoState = servoPosition;
    myservo.write(90);
  }
  else if (chosenFunction == "4")
  {
    servoState = servoPosition;
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