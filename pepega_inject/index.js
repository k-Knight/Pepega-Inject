module.exports = require('./core.asar');

const pepegaInject = require('./pepega-inject/injector');
pepegaInject.requireLib([
    'https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'
]);
//pepegaInject.enableConsole();
pepegaInject.createExecutionChannel();