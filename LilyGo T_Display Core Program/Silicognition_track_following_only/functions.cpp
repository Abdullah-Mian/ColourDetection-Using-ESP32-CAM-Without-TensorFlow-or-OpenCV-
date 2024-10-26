#include "functions.h"
#include <Arduino.h>

// Define motor pins, servo pins, IR pins, and other constants here, or use extern for variables from main
#define motorA_forward 12
#define motorA_backward 13
#define motorB_forward 10
#define motorB_backward 11
#define leftIR 7
#define rightIR 8
#define ledPin 2

extern Servo servo1;
extern Servo servo2;

void executeCommand(char commandChar, int value) {
  switch (commandChar) {
    case 'G':
      grab(value);
      delay(1000);
      break;
    case 'S':
      releaseServo(value);
      delay(1000);
      break;
    default:
      handleMovement(commandChar, value);
      break;
  }
}

void handleMovement(char commandChar, int value) {
  if (commandChar == 'F') {
    moveForward(value);
  } else if (commandChar == 'B') {
    moveBackward(value);
  } else if (commandChar == 'L') {
    turnLeft(value);
  } else if (commandChar == 'R') {
    turnRight(value);
  }

  delay(1000);
  checkForObstacles();
  delay(1000);
}

void checkForObstacles() {
  int leftSensorValue = digitalRead(leftIR);
  int rightSensorValue = digitalRead(rightIR);
  Serial.print("Left IR Value: ");
  Serial.print(leftSensorValue);
  Serial.print(" | Right IR Value: ");
  Serial.println(rightSensorValue);

  if (leftSensorValue != HIGH) {
    Serial.println("Obstacle detected on the left!");
    moveRight(100);
  } else if (rightSensorValue != HIGH) {
    Serial.println("Obstacle detected on the right!");
    moveLeft(100);
  }
}

void checkServoPosition(Servo& servo, int servoNumber) {
  int currentPos = servo.read();
  if (servoNumber == 1) {
    if (currentPos != 0) {
      Serial.println("Servo 1 is not at zero position, resetting to zero...");
      servo.write(0);
      delay(1000);
    } else {
      Serial.println("Servo 1 is already at zero position.");
    }
  } else if (servoNumber == 2) {
    if (currentPos != 180) {
      Serial.println("Servo 2 is not at zero position, resetting to zero...");
      servo.write(180);
      delay(1000);
    } else {
      Serial.println("Servo 2 is already at zero position.");
    }
  }
}

void grab(int angle) {
  Serial.println("Grabbing...");
  servo1.write(angle);
  servo2.write(180 - angle);
  delay(1000);
  Serial.println("Grab complete.");
  delay(2000);
}

void releaseServo(int angle) {
  Serial.println("Releasing...");
  servo1.write(0);
  servo2.write(180);
  delay(1000);
  Serial.println("Release complete.");
  delay(2000);
}

void HitForward(int duration) {
  // if (!checkBlackDetection()) {
    digitalWrite(motorA_forward, HIGH);
    digitalWrite(motorB_forward, HIGH);
    delay(duration);  // Move forward for the specified duration
    stopMotors();  // Stop after moving forward
    delay(500);
    digitalWrite(motorA_backward, HIGH);
    digitalWrite(motorB_backward, HIGH);
    delay(duration);  // Move backward for the specified duration
    stopMotors();
  // } else {
    Serial.println("Hit Forward aborted: Black detected.");
  // }
}

void HitBackward(int duration) {
  // if (!checkBlackDetection()) {
    analogWrite(motorA_backward, 170);
    analogWrite(motorB_backward, 170);
    delay(duration);  // Move backward for the specified duration
    stopMotors();
    delay(500);
    analogWrite(motorA_forward, 170);
    analogWrite(motorB_forward, 170);
    delay(duration);  // Move forward for the specified duration
    stopMotors();     // Stop after moving forward
  // } else {
    Serial.println("Hit Backward aborted: Black detected.");
  // }
}

void moveForward(int duration) {
  digitalWrite(motorA_forward, HIGH);
  digitalWrite(motorB_forward, HIGH);
  delay(duration);
  stopMotors();
}

void moveBackward(int duration) {
  digitalWrite(motorA_backward, HIGH);
  digitalWrite(motorB_backward, HIGH);
  delay(duration);
  stopMotors();
}

void turnLeft(int duration) {
  digitalWrite(motorA_backward, HIGH);
  digitalWrite(motorB_forward, HIGH);
  delay(duration);
  stopMotors();
}

void turnRight(int duration) {
  digitalWrite(motorA_forward, HIGH);
  digitalWrite(motorB_backward, HIGH);
  delay(duration);
  stopMotors();
}

void moveRight(int duration) {
  digitalWrite(motorA_forward, HIGH);
  digitalWrite(motorB_backward, HIGH);
  delay(duration);
  stopMotors();
}

void moveLeft(int duration) {
  digitalWrite(motorA_backward, HIGH);
  digitalWrite(motorB_forward, HIGH);
  delay(duration);
  stopMotors();
}

void stopMotors() {
  digitalWrite(motorA_forward, LOW);
  digitalWrite(motorA_backward, LOW);
  digitalWrite(motorB_forward, LOW);
  digitalWrite(motorB_backward, LOW);
}