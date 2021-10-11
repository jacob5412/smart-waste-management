const scale = (num, in_min, in_max, out_min, out_max) => {
    return Math.round((num - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}

smartbins.limitToLast(1).on('child_added', function (childSnapshot) {
    // Get the bin data from the most recent snapshot of data
    // added to the bin list in Firebase
    smartbin = childSnapshot.val();

    // (x - from_min) * (to_max - to_min) / (from_max - from_min) + to_min
    fill = smartbin.filllevel;
    humidity = smartbin.hum;
    temperature = scale(smartbin.temp, -40, 80, 0, 100);

    var optionsbar = {
        chart: {
            height: 350,
            type: 'radialBar',
        },
        plotOptions: {
            radialBar: {
                offsetY: -10,
                startAngle: 0,
                endAngle: 270,
                hollow: {
                    margin: 5,
                    size: '30%',
                    background: 'transparent',
                    image: undefined,
                },
                dataLabels: {
                    name: {
                        show: false,

                    },
                    value: {
                        show: false,
                    }
                }
            }
        },
        colors: ['#1ab7ea', '#0084ff', '#39539E'],
        series: [fill, temperature, humidity],
        labels: ['Fill Level', 'Temperature', 'Humidity'],
        legend: {
            show: true,
            floating: true,
            fontSize: '16px',
            position: 'left',
            offsetX: 160,
            offsetY: 10,
            labels: {
                useSeriesColors: true,
            },
            markers: {
                size: 0
            },
            formatter: function (seriesName, opts) {
                return seriesName + ":  " + opts.w.globals.series[opts.seriesIndex]
            },
            itemMargin: {
                horizontal: 1,
            }
        },
        responsive: [{
            breakpoint: 480,
            options: {
                legend: {
                    show: false
                }
            }
        }]
    }

    var bar = new ApexCharts(
        document.querySelector("#bar"),
        optionsbar
    );

    bar.render();
});