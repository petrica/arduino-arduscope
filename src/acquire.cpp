#include <acquire.h>

Acquire::Acquire() {
    reset();
}

uint16_t Acquire::getBufferSize() {
    return ACQUIRE_BUFFER_SIZE;
}

boolean Acquire::hasData() {
    return has_data_;
}

void Acquire::setTrigger(int8_t trigger) {
    trigger_ = trigger_;
}

void Acquire::setTriggerLevel(uint8_t trigger_level) {
    trigger_level_ = trigger_level_;
}

volatile uint8_t *Acquire::getBuffer() {
    has_data_ = false;
    return buffer_;
}

void Acquire::reset() {
    has_data_ = false;
    is_triggered_ = false;
    buffer_cursor_ = 0;
    trigger_index_ = 0;
    memset((uint8_t*)buffer_, 0, getBufferSize());
}

void Acquire::acquire(uint8_t value) {
    buffer_[buffer_cursor_] = value;

    buffer_cursor_ += 1;
    if (buffer_cursor_ == getBufferSize()) {
        // no trigger, send data when buffer is full
        if (trigger_ == 0) has_data_ = true;
        buffer_cursor_  = 0;
    }
}

// trigger set
// if (settings.trigger !=0 && !is_triggered) {
//     // get previous value
//     uint8_t previous = 0;
//     if (buffer_cursor > 0) {
//         previous = buffer[buffer_cursor - 1];
//     }
//     else {
//         previous = buffer[BUFFER_SIZE - 1];
//     }

//     // rising
//     if (settings.trigger == 1 
//         && buffer[buffer_cursor] >= settings.trigger_level
//         && previous < settings.trigger_level) {
//             is_triggered = true;
//             trigger_index = buffer_cursor;
//         }

//     // falling
//     if (settings.trigger == -1
//         && buffer[buffer_cursor] <= settings.trigger_level
//         && previous > settings.trigger_level) {
//             is_triggered = true;
//             trigger_index = buffer_cursor;
//         }
// }

// if (is_triggered) {
//     if (trigger_index == 0 && buffer_cursor == BUFFER_SIZE - 1) is_send_data = true;
//     if (trigger_index > 0 && buffer_cursor == trigger_index - 1) is_send_data = true;
// }