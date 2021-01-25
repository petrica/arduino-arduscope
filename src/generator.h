#include <Arduino.h>

class Generator
{
    public:
        Generator();
        void start();
        void stop();

    private:
        void initTimer();
};