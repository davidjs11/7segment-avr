#include "display_7seg.h"

// initialize the display
void disp_7seg_init(struct disp_7seg *disp,
                    uint8_t digit_pins[4], uint8_t segment_pins[8])
{
    // set and init digit pins
    for(uint8_t i=0; i<4; i++) {
        disp->digit_pins[i] = digit_pins[i];
        disp->digit[i] = 0;
        pinMode(digit_pins[i], OUTPUT); /////////////////////////////
        digitalWrite(digit_pins[i], HIGH); //////////////////////////
    }

    // set and init segment pins
    for(uint8_t i=0; i<8; i++) {
        disp->segment_pins[i] = segment_pins[i];
        pinMode(segment_pins[i], OUTPUT); ///////////////////////////
        digitalWrite(segment_pins[i], LOW); /////////////////////////
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
        for(uint8_t j=0; j<8; j++)
            /////////////////////////////////////////////////////////
            digitalWrite(disp->segment_pins[j], (disp->digit[i]<<j)&0x80);

        // select the digit
        digitalWrite(disp->digit_pins[i], LOW); /////////////////////
        delay(1000/60/4); // 60Hz for each digit ////////////////////
        digitalWrite(disp->digit_pins[i], HIGH); ////////////////////
    }
}

// set a digit on the display
void disp_7seg_setdigit(struct disp_7seg *disp, u8 index, u8 digit)
{
    // modify the digit
    disp->digit[index] = digits_code[digit];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) sseg_refresh(disp);
}


// print an unsigned number
void disp_7seg_printnumber(struct disp_7seg *, uint16_t number)
{
    // modify the digits
    disp->digit[0] = digits_code[(number/1000)%10];
    disp->digit[1] = digits_code[(number/100)%10];
    disp->digit[2] = digits_code[(number/10)%10];
    disp->digit[3] = digits_code[number%10];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) sseg_refresh(disp);
}

// print a signed number
void disp_7seg_printsigned(struct disp_7seg *, int16_t number)
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
    if (disp->flags & SSEG_AUTOREFRESH) sseg_refresh(disp);
}

// print 2 bytes
void disp_7seg_print2bytes(struct disp_7seg *disp, uint16_t bytes)
{
    // modify the digits
    disp->digit[0] = digits_code[(bytes&0xF000)>>12];
    disp->digit[1] = digits_code[(bytes&0x0F00)>>8];
    disp->digit[2] = digits_code[(bytes&0x00F0)>>4];
    disp->digit[3] = digits_code[(bytes&0x000F)];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) sseg_refresh(disp);
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
