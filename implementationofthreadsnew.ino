//multithreading libraries
#include <FreeRTOS_TEENSY4.h>
#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
//sd libraries
#include <SPI.h>
#include <SD.h>
//servo
#include <Servo.h>
//accelerometer libraries
#include <Adafruit_FXAS21002C.h>
#include <Wire.h>
//pressure libraries
#include "Adafruit_BMP3XX.h" 

//define pressure pins
#define BMP_SCK 13
#define BMP_MISO 14
#define BMP_MOSI 15
#define BMP_CS 16
//define pressure values
#define SEALEVELPRESSURE_HPA (1013.25)

File SD_File;

Servo servo_1;

SemaphoreHandle_t startThread2Semaphore;

volatile bool startThread2 = false; // Flag to start Thread 2

//accelerometer
Adafruit_FXAS21002C gyro = Adafruit_FXAS21002C(0x0021002C);
//pressure
Adafruit_BMP3XX bmp;

//accelerometer function
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

// Function to be executed by Thread 1
void threadFunction1(void* parameter) {
  while (1) {
    // Code for Thread 1
      //accelerometer
  // Get a new accelerometer event 
  sensors_event_t event;
  gyro.getEvent(&event);
  // re-open the accelerometer file for writing:
  
  double mag = sqrt(event.gyro.x*event.gyro.x + event.gyro.y*event.gyro.y + event.gyro.z*event.gyro.z);
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    if (SD_File) {
      // Display the results (speed is measured in rad/s) 
  SD_File.print("X: ");
  SD_File.print(event.gyro.x);
  SD_File.print("  ");
  SD_File.print("Y: ");
  SD_File.print(event.gyro.y);
  SD_File.print("  ");
  SD_File.print("Z: ");
  SD_File.print(event.gyro.z);
  SD_File.print("  Mag: ");
  SD_File.print(mag);
  SD_File.println("rad/s ");
      // close the file:
    SD_File.close();
    }
  delay(500);

//pressure
  if (! bmp.performReading()) {
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    if (SD_File) { 
      SD_File.println("Failed to perform reading :(");
    SD_File.close();
    return; 
    }
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    if (SD_File) { 
      //temp reading
  /*
  SD_File.print("Temperature = ");
  SD_File.print(bmp.temperature);
  SD_File.println(" *C");
  */
  //pressure reading
  SD_File.print("Pressure = ");
  SD_File.print(bmp.pressure / 100.0);
  SD_File.println(" hPa");
  //altitude reading
  SD_File.print("Approx. Altitude = ");
  SD_File.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  SD_File.println(" m");
  //adds a gap between reading values
  SD_File.println();
  SD_File.close();
    }
    delay(500);
  }


    
    int sensorValue = mag; 
    // Check the condition to start Thread 2
  if (!startThread2) {
      if (sensorValue >= 5) {
        xSemaphoreGive(startThread2Semaphore);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(100)); // Delay for .1 second
  }
}


// Function to be executed by Thread 2
void threadFunction2(void* parameter) {
  while (1) {
    if (xSemaphoreTake(startThread2Semaphore,  portMAX_DELAY)) {
      // Code for Thread 2
      /*
      servo_1.write(10);
      vTaskDelay(pdMS_TO_TICKS(500));
      servo_1.write(90);
      vTaskDelay(pdMS_TO_TICKS(500));
      */
    }
    else {
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}


void setup() {
//start sd card
SD.begin(BUILTIN_SDCARD);

//accelerometer setup
 // open the accelerometer file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
SD_File = SD.open("LaunchAccel.txt", FILE_WRITE);
 // if the file opened okay, write to it:
  if (SD_File) {
    SD_File.println("Power ON Complete");
    SD_File.println("Starting Accelerometer Recording");
    // close the file:
    SD_File.close();
// Initialise the accelerometer 
  if (!gyro.begin()) {
       // re-open the file for writing:
   SD_File = SD.open("LaunchAccel.txt", FILE_WRITE);
    // There was a problem detecting the FXAS21002C ... check your connections
    SD_File.println("Ooops, no FXAS21002C detected ... Check your wiring!");
        // close the file:
   SD_File.close();
    while (1)
      ;
  }
  displaySensorDetails();
  }

  //pressure setup
  //if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
  //if (! bmp.begin_SPI(BMP_CS)) {  // hardware SPI mode  
  if (! bmp.begin_SPI(BMP_CS, BMP_SCK, BMP_MISO, BMP_MOSI)) {  // software SPI mode
    SD_File = SD.open("LaunchPressure.txt", FILE_WRITE);
    SD_File.println("Could not find a valid BMP3 sensor, check wiring!");
     SD_File.close();
    while (1);
  }  
    // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_8X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);

  servo_1.attach(35, 500, 2500);//attaches servo to pin 35

  // Create and start Thread 1
  xTaskCreate(threadFunction1, "Thread1", 1000, NULL, 1, NULL);//runs task 1, with a priority of 1, every 1 second

  // Create and start Thread 2
  xTaskCreate(threadFunction2, "Thread2", 1000, NULL, 2, NULL);//runs task 2, with a priority of 2, every 1 second

  startThread2Semaphore = xSemaphoreCreateBinary(); //creates the semaphore used to pass the value of StartThread2 between threads

  vTaskStartScheduler();
}

void loop() {
    // The FreeRTOS scheduler is running, this loop will not be used.
}
