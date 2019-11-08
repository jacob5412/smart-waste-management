var level = [];
var temps = [];
var hums = [];
var dataSeries = []
// display all bins
smartbins.orderByChild('timestamp').once("value").then(function (snapshot) {
    // Get the bin data from the most recent snapshot of data
    // added to the bin list in Firebase
    snapshot.forEach(function (childSnapshot) {
        smartbin = childSnapshot.val();
        dataSeries.push({
            "date": smartbin.timestamp,
            "fill": smartbin.filllevel,
            "temp": smartbin.temp,
            "hum": smartbin.hum
        }, )
    })
    console.log(dataSeries);

    for (var i = 0; i < dataSeries.length; i++) {
        var innerArr = [dataSeries[i].date, dataSeries[i].fill];
        level.push(innerArr);
        innerArr = [dataSeries[i].date, dataSeries[i].temp];
        temps.push(innerArr);
        innerArr = [dataSeries[i].date, dataSeries[i].hum];
        hums.push(innerArr);
    }
    
    var options = {
        chart: {
            type: 'area',
            stacked: false,
            height: 350,
            zoom: {
                type: 'x',
                enabled: true,
                autoScaleYaxis: true
            },
            toolbar: {
                autoSelected: 'zoom'
            }
        },
        dataLabels: {
            enabled: false
        },
        series: [{
            name: 'Fill Level',
            data: level
        }],
        markers: {
            size: 0,
        },
        title: {
            text: 'Garbage in bin',
            align: 'left'
        },
        fill: {
            type: 'gradient',
            gradient: {
                shadeIntensity: 1,
                inverseColors: false,
                opacityFrom: 0.5,
                opacityTo: 0,
                stops: [0, 90, 100]
            },
        },
        yaxis: {
            labels: {
                formatter: function (val) {
                    return (val).toFixed(0);
                },
            },
            title: {
                text: 'Level (cms)'
            },
        },
        xaxis: {
            type: 'datetime',
        },

        tooltip: {
            shared: false,
            y: {
                formatter: function (val) {
                    return (val);
                }
            }
        }
    }

    var fillchart = new ApexCharts(
        document.querySelector("#fillchart"),
        options
    );

    fillchart.render();

    var options2 = {
        chart: {
            type: 'area',
            stacked: false,
            height: 350,
            zoom: {
                type: 'x',
                enabled: true,
                autoScaleYaxis: true
            },
            toolbar: {
                autoSelected: 'zoom'
            }
        },
        dataLabels: {
            enabled: false
        },
        series: [{
            name: 'Temperature',
            data: temps
        }],
        markers: {
            size: 0,
        },
        title: {
            text: 'Temperature in bin',
            align: 'left'
        },
        fill: {
            type: 'gradient',
            gradient: {
                shadeIntensity: 1,
                inverseColors: false,
                opacityFrom: 0.5,
                opacityTo: 0,
                stops: [0, 90, 100]
            },
        },
        yaxis: {
            labels: {
                formatter: function (val) {
                    return (val).toFixed(0);
                },
            },
            title: {
                text: 'Temperature (*C)'
            },
        },
        xaxis: {
            type: 'datetime',
        },

        tooltip: {
            shared: false,
            y: {
                formatter: function (val) {
                    return (val);
                }
            }
        }
    }

    var tempchart = new ApexCharts(
        document.querySelector("#tempchart"),
        options2
    );

    tempchart.render();

    var options3 = {
        chart: {
            type: 'area',
            stacked: false,
            height: 350,
            zoom: {
                type: 'x',
                enabled: true,
                autoScaleYaxis: true
            },
            toolbar: {
                autoSelected: 'zoom'
            }
        },
        dataLabels: {
            enabled: false
        },
        series: [{
            name: 'Humidity',
            data: hums
        }],
        markers: {
            size: 0,
        },
        title: {
            text: 'Humidity in bin',
            align: 'left'
        },
        fill: {
            type: 'gradient',
            gradient: {
                shadeIntensity: 1,
                inverseColors: false,
                opacityFrom: 0.5,
                opacityTo: 0,
                stops: [0, 90, 100]
            },
        },
        yaxis: {
            labels: {
                formatter: function (val) {
                    return (val).toFixed(0);
                },
            },
            title: {
                text: 'Humidity (%)'
            },
        },
        xaxis: {
            type: 'datetime',
        },

        tooltip: {
            shared: false,
            y: {
                formatter: function (val) {
                    return (val);
                }
            }
        }
    }

    var humchart = new ApexCharts(
        document.querySelector("#humchart"),
        options3
    );

    humchart.render();
});