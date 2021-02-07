import _ from 'lodash';
import 'purecss';
import './style.css';
import html from './index.html';
import $ from 'jquery';
import Visualiser from './visualiser';
import Comms from './comms';
import Controls from './controls';

//https://www.zingchart.com/gallery/time-series-line-chart


if (process.env.NODE_ENV !== 'production') {
    console.log('Looks like we are in development mode!');
}

document.body.innerHTML = html;

const visualiser = new Visualiser('chart', 5);
const controls = new Controls('#controls', control);
const comms = new Comms(receive);

controls.init();
comms.init();

function connect() {
    const port = controls.getSerialPort();

    if (comms.port && port != comms.port) {
        disconnect();
    }

    if (port) {
        comms.connect(port);
        controls.setIsConnected(true);
    }
}

function disconnect() {
    comms.disconnect();
    controls.setIsConnected(false);
    controls.setIsAcquire(false);
}

function control(key) {
    switch(key) {
        case 'connect':
                if (comms.port) {
                    disconnect();
                }
                else {
                    connect();
                }
            break;
        case 'acquire':
                if (comms.port && !controls.isAcquire()) {
                    acquire(true);
                }
                else if (controls.isAcquire()) {
                    acquire(false);
                }
            break;
        case 'trigger':
                if (controls.getTrigger() != 0) {
                    visualiser.showTrigger(controls.getTriggerLevel());
                }
                else {
                    visualiser.hideTrigger();
                }

                if (controls.isAcquire()) acquire();
            break;
    }
}

function acquire(acquire = true) {
    controls.setIsAcquire(acquire);
    visualiser.setSampleRate(controls.getSampleRate());

    comms.send('r', controls.getSampleRate());
    comms.send('t', controls.getTrigger());
    comms.send('l', controls.getTriggerLevel());
    comms.send('s', acquire ? 1 : 0);
}

function receive(key, value) {
    switch(key) {
        case 's':
                // sample rate values
                controls.setSampleRates(value);
            break;
        case 'b':
                // buffer size
                controls.setBufferSize(value);
                visualiser.setBufferSize(value);
            break;
        case 'v':
                visualiser.display(value);
                if (controls.isAcquire()) acquire(true);
            break;
        case 'ports':
                controls.setSerialPorts(value);
                if (controls.getSerialPort() != comms.port) {
                    disconnect();
                }
            break;
        case 'agent':
                controls.setHasAgent(value);
                if (!value) {
                    disconnect();   
                }
            break;
    }
};

$(window).on('unload', function() {
    if (comms.port) {
        disconnect();
    }
});



