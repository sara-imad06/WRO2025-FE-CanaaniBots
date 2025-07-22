#include <Wire.h>
#include <Servo.h>
#include <VL53L0X.h>
#include <Adafruit_BNO08x.h>

// ===== Motor pins =====
const int ENA = 3;    // PWM speed control pin for motor
const int IN1 = 4;    // Direction control pin 1
const int IN2 = 5;    // Direction control pin 2

// ===== Servo =====
const int servoPin = 9;   // Servo signal pin
Servo steeringServo;

// ===== Encoder =====
const int ENCODER_A = 2;  // Encoder A signal (interrupt)
volatile long encoderTicks = 0;
void encoderISR() { encoderTicks++; }

// ===== VL53L0X sensors =====
VL53L0X sensorFront, sensorLeft, sensorRight;
const int XSHUT_LEFT = 6;   // XSHUT pin for left sensor
const int XSHUT_FRONT = 7;  // XSHUT pin for front sensor
const int XSHUT_RIGHT = 8;  // XSHUT pin for right sensor

// ===== IMU =====
Adafruit_BNO08x bno = Adafruit_BNO08x();
float yaw = 0.0; // current yaw in degrees

// ===== Settings =====
const int angleStraight = 90;  // Servo angle for straight movement
const int maxSteer = 25;       // Maximum steering adjustment in degrees
const int normalSpeed = 180;   // PWM value for normal speed
const int slowSpeed = 120;     // PWM value for slow speed
const float TICKS_PER_CM = 16.7; // Encoder ticks per centimeter (calibrate!)
int drivingDirection = +1;    // +1 = CCW (turn left), -1 = CW (turn right)

// ===== Motor control functions =====
void motorForward(int pwm) {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, pwm);
}

void motorBackward(int pwm) {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, pwm);
}

void motorStop() {
  analogWrite(ENA, 0);
}

// ===== Encoder helper =====
void resetEncoder() {
  noInterrupts();
  encoderTicks = 0;
  interrupts();
}

// ===== IMU yaw reading =====
float getYaw() {
  sh2_SensorValue_t val;
  if (bno.getSensorEvent(&val)) {
    if (val.sensorId == SH2_GAME_ROTATION_VECTOR) {
      yaw = val.un.gameRotationVector.yaw * 180.0 / PI;
      if (yaw < 0) yaw += 360;
    }
  }
  return yaw;
}

// ===== VL53L0X initialization =====
void setupSensors() {
  pinMode(XSHUT_LEFT, OUTPUT);
  pinMode(XSHUT_FRONT, OUTPUT);
  pinMode(XSHUT_RIGHT, OUTPUT);

  // Reset all sensors
  digitalWrite(XSHUT_LEFT, LOW);
  digitalWrite(XSHUT_FRONT, LOW);
  digitalWrite(XSHUT_RIGHT, LOW);
  delay(10);

  // Initialize left sensor
  digitalWrite(XSHUT_LEFT, HIGH);
  delay(10);
  sensorLeft.init(true);
  sensorLeft.setAddress(0x30);

  // Initialize front sensor
  digitalWrite(XSHUT_FRONT, HIGH);
  delay(10);
  sensorFront.init(true);
  sensorFront.setAddress(0x31);

  // Initialize right sensor
  digitalWrite(XSHUT_RIGHT, HIGH);
  delay(10);
  sensorRight.init(true);
  sensorRight.setAddress(0x32);

  // Start continuous ranging
  sensorLeft.startContinuous();
  sensorFront.startContinuous();
  sensorRight.startContinuous();
}

// ===== Wall-following step =====
void wallFollowStep() {
  int distL = sensorLeft.readRangeContinuousMillimeters();
  int distF = sensorFront.readRangeContinuousMillimeters();
  int distR = sensorRight.readRangeContinuousMillimeters();

  // If obstacle is too close in front
  if (distF < 150) {
    motorBackward(slowSpeed);
    delay(300); // back up for 0.3 s
    if (distL > distR) steeringServo.write(angleStraight - maxSteer);
    else steeringServo.write(angleStraight + maxSteer);
    delay(300); // steer away briefly
    steeringServo.write(angleStraight);
    motorForward(normalSpeed);
  } else {
    // Normal wall following
    if (distL < 80) steeringServo.write(angleStraight + maxSteer);
    else if (distR < 80) steeringServo.write(angleStraight - maxSteer);
    else {
      int diff = distL - distR;
      int adjust = constrain(diff / 10, -maxSteer, maxSteer);
      steeringServo.write(angleStraight - adjust);
    }
    motorForward(normalSpeed);
  }
}

