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
    disp->digit[0] = digits_code[(number/1000)%10];
    disp->digit[1] = digits_code[(number/100)%10];
    disp->digit[2] = digits_code[(number/10)%10];
    disp->digit[3] = digits_code[number%10];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) disp_7seg_refresh(disp);
}

// print a signed number
void disp_7seg_printsigned(struct disp_7seg *disp, int16_t number)
{
    // set sign digit if negative and first digit if not
    if (number < 0) {
        disp->digit[0] = 0x02;
        number *= -1;
    }
    else disp->digit[0] = digits_code[(number/1000)%10];

    // modify other digits
    disp->digit[1] = digits_code[(number/100)%10];
    disp->digit[2] = digits_code[(number/10)%10];
    disp->digit[3] = digits_code[number%10];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) disp_7seg_refresh(disp);
}

// print 2 bytes
void disp_7seg_printhex(struct disp_7seg *disp, uint16_t bytes)
{
    // modify the digits
    disp->digit[0] = digits_code[(bytes&0xF000)>>12];
    disp->digit[1] = digits_code[(bytes&0x0F00)>>8];
    disp->digit[2] = digits_code[(bytes&0x00F0)>>4];
    disp->digit[3] = digits_code[(bytes&0x000F)];

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

volatile uint16_t i = 0;
int main(void) {
    // initialize display
    struct disp_7seg *disp;
    disp = (struct disp_7seg *)malloc(sizeof(struct disp_7seg));
    disp_7seg_init(disp, &PORTD, &PORTB, 3);

    while(1) {
        i++;
        disp_7seg_printhex(disp, i);
    }
}
