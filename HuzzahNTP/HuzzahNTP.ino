/*
  Recieves time from Network Time Protocol (NTP) to control the lights

  Green indicates class time or "go time"
  Yellow indicates warning time or 5 minutes before the period ends
  Red indicates passing time or lunch time (when class is not going on)


  The schedule times are in a multidimensional array and you can change the variable values to control which schedule you want to follow.
  It runs the periods per "block" and tests to see if the converted time of hours and minutes is within a range of the block.
  Once the time exceeds the first "block" the code proceeds to the next block and repeats until the day is over and is reset.

  created 28 Oct 2022
  by Clark Barayuga '23

*/


#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DAYLIGHTSAVINGS 0 //Change either to 1 or 0 depending on daylight savings. 0 = fall back (no daylight savings)
#define GREEN_PIN 5
#define YELLOW_PIN 0
#define RED_PIN 2
#define LIGHT_ON 1
#define LIGHT_OFF 0
#define WARNING_TIME 5

//Wifi Credentials (edit as required)
const char *ssid     = "TP-Link_51CA";
const char *password = "password";

const long utcOffsetInSeconds = -18000;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// The times of the schedules
// Change these to change which schedule it is (Regular Time Schedule, Early Release, Advisory Activity, or Extended Advisory Activity)
int schStart = 0; //Example: for Advisory Activity Schedule change the value to 30
int schEnd = 15; //Example: for Advisory Activity Schedule change the value to 47
int schJump = 7; //how many blocks there are in the day, how many times it jumps
int lunch = 10;  //Change this value to the position in the schedule of the start of the lunch period. Example: for Advisory Activity Schedule change the value to 42

int schIndex = 0;
int block = schIndex;
int nowTime = 460;

int schReg[65][2] {
  //The hours is the first value, the minutes is the second value
  //When it says "Block #" that's how the computer is going through each period.
  //REGULAR TIME SCHEDULE:
  //Index (#0 - 15) <- Range of Positions for this schedule
  // Block 0 (ADVISORY)
  {7, 45},  // #0: 465 <- Position # and the conversion of hours and minutes to number of minutes since 12:00 am
  {7, 49},  // #1: 469
  // Block 1 (PERIOD 1)
  {7, 52},  // #2: 472
  {8, 48},  // #3: 528
  // Block 2 (PERIOD 2)
  {8, 51},  // #4: 531
  {9, 47},  // #5: 587
  // Block 3 (PERIOD 3)
  {9, 50},  // #6: 590
  {10, 46}, // #7: 640
  // Block 4 (PERIOD 4)
  {10, 51}, // #8: 651
  {11, 47}, // #9: 707
  // Block 5 (LUNCH)
  {11, 50}, // #10: 710
  {12, 18}, // #11: 738
  // Block 6 (PERIOD 5)
  {12, 23}, // #12: 743
  {13, 19}, // #13: 799
  // Block 7 (PERIOD 6)
  {13, 22}, // #14: 802
  {14, 18}, // #15: 858


  //EARLY RELEASE SCHEDULE:

  //Index (16-29)
  // Block 0 (ADVISORY)
  {7, 45}, // #16: 465
  {7, 50}, // #17: 470
  // Block 1 (PERIOD 1)
  {7, 53}, // #18: 473
  {8, 35}, // #19: 515
  // Block 2 (PERIOD 2)
  {8, 38}, // #20: 518
  {9, 16}, // #21: 556
  // Block 3 (PERIOD 3)
  {9, 19}, // #22: 559
  {9, 57}, // #23: 597
  // Block 4 (PERIOD 4)
  {10, 00}, // #24: 600
  {10, 38}, // #25: 638
  // Block 5 (PERIOD 5)
  {10, 41}, // #26: 641
  {11, 19}, // #27: 679
  // Block 6 (PERIOD 6)
  {11, 22}, //#28: 682
  {12, 00}, //#29: 720

  //ADVISORY ACTIVITY SCHEDULE:

  //Index (30-47)
  // Block 0 (ADVISORY)
  {7, 45}, // #30: 465
  {7, 50}, // #31: 470
  // Block 1 (PERIOD 1)
  {8, 35}, // #32: 515
  {8, 41}, // #33: 521
  // Block 2 (PERIOD 2)
  {8, 44}, // #34: 524
  {9, 32}, // #35: 572
  // Block 3 (ADVISORY ACTIVITY)
  {9, 35}, // #36: 575
  {10, 19}, //#37: 619
  // Block 4 (PERIOD 3)
  {10, 22}, // #38: 622
  {11, 10}, // #39: 670
  // Block 5 (PERIOD 4)
  {11, 15}, // #40: 675
  {12, 03}, // #41: 723
  // Block 6 (LUNCH)
  {12, 06}, // #42: 726
  {12, 36}, // #43: 756
  // Block 7 (PERIOD 5)
  {12, 39}, // #44: 759
  {13, 27}, // #45: 807
  // Block 8 (PERIOD 6)
  {13, 30}, // #46: 810
  {14, 18}, // #47: 858


  //EXTENDED ADVISORY ACTIVITY:

  //Index (48-63)
  // Block 0 (ADVISORY)
  {7, 45}, // #48: 465
  {8, 15}, // #49: 495
  // Block 1 (PERIOD 1)
  {8, 18}, // #50: 498
  {9, 10}, // #51: 550
  // Block 2 (PERIOD 2)
  {9, 13}, // #52: 553
  {10, 05}, // #53: 605
  // Block 3 (PERIOD 3)
  {10, 8}, // #54: 608
  {11, 00}, // #55: 660
  // Block 4 (PERIOD 4)
  {11, 03}, // #56: 663
  {11, 55}, // #57: 715
  // Block 5 (LUNCH)
  {11, 58}, // #58: 718
  {12, 28}, // #59: 748
  // Block 6 (PERIOD 5)
  {12, 31}, // #60: 751
  {13, 23}, // #61: 803
  // Block 7 (PERIOD 6)
  {13, 26}, // #62: 806
  {14, 18}, // #63: 858
};

