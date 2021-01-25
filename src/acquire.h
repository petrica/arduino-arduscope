#include <Arduino.h>

#ifndef ACQUIRE_BUFFER_SIZE
#define ACQUIRE_BUFFER_SIZE 1000
#endif

class Acquire
{
    public:
        Acquire();
        void acquire(uint8_t value);
        void setTriggerLevel(uint8_t value);
        void setTrigger(int8_t trigger);
        void reset();
        boolean hasData();
        uint16_t getBufferSize();
        volatile uint8_t *getBuffer();

    private:
        volatile uint8_t buffer_[ACQUIRE_BUFFER_SIZE];
        uint8_t trigger_level_ = 0;
        int8_t trigger_ = 0;
        volatile uint16_t buffer_cursor_ = 0;
        volatile bool has_data_ = false;
        volatile uint16_t trigger_index_ = 0;
        volatile bool is_triggered_ = false;
};