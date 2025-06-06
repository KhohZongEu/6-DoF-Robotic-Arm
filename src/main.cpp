#include <Arduino.h>
#include <Servo.h>

//all lengths are in cm
#define ELBOW_LENGTH 10
#define BASE_LENGTH 10

String XAXIS = "x-axis";
String YAXIS = "y-axis";

Servo base;
Servo elbow;

int readSerial(String name);
int calc(int x, int y);
void calibration();

void setup() {
  Serial.begin(9600);
  base.attach(9);
  elbow.attach(10);
  calibration();
}

void loop() {
  int x = readSerial(XAXIS);
  Serial.println(x);
  int y = readSerial(YAXIS);
  Serial.println(y);
  if (calc(x,y)) {
    Serial.println("Coordinates out of range. Please re-enter");
  };
}

int calc(int x, int y) {
  float distance = sqrt(pow(x,2)+pow(y,2));
  float maxReach = BASE_LENGTH + ELBOW_LENGTH;
  if (distance > maxReach) return 1;

  float elbowAngle = acos((pow(BASE_LENGTH, 2) + pow(ELBOW_LENGTH, 2) - pow(distance, 2)) / (2 * BASE_LENGTH * ELBOW_LENGTH));
  float baseAngle = acos((pow(distance, 2) + pow(BASE_LENGTH, 2) - pow(ELBOW_LENGTH, 2)) / (2 * distance * BASE_LENGTH));
  float directAngle = atan2(y, x);
  float finalBase = directAngle - baseAngle;
  
  Serial.print("Direct Distance: ");
  Serial.println(distance);
  Serial.print("Elbow Angle(rad): ");
  Serial.println(elbowAngle);
  Serial.print("Base Angle(rad): ");
  Serial.println(baseAngle);
  Serial.print("Direct Angle(rad): ");
  Serial.println(directAngle);
  Serial.print("Final Base Angle(rad): ");
  Serial.println(finalBase);
  
  //Change to degrees
  float elbowAngleDegrees = elbowAngle / PI * 180;
  float finalBaseDegrees = finalBase / PI * 180;
  Serial.print("Elbow Angle: ");
  Serial.println(elbowAngleDegrees);
  Serial.print("Base Angle: ");
  Serial.println(finalBaseDegrees);
  elbow.write(180-elbowAngleDegrees+90);
  base.write(finalBaseDegrees+90);

  return 0;
}

int readSerial(String name) {
  Serial.print(name += ": ");
  String input = "";
  bool waitInput = true;
  while (waitInput) {
    if (Serial.available()) {
      char lastChar = Serial.read();
      
      if (lastChar != '\n') input += lastChar;
      else waitInput = false;
    }
  }
  return input.toInt();
}

void calibration() {
  elbow.write(90);
  base.write(90);
}
