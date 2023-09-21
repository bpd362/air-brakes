#include <Wire.h>
#include <MS5611.h>

MS5611 ms5611;
float measuredPressures[4];
uint8_t measuredPressuresIndex = 0;
float measuredTemperatures[4];
uint8_t measuredTemperaturesIndex = 0;
float padTemperature;
float padPressure;

// Altimeter_Functions
// by Preston Hager

bool Barometric_Sensor_Startup() {
  // Try to setup the altimeter.
  bool successfulSetup = false;
  for (int i = 0; i < 10; i++) {
    if (ms5611.begin()) {
      successfulSetup = true;
      break;
    }
    Serial.println("Could not find a valid MS5611 sensor, check wiring!");
    delay(500);
    // If the sensor did not setup, return false
    if (i == 9)
      return false;
  }

  // Get the average pad pressure for calculations
  for (uint8_t i = 0; i < 4; i++) {
    Barometric_Sensor_Read();
    delay(100);
  }

  // Set the temperature offset
  ms5611.setTemperatureOffset(273.15);

  return successfulSetup;
}

void Barometric_Sensor_Read() {
  // Read the current pressure and put it in the buffer
  ms5611.read();
  measuredPressures[measuredPressuresIndex] = ms5611.getPressure();
  measuredTemperatures[measuredTemperaturesIndex] = ms5611.getTemperature();
  // Find the average of the recorded pressures, not including the current index.
  float pressureSum = 0.0f;
  float temperatureSum = 0.0f;
  for (uint8_t i = 0; i < 4; i++) {
    if (i != measuredPressuresIndex) { // Skip over the last read value.
      pressureSum += measuredPressures[i];
      temperatureSum += measuredTemperatures[i];
    }
  }
  padPressure = pressureSum / 3.0;
  padTemperature = temperatureSum / 3.0;
  // Increase where we are in the buffer for next time.
  measuredPressuresIndex++;
  measuredTemperaturesIndex++;
  // Loop around if we have reached the end of the array.
  if (measuredPressuresIndex == 4) {
    measuredPressuresIndex = 0;
    measuredTemperaturesIndex = 0;
  }
}

float Barometric_Sensor_Current_Abs() {
  ms5611.read();
  return ms5611.getPressure();
}

float Barometric_Get_Altitude() {
  ms5611.read();
  float currentPressure = ms5611.getPressure();
  return (padTemperature / -0.0065) * (pow((currentPressure / padPressure), ((8.31432 * 0.0065) / (9.80665 * 0.0289644))) - 1);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Barometric_Sensor_Startup();
}

void loop() {
  // put your main code here, to run repeatedly:
//  float pressure = Barometric_Sensor_Current_Abs();
//  Serial.print("Pressure: ");
//  Serial.println(pressure);
  float altitude = Barometric_Get_Altitude();
  Serial.print("Altitude: ");
  Serial.println(altitude);
  delay(100);

}
