#include <Adafruit_NeoPixel.h>
#define PIN_NEO_PIXEL 6  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 12    // The number of LEDs (pixels) on NeoPixel
#define DELAY_INTERVAL 200
#define NUM_STATES 155
#define pin_ir 3




volatile unsigned long pseudospins = 0;
volatile unsigned long tiempos[NUM_STATES];
unsigned int temp = 0;
volatile unsigned long lastMillis = 0;
volatile unsigned long previousMillis = 0;
volatile unsigned long lastMillisTh = 0;

//int pin_ir=3;

volatile int threshold = 1000;  //miliseconds between possible interrupts

volatile int mode = 0;

volatile byte checkActual = 0;



int checkpoints[NUM_STATES] = { 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244 };
int comienzos[NUM_STATES] = { 5, 4, 4, 4, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 5, 5, 6, 0 };   //el primer led que se enciende de la tira en cada estado
int finales[NUM_STATES] = { 11, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 11, 7, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 11, 0 };  //el ultimo led que se enciende de la tira en cada estado


Adafruit_NeoPixel neoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

uint32_t Colores[3] = {
  neoPixel.Color(255, 255, 0),
  neoPixel.Color(192, 192, 192),
  neoPixel.Color(128, 0, 0)
};

int rangeDonuts[3] = { 2, 6, 11 };


void setup() {
  neoPixel.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(115200);
  pinMode(pin_ir, INPUT_PULLUP);  //setup pin 3 with internal pull-up
  delayMicroseconds(50);
  attachInterrupt(digitalPinToInterrupt(pin_ir), ir_detect, CHANGE);  //En el nano hay que conectarlo al pin 8 aunque aqui ponga 10
  //Usamos change ya que rising y falling hacen lo mismo. Cada deteccion es media vuelta.
  neoPixel.clear();  // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  neoPixel.show();   // send the updated pixel colors to the NeoPixel hardware.
}

void loop() {
  //neoPixel.clear(); // set all pixel colors to 'off'. It only takes effect if pixels.show() is called
  unsigned long act_milis = micros();

  if (act_milis >= tiempos[checkActual]) {
    neoPixel.clear();
    if (finales[checkActual] > 0) {
      for (int pixel = comienzos[checkActual]; pixel <= finales[checkActual]; pixel++) {
        neoPixel.setPixelColor(pixel, neoPixel.Color(0, 0, 255));
      }
    } else{

    }
    neoPixel.show();
    checkActual++;
  }
}

void ir_detect() {

  noInterrupts();

  if (micros() - lastMillisTh > threshold) {
    lastMillisTh = micros();
    pseudospins++;
    if (pseudospins % 2 == 0) {
      checkActual = 0;
      mode = 0;
      //Serial.println("MEDIA VUELTA"); // Dos veces
      previousMillis = lastMillis;
      lastMillis = micros();
      for (int angle = 0; angle < NUM_STATES; angle++) {
        tiempos[angle] = lastMillis + (lastMillis - previousMillis) * (checkpoints[angle]) / 360;
      }
    }
  }

  interrupts();
}