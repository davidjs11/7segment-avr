#ifndef DISPLAY_7SEG
#define DISPLAY_7SEG

// hex code for each digit
const uint8_t digits_code[16] =
    {0x3F, 0x06, 0x5B, 0x4F,
     0x66, 0x6D, 0x7D, 0x07,
     0x7F, 0x67, 0x77, 0x7C,
     0x39, 0x5E, 0x79, 0x71};



// display info
struct disp_7seg {
    uint8_t num_digits;
    uint8_t *digit;
    volatile uint8_t *digits_port;
    volatile uint8_t *segments_port;
    uint8_t flags;
};

// display flag bits
#define SSEG_ENABLED     0x80
#define SSEG_AUTOREFRESH 0x40


// initialize the display (given digit and segment ports)
void disp_7seg_init(struct disp_7seg *, volatile uint8_t *seg_port,
                    volatile uint8_t *dig_port, uint8_t digits);

// refresh the display
void disp_7seg_refresh(struct disp_7seg *);

// set a digit on the display
void disp_7seg_setdigit(struct disp_7seg *, uint8_t index, uint8_t digit);

/* PRINTING */
// print an unsigned number
void disp_7seg_printnumber(struct disp_7seg *, uint16_t number);

// print a signed number
void disp_7seg_printsigned(struct disp_7seg *, int16_t number);

// print 2 bytes
void disp_7seg_printhex(struct disp_7seg *, uint16_t bytes);

// get/set flags from the display
uint8_t disp_7seg_getflags(struct disp_7seg *);
void disp_7seg_setflags(struct disp_7seg *, uint8_t flags);


#endif // DISPLAY_7SEG
