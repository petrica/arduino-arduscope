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
    trigger_ = trigger;
}

void Acquire::setTriggerLevel(uint8_t trigger_level) {
    trigger_level_ = trigger_level;
}

volatile uint8_t *Acquire::getBuffer() {
    has_data_ = false;
    return buffer_;
}

uint16_t Acquire::getTriggerIndex() {
    return trigger_index_;
}

void Acquire::reset() {
    has_data_ = false;
    is_triggered_ = false;
    buffer_cursor_ = 0;
    trigger_index_ = 0;
    is_first_value_ = true;
    memset((uint8_t*)buffer_, 0, getBufferSize());
}

uint8_t Acquire::getPreviousValue() {
    if (buffer_cursor_ > 0) {
        return buffer_[buffer_cursor_ - 1];
    }
    else {
        return buffer_[getBufferSize() - 1];
    }
}

void Acquire::checkTrigger() {
    uint8_t previous = getPreviousValue();

    if (trigger_ == 1 
        && buffer_[buffer_cursor_] >= trigger_level_
        && previous < trigger_level_) {
            is_triggered_ = true;
            trigger_index_ = buffer_cursor_;
    }

    if (trigger_ == -1
        && buffer_[buffer_cursor_] <= trigger_level_
        && previous > trigger_level_) {
            is_triggered_ = true;
            trigger_index_ = buffer_cursor_;
    }
}

void Acquire::acquire(uint8_t value) {
    buffer_[buffer_cursor_] = value;

    if (trigger_ != 0 && !is_triggered_ && !is_first_value_) {
        checkTrigger();
    }

    is_first_value_ = false;
    buffer_cursor_ += 1;
    if (buffer_cursor_ == getBufferSize()) {
        // no trigger, send data when buffer is full
        if (trigger_ == 0) has_data_ = true;
        buffer_cursor_  = 0;
    }

    if (is_triggered_ && trigger_index_ == buffer_cursor_) {
        has_data_ = true;
    }
}
