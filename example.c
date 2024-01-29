/// example.c ///////////////////////////////////////////////////////
#include "display_7seg.h"

int main(void)
{
    // initialize display
    struct disp_7seg disp;
    disp_7seg_init(&disp, &PORTD, &PORTB, 4);

    // display number
    volatile uint16_t i = 0;
    while(1) disp_7seg_printhex(&disp, i++);
}
