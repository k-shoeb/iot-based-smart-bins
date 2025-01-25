/* IoT BAsed Smart dustbin */
#include <Servo.h>              // Servo Library
#include <TinyGPSPlus.h>        // GPS Library
#include <SoftwareSerial.h>     // SoftwareSerial library

Servo servo;
TinyGPSPlus gps;                 // TinyGPS++ object for parsing GPS data
SoftwareSerial gsm(3, 2);        // SoftwareSerial object for the GSM module
SoftwareSerial gpsSerial(4, 5);  // (rX, tX) for GPS module

int binHeight = 25;   // Change it to your Bin height in Centimeters

#define echopin1 6
#define trigpin1 7             // Human detector 
#define echopin2 8
#define trigpin2 9             // Trash detector
//    servopin 10
#define G_led 11                // Green LED for Empty
#define R_led 12               // red LED for Full
//#define Y_led                // Yellow LED for Low
#define buzzer 13             // buzzer pin 

// Variables
float latitude, longitude;
long distance1, duration1;
long distance2, duration2;
bool lidOpen = false;
bool binFull = false;

float trashLevel;
String binStatus;

void setup() {
  // Setup code to run only once
  gpsSerial.begin(4800);         // Initialize the SoftwareSerial communication with the GPS module
  Serial.begin(9600);           // Initialize the serial communication with the computer
  gsm.begin(9600);             // Initialize the SoftwareSerial communication with the GSM module
  delay(100);

  // Check Servo Motor
  servo.attach(10);          
  delay(100);
  servo.write(0);
  delay(100);
  servo.detach();

  // Set the PIN Mode
  pinMode(trigpin1, OUTPUT);   
  pinMode(trigpin2, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(echopin2, INPUT);

  pinMode(G_led, OUTPUT);
  pinMode(R_led, OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Check GPS Module Connection 
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      if (gps.location.isValid()){ 
        latitude = gps.location.lat();
        longitude = gps.location.lng(); 
        Serial.print(latitude, 6);
        Serial.print(F(","));
        Serial.print(longitude, 6);
      }
      else
      {
        Serial.print(F("INVALID"));
      }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected"));
    while(true);
  }

  delay(1000);  
  Serial.println("Smart Trash Bin is ready to work");
}

void loop() {
  checkUserPresence();
  if (lidOpen){
    measureTrashLevel();
  }
}

void checkUserPresence() {
  digitalWrite(trigpin1, LOW);     
  delayMicroseconds(2);
  digitalWrite(trigpin1, HIGH);            //Transmit UltraSonic Wave
  delayMicroseconds(10);
  digitalWrite(trigpin1, LOW);

  duration1 = pulseIn(echopin1, HIGH);   //Receive the Reflected wave
  distance1 = (duration1 / 2) / 29.4;   //Convert the delay time into distance

  if (distance1 < 35 && !binFull) {
    servo.attach(10);    
    servo.write(180);     // Turn Servo to open the lid
    delay(3000);         // Wait for 3 seconds
    servo.write(0);     // turn Servo back to close the lid
    delay(100);
    servo.detach();
    lidOpen = true;
  }
  else if (distance1 < 35 && binFull) {
      digitalWrite(buzzer, HIGH);    // Turn on the buzzer
      delay(1000);                  // Beep for 1 second
      digitalWrite(buzzer, LOW);   // Turn off the buzzer
      lidOpen = false;
  } 
}
  
void measureTrashLevel() {
  digitalWrite(trigpin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin2, HIGH);              //Transmit UltraSonic Wave
  delayMicroseconds(10);
  digitalWrite(trigpin2, LOW);

  duration2 = pulseIn(echopin2, HIGH);      //Receive the Reflected wave
  distance2 = (duration2 / 58.8);          //Convert the delay time into distance
  Serial.println(distance2);

  // convert into percentage
  trashLevel = (distance2 / binHeight) * 100;

  if (trashLevel < 50) {
    binStatus = "Empty";
    binFull = false;
    digitalWrite(G_led, HIGH);     // Turn ON the Green LED
    //digitalWrite(Y_led, LOW); 
    digitalWrite(R_led, LOW);
  } 
  else if (trashLevel < 80) { 
    binStatus = "Low";
    binFull = false;
    digitalWrite(G_led, LOW);
    //digitalWrite(Y_led, HIGH);     // Turn ON the Yellow LED
    digitalWrite(R_led, LOW);
  }
  else if (trashLevel < 100) { 
    binStatus = "Full";
    binFull = true;
    digitalWrite(G_led, LOW);
    //digitalWrite(Y_led, LOW);
    digitalWrite(R_led, HIGH);     // Turn ON the Red LED 

    send_msg();
  }
  else {
    Serial.println("Ultra Sonic Sensor is not connected");
    digitalWrite(G_led, LOW);
    //digitalWrite(Y_led, LOW);
    digitalWrite(R_led, LOW);
  }

  Serial.print("Trash Bin status: ");
  Serial.println(binStatus);
}

// function for sending MESSAGE
void send_msg(){

  delay(50);
/*
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      if (gps.location.isValid()){ 
        latitude = gps.location.lat();
        longitude = gps.location.lng();
*/
  Serial.println("DUSTBIN AT THE GIVEN LOCATION IS FULL. PLEASE COLLECT THE WASTE.");
  delay(50);
  Serial.println("https://maps.google.com/?q=<"+ String(latitude, 6) +">,<" + String(longitude, 6) + ">");
  delay(50);

  gsm.println("AT+CMGF=1");                      //Sets the GSM Module in Text Mode
  delay(100);  
  gsm.println("AT+CMGS=\"+919372xxxxxx\"\r");   // Mobile number on which The SMS is to send
  delay(100);
  gsm.println("DUSTBIN AT THE GIVEN LOCATION IS FULL. PLEASE COLLECT THE WASTE.\n");   // The SMS text 
  delay(100);
  gsm.println("https://maps.google.com/?q=<"+ String(latitude, 6) +">,<" + String(longitude, 6) + ">");  //Location
  delay(100);
  gsm.println((char)26);       // ASCII for CTRL+Z 
  delay(1000);
}