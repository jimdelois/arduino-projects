#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

const byte PIN_LED = A3;
const unsigned int DURATION_LED_PULSE = 250; // milliseconds
const unsigned long SENSITIVITY_IR = 7000;

unsigned long ledStart = 0;

MAX30105 sensor;

void setup() {

  pinMode(PIN_LED, OUTPUT);

  Serial.begin(9600);
  digitalWrite(PIN_LED, LOW);

  Serial.println("Booting...");

  // Beginning with Wire is A4 (SDA), A5 (SCL)
  if (sensor.begin(Wire, I2C_SPEED_FAST) == false) {
    Serial.println("MAX30102 not found.");
    while(1);
  }
  Serial.println("MAX30102 found; Configuring...");

  byte powerLevel    = 0xFF;  // Presence detection - https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/MAX30105.cpp#L495
  byte ledMode       = 2;     // Red & IR           - https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/MAX30105.cpp#L458
  byte sampleAverage = 4;     // Don't average      - https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/MAX30105.cpp#L442
  int  sampleRate    = 50;    //                    - https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/MAX30105.cpp#L470
  int  pulseWidth    = 216;   //                    - https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/MAX30105.cpp#L482
  int  adcRange      = 2048;  //                    - https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library/blob/master/src/MAX30105.cpp#L467
//  sensor.setup(powerLevel, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange);
  sensor.setup();

//  sensor.setPulseAmplitudeRed(0x0A);
  Serial.println("MAX30102 Configuration complete.");
}

void loop() {
  checkLed();
  long infraRedVal = sensor.getIR();
  if(infraRedVal > SENSITIVITY_IR) {
    if (checkForBeat(infraRedVal) == true) {
      Serial.print(millis());
      Serial.println(": Beat");
      ledOn();
    }
  }
}

void ledOn() {
  digitalWrite(PIN_LED, HIGH);
  ledStart = millis();
}

void ledOff() {
  digitalWrite(PIN_LED, LOW);
}

void checkLed() {
  if ((millis() - ledStart) >= DURATION_LED_PULSE) {
    ledOff();
  }
}
