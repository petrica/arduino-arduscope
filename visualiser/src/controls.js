import controlsHtml from './controls.html'
import $ from 'jquery'

class Controls
{
    constructor(placeholder, callback) {
        $(placeholder).html(controlsHtml);
        this.sampleRateSelect = $('#sample-rate');
        this.sampleRateInfo = $('#sample-rate-info');
        this.serialPortsSelect = $('#comm-port');
        this.triggerLevelSelect = $('#trigger-level');
        this.connectButton = $('#connect');
        this.acquireButton = $('#acquire');
        this.triggerType = $('input[name="triggering"]');
        this.bufferSize = 0;
        this.callback = callback;
    }

    init() {
        this.connectButton.on('click', (event) => {
            this.callback.call(this, 'connect');
        });

        this.acquireButton.on('click', (event) => {
            this.callback.call(this, 'acquire');
        });

        this.sampleRateSelect.on('change', (event) => {
            this.updateSampleRateInfo();
        });

        this.updateTriggerLevels();
    }

    syncSelect(select, values) {
        values.forEach((item) => {
            if ($('option[value="' + item.key + '"]', select).length == 0) {
                select.append($('<option>', {
                    value: item.key,
                    text: item.value
                }));
            }
        });
    
        $('option', select).each((index, option) => {
            if (!_.find(values, ['key', option.value])) {
                option.remove();
            }
        });
    }

    setSampleRates(sampleRates) {
        var rates = [];
        var display = "";
        sampleRates.forEach((sampleRate) => {
            if (sampleRate == 0) return;
            if (sampleRate >= 1000) display = Math.round(sampleRate / 1000) + ' KHz';
            else display = sampleRate + ' Hz';
            rates.push({
                key: sampleRate,
                value: display
            });
        });
        this.syncSelect(this.sampleRateSelect, rates);
        this.updateSampleRateInfo();
    }

    updateTriggerLevels() {
        // generate level predefined values
        var levels = [];
        // millivolts per division
        var resolution = 5000 / 255;
        for(var i = 0; i <= 50; i++) {
            levels.push({
                key: Math.round(i * 100 / resolution).toString(),
                value: i / 10 + ' V'
            })
        }
        this.syncSelect(this.triggerLevelSelect, levels);
    }

    getTriggerLevel() {
        return this.triggerLevelSelect.val();
    }

    getTrigger() {
        return this.triggerType.filter(':checked').val();
    }

    updateSampleRateInfo() {
        const sampleRate = this.sampleRateSelect.val();
        const bufferTime = this.bufferSize / sampleRate * 1000;
        this.sampleRateInfo.html("Record: " + bufferTime + " ms");
    }

    getSampleRate() {
        return this.sampleRateSelect.val();
    }

    setSerialPorts(serialPorts) {
        this.syncSelect(this.serialPortsSelect, _.map(serialPorts, (o) => { return { key: o, value: o}}));
    }

    getSerialPort() {
        return this.serialPortsSelect.val();
    }

    setBufferSize(bufferSize) {
        this.bufferSize = bufferSize;
        this.updateSampleRateInfo();
    }

    setConnected(value) {
        if (value) {
            this.connectButton.html('Disconnect');
        }
        else {
            this.connectButton.html('Connect');
        }
    }

    setIsAcquire(isAcquire) {
        if (isAcquire) {
            this.acquireButton
                .text('Stop')
                .removeClass('pure-button-primary')
                .addClass('button-warning');
        }
        else {
            this.acquireButton
                .text('Capture')
                .removeClass('button-warning')
                .addClass('pure-button-primary');
        }
    }
}

export default Controls;