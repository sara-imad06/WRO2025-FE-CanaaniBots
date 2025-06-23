#include <Servo.h>
#include <Arduino.h>
#include <VL53L0X.h>

// Motor driver pins
const int ENA = 3;    
const int IN1 = 4;     
const int IN2 = 5;

// Servo pin
const int servoPin = 6;
Servo steeringServo;

const int ledPin = 10;


VL53L0X sensorFront, sensorLeft, sensorRight;
const int XSHUT_LEFT = 6;
const int XSHUT_FRONT = 7;
const int XSHUT_RIGHT = 8;

int servoAngle = 90;   // Initial angle (center)
unsigned long lastTurnTime = 0;
int turnState = 0;

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

  // move motor forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 120);  // speed(range 0-255)
  
  steeringServo.attach(servoPin);
  steeringServo.write(servoAngle); // senter position

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.begin(9600);
  Wire.begin();
  setupSensors();
  Serial.println("Robot started.");
}



void loop() {
  unsigned long currentMillis = millis();

  // Sensors distances
  int distLeft = sensorLeft.readRangeContinuousMillimeters();
  int distFront = sensorFront.readRangeContinuousMillimeters();
  int distRight = sensorRight.readRangeContinuousMillimeters();

  Serial.print("L: "); Serial.print(distLeft);
  Serial.print(" F: "); Serial.print(distFront);
  Serial.print(" R: "); Serial.println(distRight);


  bool tooClose = (distFront < 150 || distLeft < 150 || distRight < 150);
  digitalWrite(ledPin, tooClose ? HIGH : LOW);


  if (distFront < 150) {  //front 
    servoAngle = 110; 
  } else if (distRight < 100) {  //right
    servoAngle = 70;  
  } else if (distLeft < 100) { //left
    servoAngle = 110; 
  } else {
    servoAngle = 90; 
  }

  steeringServo.write(servoAngle);

  // turnnig every 5 seconds (right)
  if (currentMillis - forwardStartTime >= forwardDuration) {
    Serial.println("Turning right to follow square path...");
    steeringServo.write(110);  // turn right
    delay(600);                // turn duration
    steeringServo.write(90);   // back to center
    forwardStartTime = currentMillis;
  }

  // Send distances to Python visualizer
  Serial.print("D:");
  Serial.print(distLeft);
  Serial.print(",");
  Serial.print(distFront);
  Serial.print(",");
  Serial.println(distRight);

  delay(100);


}