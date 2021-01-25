#include <comms.h>

Comms::Comms(unsigned long baud, ReceiveCallback callback) : callback_(callback), baud_(baud) {
}

void Comms::begin() {
    Serial.begin(baud_);
}

void Comms::send(char key, uint32_t value) {
    Serial.print(COMM_SEND_BEGIN_CHAR);
    Serial.print(key);
    Serial.print(COMM_SEND_SEPARATOR_CHAR);
    Serial.print(value);
    Serial.print(COMM_SEND_END_CHAR);
}

void Comms::receive() {
    if (Serial.available() > 0) {
        char temp_ = Serial.read();
        // start of packet, reset buffer
        if (temp_ == COMM_RECEIVE_BEGIN_CHAR) {
            is_packet_started_ = true;
            buffer_[0] = '\0';
        }
        else if (is_packet_started_ && strlen(buffer_) < COMM_BUFFER_SIZE) {
            // end of packet
            if (temp_ == COMM_RECEIVE_END_CHAR) {
                decodePacket();
                is_packet_started_ = false;
            }
            strncat(buffer_, &temp_, 1);    
        }
    }
}

void Comms::decodePacket() {
    char key_ = buffer_[0];
    int16_t value_ = atoi(&buffer_[1]);

    callback_(key_, value_);
}

