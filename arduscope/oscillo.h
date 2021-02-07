#include "Arduino.h"

class Oscillo
{
    public:
        Oscillo(uint8_t in_pin);

        void init();

        void setSampleRate(uint16_t sample_rate);
        void getCapableSampleRates(uint16_t *sample_rates, size_t size);

        void start();
        void stop();

    private:
        const uint8_t in_pin_;
        void initADC();
        void initTimer();
};