#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

#include "PageIndex.h"; //--> Include the contents of the User Interface Web page from website, stored in the same folder as the .ino file  

#define ServoPort D5   //--> Defining Servo Port

#ifndef STASSID
#define STASSID "Baeta"
#define STAPSK "senha123456789"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

Servo myservo;

ESP8266WebServer server(80);

String SERstate1 = "OFF";

//This routine is executed when you open NodeMCU ESP8266 IP Address in browser

void handleRoot() {
  String main = MAIN_page;
  server.send(200, "text/html", main);
}

//Procedure for handling servo control

void handleServo() {
  String servoRespPosition = server.arg("servoPOS");
  int servoPosition = servoRespPosition.toInt();
  myservo.write(servoPosition);   //--> Move the servo motor according to the POS value
  delay(15);
  Serial.print("Servo Angle:");
  Serial.println(servoPosition);
  server.send(200, "text/plane", "");
}

void controlServo() {
  String t_state = server.arg("SERstate1");
  Serial.println(t_state);
  int pos;
  if (t_state == "1") {
    SERstate1 = pos;

    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
      myservo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
    }
 
  }
  else if (t_state == "2") {
    SERstate1 = pos;
    myservo.write(0);
  }
  else if (t_state == "3") {
    SERstate1 = pos;
    myservo.write(90);
  }
  else if (t_state == "4") {
    SERstate1 = pos;
    myservo.write(180);
  }
  else {
    SERstate1 = "OFF";
  }
  server.send(200, "text/plane", SERstate1);
}

void statusSER() {
  server.send(200, "text/plane", SERstate1);
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  myservo.attach(ServoPort, 500, 2400);
  
  Serial.print("Trying to connect on: ");
  Serial.print(ssid);
  Serial.print("...");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(".");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  //Initialize Webserver
  server.on("/", handleRoot);
  server.on("/setPOS", handleServo);
  server.on("/setSER1", controlServo);
  server.on("/readSER1", statusSER);
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}