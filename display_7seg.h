#ifndef DISPLAY_7SEG
#define DISPLAY_7SEG

// hex code for each digit
const uint8_t digits_code[16] =
    {0xFC, 0x60, 0xDA, 0xF2,
    0x66, 0xB6, 0xBE, 0xE0,
    0xFE, 0xE6, 0xEE, 0x3E,
    0x9C, 0x7A, 0x9E, 0x8E};

// display info
struct disp_7seg {
    uint8_t digit[4];
    uint8_t digit_pins[4];
    uint8_t segment_pins[8];
    uint8_t flags;
};

// display flag bits
#define SSEG_ENABLED     0x80
#define SSEG_AUTOREFRESH 0x40


// initialize the display (given digit and segment pins)
void disp_7seg_init(struct disp_7seg *, uint8_t[4], uint8_t[8]);

// refresh the display
void disp_7seg_refresh(struct disp_7seg *);

// set a digit on the display
void disp_7seg_refresh(struct disp_7seg *, u8 index, u8 digit);

/* PRINTING */
// print an unsigned number
void disp_7seg_printnumber(struct disp_7seg *, uint16_t number);

// print a signed number
void disp_7seg_printsigned(struct disp_7seg *, int16_t number);

// print 2 bytes
void disp_7seg_print2bytes(struct disp_7seg *disp, uint16_t bytes);

// get/set flags from the display
uint8_t disp_7seg_getflags(struct disp_7seg *);
void disp_7seg_setflags(struct disp_7seg *, uint8_t flags);


#endif // DISPLAY_7SEG
