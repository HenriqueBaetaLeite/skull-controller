#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "***"
#define STAPSK "***"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

const char index_html[] PROGMEM = R"rawliteral(
 <!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <link
      rel="stylesheet"
      href="https://cdn.jsdelivr.net/npm/bulma@0.9.4/css/bulma.min.css"
    />
    <style>
     
    </style>
  </head>

  <body>
    <h1>The Skull controller</h1>
    <h2>Escolha um range</h2>
    <br />
    <form method="post" enctype="text/plain" action="/baetapost" id="myform">
      <input
        class="slot"
        type="range"
        name="the-range"
        min="0"
        max="40"
        value=""
        id="mouth-range"
      />

      <input
        id="toggle"
        class="toggle"
        type="checkbox"
        role="switch"
        name="toggle"
        value="on"
      />
      <label for="toggle" class="slot">
        <span id="led-on-off">Off</span>
      </label>
      <div class="curtain"></div>

      <input class="slot" type="submit" value="Submit" />
    </form>
  </body>

  <script>
    console.log("HTML and JS on Arduino!");

    const mouthRange = document.getElementById("my-range");

    const ledOnOff = document.querySelector("#led-on-off");

    document.getElementById("toggle").addEventListener("change", function () {
      this.setAttribute("aria-checked", this.checked);
      this.checked ? (ledOnOff.innerHTML = "On") : (ledOnOff.innerHTML = "Off");
    });
  </script>
</html>

  
 )rawliteral";

void handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!\r\n");
}

void handlePostPlain() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    String valueFromClientPost = server.arg("plain");
    server.send(200, "text/html", index_html);
    Serial.print("Aqui o valor gerado pelo client:");
    Serial.println(valueFromClientPost);
    String valueMouth = valueFromClientPost.substring(9, 11);
    Serial.print("somente o range: ");
    Serial.println(valueMouth);
    int valueMouthMapped = map(valueMouth.toInt(), 0, 40, 0, 180);
    Serial.print("valueMapped: ");
    Serial.println(valueMouthMapped);
//    myServo.write(valueMouthMapped);
  }
}

void handleBaeta() {
  server.send(200, "text/html", index_html);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/baeta", handleBaeta);

  server.on("/baetapost", handlePostPlain);

  server.on("/amor", []() {
    server.send(200, "text/plain", "Thatiane, eu te amo muito! Obrigado por tudo!");
  });

  server.onNotFound(handleNotFound);

  /////////////////////////////////////////////////////////
  // Hook examples

  server.addHook([](const String & method, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction contentType) {
    (void)method;       // GET, PUT, ...
    (void)url;          // example: /root/myfile.html
    (void)client;       // the webserver tcp client connection
    (void)contentType;  // contentType(".html") => "text/html"
    Serial.printf("A useless web hook has passed\n");
    Serial.printf("(this hook is in 0x%08x area (401x=IRAM 402x=FLASH))\n", esp_get_program_counter());
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  server.addHook([](const String&, const String & url, WiFiClient*, ESP8266WebServer::ContentTypeFunction) {
    if (url.startsWith("/fail")) {
      Serial.printf("An always failing web hook has been triggered\n");
      return ESP8266WebServer::CLIENT_MUST_STOP;
    }
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  server.addHook([](const String&, const String & url, WiFiClient * client, ESP8266WebServer::ContentTypeFunction) {
    if (url.startsWith("/dump")) {
      Serial.printf("The dumper web hook is on the run\n");

      // Here the request is not interpreted, so we cannot for sure
      // swallow the exact amount matching the full request+content,
      // hence the tcp connection cannot be handled anymore by the
      // webserver.
#ifdef STREAMSEND_API
      // we are lucky
      client->sendAll(Serial, 500);
#else
      auto last = millis();
      while ((millis() - last) < 500) {
        char buf[32];
        size_t len = client->read((uint8_t*)buf, sizeof(buf));
        if (len > 0) {
          Serial.printf("(<%d> chars)", (int)len);
          Serial.write(buf, len);
          last = millis();
        }
      }
#endif
      // Two choices: return MUST STOP and webserver will close it
      //                       (we already have the example with '/fail' hook)
      // or                  IS GIVEN and webserver will forget it
      // trying with IS GIVEN and storing it on a dumb WiFiClient.
      // check the client connection: it should not immediately be closed
      // (make another '/dump' one to close the first)
      Serial.printf("\nTelling server to forget this connection\n");
      static WiFiClient forgetme = *client;  // stop previous one if present and transfer client refcounter
      return ESP8266WebServer::CLIENT_IS_GIVEN;
    }
    return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
  });

  // Hook examples
  /////////////////////////////////////////////////////////

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
