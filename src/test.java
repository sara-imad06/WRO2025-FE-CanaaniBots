#include <Servo.h>

const int ENA = 3;     
const int IN1 = 4;     
const int IN2 = 5;     
const int servoPin = 6;

Servo steeringServo;
int servoAngle = 90;   // Start at center

unsigned long lastTurnTime = 0;
int turnState = 0;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Move forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 120);  // Speed

  steeringServo.attach(servoPin);
  steeringServo.write(servoAngle); // Center

  Serial.begin(9600);
  Serial.println("Robot started.");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastTurnTime >= 5000) {
    lastTurnTime = currentMillis;

    if (turnState == 0) {
      servoAngle = 90;    // Left (min 65)
      Serial.println("Turning Left");
    } else if (turnState == 1) {
      servoAngle = 90;    // Center (90)

      Serial.println("Centering");
    } else {
      servoAngle = 90;   // Right (max 125)
      Serial.println("Turning Right");
    }
  // When all angels = 90 --> the robot is moving forward
    steeringServo.write(servoAngle);
    turnState = (turnState + 1) % 3;
    Serial.print("turnstate  ");
    Serial.println(turnState);

  }

  // Motor keeps going forward
}
