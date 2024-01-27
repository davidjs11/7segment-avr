#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>
#include "display_7seg.h"

// initialize the display
void disp_7seg_init(struct disp_7seg *disp, volatile uint8_t *seg_port,
                    volatile uint8_t *dig_port, uint8_t digits)
{
    // reserve memory for digits
    disp->digit = (uint8_t *) malloc(digits);

    // set and init digit pins
    for(uint8_t i=0; i<digits; i++) {
        // struct parameters
        disp->num_digits = digits;
        disp->digits_port = dig_port;
        disp->digit[i] = 0;

        // initialize digit pins (output - high)
        *(disp->digits_port-1) |= (1 << i); // output
        *(disp->digits_port) |= (1 << i);   // high
    }

    // set and init segment pins
    for(uint8_t i=0; i<8; i++) {
        disp->segments_port = seg_port;

        // initialize segment pins (output - low)
        *(disp->segments_port-1) |= (1 << i); // output
        *(disp->segments_port) &= ~(1 << i);  // low
    }

    // set default flags
    disp->flags |= (SSEG_ENABLED | SSEG_AUTOREFRESH);
}

// refresh the display
void disp_7seg_refresh(struct disp_7seg *disp)
{
    // exit if display is not enabled
    if (!(disp->flags & SSEG_ENABLED)) return;

    // iterate the digits 
    for(uint8_t i=0; i<4; i++) {
        // turn on/off the segments
        *(disp->segments_port) = (disp->digit[i]);

        // select the digit
        *(disp->digits_port) &= ~(1 << i);  // low
        _delay_ms(1000/60/4);               // 60Hz for each digit
        *(disp->digits_port) |= (1 << i);   // high
    }
}

// set a digit on the display
void disp_7seg_setdigit(struct disp_7seg *disp,
                        uint8_t index, uint8_t digit)
{
    // modify the digit
    disp->digit[index] = digits_code[digit];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) disp_7seg_refresh(disp);
}


// print an unsigned number
void disp_7seg_printnumber(struct disp_7seg *disp, uint16_t number)
{

    // modify the digits
    // modify other digits
    uint16_t dig = 1;
    for(uint8_t i=1; i<=disp->num_digits; i++) {
        disp->digit[disp->num_digits-i]=digits_code[(number/dig)%10];
        dig *= 10;
    }

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) disp_7seg_refresh(disp);
}

// print a signed number
void disp_7seg_printsigned(struct disp_7seg *disp, int16_t number)
{
    uint16_t dig = 1; // digit (power of 10)

    // set sign digit if negative and first digit if not
    if (number < 0) {
        disp->digit[0] = 0x40;
        number *= -1;
    }
    else {
        for(uint8_t i=0; i<disp->num_digits-1; i++) dig *= 10;
        disp->digit[0] = digits_code[(number/dig)%10];
    }

    // modify other digits
    dig = 1;
    for(uint8_t i=1; i<=disp->num_digits-1; i++) {
        disp->digit[disp->num_digits-i]=digits_code[(number/dig)%10];
        dig *= 10;
    }

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) disp_7seg_refresh(disp);
}

// print 2 bytes
void disp_7seg_printhex(struct disp_7seg *disp, uint16_t bytes)
{
    // modify the digits
    for(uint8_t i=1; i<=disp->num_digits; i++) {
        uint8_t offset = 4*(i-1);
        disp->digit[disp->num_digits-i] =
            digits_code[(bytes&(0xF<<offset))>>offset];
    }

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) disp_7seg_refresh(disp);
}

// get display flags
uint8_t disp_7seg_getflags(struct disp_7seg *disp)
{
    return disp->flags;
}

// set display flags
void disp_7seg_setflags(struct disp_7seg *disp, uint8_t flags)
{
    disp->flags = flags;
}

int main(void)
{
    // initialize display
    struct disp_7seg disp;
    disp_7seg_init(&disp, &PORTD, &PORTB, 4);

    // display number
    volatile uint16_t i = 0;
    while(1) disp_7seg_printhex(&disp, i++);
}
