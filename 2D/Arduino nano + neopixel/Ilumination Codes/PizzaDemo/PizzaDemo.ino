#include <Adafruit_NeoPixel.h>
#define PIN_NEO_PIXEL 6// Arduino pin that connects to NeoPixel
#define NUM_PIXELS 12// The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 200

volatile unsigned int count = 0;
unsigned long next = 500;
unsigned int temp = 0;
volatile unsigned long lastMillis = 0;
int pin_ir=3;
int num_pixeles=0;
volatile int threshold = 2; //miliseconds between possible interrupts
volatile int mode=0;
Adafruit_NeoPixel neoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
 neoPixel.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 Serial.begin(115200);
 pinMode(pin_ir, INPUT_PULLUP);               //setup pin 3 with internal pull-up
 delayMicroseconds(50);
 attachInterrupt(digitalPinToInterrupt(pin_ir), ir_detect, CHANGE); //En el nano hay que conectarlo al pin 8 aunque aqui ponga 10
 //Usamos change ya que rising y falling hacen lo mismo. Cada deteccion es media vuelta.
 neoPixel.clear(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
 neoPixel.show(); // send the updated pixel colors to the NeoPixel hardware.


}

void loop() {


}
void paint(int num_leds){
  


}
void ir_detect()
{
  Serial.println(mode);
  noInterrupts();
  if(millis() - lastMillis > threshold){ 
    if (mode==0){
      for (int pixel = 0; pixel < NUM_PIXELS; pixel++){
        neoPixel.setPixelColor(pixel, neoPixel.Color(0, 255, 0));
      }
      mode=1;

    }else{
      for (int pixel = 0; pixel < NUM_PIXELS; pixel++){
        neoPixel.setPixelColor(pixel, neoPixel.Color(255, 0, 0));
      }
      mode=0;
    }
    lastMillis = millis();
    neoPixel.show();// send the updated pixel colors to the NeoPixel hardware.
  }
  

  interrupts();
      
}