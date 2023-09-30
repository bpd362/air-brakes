#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <SPI.h>
#include <SD.h>

File SD_File;

// Function to be executed by Thread 1
void threadFunction1() {
  while (1) {
    // Code for Thread 1
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    if (SD_File) {
      SD_File.println("Thread 1 is running");
      SD_File.close();
    }
    delay(1000); // Delay for 1 second
  }
}

// Function to be executed by Thread 2
void threadFunction2() {
  while (1) {
    // Code for Thread 2
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    if (SD_File) {
      SD_File.println("Thread 2 is running");
      SD_File.close();
    }
    delay(500); // Delay for 500 milliseconds
  }
}

// Create thread objects
Thread thread1 = Thread();
Thread thread2 = Thread();

void setup() {
  //Serial.begin(9600);

  // Initialize SD card
  SD.begin(BUILTIN_SDCARD);

  // Start threads
  thread1.onRun(threadFunction1);
  thread2.onRun(threadFunction2);
  thread1.setInterval(1000); // Set the interval for Thread 1
  thread2.setInterval(500);  // Set the interval for Thread 2
}

void loop() {
  // This loop will not be used when threads are running.
  thread1.run();
  thread2.run();
}
