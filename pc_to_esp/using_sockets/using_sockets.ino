/*
***** Author : Shiva Shankar
***** Remember to exclude confidential details !!!!
***** Cite the author if you are referencing this code.
*/

// serial print variable type
void types(String a)
{
  Serial.println("it's a String");
}
void types(int a)
{
  Serial.println("it's an int");
}
void types(char *a)
{
  Serial.println("it's a char*");
}
void types(float a)
{
  Serial.println("it's a float");
}
void types(bool a)
{
  Serial.println("it's a bool");
}

#include "ESP8266WiFi.h"

const char *ssid = "";
const char *password = "";

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
int time_delay = 50;

WiFiServer wifiServer(12345);

// ***** Function prototypes *****
void directionControl();
void send_command(int command, int time_delay);
void right(int time_delay);
void left(int time_delay);
void forward(int time_delay);
void reverse(int time_delay);

void setup()
{

  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting..");
  }

  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
  wifiServer.begin();

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
}

void loop()
{
  WiFiClient client = wifiServer.available();
  if (client)
  {
    while (client.connected())
    {
      while (client.available() > 0)
      {
        char c = client.read();
        Serial.println(c);
        send_command(c, time_delay);
      }
    }
    client.stop();
    Serial.println("Client disconnected");
  }
  else
  {
    reset_all();
  }
}

void right(int time_delay)
{
  analogWrite(PIN_ENA, 150); // control motor A
  analogWrite(PIN_ENB, 180); // control motor B

  // Right Motor
  digitalWrite(PIN_IN1, HIGH); // Forward motion
  digitalWrite(PIN_IN2, LOW);

  // Left Motor
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  Serial.println("RIGHT");
}

void left(int time_delay)
{
  analogWrite(PIN_ENA, 180); // control motor A
  analogWrite(PIN_ENB, 150); // control motor B

  // Right Motor
  digitalWrite(PIN_IN1, HIGH); // Forward motion
  digitalWrite(PIN_IN2, LOW);

  // Left Motor
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  Serial.println("RIGHT");
}

void forward(int time_delay)
{
  analogWrite(PIN_ENA, 150); // control motor A
  analogWrite(PIN_ENB, 150); // control motor B

  digitalWrite(PIN_IN1, HIGH); // Forward motion
  digitalWrite(PIN_IN2, LOW);

  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  Serial.println("FORWARD");
}

void reverse(int time_delay)
{
  analogWrite(PIN_ENA, 150); // control motor A
  analogWrite(PIN_ENB, 150); // control motor B

  digitalWrite(PIN_IN1, LOW); // Backward motion
  digitalWrite(PIN_IN2, HIGH);

  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  Serial.println("BACKWARD");
}

void reset_all()
{
  analogWrite(PIN_ENA, 0); // control motor A
  analogWrite(PIN_ENB, 0); // control motor B

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

void send_command(int command, int time_delay)
{
  switch (command)
  {

  // reset command
  case '5':
    reset_all();
    break;

  // single command
  case '3':
    forward(time_delay);
    break;
  case '4':
    reverse(time_delay);
    break;
  case '2':
    right(time_delay);
    break;
  case '1':
    left(time_delay);
    break;
  default:
    reset_all();
  }
}

// This function lets you control spinning direction of motors
void directionControl()
{
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  //  analogWrite(enA, 255);
  //  analogWrite(enB, 255);

  // Turn on motor A & B
  digitalWrite(PIN_IN1, HIGH);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, HIGH);
  digitalWrite(PIN_IN4, LOW);
  delay(5000);

  // Now change motor directions
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, HIGH);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, HIGH);
  delay(5000);

  // Turn off motors
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}
