#include <generator.h>

#define OC2A_PIN 11

Generator::Generator() {
    initTimer();
}

void Generator::initTimer() {
    pinMode(OC2A_PIN, OUTPUT);
    PRR &= ~(1 << PRTIM2);

    // Toggle OC2A pin on OCR2A match
    TCCR2A = 1 << COM2A0;
    // Set mode 7 to count up to OCR2A 
    TCCR2A |= 1 << WGM21 | 1 << WGM20;
    TCCR2B  = 1 << WGM22;
    // Set frequency to 1Khz, 2Khz toggle half a period
    OCR2A = 125;
}

void Generator::start() {
    // Set prescaler to 64
    TCCR2B |= 1 << CS22;
}

void Generator::stop() {
    // Stop prescaler
    TCCR2B &= ~(1 << CS22);
}