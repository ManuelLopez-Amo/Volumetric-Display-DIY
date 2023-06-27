//Libraries of the LEDs
#include <Adafruit_NeoPixel.h>
#include <LinkedList.h>

//Libraries of the communications
#include <WiFi.h>
#include <WiFiUdp.h>

#define PIN_NEO_PIXEL 19  // Arduino pin that connects to NeoPixel
#define NUM_PIXELS 21     // The number of LEDs (pixels) on NeoPixel

#define DELAY_INTERVAL 200
#define NUM_STATES 89  //Dont forget to update this!!!!
#define pin_ir 12

struct Instruction {
  short angle;
  LinkedList<int> changingPins;
  LinkedList<uint32_t> colored;
};

Instruction instructions[NUM_STATES];

volatile unsigned long pseudospins = 0;
volatile unsigned long tiempos[NUM_STATES];
volatile unsigned long lastMillis = 0;
volatile unsigned long previousMillis = 0;
volatile unsigned long lastMillisTh = 0;
volatile unsigned long sum = 0;

volatile short threshold = 1000;  //miliseconds between possible interrupts

volatile int checkActual = 0;

short checkpoints[]={0, 9, 18, 21, 24, 30, 36, 39, 42, 45, 48, 51, 54, 57, 63, 66, 69, 75, 78, 81, 84, 102, 105, 108, 111, 114, 117, 120, 123, 126, 129, 132, 135, 138, 141, 144, 147, 153, 159, 162, 165, 171, 174, 183, 198, 201, 204, 207, 210, 213, 216, 219, 222, 225, 228, 231, 234, 237, 240, 243, 246, 249, 252, 258, 261, 267, 273, 276, 279, 285, 288, 291, 294, 297, 300, 303, 306, 309, 312, 315, 318, 321, 324, 327, 330, 333, 336, 339, 342};
int rawChangingPins[]={7, 8, -1, 9, 10, -1, 11, -1, 9, 10, -1, 8, -1, 3, 12, -1, 8, 13, -1, 5, 6, 8, 14, -1, 1, 14, -1, 7, 13, -1, 4, 7, 10, -1, 9, 12, 14, 17, -1, 5, 8, 11, 12, 15, 16, 18, -1, 13, 18, -1, 12, 17, -1, 1, -1, 16, -1, 4, 12, -1, 5, 11, 13, 14, 15, -1, 11, -1, 9, 10, 11, -1, 10, 11, -1, 9, 15, -1, 13, 14, -1, 12, -1, 1, 5, 16, -1, 1, 12, -1, 4, 17, -1, 18, -1, 6, 13, -1, 6, 9, 12, 13, 14, 16, -1, 4, 15, 17, 18, -1, 8, 10, 11, 13, -1, 14, -1, 5, 8, -1, 7, -1, 6, 14, -1, 4, 13, -1, 9, -1, 10, 11, -1, 12, -1, 10, -1, 11, -1, 8, 9, -1, 6, 7, -1, 5, 6, -1, 4, 6, -1, 4, 8, -1, 7, 11, -1, 3, 9, 10, -1, 2, 12, -1, 7, 9, 10, -1, 13, -1, 2, 11, -1, 7, 13, -1, 9, -1, 13, -1, 10, 14, 15, -1, 6, 11, 13, -1, 15, -1, 8, 9, 15, 16, -1, 12, -1, 6, 7, 16, -1, 10, -1, 11, -1, 11, 15, -1, 11, -1, 10, 11, 15, -1, 8, 9, -1, 5, 11, 14, 15, -1, 4, 10, -1, 8, 9, 12, 14, -1, 7, -1, 8, 11, 12, -1, 6, 15, -1, 9, 11, 13, 16, -1, 4, 9, 15, 16, -1, 8, 12, 14, -1, 13, -1, 2, 13, -1, 1, 5, 8, 12, -1, 9, 12, -1, 6, 13, -1, 3, 10, 13, -1, 3, 7, 10, 11, -1, 8, 12, -1, 4, 5, 9, -1, 5, -1, 6, -1};
uint32_t rawColored[]={16062227, 16062227, -1, 16062227, 16062227, -1, 16761095, -1, 16761095, 16761095, -1, 16761095, -1, 13780, 16761095, -1, 16062227, 16761095, -1, 13780, 13780, 16761095, 16761095, -1, 16062227, 0, -1, 16761095, 0, -1, 16771899, 13780, 13780, -1, 13780, 0, 10368531, 10368531, -1, 16771899, 13780, 13780, 13780, 10368531, 10368531, 10368531, -1, 10368531, 0, -1, 10368531, 0, -1, 13780, -1, 0, -1, 13780, 0, -1, 13780, 0, 0, 0, 0, -1, 13780, -1, 0, 0, 0, -1, 13780, 13780, -1, 13780, 10368531, -1, 10368531, 10368531, -1, 10368531, -1, 0, 16771899, 10368531, -1, 16062227, 13780, -1, 16771899, 10368531, -1, 10368531, -1, 16771899, 13780, -1, 13780, 16761095, 16761095, 0, 0, 0, -1, 13780, 0, 0, 0, -1, 16761095, 16761095, 16761095, 16761095, -1, 16761095, -1, 16062227, 16062227, -1, 16062227, -1, 16062227, 0, -1, 16062227, 0, -1, 16062227, -1, 16062227, 16062227, -1, 0, -1, 0, -1, 0, -1, 0, 0, -1, 16761095, 0, -1, 16761095, 0, -1, 13780, 16761095, -1, 16761095, 6699306, -1, 6699306, 6699306, -1, 16761095, 6699306, 6699306, -1, 16062227, 6699306, -1, 16761095, 16761095, 16761095, -1, 6699306, -1, 16761095, 16761095, -1, 6699306, 0, -1, 6699306, -1, 6699306, -1, 6699306, 16062227, 16062227, -1, 6699306, 6699306, 16062227, -1, 0, -1, 16761095, 16761095, 16062227, 16062227, -1, 16062227, -1, 16761095, 16761095, 0, -1, 16761095, -1, 16761095, -1, 0, 0, -1, 16761095, -1, 3289650, 3289650, 16062227, -1, 3289650, 3289650, -1, 3289650, 16761095, 0, 0, -1, 3289650, 16761095, -1, 16761095, 16761095, 16761095, 16062227, -1, 3289650, -1, 3289650, 0, 0, -1, 3289650, 16062227, -1, 3289650, 16761095, 0, 16062227, -1, 16761095, 16761095, 0, 0, -1, 16761095, 16761095, 0, -1, 16761095, -1, 13780, 0, -1, 13780, 16761095, 3289650, 0, -1, 3289650, 16761095, -1, 0, 16761095, -1, 13780, 3289650, 0, -1, 16062227, 0, 0, 0, -1, 0, 0, -1, 16062227, 0, 0, -1, 16062227, -1, 16062227, -1};

