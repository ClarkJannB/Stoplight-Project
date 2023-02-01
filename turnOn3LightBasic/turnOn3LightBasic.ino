//define pin #
//In this case I'm using it to turn on and turn off green, yellow, and red leds at a basic level
#define GREEN_PIN 4
#define YELLOW_PIN 5
#define RED_PIN 15

void setup() {
  //setup pin modes
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(YELLOW_PIN, HIGH);
  digitalWrite(RED_PIN, LOW);
  //while (1);
  //Serial
  Serial.begin(115200);
}

void loop() {
  digitalWrite(GREEN_PIN, HIGH);
  Serial.print("GREEN_PIN -> ");
  Serial.println(digitalRead(GREEN_PIN));
  delay(2000);
  digitalWrite(GREEN_PIN, LOW);
  Serial.print("GREEN_PIN -> ");
  Serial.println(digitalRead(GREEN_PIN));
  delay(2000);

  digitalWrite(YELLOW_PIN, HIGH);
  Serial.print("YELLOW_PIN -> ");
  Serial.println(digitalRead(YELLOW_PIN));
  delay(2000);
  digitalWrite(YELLOW_PIN, LOW);
  Serial.print("YELLOW_PIN -> ");
  Serial.println(digitalRead(YELLOW_PIN));
  delay(2000);

  digitalWrite(RED_PIN, HIGH);
  Serial.print("RED_PIN -> ");
  Serial.println(digitalRead(RED_PIN));
  delay(2000);
  digitalWrite(RED_PIN, LOW);
  Serial.print("RED_PIN -> ");
  Serial.println(digitalRead(RED_PIN));
  delay(2000);
}
