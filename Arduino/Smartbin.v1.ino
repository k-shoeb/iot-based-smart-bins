/* IoT Based Smart Dustbin */
#include <SoftwareSerial.h>
#include <Servo.h>

#define trigpin1 9       // Human Presence detector
#define echopin1 8       // To open the Lid
#define trigpin2 5        // Trash Level detector  
#define echopin2 4 

SoftwareSerial Gsm(2, 3);
Servo servo;

bool msgSend = false;
long distance1, duration1;
long distance2, duration2;

void setup() {
  // setup code to run once
  Serial.begin(9600);
  delay(100);
  Gsm.begin(9600);
  servo.attach(11);
  delay(100);
  servo.write(0);
  delay(100);
  servo.detach();

  //Set up Pins
  pinMode(trigpin1, OUTPUT);
  pinMode(echopin1, INPUT);
  pinMode(echopin2, INPUT);
  pinMode(trigpin2, OUTPUT);
  delay(100);

  Serial.println("Smart Trash Bin is ready to work");
}


void loop() {
  checkUserPresence();
  measureTrashLevel();
}

void checkUserPresence() {
  digitalWrite(trigpin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin1, LOW);

  duration1 = pulseIn(echopin1, HIGH);
  distance1 = (duration1 / 2) / 29.4;

  if (distance1 < 35 ) {
    servo.attach(11);
    servo.write(180); // Turn Servo back to center position (90 degrees)
    delay(3000);
    servo.write(0);
    servo.detach();
  } 
}

void measureTrashLevel() {
  digitalWrite(trigpin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin1, LOW);

  duration2 = pulseIn(echopin2, HIGH);
  distance2 = (duration2 / 2) / 29.4;

  Serial.print("Distance:");
  Serial.println(distance2);

  if (distance2 > 15)
  {
    Serial.println("EMPTY");
  }
  else if ((distance1 > 7) && (distance1 <= 15))
  {
    Serial.println("LOW");
  }
  else{
    Serial.println("FULL");
    if (!msgSend)
    {
      send_msg();
      msgSend = true;
    }
  }
}

void send_msg(){
  delay(100);
  Gsm.println("AT+CMGF=1");                     // Sets the GSM Module in Text Mode
  delay(1000);    
  Gsm.println("AT+CMGS=\"+91xxxxxxxxxx\"\r");  // Replace x with your mobile number
  delay(1000);
  Gsm.println("Trash bin is full, please empty it"); // The SMS text you want to send
  delay(100);
  Gsm.println((char)26);                         // ASCII for CTRL+Z
  delay(1000);
}