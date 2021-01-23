#include <comms.h>
#include <oscillo.h>

#define SERIAL_BAUD 115200
#define BUFFER_SIZE	1000
#define BUFFER_CMD_SIZE 10
#define ADC_PIN 0

enum ReceiveStage {
    start,
    variable
};
struct Settings {
    boolean is_capture = false;
    int8_t trigger = 0;
    uint8_t trigger_level = 0;
    uint16_t sample_rate = 1000;
};

volatile uint8_t buffer[BUFFER_SIZE];
volatile uint16_t buffer_cursor = 0;
char bufferCmd[BUFFER_CMD_SIZE];
volatile bool is_send_data = false;

Comms *comms;
Oscillo *oscillo;
Settings settings;

void dataReceived(char key, int16_t value) {
    switch (key) {
        case 's':
                // start capture
                settings.is_capture = (boolean)value;
            break;
        case 't':
                // triggering type 0 - off,  -1 - falling, 1 - rising
                settings.trigger = value;
            break;
        case 'l':
                // trigger level
                settings.trigger_level = value;
            break;
        case 'r':
                // sample rate
                settings.sample_rate = value;
                oscillo->setSampleRate(settings.sample_rate);
            break;
    }
}

void advertiseCapabilities() {
    // advertise available options to client
    const size_t sample_rate_size = 10;
    uint16_t sample_rate_list[sample_rate_size];

    // send available sample rates
    oscillo->getCapableSampleRates(sample_rate_list, sample_rate_size);
    comms->send('s', sample_rate_list, sample_rate_size);

    // send buffer size
    comms->send('b', BUFFER_SIZE);
}

void setup() {
    comms = new Comms(SERIAL_BAUD, &dataReceived);
    oscillo = new Oscillo(ADC_PIN);

    advertiseCapabilities();

    // reset buffer
    memset((uint8_t*)buffer, 0, BUFFER_SIZE);
    pinMode(LED_BUILTIN, OUTPUT);  
    sei();
}

void loop() {
    comms->receive();

    // start capture if setting receive
    if (settings.is_capture) {
        settings.is_capture = false;
        oscillo->start();
    }

    // send data to UI
    if (is_send_data) {
        oscillo->stop();
        is_send_data = false;
        comms->send('v', buffer, BUFFER_SIZE);
    }
}

ISR(ADC_vect)
{
	buffer[buffer_cursor] = ADCH;
    TIFR1 |= 1 << OCF1B;

    buffer_cursor += 1;
    if (buffer_cursor == BUFFER_SIZE) {
        is_send_data = true;
        buffer_cursor = 0;
    }
}
