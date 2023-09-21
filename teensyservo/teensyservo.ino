#include <Servo.h>

int right = 55;;
int mid = 15;

Servo servo_1;

void setup()
{
  servo_1.attach(3, 500, 2500);
}

void loop()
{
    servo_1.write(mid);
    Serial.println(mid);
    delay(1000);
    
    servo_1.write(right);
    Serial.println(right);
    delay(1000);
      

   
}
