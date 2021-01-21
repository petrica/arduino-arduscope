#include <oscillo.h>

Oscillo::Oscillo(uint8_t in_pin): in_pin_(in_pin) {
    initADC();
    initTimer();
}

void Oscillo::start() {
    TCCR1B |= 1 << CS10;
}

void Oscillo::stop() {
    TCCR1B &= ~(1 << CS10);
}

void Oscillo::setSampleRate(uint16_t sample_rate) {
    switch(sample_rate) {
        case 50000:
            break;
        case 25000:
            break;
        case 10000:
            break;
        case 5000:
            break;
        default:
        case 1000:
            break;
        case 500:
            break;        
        case 100:
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

    // enable ADC
    ADCSRA = 1 << ADEN;
    // enable auto trigger option
    ADCSRA |= 1 << ADATE;
    // enable conversion interrupt
    ADCSRA |= 1 << ADIE;
    // prescalre settings
    //	ADPS2	ADPS1	ADPS0	Division Factor
	//	0	0	0	2
	//	0	0	1	2
	//	0	1	0	4
	//	0	1	1	8
	//	1	0	0	16
	//	1	0	1	32
	//	1	1	0	64
	//	1	1	1	128
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