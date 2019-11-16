var consoleEnabled = false;
var devConsoleOwner = null;

exports.isConsoleEnabled = function() {
    return consoleEnabled;
}

exports.enable_console = function(webContents) {
    devConsoleOwner = webContents;
    devConsoleOwner.openDevTools({mode: "undocked"});
    consoleEnabled = true;
}

var styles = {
    success: `color: #222;font-size: 18px;font-weight: bolder;
        text-shadow: 2px 0 #6d0, -2px 0 #6d0, 0 2px #6d0, 0 -2px #6d0,
        1px 1px #6d0, -1px -1px #6d0, -1px 1px #6d0, 1px -1px #6d0;`,
    failure: `color: #222;font-size: 18px;font-weight: bolder;
        text-shadow: 2px 0 #d40, -2px 0 #d40, 0 2px #d40, 0 -2px #d40,
        1px 1px #d40, -1px -1px #d40, -1px 1px #d40, 1px -1px #d40;`,
    process: `color: #222; font-size: 16px; font-weight: bolder;
        text-shadow: 2px 0 #08d, -2px 0 #08d, 0 2px #08d, 0 -2px #08d,
        1px 1px #08d, -1px -1px #08d, -1px 1px #08d, 1px -1px #08d;`,
    warning: `color: #222; font-size: 16px; font-weight: bolder;
        text-shadow: 2px 0 #cc0, -2px 0 #cc0, 0 2px #cc0, 0 -2px #cc0,
        1px 1px #cc0, -1px -1px #cc0, -1px 1px #cc0, 1px -1px #cc0;`
}
exports.styles = styles;

exports.log = function(object, errorLevel, style) {
    if (consoleEnabled) {
        var consoleFunction = null;
        var data = null;

        switch(errorLevel) {
            case 1: consoleFunction = "warn"; break;
            case 2: consoleFunction = "error"; break;
            default: consoleFunction = "log"; break;
        }

        if (typeof(object) == 'string' && style)
            data = JSON.stringify("%c" + object) + ", " + JSON.stringify(style);
        else
            data = JSON.stringify(object);

        devConsoleOwner.executeJavaScript("console." + consoleFunction + "(" + data + ");");
    }
}

exports.addStatusReporter = function(jsCode, successMsg, failMsg) {
    if (consoleEnabled)
        return `
            try {
                eval(${JSON.stringify(jsCode)});
                console.log('%c${successMsg}', ${JSON.stringify(styles.success)});
            } catch (err) {
                console.log('%c${failMsg}', ${JSON.stringify(styles.failure)});
                console.error(err);
            }
        `;
    else
        return jsCode;
}