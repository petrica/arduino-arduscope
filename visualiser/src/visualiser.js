import Chart from 'chart.js';

class Visualiser
{
    constructor(element) {
        this.chart = new Chart(element, {
            // The type of chart we want to create
            type: 'line',
            responsive: true,
        
            // The data for our dataset
            data: {
                datasets: [{
                    label: 'My First dataset',
                    backgroundColor: 'rgb(255, 99, 132)',
                    borderColor: 'rgb(255, 99, 132)',
                    fill: false,
                    lineTension: 0,
                    borderWidth: 1.5
                }]
            },
        
            // Configuration options go here
            options: {
                tooltips: {
                    enabled: false
                },
                elements: {
                    point: {
                        radius: 1,
                        hoverRadius: 1
                    },
                    line: {
                        lineTension: 0,
                        showLine: false
                    }
                },
                animation: {
                    duration: 0
                },
                legend: false,
                scales: {
                    yAxes: [
                        {
                            gridLines: {
                                display: false
                            },
                            ticks: {
                                suggestedMin: 0,
                                suggestedMax: 255
                            }
                        }
                    ],
                    xAxes: {
                        type: 'time',
                        distribution: 'series',
                        gridLines: {
                            display: false
                        },
                        ticks: {
                            autoSkip: false,
                            display: true,
                            major: {
                                display: true
                            }
                        }
                    }
                }
            }
        });
    };

    /**
     * Unit of measurement: number of milliseconds / division
     * 
     * @param {int} sampleRate 
     */
    setSampleRate(sampleRate) {
        this.sampleRate = sampleRate;
    };

    display(buffer) {
        const msPerPoint = 1000 / this.sampleRate;
        var labels = [];
        for(var i = 0; i < buffer.length; i++) {
            var label = "";
            label = i * msPerPoint + "ms";
            labels.push(label);
        };

        this.chart.data.labels = labels;
        this.chart.data.datasets[0].data = buffer;
        this.chart.update();
    }
}

export default Visualiser;