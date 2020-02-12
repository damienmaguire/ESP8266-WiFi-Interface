
document.addEventListener("DOMContentLoaded", function() {

  function setupForm() {
    var form = document.getElementById("form");
    form.setAttribute('action', config.saveAction);
    form.setAttribute('method', 'post');
  }

  function addEnabled(container, chartConfig, index) {
    var node = document.createElement("label");
    var enabledNode = document.createElement("input");
    enabledNode.setAttribute('name', "chart[" + index + "]");
    enabledNode.setAttribute('type', 'checkbox');
    if (chartConfig.enabled) {
      enabledNode.setAttribute('checked', 'checked');
    }
    node.appendChild(enabledNode)
    node.append(" enabled")
    container.appendChild(node)
    container.appendChild(document.createElement('br'));

  }

  function addUpdateFrequency(container, chartConfig, index)  {
    var node = document.createElement("label");
    var updateFrequencyNode = document.createElement("input");
    updateFrequencyNode.setAttribute('name', "frequency[" + index + "]");
    updateFrequencyNode.setAttribute('type', 'text');
    updateFrequencyNode.setAttribute('value', chartConfig.updateFrequency);
    updateFrequencyNode.setAttribute('size', '8');

    node.append(updateFrequencyNode);
    node.append(" update frequency (ms)")

    container.appendChild(node)
    container.appendChild(document.createElement('br'));


  }

  function addMin(container, chartConfig, index)  {
    var node = document.createElement("label");
    var updateFrequencyNode = document.createElement("input");
    updateFrequencyNode.setAttribute('name', "min[" + index + "]");
    updateFrequencyNode.setAttribute('type', 'text');
    updateFrequencyNode.setAttribute('value', chartConfig.min);
    updateFrequencyNode.setAttribute('size', '5');
    node.append(updateFrequencyNode);
    node.append(" min")

    container.appendChild(node)
    container.appendChild(document.createElement('br'));


  }

  function addMax(container, chartConfig, index)  {
    var node = document.createElement("label");
    var updateFrequencyNode = document.createElement("input");
    updateFrequencyNode.setAttribute('name', "max[" + index + "]");
    updateFrequencyNode.setAttribute('type', 'text');
    updateFrequencyNode.setAttribute('size', '5');
    updateFrequencyNode.setAttribute('value', chartConfig.max);
    node.append(updateFrequencyNode);
    node.append(" max")

    container.appendChild(node)
    container.appendChild(document.createElement('br'));


  }

  function addTitle(container, chartConfig) {
    var h2Node = document.createElement("h2");
    h2Node.append(chartConfig.title)
    container.appendChild(h2Node)
  }

  function addChart(chartConfig, index) {
    var setting = document.createElement("div");
    setting.classList.add('chart-container')

    addTitle(setting, chartConfig)
    addEnabled(setting, chartConfig, index);
    addUpdateFrequency(setting, chartConfig, index)
    addMin(setting, chartConfig, index)
    addMax(setting, chartConfig, index)

    chartContainer.appendChild(setting);

  }


  var chartContainer = document.getElementById("container");
  setupForm();
  config.charts.forEach(function(chartConfig, index) {
    addChart(chartConfig, index)
  });

});
