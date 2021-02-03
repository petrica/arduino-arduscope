#include <comms.h>
#include <oscillo.h>
#include <acquire.h>
#include <generator.h>

#define SERIAL_BAUD 115200
#define ADC_PIN 0

void dataReceived(char key, int16_t value);

Comms comms(SERIAL_BAUD, &dataReceived);
Oscillo oscillo(ADC_PIN);
Acquire acquire;
Generator generator;

void dataReceived(char key, int16_t value) {
    switch (key) {
        case 's':
                // start capture
                if (value > 0) {
                    acquire.reset();
                    oscillo.start();
                }
                else {
                    oscillo.stop();
                }
            break;
        case 't':
                // triggering type 0 - off,  -1 - falling, 1 - rising
                acquire.setTrigger(value);
            break;
        case 'l':
                // trigger level
                acquire.setTriggerLevel(value);
            break;
        case 'r':
                // sample rate
                oscillo.setSampleRate(value);
            break;
    }
}

void advertiseCapabilities() {
    // advertise available options to client
    const size_t sample_rate_size = 10;
    uint16_t sample_rate_list[sample_rate_size];

    // send available sample rates
    oscillo.getCapableSampleRates(sample_rate_list, sample_rate_size);
    comms.send('s', sample_rate_list, sample_rate_size);

    // send buffer size
    comms.send('b', acquire.getBufferSize());
}

void setup() {
    comms.begin();
    oscillo.init();
    generator.init();
    generator.start();

    sei();
    advertiseCapabilities();
}

void loop() {
    comms.receive();

    // send data to UI if available
    if (acquire.hasData()) {
        oscillo.stop();
        comms.send('v', acquire.getBuffer(), acquire.getBufferSize(), acquire.getTriggerIndex());
        acquire.reset();
    }
}

ISR(ADC_vect)
{
    acquire.acquire(ADCH);
    TIFR1 |= 1 << OCF1B;
}
