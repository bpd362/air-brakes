/***************************************************************************
  This is a library for the BMP3XX temperature & pressure sensor

  Designed specifically to work with the Adafruit BMP388 Breakout
  ----> http://www.adafruit.com/products/3966

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"

#define BMP_SCK 13
#define BMP_MISO 12
#define BMP_MOSI 11
#define BMP_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

  #include <SD.h>

  File SD_File;

Adafruit_BMP3XX bmp;

void setup() {
  SD.begin(BUILTIN_SDCARD);

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
}

void loop() {
  if (! bmp.performReading()) {
    SD_File = SD.open("LaunchPressure.txt", FILE_WRITE);
    SD_File.println("Failed to perform reading :(");
    SD_File.close();
    return;
  }
  SD_File = SD.open("LaunchPressure.txt", FILE_WRITE);
  SD_File.print("Temperature = ");
  SD_File.print(bmp.temperature);
  SD_File.println(" *C");

  SD_File.print("Pressure = ");
  SD_File.print(bmp.pressure / 100.0);
  SD_File.println(" hPa");

  SD_File.print("Approx. Altitude = ");
  SD_File.print(bmp.readAltitude(SEALEVELPRESSURE_HPA));
  SD_File.println(" m");

  SD_File.println();
  SD_File.close();
  delay(2000);
}
