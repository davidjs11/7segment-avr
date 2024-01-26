#include <stdint.h>

/* 7 SEGMENT */
const uint8_t digits_code[16] = {0xFC, 0x60, 0xDA, 0xF2,
                                 0x66, 0xB6, 0xBE, 0xE0,
                                 0xFE, 0xE6, 0xEE, 0x3E,
                                 0x9C, 0x7A, 0x9E, 0x8E};

// 7 segment display struct
struct sseg {
    uint8_t digit[4];
    uint8_t digit_pins[4];
    uint8_t segment_pins[8];
    uint8_t flags;
};

// flags
#define SSEG_ENABLED     0x80
#define SSEG_AUTOREFRESH 0x40

// intialize the display struct
void sseg_init(struct sseg *disp, uint8_t digit_pins[4], uint8_t segment_pins[8]) {
    // set and init digit pins
    for(uint8_t i=0; i<4; i++) {
        disp->digit_pins[i] = digit_pins[i];
        disp->digit[i] = 0;
        pinMode(digit_pins[i], OUTPUT);
        digitalWrite(digit_pins[i], HIGH);
    }

    // set and init segment pins
    for(uint8_t i=0; i<8; i++) {
        disp->segment_pins[i] = segment_pins[i];
        pinMode(segment_pins[i], OUTPUT);
        digitalWrite(segment_pins[i], LOW);
    }

    // set default flags
    disp->flags |= (SSEG_ENABLED | SSEG_AUTOREFRESH);
}

// refresh the display
void sseg_refresh(struct sseg *disp) {
    // exit if display is not enabled
    // if (!(disp->flags & SSEG_ENABLED)) return;

    // iterate the digits 
    for(uint8_t i=0; i<4; i++) {
        // turn on/off the segments
        for(uint8_t j=0; j<8; j++)
            digitalWrite(disp->segment_pins[j], (disp->digit[i]<<j)&0x80);

        // select the digit
        digitalWrite(disp->digit_pins[i], LOW);
        delay(1000/60/4); // 60Hz for each digit
        digitalWrite(disp->digit_pins[i], HIGH);
    }
}

// set a digit on the display
void sseg_setdigit(struct sseg *disp, u8 index, u8 digit) {
    // modify the digit
    disp->digit[index] = digits_code[digit];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) sseg_refresh(disp);
}

// print a decimal number on the display
void sseg_printnumber(struct sseg *disp, uint16_t number) {
    // modify the digits
    disp->digit[0] = digits_code[(number/1000)%10];
    disp->digit[1] = digits_code[(number/100)%10];
    disp->digit[2] = digits_code[(number/10)%10];
    disp->digit[3] = digits_code[number%10];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) sseg_refresh(disp);
}

// print a word (2 bytes in hex) on the display
void sseg_print2bytes(struct sseg *disp, uint16_t bytes) {
    // modify the digits
    disp->digit[0] = digits_code[(bytes&0xF000)>>12];
    disp->digit[1] = digits_code[(bytes&0x0F00)>>8];
    disp->digit[2] = digits_code[(bytes&0x00F0)>>4];
    disp->digit[3] = digits_code[(bytes&0x000F)];

    // auto-refresh
    if (disp->flags & SSEG_AUTOREFRESH) sseg_refresh(disp);
}

// get/set flags from the display
uint8_t sseg_getflags(struct sseg *disp) { return disp->flags;}
void sseg_setflags(struct sseg *disp, uint8_t flags) { disp->flags = flags; }


/* SETUP */
struct sseg *display = (struct sseg *) malloc(sizeof(struct sseg));

void setup() {
    uint8_t digit_pins[4] = {8,9,10,11};
    uint8_t segment_pins[8] = {0,1,2,3,4,5,6,7};
    sseg_init(display, digit_pins, segment_pins);
}

/* LOOP */
uint16_t i = 0;
void loop() { sseg_print2bytes(display, i++); }
