import Highcharts from 'highcharts'

class Visualiser
{
    constructor(element, vcc) {
        this.chart = new Highcharts.Chart({
            chart: {
                zoomType: 'x',
                renderTo: element,
                height: 500,
                animation: false
            },
            title: {
                text: 'ArduScope'
            },
            subtitle: {
                text: document.ontouchstart === undefined ?
                    'Click and drag in the plot area to zoom in' : 'Pinch the chart to zoom in'
            },
            xAxis: {
                type: 'time',
                title: {
                    text: 'Milliseconds'
                },
                units: [[
                    'milliseconds'
                ]]
            },
            yAxis: {
                title: {
                    text: 'Volts'
                },
                max: 6,
                min: -1
            },
            legend: {
                enabled: false
            },
            plotOptions: {
                area: {
                    fillColor: {
                        linearGradient: {
                            x1: 0,
                            y1: 0,
                            x2: 0,
                            y2: 1
                        },
                        stops: [
                            [0, Highcharts.getOptions().colors[0]],
                            [1, Highcharts.color(Highcharts.getOptions().colors[0]).setOpacity(0).get('rgba')]
                        ]
                    },
                    marker: {
                        radius: 2
                    },
                    lineWidth: 1,
                    states: {
                        hover: {
                            lineWidth: 1
                        }
                    },
                    threshold: null
                },
                series: {
                    animation: false
                }
            },

            series: [{
                    type: 'line',
                    name: 'Scope'
                },
                {
                    type: 'line',
                    name: 'Trigger'
                }
            ]
        });
        this.vcc = vcc;
    };

    /**
     * Unit of measurement: number of milliseconds / division
     * 
     * @param {int} sampleRate 
     */
    setSampleRate(sampleRate) {
        this.sampleRate = sampleRate;
    };

    showTrigger(triggerLevel) {
        var level = this.vcc/255 * triggerLevel;
        var line = [];
        line[0] = level;
        this.chart.series[1].update({
            pointStart: 0,
            data: line
        });
    }

    hideTrigger() {
        this.chart.series[1].update({
            data: []
        });
    }

    display(buffer) {
        const pointInterval = 1000 / this.sampleRate;
        for(var i = 0; i < buffer.length; i++) {
            buffer[i] = this.vcc/255 * parseInt(buffer[i]);
        };
        this.chart.series[0].update({
            pointStart: 0,
            pointInterval: pointInterval,
            data: buffer
        });
    }
}

export default Visualiser;