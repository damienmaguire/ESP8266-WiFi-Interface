var config = {
  saveAction: '/SaveConfig',
  defaultChartConfig: {
      chart: {
          type: 'solidgauge'
      },
      title: null,
      pane: {
          center: ['50%', '85%'],
          size: '140%',
          startAngle: -90,
          endAngle: 90,
          background: {
              backgroundColor:
                  Highcharts.defaultOptions.legend.backgroundColor || '#EEE',
              innerRadius: '60%',
              outerRadius: '100%',
              shape: 'arc'
          }
      },
      exporting: {
          enabled: false
      },
      tooltip: {
          enabled: false
      },
      // the value axis
      yAxis: {
          stops: [
              [0.1, '#55BF3B'], // green
              [0.5, '#DDDF0D'], // yellow
              [0.9, '#DF5353']  // red
          ],
          lineWidth: 0,
          tickWidth: 0,
          minorTickInterval: null,
          tickAmount: 2,
          title: {
              y: -70
          },
          labels: {
              y: 16
          }
      },
      plotOptions: {
          solidgauge: {
              dataLabels: {
                  y: 5,
                  borderWidth: 0,
                  useHTML: true
              }
          }
      }
  },
  charts: [{
    min: 100,
    max: 300,
    initialValue: 203,
    title: 'Pack Voltage',
    valueSuffix: ' V',
    updateFrequency: 10000,
    dataEndpoint: '/PackVoltageV1',
    enabled: true
  }, {
    min: 0,
    max: 200,
    initialValue: 70,
    title: 'Power',
    valueSuffix: ' KW',
    updateFrequency: 1000,
    dataEndpoint: '/Power',
    enabled: true
  }, {
    min: 0,
    max: 100,
    initialValue: 0,
    title: 'Temperature',
    valueSuffix: ' °C',
    updateFrequency: 1000,
    dataEndpoint: '/Temperature',
    enabled: true
  }]
}
