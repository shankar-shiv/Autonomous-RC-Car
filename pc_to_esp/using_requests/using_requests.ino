#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid = "DilipAsha_2.4GHz";
const char* password = "Aish280102";

ESP8266WebServer server(80);

const int led = 13;

/* ***** PIN declarations *****
    PIN_IN1 and PIN_IN2 controls Left motor of L298N.
    PIN_IN3 and PIN_IN4 controls Right motor of L298N.
    PIN_IN1 = 14, PIN_IN2 = 12, PIN_IN3 = 13, PIN_IN4 = 15

  It is simple to control the speed of the DC motor by generating a PWM signal to the ENA/ENB pin of L298N.
  We can do this by:
  - Connecting an ESP32's digital output pin to L298N's ENA/ENB pin.
  - Creating a PWM signal to the ENA/ENB pin by using analogWrite() function.
  - This PWM signal goes though L298N Driver and amplified current and voltage before going to to the DC motor.
  - The speed is between 0 and 255.
*/

// Right Motor A connections
int PIN_ENA = 0;
int PIN_IN1 = 14;
int PIN_IN2 = 12;

// Left Motor B connections
int PIN_ENB = 16;
int PIN_IN3 = 13;
int PIN_IN4 = 15;

void right() {
  server.send(200, "text/plain", "Right!\r\n");
  analogWrite(PIN_ENA, 160);  // control motor A
  analogWrite(PIN_ENB, 230);  // control motor B

  // Right Motor
  digitalWrite(PIN_IN1, HIGH);  // Forward motion
  digitalWrite(PIN_IN2, LOW);

  // Left Motor
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  Serial.println("RIGHT");
}

void left() {
  server.send(200, "text/plain", "Left!\r\n");
  
  analogWrite(PIN_ENA, 23 0);  // control motor A
  analogWrite(PIN_ENB, 160);  // control motor B

  // Right Motor
  digitalWrite(PIN_IN1, HIGH);  // Forward motion
  digitalWrite(PIN_IN2, LOW);

  // Left Motor
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  Serial.println("LEFT");
}

void forward() {
  server.send(200, "text/plain", "Forward!\r\n");
  
  analogWrite(PIN_ENA, 175);  // control motor A
  analogWrite(PIN_ENB, 175);  // control motor B

  digitalWrite(PIN_IN1, HIGH);  // Forward motion
  digitalWrite(PIN_IN2, LOW);

  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  Serial.println("FORWARD");
}

void backward() {
  server.send(200, "text/plain", "Backward!\r\n");
  
  analogWrite(PIN_ENA, 160);  // control motor A
  analogWrite(PIN_ENB, 160);  // control motor B

  digitalWrite(PIN_IN1, LOW);  // Backward motion
  digitalWrite(PIN_IN2, HIGH);

  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  Serial.println("BACKWARD");
}

void reset_all() {
  server.send(200, "text/plain", "Stop!\r\n");
  
  analogWrite(PIN_ENA, 0);  // control motor A
  analogWrite(PIN_ENB, 0);  // control motor B

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
  
  Serial.println("STOP");
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!\r\n");
  digitalWrite(led, 0);
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
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

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

  // Motor configuration
  // Set all the motor control pins to outputs
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  // Turn off motors - Initial state
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);

  server.on("/", handleRoot);

  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });

  server.on("/left", left);
  server.on("/right", right);
  server.on("/forward", forward);
  server.on("/backward", backward); // do not name as reverse, reset_all
  server.on("/reset_all", reset_all);
  
  server.on("/gif", []() {
    static const uint8_t gif[] PROGMEM = {
      0x47, 0x49, 0x46, 0x38, 0x37, 0x61, 0x10, 0x00, 0x10, 0x00, 0x80, 0x01,
      0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x2c, 0x00, 0x00, 0x00, 0x00,
      0x10, 0x00, 0x10, 0x00, 0x00, 0x02, 0x19, 0x8c, 0x8f, 0xa9, 0xcb, 0x9d,
      0x00, 0x5f, 0x74, 0xb4, 0x56, 0xb0, 0xb0, 0xd2, 0xf2, 0x35, 0x1e, 0x4c,
      0x0c, 0x24, 0x5a, 0xe6, 0x89, 0xa6, 0x4d, 0x01, 0x00, 0x3b
    };
    char gif_colored[sizeof(gif)];
    memcpy_P(gif_colored, gif, sizeof(gif));
    // Set the background to a random set of colors
    gif_colored[16] = millis() % 256;
    gif_colored[17] = millis() % 256;
    gif_colored[18] = millis() % 256;
    server.send(200, "image/gif", gif_colored, sizeof(gif_colored));
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  MDNS.update();
}
