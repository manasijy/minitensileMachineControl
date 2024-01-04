// The stepper motor drive is set at 20000 steps/rev for smooth motion at lower cross-head speeds
// setup the arduino (nano used here) pins
// The motor is controlled through the serial monitor i/o
// The pulse delay to control motor is adjusted for drive steps/rev of 20000
// one need to verify the actual cross head speed and adjust the pulse delay accordingly

#include <Wire.h>

int PulsePin = 2; //Stepper pin
int DirectionPin = 3; //Direction pin
int EnablePin = 5; //Stop pinbool setdir;
volatile int cmd_id = 0; // read in from interrupt
volatile bool cmd_in = false;
int pd = 10;
int dir;

void setup(){
  /*Sets all pin to output; the microcontroller will send them(the pins) 
   bits, it will not expect to receive any bits from thiese pins.*/
  Serial.begin(9600);
  pinMode(DirectionPin, OUTPUT);
  pinMode(PulsePin, OUTPUT);
  pinMode(EnablePin, OUTPUT);   
  digitalWrite(EnablePin, LOW);

//timer settings begins
    
  cli();//stop interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCNT1  = 0;//initialize counter value to 0
         // set compare match register for 1hz increments
  OCR1A = 15624;// = (16*10^6) / (1*1024) - 1 (must be <65536)
   TCCR1A |= (1 << WGM01);
         // Set CS01 and CS00 bits for 64 prescaler
  TCCR1A |= (1 << CS01) | (1 << CS00);   
  TIMSK1 |= (1 << OCIE1A);
  interrupts();             // enable all interrupts    
  sei();//allow interrupts
  }//end setup
  
   ISR(TIMER1_COMPA_vect){ 
   TCNT1=0;
   if (Serial.available()){
                            cmd_in = true;
                            cmd_id = Serial.read()-48;
                          }
// digitalWrite(EnablePin, digitalRead(EnablePin) ^ 1); //make the EnablePin = 0 i.e. down
// the ISR stops the motor when a signal is available at the serial  
// later I will change it to read the serial in interger - 0,1,2,3
// for 0 - it will stop the motor
// for 1 - it will move the motor fast in the forward direction
// for 2 - it will move the motor fast in the reverse direction
// for 3 - it will move the motor slow for 10^-3 strain rate
}

//timer settings ends

void loop(){   

  
        switch (cmd_id){
            case 0:
                         digitalWrite(DirectionPin, HIGH); 
                         digitalWrite(PulsePin,LOW);
                         digitalWrite(EnablePin, HIGH);
                         break;
                    

           case 1: //  direction forward  fast                 
                        pd = 1;
                        dir = HIGH;
//                        digitalWrite(EnablePin, LOW);
                        if (EnablePin == HIGH){digitalWrite(EnablePin, LOW);}                       
                        run_motor_slow(dir,pd);
                        cmd_in=false;
                        Serial.print("cmd_id=");
                        Serial.println(cmd_id);
                        Serial.println(cmd_in);
                        break; 
                    
            case 2: //  direction reverse   fast              
                        pd = 1;//1 s-1
                        dir = LOW;
//                        digitalWrite(EnablePin, LOW);
                        if (EnablePin == HIGH){digitalWrite(EnablePin, LOW);}                       
                        run_motor_slow(dir,pd);
                        cmd_in=false;
                        Serial.print("cmd_id=");
                        Serial.println(cmd_id);
                        Serial.println(cmd_in);
                        break; 
                    
            case 3: //test at strain rates equal or slower than 0.001s-1                          
                        pd = 1;//1e-1 s-1  
                        dir = LOW;
//                        digitalWrite(EnablePin, LOW);
                        if (EnablePin == HIGH){digitalWrite(EnablePin, LOW);}
                        run_motor_slow(dir,pd);
                        cmd_in=false; 
                        Serial.print("cmd_id=");
                        Serial.println(cmd_id);
                        Serial.println(cmd_in);                                               
                        break; 
            case 4: //test at strain rates equal or slower than 0.001s-1                          
                        pd = 10;//1e-2 s-1 
                        dir = LOW;
//                        digitalWrite(EnablePin, LOW);
                        if (EnablePin == HIGH){digitalWrite(EnablePin, LOW);}
                        run_motor_slow(dir,pd);
                        cmd_in=false; 
                        Serial.print("cmd_id=");
                        Serial.println(cmd_id);
                        Serial.println(cmd_in);                                               
                        break;   
            case 5: //test at strain rates equal or slower than 0.001s-1                          
                        pd = 100; //1e-3 s-1
                        dir = LOW;
//                        digitalWrite(EnablePin, LOW);
                        if (EnablePin == HIGH){digitalWrite(EnablePin, LOW);}
                        run_motor_slow(dir,pd);
                        cmd_in=false; 
                        Serial.print("cmd_id=");
                        Serial.println(cmd_id);
                        Serial.println(cmd_in);                                               
                        break;                    
            default:{
            }
        } // switch ends here
    //} // while ends here
} // loop() ends here

// Motor run functions
   void run_motor_slow(int setdir, int pd) 
                    {  
                 unsigned long previousMillis = 0;
        
        while (!cmd_in){
          unsigned long currentMillis = millis();
                  if (currentMillis - previousMillis >= pd) 
                    {
                    previousMillis = currentMillis;                   
                    digitalWrite(DirectionPin, setdir); 
                    digitalWrite(PulsePin,HIGH);
                    digitalWrite(PulsePin,LOW);                  
                    }  
    }
    }
