#include <Servo.h>
#include <Arduino.h>

// Motor driver pins
const int ENA = 3;     // PWM for motor speed
const int IN1 = 4;     // Motor direction
const int IN2 = 5;

// Servo pin
const int servoPin = 6;
Servo steeringServo;

int servoAngle = 90;   // Initial angle (center)
unsigned long lastTurnTime = 0;
int turnState = 0;

void setup() {
  // Set motor control pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Move motor forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 120);  // Speed: range 0-255

  // Setup servo
  steeringServo.attach(servoPin);
  steeringServo.write(servoAngle); // Center position

  Serial.begin(9600);
  Serial.println("Robot started.");
}

void loop() {
  unsigned long currentMillis = millis();

  // Change servo direction every 5 seconds
  if (currentMillis - lastTurnTime >= 5000) {
    lastTurnTime = currentMillis;

    if (turnState == 0) {
      servoAngle = 70;    // Turn left
      Serial.println("Turning Left");
    } else if (turnState == 1) {
      servoAngle = 90;    // Center
      Serial.println("Centering");
    } else {
      servoAngle = 110;   // Turn right
      Serial.println("Turning Right");
    }

    steeringServo.write(servoAngle);
    turnState = (turnState + 1) % 3;

    Serial.print("Turn state: ");
    Serial.println(turnState);
  }

  // The motor continues to move forward constantly
}
