//using arduino and nodemcu


#include <Servo.h>
#include <SoftwareSerial.h>

// Define the ultrasonic sensor pins
const int trigPin1 = 2;
const int echoPin1 = 3;
const int trigPin2 = 4;
const int echoPin2 = 5;

// Define the servo motor pin
const int servoPin = 9;

// Define the serial communication pins
SoftwareSerial serial(10, 11);

// Define the distance thresholds
const int distanceThreshold1 = 10;
const int distanceThreshold2 = 20;

// Define the state of the bin
int binState = 0;

// Define the servo motor object
Servo servo;

void setup() {
  // Set up the ultrasonic sensor pins
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  // Set up the servo motor pin
  servo.attach(servoPin);

  // Set up the serial communication
  Serial.begin(9600);
  serial.begin(9600);
}

void loop() {
  // Measure the distance from the top of the bin to the lid
  long duration1, duration2;
  float distance1, distance2;
  
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  // Determine the state of the bin based on the distances
  if (distance1 < distanceThreshold1 || distance2 < distanceThreshold2) {
    binState = 1;
  } else {
    binState = 0;
  }

  // Send a message to the NodeMCU indicating the bin state
  if (binState == 1) {
    serial.write("Bin Full");
  } else {
    serial.write("Bin Empty");
  }

  // If the bin is full, open the lid with the servo motor
  if (binState == 1) {
    servo.write(180);
  } else {
    servo.write(0);
  }

  // Wait for a moment before taking another measurement
  delay(500);
}
