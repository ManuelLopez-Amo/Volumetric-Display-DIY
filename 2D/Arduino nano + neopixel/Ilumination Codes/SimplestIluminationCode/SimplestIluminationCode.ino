#include <Adafruit_NeoPixel.h>

#define PIN_NEO_PIXEL 2// Arduino pin that connects to NeoPixel
#define NUM_PIXELS 10// The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 200

Adafruit_NeoPixel neoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
 neoPixel.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void loop() {
 neoPixel.clear(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called

 for (int pixel = 0; pixel < NUM_PIXELS; pixel++) { // for each pixel
  neoPixel.setPixelColor(pixel, neoPixel.Color(0, 255, 0)); // it only takes effect if pixels.show() is called
  neoPixel.show();// send the updated pixel colors to the NeoPixel hardware.

  delay(DELAY_INTERVAL); // pause between each pixel
 }

 neoPixel.clear();
 neoPixel.show(); // send the updated pixel colors to the NeoPixel hardware.
 delay(500);// off time

 for (int pixel = 0; pixel < NUM_PIXELS; pixel++) { // for each pixel
  neoPixel.setPixelColor(pixel, neoPixel.Color(255, 0, 0)); // it only takes effect if pixels.show() is called
 }
 neoPixel.show(); // send the updated pixel colors to the NeoPixel hardware.
 delay(500);// on time

 neoPixel.clear();
 neoPixel.show(); // send the updated pixel colors to the NeoPixel hardware.
 delay(500);// off time
}