// ===== Drive forward a precise distance =====
void driveForwardCM(float cm) {
  resetEncoder();
  while (true) {
    long ticks;
    noInterrupts();
    ticks = encoderTicks;
    interrupts();
    if (ticks >= cm * TICKS_PER_CM) {
      motorStop();
      break;
    }
    wallFollowStep();
    delay(50);
  }
}

// ===== Turn to a specific absolute angle =====
void turnToAngle(float target) {
  if (drivingDirection == +1) { // CCW (turn left)
    steeringServo.write(angleStraight - maxSteer);
    motorForward(slowSpeed);
    while (true) {
      float current = getYaw();
      float error = target - current;
      if (error < 0) error += 360;
      if (error < 3) break;
      delay(20);
    }
  } else { // CW (turn right)
    steeringServo.write(angleStraight + maxSteer);
    motorForward(slowSpeed);
    while (true) {
      float current = getYaw();
      float error = current - target;
      if (error < 0) error += 360;
      if (error < 3) break;
      delay(20);
    }
  }
  motorStop();
  steeringServo.write(angleStraight);
}

// ===== Determine initial driving direction =====
void detectDrivingDirection() {
  int distL = sensorLeft.readRangeContinuousMillimeters();
  int distR = sensorRight.readRangeContinuousMillimeters();

  if (distL < 200) {
    // Wall on the left
    drivingDirection = +1; // CCW
    steeringServo.write(angleStraight - 30);
    motorForward(normalSpeed);
    delay(1000); // move diagonally for 1 s
    motorStop();
    steeringServo.write(angleStraight);
  } else if (distR < 200) {
    // Wall on the right
    drivingDirection = -1; // CW
    steeringServo.write(angleStraight + 30);
    motorForward(normalSpeed);
    delay(1000);
    motorStop();
    steeringServo.write(angleStraight);
  } else {
    // No nearby walls, move a bit and decide
    resetEncoder();
    while (true) {
      long ticks;
      noInterrupts();
      ticks = encoderTicks;
      interrupts();
      if (ticks >= 20 * TICKS_PER_CM) break;
      motorForward(normalSpeed);
      delay(50);
    }
    motorStop();
    if (sensorLeft.readRangeContinuousMillimeters() > sensorRight.readRangeContinuousMillimeters()) {
      drivingDirection = +1;
    } else {
      drivingDirection = -1;
    }
  }
}

// ===== Arduino setup =====
void setup() {
  Serial.begin(115200);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  steeringServo.attach(servoPin);
  steeringServo.write(angleStraight);

  pinMode(ENCODER_A, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR, RISING);

  Wire.begin();
  setupSensors();

  if (!bno.begin_I2C()) {
    Serial.println("IMU not found!");
    while (1);
  }
  bno.enableReport(SH2_GAME_ROTATION_VECTOR, 10);
  delay(100);

  getYaw();
  detectDrivingDirection();
  Serial.println("Ready to start open challenge!");
}

// ===== Main loop =====
void loop() {
  float targetYaw = getYaw();
  for (int i = 0; i < 12; i++) {
    // Go forward one section (100 cm)
    driveForwardCM(100);

    // Update target angle for next corner
    if (drivingDirection == +1) {
      targetYaw -= 90;
      if (targetYaw < 0) targetYaw += 360;
    } else {
      targetYaw += 90;
      if (targetYaw >= 360) targetYaw -= 360;
    }

    // Turn to new angle
    turnToAngle(targetYaw);
  }

  // After 3 laps, drive forward until front wall is ~150 mm
  while (true) {
    int distF = sensorFront.readRangeContinuousMillimeters();
    if (distF <= 150) {
      motorStop();
      break;
    }
    motorForward(normalSpeed);
    delay(50);
  }

  motorStop();
  while (true) delay(1000); // done
}
