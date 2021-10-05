#include <avr/io.h>
#define INPUT 0
#define OUTPUT 1
#define INPUT_PULLUP 2
#define TOG 3
#define HIGH 1
#define LOW 0
#define delay _delay_ms

#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5

void off_led () ;

void on_led () ;

void tog_led () ;

void set_bit (volatile uint8_t *port, uint8_t bit) ;

void clr_bit (volatile uint8_t *port, uint8_t bit) ;

void tog_bit (volatile uint8_t *port, uint8_t bit) ;
