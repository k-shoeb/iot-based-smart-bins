/* IoT-Based Smart Bin */
#include <WiFi.h>
#include "thingProperties.h"

#include <ESP32Servo.h>

// Assign unique ID to every Bin if using multiple Bins
const char* binId = "01";
const int binHeight = 25;  // Change it to your Bin height in Centimeters

Servo servo;

// Ultrasonic sensor pins
#define echopin1 14
#define trigpin1 12             // Human detector 
#define echopin2 18
#define trigpin2 19             // Trash detector
//    servopin 26
// LEDs and buzzer pins
#define G_led 25          // Green LED
#define Y_led 32          // Yellow LED for Low
#define R_led 33          // Red LED
#define buzzer 23         // Buzzer

// Variables
long distance1, duration1;
long distance2, duration2;
bool lidOpen = false;
bool binFull = false;

String binStatus;


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 
  // Defined in thingProperties.h
  initProperties();
  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Initialize servo motor
  servo.attach(26);
  servo.write(0);
  delay(100);
  servo.detach();

  // Set up pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(G_led, OUTPUT);
  pinMode(Y_led, OUTPUT);
  pinMode(R_led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  delay(100);

  Serial.println("Smart Trash Bin" + String(binId) + "is ready to work.");
}

void loop() {
  ArduinoCloud.update(); // Sync with Arduino Cloud

  checkUserPresence();

  if (lidOpen){
    measureTrashLevel();
  }
}


void checkUserPresence() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = (duration1 / 2) / 29.4;

  if (distance1 < 35 && !binFull) {
    servo.attach(26);
    servo.write(180);
    delay(3000);
    servo.write(0);
    delay(100);
    servo.detach();
    lidOpen = true;
  } 
  else if (distance1 < 35 && binFull) {
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
    lidOpen = false;
  }
}


void measureTrashLevel() {
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = (duration2 / 2) / 29.4;

  // Calculate Trash bin ration to find out Percentage
  trashLevel = (distance2 / binHeight) * 100;
  //percentage = trashLevel

  if (trashLevel < 50) {
    binStatus = "Empty";
    binFull = false;
    digitalWrite(G_led, HIGH);
    digitalWrite(Y_led, LOW);
    digitalWrite(R_led, LOW);
  } 
  else if (trashLevel < 90) { 
    binStatus = "Low";
    binFull = false;
    digitalWrite(G_led, LOW);
    digitalWrite(Y_led, HIGH);
    digitalWrite(R_led, LOW);
  } 
  else {
    message = "Trash Can " + String(binId) + " is Full";
    binStatus = "Full";
    binFull = true;
    digitalWrite(G_led, LOW);
    digitalWrite(Y_led, LOW);
    digitalWrite(R_led, HIGH);
  }

  Serial.print("Trash Bin status: ");
  Serial.println(binStatus);
}
