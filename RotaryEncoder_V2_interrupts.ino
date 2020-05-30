#include <avr/io.h>
#include <avr/interrupt.h>

#define outputA 6 //PD6
#define outputB 7 //PD7
#define redLED 5 //PD5

int counter = 0;
int currStateA;
int prevStateA;
int button = 1;
short buttonState = 0;

void interruptSetup(void){
    DDRD &= ~(1 << DDD2);     // Clear the PD2 pin
    // PD2 (PCINT0 pin) is now an input

    PORTD |= (1 << PORTD2);    // turn On the Pull-up
    // PD2 is now an input with pull-up enabled

    EICRA |= (1 << ISC00);    // set INT0 to trigger on: 1 << for ANY logic change, 2 << for HIGH 
    EIMSK |= (1 << INT0);     // Turns on INT0

    sei();                    // turn on interrupts  
}

void setup() {
  interruptSetup(); 
  
  pinMode (outputA, INPUT_PULLUP);
  pinMode (outputB, INPUT_PULLUP);
  pinMode (redLED, OUTPUT);

  Serial.begin (9600);
  // Reads the initial state of the outputA
  prevStateA = digitalRead(outputA);
}

void loop() {

  currStateA = digitalRead(outputA); // Reads the "current" state of the outputA
  // Looking for a rising edge, and checking if the previous and the current state of the outputA are different (a pulse has occured)
  if ((currStateA != prevStateA)&&(prevStateA == HIGH)) {
    // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
    if (digitalRead(outputB) != currStateA) {
      counter ++;
      digitalWrite(redLED, HIGH);
    } else {
      counter --;
      digitalWrite(redLED, LOW);
    }
    Serial.print("Position: ");
    Serial.println(counter);
  }
  prevStateA = currStateA; // Updates the previous state of the outputA with the current state
}

ISR (INT0_vect)
{
    /* interrupt code here */    
    
    Serial.println("Button pressed, counter reset!");
    for(short i = 0; i<3;i++){
      digitalWrite(redLED, HIGH); 
      delay(300);
      digitalWrite(redLED, LOW);
      delay(300);
    }
    counter = 0;
}
