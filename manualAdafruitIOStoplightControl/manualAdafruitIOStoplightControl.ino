#include "config.h"
// digital define pins
#define GREEN_PIN 5
#define YELLOW_PIN 2
#define RED_PIN 15

// set up the feeds
AdafruitIO_Feed *green = io.feed("green");
AdafruitIO_Feed *yellow = io.feed("yellow");
AdafruitIO_Feed *red = io.feed("red");

void setup() {

  // set button pin as an output
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while (! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'digital' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  green->onMessage(greenValue);
  yellow->onMessage(yellowValue);
  red->onMessage(redValue);

  // wait for a connection
  while (io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

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
void greenValue(AdafruitIO_Data * data) {

  Serial.print("received <- ");

  if (data->toPinLevel() == HIGH)
    Serial.println(" GREEN HIGH");
  else
    Serial.println(" GREEN LOW");


  digitalWrite(GREEN_PIN, data->toPinLevel());

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'yellow' feed in the setup() function above.
void yellowValue(AdafruitIO_Data * data) {

  Serial.print("received <- ");

  if (data->toPinLevel() == HIGH)
    Serial.println(" YELLOW HIGH");
  else
    Serial.println(" YELLOW LOW");


  digitalWrite(YELLOW_PIN, data->toPinLevel());

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'red' feed in the setup() function above.
void redValue(AdafruitIO_Data * data) {

  Serial.print("received <- ");

  if (data->toPinLevel() == HIGH)
    Serial.println(" RED HIGH");
  else
    Serial.println(" RED LOW");


  digitalWrite(RED_PIN, data->toPinLevel());

}
