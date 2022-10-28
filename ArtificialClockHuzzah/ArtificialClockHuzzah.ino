#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#define DAYLIGHTSAVINGS 1
#define GREEN_PIN 5
#define YELLOW_PIN 15
#define RED_PIN 16
#define LIGHT_ON 1
#define LIGHT_OFF 0
#define WARNING_TIME 5

int currentPeriod = 0;
int period = 0;
int nowTime = 466;


int schReg[65][2] {

  //REGULAR TIME SCHEDULE:
  //Index (0 - 15)
  // Block 0 (ADVISORY)
  {7, 45},  //465
  {7, 49},  //469
  // Block 1 (PERIOD 1)
  {7, 52},  //472
  {8, 48},  //528
  // Block 2 (PERIOD 2)
  {8, 51},  //531
  {9, 47},  //587
  // Block 3 (PERIOD 3)
  {9, 50},  //590
  {10, 46}, //640
  // Block 4 (PERIOD 4)
  {10, 51}, //651
  {11, 47}, //707
  // Block 5 (LUNCH)
  {11, 50}, //710
  {12, 18}, //738
  // Block 6 (PERIOD 5)
  {12, 23}, //743
  {13, 19}, //799
  // Block 7 (PERIOD 6)
  {13, 22}, //802
  {14, 18}, //858


  //EARLY RELEASE SCHEDULE:

  //Index (16-29)
  // Block 0 (ADVISORY)
  {7, 45}, //465
  {7, 50}, //470
  // Block 1 (PERIOD 1)
  {7, 53}, //473
  {8, 35}, //515
  // Block 2 (PERIOD 2)
  {8, 38}, //518
  {9, 16}, //556
  // Block 3 (PERIOD 3)
  {9, 19}, //559
  {9, 57}, //597
  // Block 4 (PERIOD 4)
  {10, 00}, //600
  {10, 38}, //638
  // Block 5 (PERIOD 5)
  {10, 41}, //641
  {11, 19}, //679
  // Block 6 (PERIOD 6)
  {11, 22}, //682
  {12, 00}, //720

  //ADVISORY ACTIVITY SCHEDULE:

  //Index (30-47)
  // Block 0 (ADVISORY)
  {7, 45}, //465
  {7, 50}, //470
  // Block 1 (PERIOD 1)
  {8, 35}, //515
  {8, 41}, //521
  // Block 2 (PERIOD 2)
  {8, 44}, //524
  {9, 32}, //572
  // Block 3 (ADVISORY ACTIVITY)
  {9, 35}, //575
  {10, 19}, //619
  // Block 4 (PERIOD 3)
  {10, 22}, //622
  {11, 10}, //670
  // Block 5 (PERIOD 4)
  {11, 15}, //675
  {12, 03}, //723
  // Block 6 (LUNCH)
  {12, 06}, //726
  {12, 36}, //756
  // Block 7 (PERIOD 5)
  {12, 39}, //759
  {13, 27}, //807
  // Block 8 (PERIOD 6)
  {13, 30}, //810
  {14, 18}, //858


  //EXTENDED ADVISORY ACTIVITY:

  //Index (48-65)
  // Block 0 (ADVISORY)
  {7, 45}, //465
  {8, 15}, //495
  // Block 1 (PERIOD 1)
  {8, 18}, //498
  {9, 10}, //550
  // Block 2 (PERIOD 2)
  {9, 13}, //553
  {10, 05}, //605
  // Block 3 (PERIOD 3)
  {10, 8}, //608
  {11, 00}, //660
  // Block 4 (PERIOD 4)
  {11, 03}, //663
  {11, 55}, //715
  // Block 5 (LUNCH)
  {11, 58}, //718
  {12, 28}, //748
  // Block 6 (PERIOD 5)
  {12, 31}, //751
  {13, 23}, //803
  // Block 7 (PERIOD 6)
  {13, 26}, //806
  {14, 18}, //858
};

// Converts the hours and minutes of the array into # of minutes
int convert_time(int hrs, int mns) {
  int conv_time;
  conv_time = hrs * 60;
  conv_time = conv_time + mns;
  return conv_time;
};

void setup() {
  int i;
  Serial.begin(115200);

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);


  // Gets initial block
  for ( i = 0; i < 16; i += 2) {
    int startPeriod = convert_time(schReg[i][0], schReg[i][1]);
    int nextPeriod = convert_time(schReg[i + 2][0], schReg[i + 2][1]);

    if (nowTime >= startPeriod && nowTime <= nextPeriod) {
      break;
    }
  }
  period = i ;
  Serial.print("period=");
  Serial.println(period);
}

void loop() {
  nowTime += 1;
  // Gets time values for current period

  int startPeriod = convert_time(schReg[period][0], schReg[period][1]); //green is on between startPeriod and endPeriod - 5
  int endPeriod = convert_time(schReg[period + 1][0], schReg[period + 1][1]); // yellow is on between endPeriod - 5 and endPeriod
  int nextPeriod = convert_time(schReg[period + 2][0], schReg[period + 2][1]);  //red is on between endPeriod and nextPeriod

  Serial.print("nowTime: ");
  Serial.print(nowTime);
  Serial.print(" period = ");
  Serial.print(period/2);

  // In class time
  if (nowTime <= endPeriod - WARNING_TIME) {
    digitalWrite(GREEN_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.println(" GREEN ON");

    // Five minutes before period ends
  } else if (nowTime < endPeriod) {
    digitalWrite(YELLOW_PIN, LIGHT_ON);
    digitalWrite(GREEN_PIN, LIGHT_OFF);
    digitalWrite(RED_PIN, LIGHT_OFF);
    Serial.println(" YELLOW ON");

    // Passing time
  } else if (nowTime >= endPeriod && nowTime <= nextPeriod) {
    digitalWrite(RED_PIN, LIGHT_ON);
    digitalWrite(YELLOW_PIN, LIGHT_OFF);

    // Cycle the light testing and go to next period
  } else if (nowTime >= nextPeriod) {
    digitalWrite(RED_PIN, LIGHT_OFF);
    digitalWrite(GREEN_PIN, LIGHT_ON);
    Serial.println(" RED ON");
    period += 2;
  }

  delay(500); 
}
