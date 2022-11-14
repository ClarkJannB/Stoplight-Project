//Be able to change the value of a variable from the Adafruit dashboard
//This will control which schedule the stoplight will follow
#include "config.h"
// set up the feeds
AdafruitIO_Feed *schStart = io.feed("schStart");
AdafruitIO_Feed *schEnd = io.feed("schEnd");
AdafruitIO_Feed *schJump = io.feed("schJump");
AdafruitIO_Feed *lunch = io.feed("lunch");

unsigned char schStartTestVal = 0;
unsigned char schEndTestVal = 0;
unsigned char schJumpTestVal = 0;
unsigned char lunchTestVal = 0;

void setup() {

  // start the serial connection
  Serial.begin(115200);
  // wait for serial monitor to open
  while (! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the feeds.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  schStart->onMessage(schStartValue);
  schEnd->onMessage(schEndValue);
  schJump->onMessage(schJumpValue);
  lunch->onMessage(lunchValue);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  // make sure all feeds get their current values right away
  schStart->get();
  schEnd->get();
  schJump->get();
  lunch->get();


}

void loop() {
  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();


}



// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'green' feed in the setup() function above.
void schStartValue(AdafruitIO_Data * data) {

  //convert data to integer
  int schStartRead = data->toInt();
  Serial.print("received <- ");

  // we can use feedName() in order to find out which feed the message came from.
  Serial.print(data->feedName());
  Serial.print(" ");

  // print out the received value
  Serial.println(schStartRead);

  schStartTestVal = schStartRead;

}

void schEndValue (AdafruitIO_Data * data) {
  //convert data to integer
  int schEndRead = data->toInt();
  Serial.print("received <-");

  Serial.print(data->feedName());
  Serial.print(" ");

  // print out the received value
  Serial.println(schEndRead);

  schEndTestVal = schEndRead;
}


void schJumpValue (AdafruitIO_Data * data) {
  //convert data to integer
  int schJumpRead = data->toInt();
  Serial.print("received <-");

  Serial.print(data->feedName());
  Serial.print(" ");

  // print out the received value
  Serial.println(schJumpRead);

  schJumpTestVal = schJumpRead;
}

void lunchValue (AdafruitIO_Data * data) {
  //convert data to integer
  int lunchRead = data->toInt();
  Serial.print("received <-");

  Serial.print(data->feedName());
  Serial.print(" ");

  // print out the received value
  Serial.println(lunchRead);

  lunchTestVal = lunchRead;
}
