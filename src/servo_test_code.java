#include <Servo.h>
class DCMotor {
private:
  int in1Pin, in2Pin, enPin;

public:
  DCMotor(int in1, int in2, int en) {
    in1Pin = in1;
    in2Pin = in2;
    enPin = en;
    pinMode(in1Pin, OUTPUT);
    pinMode(in2Pin, OUTPUT);
    pinMode(enPin, OUTPUT);
  }

  void forward(int speed) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
    analogWrite(enPin, constrain(speed, 0, 255));
  }

  void stop() {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
    analogWrite(enPin, 0);
  }
};

class Steering {
private:
  Servo servo;
  int pin;
  int centerAngle = 90;
  int maxDeviation = 10; 

public:
  Steering(int servoPin) {
    pin = servoPin;
  }

  void begin() {
    servo.attach(pin);
    center();
  }

  void center() {
    servo.write(centerAngle);
  }

  
  void turn(int degreesFromCenter) {
    degreesFromCenter = constrain(degreesFromCenter, -maxDeviation, maxDeviation);
    servo.write(centerAngle + degreesFromCenter);
  }
};


DCMotor drive(4, 5, 3);   
Steering steer(9);       


void setup() {
  steer.begin();         
  steer.center();       
