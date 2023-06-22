#include <Adafruit_NeoPixel.h>
#define PIN_NEO_PIXEL 6// Arduino pin that connects to NeoPixel
#define NUM_PIXELS 12// The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 200
#define NUM_STATES 7

volatile unsigned long pseudospins = 0;
volatile unsigned long tiempos[NUM_STATES];
volatile unsigned int count = 0;
unsigned long next = 500;
unsigned int temp = 0;
volatile unsigned long lastMillis = 0;
volatile unsigned long previousMillis = 0;
volatile unsigned long lastMillisTh = 0;

int pin_ir=3;
int num_pixeles=0;
volatile int threshold = 1000; //miliseconds between possible interrupts
volatile int angulo_linea = 180;
volatile int mode=0;

volatile byte checkActual=0; 

int checkpoints[NUM_STATES]={60,70,80,90,100,110,120};
int comienzos[NUM_STATES]={3,3,2,2,3,3,0};
int finales[NUM_STATES]={4,8,10,9,10,6,0};


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
 //neoPixel.clear(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  unsigned long act_milis=micros();

  if (act_milis>=tiempos[checkActual]){
    neoPixel.clear();    
    for (int pixel = comienzos[checkActual]; pixel < finales[checkActual]; pixel++){
      neoPixel.setPixelColor(pixel, neoPixel.Color(0, 0, 255));
    }
    checkActual++;
  }
  neoPixel.show();

}

void ir_detect(){

  noInterrupts();

  if(micros() - lastMillisTh > threshold){
    lastMillisTh = micros();
    pseudospins++;
    if(pseudospins%2 == 0){
      checkActual = 0;
      mode=0;
      //Serial.println("MEDIA VUELTA"); // Dos veces
      previousMillis = lastMillis;
      lastMillis = micros();
      for(int angle = 0; angle < NUM_STATES; angle++){
        tiempos[angle]= lastMillis+(lastMillis-previousMillis)*(checkpoints[angle])/360;
      }
    }
  }
  
  interrupts();
}