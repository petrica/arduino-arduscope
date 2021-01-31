import controlsHtml from './controls.html'
import ko from 'knockout'
import $ from 'jquery'

class Controls
{
    constructor(placeholder, callback) {
        $(placeholder).html(controlsHtml);
        this.viewModel = this.getViewModel();
        this.callback = callback;
        
        ko.applyBindings(this.viewModel, $(placeholder).get(0));
    }

    init() {
        this.initTriggerLevels();
        this.initTriggerPaddings();
        
        this.viewModel.triggerLevel.subscribe(() => {
            this.callback.call(this, 'trigger');
        });

        this.viewModel.trigger.subscribe(() => {
            this.callback.call(this, 'trigger');
        });
    }

    initTriggerLevels() {
        // millivolts per division
        var resolution = 5000 / 255;
        for(var i = 0; i <= 50; i++) {
            this.viewModel.triggerLevels.push({
                key: Math.round(i * 100 / resolution).toString(),
                value: i / 10 + ' V'
            })
        }
    }

    initTriggerPaddings() {
        for(var i = -50; i <= 50; i += 25) {
            this.viewModel.triggerPaddings.push(i);
        }
        this.viewModel.triggerPadding(0);
    }

    getViewModel() {
        return {
            bufferSize: ko.observable(0),
            sampleRates: ko.observableArray(),
            sampleRate: ko.observable(),
            sampleRateInfo: ko.pureComputed(function () {
                if (this.viewModel && this.viewModel.sampleRate()) {
                    return "Record: " 
                        + (this.viewModel.bufferSize() / this.viewModel.sampleRate() * 1000 
                        + " ms");
                }   

                return '';
            }, this),
            triggerLevels: ko.observableArray(),
            triggerLevel: ko.observable(),
            triggerPaddings: ko.observableArray(),
            triggerPadding: ko.observable(0),
            serialPorts: ko.observableArray(),
            serialPort: ko.observable(),
            isConnected: ko.observable(false),
            isAcquire: ko.observable(false),
            trigger: ko.observable("0"),
            connect: () => {
                this.callback.call(this, 'connect');
            },
            acquire: () => {
                this.callback.call(this, 'acquire');
            },
            acquireCssClass: ko.pureComputed(function () {
                return this.viewModel.isAcquire() ? 'button-warning' : 'pure-button-primary';
            }, this)
        };
    }

    syncArrays(source, destinationObservable) {
        source.forEach((value) => {
            if (destinationObservable().indexOf(value) == -1) {
                destinationObservable.push(value);
            }
        });

        var toRemove = [];
        destinationObservable().forEach((value) => {
            if (source.indexOf(value) == -1) {
                toRemove.push(value);
            }
        });

        destinationObservable.removeAll(toRemove);
    }

    setSampleRates(sampleRates) {
        this.syncArrays(
            _.filter(sampleRates, (o) => o > 0), 
            this.viewModel.sampleRates);
    }

    getTriggerLevel() {
        return this.viewModel.triggerLevel().key;
    }

    getTrigger() {
        return this.viewModel.trigger();
    }

    getSampleRate() {
        return this.viewModel.sampleRate();
    }

    setSerialPorts(serialPorts) {
        this.syncArrays(serialPorts, this.viewModel.serialPorts);
    }

    getSerialPort() {
        return this.viewModel.serialPort();
    }

    setBufferSize(bufferSize) {
        this.viewModel.bufferSize(bufferSize);
    }

    setConnected(value) {
        this.viewModel.isConnected(value);
    }

    setIsAcquire(isAcquire) {
        this.viewModel.isAcquire(isAcquire);
    }
}

export default Controls;