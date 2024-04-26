#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>
#include <avr/interrupt.h>
#include "usart.h"
#include "bit.h"
#include "timer.h"

#define TRIGGER_PIN PIND5
#define ECHO_PIN PIND6

#define FOSC 16000000 // Frequency Oscillator 16Mhz for Uno R3
#define BAUD 9600 // 9600 Bits per second
#define MYUBRR FOSC / 16 / BAUD - 1 // My USART Baud Rate Register
#define MAXPWM 1023
#define MAXDELAY 61 // 1 second

#define MAXDISTANCE 4 // m
#define SPEED 343 // m/s
#define TIMEOUT 23000//((MAXDISTANCE / SPEED) * 2) * 1000000 ///2 for round trip

// function prototypes
void pulseTrigger();
float listen();


// trigger the ultrasonic sensor
void pulseTrigger()
{
  bitSet(PORTD, TRIGGER_PIN);
  _delay_us(10);
  bitClear(PORTD, TRIGGER_PIN);
}

// get time of ultrasonic sensor
float listen()
{
    int time=0;
    while(!(bitRead(PIND,ECHO_PIN)));
    while((bitRead(PIND,ECHO_PIN))){
        _delay_us(1);
        time++;
        if (time > TIMEOUT){
            return -1;
        }
    }
    float duration = (float)time * 0.0343 / 2;
    return duration;
}

void setup(){
  usart_init(MYUBRR); // 103-9600 bps; 8-115200
  sei(); // enable global interrupts
  bitSet(DDRD, TRIGGER_PIN); 
  bitClear(DDRD, ECHO_PIN);
}

int main(){
    setup();
    while(1){
      // send ultrasound signal and listen for echo
      pulseTrigger();
      float buffer = listen();
      _delay_us(10);
      // checks if there is a valid result from ultrasonic sensor
      if (buffer != -1){
        usart_tx_string("Distance: ");
        usart_tx_float(buffer, 3, 2);
        usart_transmit('\n');
        _delay_us(10);
      }
    }
}