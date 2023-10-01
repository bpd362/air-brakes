#include <Arduino.h>
#include <Thread.h>
#include <ThreadController.h>
#include <SPI.h>
#include <SD.h>

File SD_File;

bool startThread2 = false; // Flag to start Thread 2

int i;

// Function to be executed by Thread 1
void threadFunction1() {
  while (1) {
    // Code for Thread 1
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    if (SD_File) {
      SD_File.print(i);
      SD_File.println("Thread 1 is running");
      SD_File.close();
    }

    // Check the condition to start Thread 2
    if (!startThread2) {
      int sensorValue = i++; //analogRead(A0); // Replace A0 with your sensor's pin
      if (sensorValue >= 50) {
        startThread2 = true;
      }
    }

  // Check and start Thread 2 as needed
  startThread2IfNeeded();
  
    delay(100); // Delay for 1 second
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

// Create thread objects with priority
Thread thread1 = Thread();
Thread thread2 = Thread(2); // Assign a higher priority (e.g., 2)

void startThread2IfNeeded() {
  // Check if Thread 2 should be started
  if (startThread2) {
    // Start Thread 2
    thread2.onRun(threadFunction2);
    thread2.setInterval(500); // Set the interval for Thread 2
    thread2.run();
    startThread2 = false; // Reset the flag
  }
}

void setup() {
  // Initialize SD card
  SD.begin(BUILTIN_SDCARD);

  // Start Thread 1
  thread1.onRun(threadFunction1);
  thread1.setInterval(1000); // Set the interval for Thread 1
    thread1.run();
}

void loop() {
  // This loop will not be used when threads are running.
  //thread1.run();


}
