#include <Adafruit_NeoPixel.h>
#define PIN        6
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int x = 0;
int r;
int g;
int b;
void setup() {
 Serial.begin(9600);
 pinMode(9, INPUT);
 pixels.begin();
}

void loop() {
  int p = digitalRead(13);
  int s = digitalRead(12);
  int u = digitalRead(11);
  int t = digitalRead(10);
  int z = digitalRead(9);
  int y = round(analogRead(5) / 127.875);
  
  

  for(int i = 0; i < y; i++) {

    pixels.setPixelColor(i, pixels.Color(r, g, b));
   //x = x + 50;


}
    
    delay(10);
     pixels.show();
     pixels.clear();

    
     if(z == HIGH) {
      g = g - g;
      b = b - b;
      r++;
     }

  if(t == HIGH) {
      r = r- r;
      b = b - b;
      g++;
     }
  if(u == HIGH) {
      r = r - r;
      g = g - g;
      b++;
     }
    
     
    

   
 Serial.print("blue:    ");
  Serial.println(b);
  Serial.print("green:   ");
  Serial.println(g);
  Serial.print("red:    ");
  Serial.println(r);
  delay(20);

}
