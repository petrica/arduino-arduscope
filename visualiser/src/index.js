import _, { curry, isNumber } from 'lodash';
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
const controls = new Controls('#form-placeholder', control);
const comms = new Comms(receive);
var isAcquire = false;

controls.init();
comms.init();

function connect() {
    const port = controls.getSerialPort();

    if (comms.port && port != comms.port) {
        disconnect();
    }

    if (port) {
        comms.connect(port);
        controls.setConnected(true);
    }
}

function disconnect() {
    comms.disconnect();
    controls.setConnected(false);
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
                if (comms.port && !isAcquire) {
                    controls.setIsAcquire(true);
                    acquire();
                }
                else if (isAcquire) {
                    controls.setIsAcquire(false);
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
    isAcquire = acquire;
    visualiser.setSampleRate(controls.getSampleRate());

    comms.send('r', controls.getSampleRate());
    comms.send('t', controls.getTrigger());
    comms.send('l', controls.getTriggerLevel());
    comms.send('s', acquire ? 1 : 0);
}

function receive(key, value) {
    switch(key) {
        case 's':
                controls.setSampleRates(value);
            break;
        case 'b':
                // buffer size
                controls.setBufferSize(value);
            break;
        case 'ports':
                controls.setSerialPorts(value);
            break;
        case 't':
                console.log('Timer', value);
            break;
        case 'v':
                visualiser.display(value);
                if (isAcquire) acquire();
            break;
    }
};

$(window).on('unload', function() {
    if (comms.port) {
        disconnect();
    }
});



