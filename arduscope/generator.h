#include "Arduino.h"

class Generator
{
    public:
        void init();
        void start();
        void stop();

    private:
        void initTimer();
};