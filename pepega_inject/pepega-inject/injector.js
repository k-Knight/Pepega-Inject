const electron = require('electron');
const path = require('path');
const fs = require("fs");
const filework = require('./filework');
const request = require('./request');
const logger = require('./logger');
const ipcDiscord = require('../core.asar/app/ipcMain');

initialized = false;
windowWebContents = null;
consoleRequest = false;
executionChannelRequest = false;
jsLibs = [];

exports.requireLib = function(libs) {
    if (libs) {
        if (typeof(libs) == "string")
            jsLibs.push(libs);
        else if (typeof(libs.length) == "number" && libs.length > 0)
            jsLibs = jsLibs.concat(libs);
    }
}

exports.enableConsole = function() {
    consoleRequest = true;
};

const pepegaLoader = fs.readFileSync(path.resolve(__dirname, './pepega-loader.js'), 'utf8');

const inject = function(libJsCode) {
    let plugins = filework.scanAndLoadPlugins("./plugins");
    let jsPayload = fs.readFileSync(path.resolve(__dirname, './css-loader.js'), 'utf8');

    jsPayload = "window.pepegaInject = { pepegaCssCode: " + JSON.stringify(plugins.cssCode) + " }\n" +
        jsPayload + (libJsCode ? libJsCode : "");

    for (let pluginCode of plugins.jsScripts)
        jsPayload += `\nnew Function(${JSON.stringify(pepegaLoader)} + ${JSON.stringify(pluginCode)})();`

    jsPayload = logger.addStatusReporter(
        jsPayload,
        "Pepega: code injection succeeded",
        "Pepega: code injection failed"
    );

    logger.log("Pepega: performing code injection", 0, logger.styles.process);

    windowWebContents.executeJavaScript(jsPayload);
}

const initialize = async function() {
    try {
        if (jsLibs.length > 0) {
            request.requestConcurrent(
                jsLibs,
                {method: 'GET', timeout: 5000},
                (results) => {
                    let jsCode = "";
                    for (let result of results)
                        jsCode += result ? result : "";
                    inject(jsCode);
                }
            );
        }
        else
            inject(null);
    }
    catch (err) {
        logger.log("Error happened in initializer", 0, logger.styles.failure);
        console.log(err);
    }
}

electron.app.on('web-contents-created', (event, webContents) => {
    if (!initialized) {
        windowWebContents = webContents;

        setTimeout(() => {
            initialize();
        }, 0);

        initialized = true;

        if (consoleRequest) {
            logger.enable_console(windowWebContents);
        }

        if (executionChannelRequest) {
            setTimeout(() => {
                ipcDiscord.on("pepega-execute", (event, args) => {
                    if (typeof(args) == "string")
                        eval(args);
                });
                logger.log("Pepega: execution channel created", 0, logger.styles.warning);
            }, 0);
        }
    }
})

exports.createExecutionChannel = function() {
    executionChannelRequest = true;
}