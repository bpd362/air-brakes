#include <Arduino.h>
#include <TeensyThreads.h>

#include <SPI.h>
#include <SD.h>

File SD_File;

// Function to be executed by Thread 1
void threadFunction1() {
  while (1) {
    // Code for Thread 1
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    SD_File.println("Thread 1 is running");
    SD_File.close();
    delay(1000); // Delay for 1 second
    
  }
}

// Function to be executed by Thread 2
void threadFunction2() {
  while (1) {
    // Code for Thread 2
    SD_File = SD.open("Thread.txt", FILE_WRITE);
    SD_File.println("Thread 2 is running");
    SD_File.close();
    delay(500); // Delay for 500 milliseconds
  }
}

/*// Create thread objects
threads.addThread(threadFunction1, 1);
threads::addThread(threadFunction2, 2);
*/
void setup() {
  SD.begin(BUILTIN_SDCARD);

  // Create thread objects
threads.addThread(threadFunction1, 1);
threads.addThread(threadFunction2, 2);

  //threadFunction1.start();
  //threadFunction2.start();
}

void loop() {
  // This loop will not be used when threads are running.
}
