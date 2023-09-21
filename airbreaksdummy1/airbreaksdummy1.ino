#include <Servo.h>

void setup() {
  Serial.begin(9600);
  delay(1600);
  Serial.println("Main power on");
  Serial.println("Initilizing systems");
  Servo servo;

  servo.write(0); //set servo to home position
  Serial.println(servo.read());
  Serial.print("0 if in home position");

  servo.write(66); //set servo to deployed position
  Serial.println(servo.read());
  Serial.print("66 if deployed");

  servo.write(0); //set servo back to home position
  Serial.println(servo.read());
  Serial.print("0 if retracted");
}

void loop() {
  // put your main code here, to run repeatedly:
}
