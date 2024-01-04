#include <Q2HX711.h>

const byte hx711_data_pin = 6;
const byte hx711_clock_pin = 4;
int t = 0;
int interval = 1;
float Load_N = 0.0;

// now for linear scale
const int pinA = 2, pinB = 3, phaseLookup[] = {0,-1,1,-1,1,0,-1,-1,-1,1,0,1,1,1,-1,0};

volatile bool A = false, B = false;//, updated = false;
volatile int counter = 0, phase = 0;

Q2HX711 hx711(hx711_data_pin, hx711_clock_pin);

void setup() {
  Serial.begin(9600);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  
  attachInterrupt(digitalPinToInterrupt(pinA), trig, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinB), trig, CHANGE);
  
}
//Serial.println("Loadcell data for sample ");
//Serial.println("time interval is %d",interval);
void loop() {
//  t = millis();
//  t = t/100;
  Serial.print(t);
  Serial.print(" ; ");
  Serial.print(millis());
  Serial.print(" ; ");
  Load_N = 0.0075*(hx711.read()/100.0)-622.26;
  Serial.print(Load_N);
  Serial.print(" ; ");
  Serial.println(String(counter));
  delay(interval);
  t++;
}

void trig()
{
  A = digitalRead(pinA);
  B = digitalRead(pinB);

  phase <<= 1;
  phase |= A;
  phase <<= 1;
  phase |= B;
  phase &= 0xF;

  counter += phaseLookup[phase];
}


//  Serial.begin(115200);
  
