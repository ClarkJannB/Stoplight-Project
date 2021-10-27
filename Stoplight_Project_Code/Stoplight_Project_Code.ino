// Date and time functions using a PCF8523 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_PCF8523 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int hoursDay[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 ,21, 22, 23, 24};
int mins[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60};
int secs[] = {-1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60};

void setup () {
  Serial.begin(57600);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (! rtc.initialized() || rtc.lostPower()) {
    Serial.println("RTC is NOT initialized, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }


  rtc.start();


  float drift = 43; // seconds plus or minus over oservation period - set to 0 to cancel previous calibration.
  float period_sec = (7 * 86400);  // total obsevation period in seconds (86400 = seconds in 1 day:  7 days = (7 * 86400) seconds )
  float deviation_ppm = (drift / period_sec * 1000000); //  deviation in parts per million (μs)
  float drift_unit = 4.34; // use with offset mode PCF8523_TwoHours

  int offset = round(deviation_ppm / drift_unit);


  Serial.print("Offset is "); Serial.println(offset); // Print to control offset

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
}

void loop () {
  DateTime now = rtc.now();


  //prints time readout
  Serial.print(now.hour());
  Serial.print(" h ");
  Serial.print(now.minute());
  Serial.print(" m ");
  Serial.print(now.second());
  Serial.print(" s ");
  Serial.println();

  Serial.print(mins[7]);


  // (BASIC DOESN"T USE ARRAYS) goes green 1 second, yellow 2 seconds, red 3 seconds, then repeats through the whole minute
  if (now.second() == 1 || now.second() == 4 || now.second() == 7 || now.second() == 10 || now.second() == 13 || now.second() == 16 || now.second() == 19 || now.second() == 22 || now.second() == 25 || now.second() == 28 || now.second() == 31 || now.second() == 34
      || now.second() == 37 || now.second() == 40 || now.second() == 43 || now.second() == 46 || now.second() == 49 || now.second() == 52 || now.second() == 55 || now.second() == 58) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }

  if (now.second() == 2 || now.second() == 5 || now.second() == 8 || now.second() == 11 || now.second() == 14 || now.second() == 17 || now.second() == 20 || now.second() == 23 || now.second() == 26 || now.second() == 29 || now.second() == 32 || now.second() == 35
      || now.second() == 38 || now.second() == 41 || now.second() == 44 || now.second() == 47 || now.second() == 50 || now.second() == 53 || now.second() == 56 || now.second() == 59) {
    digitalWrite(3, HIGH);
  } else {
    digitalWrite(3, LOW);
  }

  if (now.second() == 3 || now.second() == 6 || now.second() == 9 || now.second() == 12 || now.second() == 15 || now.second() == 18 || now.second() == 21 || now.second() == 24 || now.second() == 27 || now.second() == 30 || now.second() == 33 || now.second() == 36
      || now.second() == 39 || now.second() == 42 || now.second() == 45 || now.second() == 48 || now.second() == 51 || now.second() == 54 || now.second() == 57 || now.second() == 0) {
    digitalWrite(4, HIGH);
  } else {
    digitalWrite(4, LOW);
  }





  delay(1000);
}
