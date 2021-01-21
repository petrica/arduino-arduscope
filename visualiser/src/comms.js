import _ from 'lodash'
import Daemon from 'arduino-create-agent-js-client';

const SEND_PACKET_START_CHAR = 'V';
const SEND_PACKET_END_CHAR = ';';
const RECEIVE_PACKET_START_CHAR = 'S';
const RECEIVE_PACKET_END_CHAR = 'E';
const RECEIVE_PACKET_SPLIT_CHAR = ';';
const MAX_BUFFER_SIZE = 10000;
const SERIAL_RATE = 115200;

class Comms
{
    constructor(receiveCallback) {
        this.arduinoAgent = new Daemon();
        this.serialPorts = [];
        this.port = null;
        this.receiveCallback = receiveCallback;
        this.buffer = "";
    }

    init() {
        this.arduinoAgent.serialMonitorMessages.subscribe(_.bind(this.onMessage, this));
        this.arduinoAgent.devicesList.subscribe(_.bind(this.onDeviceList, this));

        this.arduinoAgent.agentFound.subscribe(status => {
            console.log("Agent found: " + status);
        });
    }

    connect(port) {
        this.port = port;
        this.arduinoAgent.openSerialMonitor(this.port, SERIAL_RATE);
    }

    disconnect() {
        if (this.port) {
            this.arduinoAgent.closeSerialMonitor(this.port);
            this.port = null;
        }
    }

    send(key, value) {
        if (this.port) {
            this.arduinoAgent.writeSerial(this.port, 
                SEND_PACKET_START_CHAR + key + value + SEND_PACKET_END_CHAR);
        }
    }

    receive() {
        return new Promise((resolve) => {
            this.resolveMessage.push(resolve);
        });
    }

    onDeviceList({serial}) {
        this.serialPorts = _.map(serial, 'Name');
        this.receiveCallback.call(this, 'ports', this.serialPorts);
    }

    onMessage(message) {
        message.split(RECEIVE_PACKET_START_CHAR).forEach((packet, index) => {
            const end = packet.indexOf(RECEIVE_PACKET_END_CHAR);
            // we have another packet started
            if (index > 0) {
                this.buffer = "";
            }
            
            if (end > -1) {
                var key;
                var value = [];
                this.buffer += packet.substr(0, end);
                this.buffer.split(RECEIVE_PACKET_SPLIT_CHAR).forEach((part, i) => {
                    if (i == 0) key = part;
                    else {
                        value.push(part);
                    }
                });
    
                this.receiveCallback.call(this, key, (value.length == 1) ? value[0] : value);
            }
            else {
                this.buffer += packet;
            }
        });

        if (this.buffer.length > MAX_BUFFER_SIZE) {
            this.buffer = "";
        }
    }
}

export default Comms;