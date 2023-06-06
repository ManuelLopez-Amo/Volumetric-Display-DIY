#include <Adafruit_NeoPixel.h>
#define PIN_NEO_PIXEL 6// Arduino pin that connects to NeoPixel
#define NUM_PIXELS 12// The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 200

volatile long pseudospins = 0;
volatile unsigned int count = 0;
unsigned long next = 500;
unsigned int temp = 0;
volatile unsigned long lastMillis = 0;
volatile unsigned long lastMillisTh = 0;
volatile unsigned long previousMillis = 0;
volatile unsigned long startLine = 0;
volatile unsigned long finalLine = 0;
int pin_ir=3;
int num_pixeles=0;
volatile int threshold = 2; //miliseconds between possible interrupts
volatile int angulo_linea = 180;
volatile int ancho_linea = 45;
volatile int mode=0;
volatile bool change=false;


Adafruit_NeoPixel neoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);
int pruebo = 1;
uint32_t Colores[3] = {
  neoPixel.Color(255, 255, 0),
  neoPixel.Color(192, 192, 192),
  neoPixel.Color(128, 0, 0),
};

int rangeDonuts[3]{2,6,10};


volatile int colorDonut=0;



void setup() {
 neoPixel.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
 Serial.begin(115200);
 //pinMode(pin_ir, INPUT_PULLUP);               //setup pin 3 with internal pull-up
 delayMicroseconds(50);
 attachInterrupt(digitalPinToInterrupt(pin_ir), ir_detect, HIGH); //En el nano hay que conectarlo al pin 8 aunque aqui ponga 10
 //Usamos change ya que rising y falling hacen lo mismo. Cada deteccion es media vuelta.
 neoPixel.clear(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
 neoPixel.show(); // send the updated pixel colors to the NeoPixel hardware.


}

void loop() {
 //neoPixel.clear(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  unsigned long act_milis=millis();


  if (change){
    change=false;
    for (int pixel = 0; pixel < NUM_PIXELS; pixel++){
      if (pixel==rangeDonuts[0]){
        neoPixel.setPixelColor(pixel, Colores[colorDonut]);
      }
      else if (pixel==rangeDonuts[1]){
        neoPixel.setPixelColor(pixel, Colores[(colorDonut+1)%3]);
      }

      else if (pixel==rangeDonuts[2]){
        neoPixel.setPixelColor(pixel, Colores[(colorDonut+2)%3]);
      }
      else{
        neoPixel.setPixelColor(pixel, neoPixel.Color(0, 0, 0));
      }
      
    }
    
    neoPixel.show();
  }

}

void ir_detect()
{

  if(millis() - lastMillisTh > threshold){
    lastMillisTh = millis();
    pseudospins++;
    if(pseudospins%2 == 0){

      Serial.println("MEDIA VUELTA"); // Dos veces
      //colorDonut=(colorDonut+1)%3; //si comentas esta linea los colores se quedaran iguales para cada aro de donut
      change=true;
      for (int position = 0; position < 3; position++){
        rangeDonuts[position]=(rangeDonuts[position]+1)%NUM_PIXELS; //si comentas este for los aros del donut no cambiaran
      }
    }

  }

}