int pixel;
short angle;
short listSize;

const char* ssid = "display2D";
const char* password = "11111111";
WiFiUDP udp;
unsigned int udpPort=1234;

char pktbuf[30000]; //buffer to store udp data

Adafruit_NeoPixel neoPixel(NUM_PIXELS, PIN_NEO_PIXEL, NEO_GRB + NEO_KHZ800);

void setup() {
  neoPixel.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(9600);
  pinMode(pin_ir, INPUT_PULLUP);  //setup pin 3 with internal pull-up
  delayMicroseconds(50);
  attachInterrupt(digitalPinToInterrupt(pin_ir), ir_detect, CHANGE);  //En el nano hay que conectarlo al pin 8 aunque aqui ponga 10
  neoPixel.clear();
  neoPixel.show();

  converter();  //We initialize our data structure

  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  Serial.println(WiFi.localIP());
  Serial.println("Status: Connected");

  /* Enable udp */
  udp.begin(udpPort);
  Serial.println(udpPort);

}

void loop() {

  //LED Painting

  if (tiempos[checkActual] != 0 && micros() >= tiempos[checkActual] && checkActual < NUM_STATES) {

    listSize = instructions[checkActual].changingPins.size();

    for (pixel = 0; pixel < listSize; pixel++) {
      neoPixel.setPixelColor(instructions[checkActual].changingPins.get(pixel), instructions[checkActual].colored.get(pixel));
    }

    neoPixel.show();
    checkActual++;
  }

  //Connection code

  int rp=udp.parsePacket(); //check for udp packets
  if(rp)
  { 
      udp.read(pktbuf,30000);
      Serial.println(pktbuf);
  }

}

void ir_detect() {

  noInterrupts();
  if (micros() - lastMillisTh > threshold) {
    lastMillisTh = micros();
    pseudospins++;
    if (pseudospins % 2 == 0) {
      checkActual = 0;
      previousMillis = lastMillis;
      lastMillis = micros();
      sum = lastMillis - previousMillis;
      for (angle = 0; angle < NUM_STATES; angle++) {
        tiempos[angle] = lastMillis + sum * (checkpoints[angle]) / 360;
      }
    }
  }
  interrupts();
}

void converter() {

  int elemento = 0;
  int index = 0;
  uint32_t elemColor = 0;

  for (short numAngle = 0; numAngle < NUM_STATES; numAngle++) {
    //We introduce: ANGLE
    instructions[numAngle].angle = checkpoints[numAngle];
    elemento = rawChangingPins[index];

    while (elemento != -1) {
      instructions[numAngle].changingPins.add(elemento);
      elemColor = rawColored[index];
      instructions[numAngle].colored.add(elemColor);

      index++;
      elemento = rawChangingPins[index];
    }
    index++;
  }
}