// Converts the hours and minutes of the array into # of minutes since 12:00 am
int convert_time(int hrs, int mns) {
  int conv_time;
  conv_time = hrs * 60;
  conv_time = conv_time + mns;
  return conv_time;
};


void setup() {

  Serial.begin(115200);
  int conv_time;
  int i;

  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

  //Setup Pins (change value with defines up top)
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);


  int hrs = timeClient.getHours() + DAYLIGHTSAVINGS;
  int mins = timeClient.getMinutes();
  int secs = timeClient.getSeconds();
  convert_time(hrs, mins);
  int nowTime = conv_time;



  // Gets initial block
  for ( i = 0; i < 8; i += 2) {
    int startPeriod = convert_time(schReg[i][0], schReg[i][1]);
    int nextPeriod = convert_time(schReg[i + 2][0], schReg[i + 2][1]);

    if (nowTime >= startPeriod && nowTime <= nextPeriod) {
      break;
    }

  }

  block = i;

}

void loop() {
  timeClient.update();

  Serial.print(timeClient.getDay()); 

  //Date and time print statements
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours() + DAYLIGHTSAVINGS);
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.print(timeClient.getSeconds());
  Serial.print(" ");
  //Serial.println(timeClient.getFormattedTime());

  int conv_time;
  int hrs = timeClient.getHours() + DAYLIGHTSAVINGS;
  int mins = timeClient.getMinutes();
  int sec = timeClient.getSeconds();
  int nowTime = convert_time(hrs, mins);

  // Gets time values for current period
  int startPeriod = convert_time(schReg[block][0], schReg[block][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[block + 1][0], schReg[block + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[block + 2][0], schReg[block + 2][1]);  //red is on between endPeriod and nextPeriod

  //Uncomment to see the values of nowTime, StartPeriod, endPeriod, nextPeriod, and block
  Serial.print("nowTime: ");
  Serial.print(nowTime);
  Serial.print(" StartP: ");
  Serial.print(startPeriod);
  Serial.print("  ");
  Serial.print("EndP: ");
  Serial.print(endPeriod);
  Serial.print("  ");
  Serial.print("NextP: ");
  Serial.print(nextPeriod);
  Serial.print("  ");
  Serial.print("Block: ");
  Serial.print(block/2);


  //delay(1000);



  // Function to turn off the lights when theres no school
  if (nowTime < convert_time(schReg[schStart][0], schReg[schStart][1]) || nowTime >= convert_time(schReg[schEnd][0], schReg[schEnd][1])) {
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    block = schIndex;
    Serial.print(" (OUTSIDE SCHOOL)");

    //Turn only red light on during lunch time
  } else if ((nowTime >= convert_time(schReg[lunch][0], schReg[lunch][1])) && nowTime <= convert_time(schReg[lunch + 1][0], schReg[lunch + 1][1])) {
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_ON);
    Serial.print(" (LUNCH)");

    // Class time
  } else if (nowTime < endPeriod - WARNING_TIME) {
    digitalWrite(GREEN_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.print(" (CLASS)");

    // Five minutes before period ends
  } else if (nowTime < endPeriod) {
    digitalWrite(YELLOW_PIN, LIGHT_ON);
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.print(" (WARNING)");

    // Passing time
  } else if (nowTime >= endPeriod && nowTime <= nextPeriod) {
    Serial.print(" (PASSING)");
    digitalWrite(RED_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);

    // Cycle the light and go to next block
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED_PIN, LIGHT_OFF);
    digitalWrite(GREEN_PIN, LIGHT_ON);
    Serial.print(" !NEW PERIOD!");
    block += 2;
  }

  //Indicates on the Serial monitor which light is on

  if (digitalRead(GREEN_PIN) ==  LIGHT_ON) {
    Serial.print(" |GREEN|");
  }
  if (digitalRead(YELLOW_PIN) == LIGHT_ON) {
    Serial.print(" |YELLOW|");
  }
  if (digitalRead(RED_PIN) ==  LIGHT_ON) {
    Serial.print(" |RED|");
  }

  Serial.println();
  delay(1000);

}
