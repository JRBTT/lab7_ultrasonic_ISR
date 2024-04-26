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


void setup(){
  usart_init(MYUBRR); // 103-9600 bps; 8-115200
  sei(); // enable global interrupts
  bitSet(DDRD, TRIGGER_PIN); 
  bitClear(DDRD, ECHO_PIN);
}

int main(){
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