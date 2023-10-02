#include <FreeRTOS_TEENSY4.h>
#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <SPI.h>
#include <SD.h>
#include <Servo.h>

File SD_File;
Servo servo_1;
SemaphoreHandle_t startThread2Semaphore;

volatile bool startThread2 = false; // Flag to start Thread 2

int i;

// Function to be executed by Thread 1
void threadFunction1(void* parameter) {
  while (1) {
    // Code for Thread 1
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    if (SD_File) {
      SD_File.print(i);
      SD_File.println(" Thread 1 is running");
      SD_File.close();
    }
    int sensorValue = i++; 
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
      servo_1.write(10);
      vTaskDelay(pdMS_TO_TICKS(500));
      servo_1.write(90);
      vTaskDelay(pdMS_TO_TICKS(500));
    }
    else {
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}

void setup() {
  // Initialize SD card
  SD.begin(BUILTIN_SDCARD);

  servo_1.attach(35, 500, 2500);

  // Create and start Thread 1
  xTaskCreate(threadFunction1, "Thread1", 1000, NULL, 1, NULL);

  // Create and start Thread 2
  xTaskCreate(threadFunction2, "Thread2", 1000, NULL, 2, NULL);

  startThread2Semaphore = xSemaphoreCreateBinary();

  vTaskStartScheduler();
}

void loop() {
  // The FreeRTOS scheduler is running, so this loop will not be used.
}
