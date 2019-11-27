#include "SR04.h"
#include "pitches.h"

const int PIN_LED     = 7;
const int PIN_TRIGGER = 5;
const int PIN_ECHO    = 4;
const int PIN_BUZZER  = 2;

const int DISTANCE_MAX = 100;

const int DURATION_BEEP = 125;

SR04 sr04 = SR04(PIN_ECHO, PIN_TRIGGER);
int distance;
int freq;
int i = 0;

void setup() {
  Serial.begin(9600);
  delay(2500);
}

void loop() {
  distance = constrain(sr04.Distance(), 0, DISTANCE_MAX);
  //   distance = constrain(sr04.DistanceAvg(5, 3), 0, MAX_DISTANCE);
  Serial.print("Distance: ");
  Serial.println(distance);

  alert(distance);

  delay(100);
  digitalWrite(PIN_LED, LOW);
}

void alert(long distance) {
  freq = map(distance, 0, DISTANCE_MAX, 1, 10);
  if (++i % freq == 0) {
    beep();
    blink();
    i = 0;
  }
}

int note;
void beep() {
  note = map(10 - freq, 0, 9, NOTE_C5, NOTE_E6); 
  tone(PIN_BUZZER, note, DURATION_BEEP);
}

void blink() {
  digitalWrite(PIN_LED, HIGH);
}
