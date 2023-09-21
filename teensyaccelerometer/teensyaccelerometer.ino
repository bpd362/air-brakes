#include <Adafruit_FXAS21002C.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

File SD_File;

Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);

void displaySensorDetails() {
  sensor_t sensor;
  gyro.getSensor(&sensor);
  // re-open the file for writing:
  SD_File = SD.open("LaunchAccel.txt", FILE_WRITE);
  SD_File.println("------------------------------------");
  SD_File.print("Sensor:       ");
  SD_File.println(sensor.name);
  SD_File.print("Driver Ver:   ");
  SD_File.println(sensor.version);
  SD_File.print("Unique ID:    0x");
  SD_File.println(sensor.sensor_id, HEX);
  SD_File.print("Max Value:    ");
  SD_File.print(sensor.max_value);
  SD_File.println(" rad/s");
  SD_File.print("Min Value:    ");
  SD_File.print(sensor.min_value);
  SD_File.println(" rad/s");
  SD_File.print("Resolution:   ");
  SD_File.print(sensor.resolution);
  SD_File.println(" rad/s");
  SD_File.println("------------------------------------");
  SD_File.println("");
  // close the file:
  SD_File.close();
  delay(500);
}

void setup() {

SD.begin(BUILTIN_SDCARD);

 // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
SD_File = SD.open("LaunchAccel.txt", FILE_WRITE);

 // if the file opened okay, write to it:
  if (SD_File) {
    SD_File.println("Power ON Complete");
    SD_File.println("Starting Accelerometer Recording");
    // close the file:
    SD_File.close();

/* Initialise the sensor */
  if (!gyro.begin()) {
       // re-open the file for writing:
   SD_File = SD.open("LaunchAccel.txt", FILE_WRITE);
    /* There was a problem detecting the FXAS21002C ... check your connections
     */
    SD_File.println("Ooops, no FXAS21002C detected ... Check your wiring!");
        // close the file:
   SD_File.close();
    while (1)
      ;
  }

  displaySensorDetails();
  
  }
}

void loop() {
  /* Get a new sensor event */
  sensors_event_t event;
  gyro.getEvent(&event);

  // re-open the file for writing:
  SD_File = SD.open("LaunchAccel.txt", FILE_WRITE);
  /* Display the results (speed is measured in rad/s) */
  SD_File.print("X: ");
  SD_File.print(event.gyro.x);
  SD_File.print("  ");
  SD_File.print("Y: ");
  SD_File.print(event.gyro.y);
  SD_File.print("  ");
  SD_File.print("Z: ");
  SD_File.print(event.gyro.z);
  SD_File.print("  ");
  SD_File.println("rad/s ");
      // close the file:
    SD_File.close();
  delay(500);
}
