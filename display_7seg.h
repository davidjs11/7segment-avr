/// display_7seg.h //////////////////////////////////////////////////

#ifndef DISPLAY_7SEG
#define DISPLAY_7SEG

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdlib.h>


/* DIGITS (from 0-9 and A-F)*/
extern const uint8_t digits_code[16];


/* DISPLAY STRUCT */
struct disp_7seg {
    uint8_t num_digits; /* number of digits on the display*/
    uint8_t *digit; /* array of values shown of each digit */
    volatile uint8_t *digits_port; /* avr port to the digit pins */
    volatile uint8_t *segments_port; /* avr port to the segment pins */
    uint8_t flags; /* flags for the screen */
};


/* FLAG MACROS */
#define SSEG_ENABLED     0x80 /* enables the screen */
#define SSEG_AUTOREFRESH 0x40 /* refresh automatically when printing */


/* INITIALIZE/FREE STRUCT */
// initialize the display (given digit and segment ports)
void disp_7seg_init(struct disp_7seg *, volatile uint8_t *seg_port,
                    volatile uint8_t *dig_port, uint8_t digits);

// free dynamic memory from struct
void disp_7seg_free(struct disp_7seg *);


/* DISPLAY BASICS */
// refresh the display
void disp_7seg_refresh(struct disp_7seg *);

// set a digit on the display
void disp_7seg_setdigit(struct disp_7seg *, uint8_t index, uint8_t digit);


/* FLAGS */
// get flags from the display
uint8_t disp_7seg_getflags(struct disp_7seg *);

// get flags from the display
void disp_7seg_setflags(struct disp_7seg *, uint8_t flags);


/* PRINTING */
// print an unsigned number
void disp_7seg_printnumber(struct disp_7seg *, uint16_t number);

// print a signed number
void disp_7seg_printsigned(struct disp_7seg *, int16_t number);

// print 2 bytes
void disp_7seg_printhex(struct disp_7seg *, uint16_t bytes);


#endif // DISPLAY_7SEG
