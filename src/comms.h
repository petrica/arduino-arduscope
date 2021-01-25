#include <Arduino.h>

#ifndef COMM_BUFFER_SIZE
#define COMM_BUFFER_SIZE 10
#endif

#ifndef COMM_RECEIVE_BEGIN_CHAR
#define COMM_RECEIVE_BEGIN_CHAR 'V'
#endif
#ifndef COMM_RECEIVE_END_CHAR
#define COMM_RECEIVE_END_CHAR ';'
#endif

#ifndef COMM_SEND_BEGIN_CHAR
#define COMM_SEND_BEGIN_CHAR 'S'
#endif
#ifndef COMM_SEND_END_CHAR
#define COMM_SEND_END_CHAR 'E'
#endif
#ifndef COMM_SEND_SEPARATOR_CHAR
#define COMM_SEND_SEPARATOR_CHAR ';'
#endif

typedef void (*ReceiveCallback)(char key, int16_t value);

class Comms
{
    public:
        Comms(unsigned long baud, ReceiveCallback callback);

        void begin();
        void receive();

        template<typename T>
        void send(char key, T *values, size_t size) {
            Serial.print(COMM_SEND_BEGIN_CHAR);
            Serial.print(key);
            Serial.print(COMM_SEND_SEPARATOR_CHAR);
            for(uint16_t i = 0; i < size; i++) {
                Serial.print(values[i]);
                if (i + 1 < size) {
                    Serial.print(COMM_SEND_SEPARATOR_CHAR);
                }
            }
            Serial.print(COMM_SEND_END_CHAR);
        }

        void send(char key, uint16_t value);

    private:
        char buffer_[COMM_BUFFER_SIZE];
        boolean is_packet_started_ = false;
        const ReceiveCallback callback_;
        const unsigned long baud_;

    void decodePacket();

};