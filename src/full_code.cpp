#include <Servo.h>
#include <Wire.h>
#include <VL53L0X.h>

const int ENA = 3;
const int IN1 = 4;
const int IN2 = 5;

const int servoPin = 9;
Servo steeringServo;

// LED 
const int ledPin = 10;

// VL53L0X
VL53L0X sensorFront, sensorLeft, sensorRight;
const int XSHUT_LEFT = 6;
const int XSHUT_FRONT = 7;
const int XSHUT_RIGHT = 8;


int servoAngle = 90;

int normalSpeed = 255;  // med speed
int slowSpeed   = 100;  // high speed

void setupSensors() {
  pinMode(XSHUT_LEFT, OUTPUT);
  pinMode(XSHUT_FRONT, OUTPUT);
  pinMode(XSHUT_RIGHT, OUTPUT);

  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_FRONT, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);
  delay(10);

  digitalWrite(XSHUT_LEFT, HIGH);
  delay(10);
  sensorLeft.init(true);
  sensorLeft.setAddress(0x30);

  digitalWrite(XSHUT_FRONT, HIGH);
  delay(10);
  sensorFront.init(true);
  sensorFront.setAddress(0x31);

  digitalWrite(XSHUT_RIGHT, HIGH);
  delay(10);
  sensorRight.init(true);
  sensorRight.setAddress(0x32);

  sensorLeft.startContinuous();
  sensorFront.startContinuous();
  sensorRight.startContinuous();
}

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  steeringServo.attach(servoPin);
  steeringServo.write(servoAngle);

  Serial.begin(9600);
  Wire.begin();
  setupSensors();

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, normalSpeed);
}

void loop() {
  int distLeft  = sensorLeft.readRangeContinuousMillimeters();
  int distFront = sensorFront.readRangeContinuousMillimeters();
  int distRight = sensorRight.readRangeContinuousMillimeters();

  Serial.print("L: "); Serial.print(distLeft);
  Serial.print(" F: "); Serial.print(distFront);
  Serial.print(" R: "); Serial.println(distRight);

  // detact wall
  bool obstacle = (distFront < 150 || distLeft < 100 || distRight < 100);

  if (obstacle) {
    analogWrite(ENA, slowSpeed);
    digitalWrite(ledPin, HIGH);

    if (distFront < 150) {
      servoAngle = 110; // 
    } else if (distRight < 100) {
      servoAngle = 70;  // left
    } else if (distLeft < 100) {
      servoAngle = 110; // right
    }
  } else {
    analogWrite(ENA, normalSpeed);
    digitalWrite(ledPin, LOW);
    servoAngle = 90;
  }

  steeringServo.write(servoAngle);

  delay(10); 
}
