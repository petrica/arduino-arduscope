#include "oscillo.h"

Oscillo::Oscillo(uint8_t in_pin): in_pin_(in_pin) {
}

void Oscillo::init() {
    initADC();
    initTimer();
}

void Oscillo::start() {
    ADCSRA |= 1 << ADATE;
}

void Oscillo::stop() {
    ADCSRA &= ~(1 << ADATE);
}

void Oscillo::setSampleRate(uint16_t sample_rate) {
    switch(sample_rate) {
        case 50000:
                // ADC 76.9KHz
                ADCSRA &= ~(1 << ADPS1) & ~(1 << ADPS0);
                ADCSRA |= 1 << ADPS2;
                // TIM 50KHz
                TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
                TCCR1B |= 1 << CS10;
                OCR1A = 320;
            break;
        case 25000:
                // ADC 38.5KHz
                ADCSRA &= ~(1 << ADPS1);
                ADCSRA |= 1 << ADPS2 | 1 << ADPS0;
                // TIM 25Khz
                TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
                TCCR1B |= 1 << CS10;
                OCR1A = 640;
            break;
        case 10000:
                // ADC 19.2Khz
                ADCSRA &= ~(1 << ADPS0);
                ADCSRA |= 1 << ADPS2 | 1 << ADPS1;
                // TIM 10KHz
                TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
                TCCR1B |= 1 << CS10;
                OCR1A = 1600;
            break;
        case 5000:
                // ADC 9.6KHz
                ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
                // TIM 5KHz
                TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
                TCCR1B |= 1 << CS10;
                OCR1A = 3200;
            break;
        default:
        case 1000:
                // ADC 9.6KHz
                ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
                // TIM 1KHz
                TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
                TCCR1B |= 1 << CS10;
                OCR1A = 16000;
            break;
        case 500:
                // ADC 9.6KHz
                ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
                // TIM 500Hz
                TCCR1B &= ~(1 << CS12) & ~(1 << CS11);
                TCCR1B |= 1 << CS10;
                OCR1A = 32000;
            break;        
        case 100:
                // ADC 9.6KHz
                ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;
                // TIM 100Hz
                TCCR1B &= ~(1 << CS12) & ~(1 << CS10);
                TCCR1B |= 1 << CS11;
                OCR1A = 20000;
            break;
    }
}

void Oscillo::getCapableSampleRates(uint16_t *sample_rates, size_t size) {
    memset(sample_rates, 0, sizeof(uint16_t) * size);
    sample_rates[0] = 50000;
    sample_rates[1] = 25000;
    sample_rates[2] = 10000;
    sample_rates[3] = 5000;
    sample_rates[4] = 1000;
    sample_rates[5] = 500;
    sample_rates[6] = 100;
}

void Oscillo::initADC() { 
    // AVCC with external capacitor at AREF pin
    ADMUX = 1 << REFS0;
    // left adjust ADC Data Register, 0 - 255 conversion resolution
    ADMUX |= 1 << ADLAR;
    // set inputs connected to the ADC
    ADMUX |= in_pin_ & 0x07;

    // disable ADC
    ADCSRA = 1 << ADEN;
    // enable auto trigger option
    //ADCSRA |= 1 << ADATE;
    // enable conversion interrupt
    ADCSRA |= 1 << ADIE;
    // prescalre settings
    //	ADPS2	ADPS1	ADPS0	Division Factor
	//	0	0	0	2
	//	0	0	1	2
	//	0	1	0	4
	//	0	1	1	8
	//	1	0	0	16 -  76.9KHz
	//	1	0	1	32 -  38.5KHz
	//	1	1	0	64 -  19.2KHz
	//	1	1	1	128 - 9.6Khz
    ADCSRA |= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0;

    // set timer 1 compare match B as trigger
    ADCSRB = 1 << ADTS2 | 1 << ADTS0;

    // disable digital input buffer to improve noise reduction
    DIDR0 = 1 << ADC0D | 1 << ADC1D | 1 << ADC2D | 1 << ADC3D | 1 << ADC4D | 1 << ADC5D;
}

void Oscillo::initTimer() {
    // disable PWM set CTC mode    
    TCCR1A = 0x00;
    // CTC mode, timer stopped
    TCCR1B = 1 << WGM12;
    // disable interrupt
    TIMSK1 = 0x00;
    // output compare registry A, timer cycle default to 0, this is updated before starting conversion
    OCR1A = 0x00;
    // output compare regitry B default to 0
    OCR1B = 0x00; 
}