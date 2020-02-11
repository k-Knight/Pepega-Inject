const path = require('path');
const fs = require('fs');

const appendItems = function (items, folderPath) {
    var scannedItems = fs.readdirSync(folderPath, {withFileTypes: true});
    if (scannedItems && scannedItems.length > 0)
        for (let item of scannedItems) {
            items.push(folderPath + "\\" + item.name);
        }
}

const scanFolder = function(folderPath, recursive, onlyFolders) {
    var files = []
    if (!path.isAbsolute(folderPath))
        folderPath = path.join(__dirname, folderPath);

    var items = [];
    appendItems(items, folderPath);

    while (items.length > 0) {
        var curItem = items.shift();

        var stat = fs.statSync(curItem);
        if (stat && stat.isDirectory()) {
            if (recursive)
                appendItems(items, curItem);

            if (onlyFolders)
                files.push(curItem);
        }
        else if (!onlyFolders)
            files.push(curItem);
    }

    return files;
}

const tryRead = function(filePath, resolve) {
    try {
        if (resolve)
            return fs.readFileSync(path.resolve(__dirname, filePath), 'utf8');
        else
            return fs.readFileSync(filePath, 'utf8');
    } catch (err) {
        return "";
    }
}

const scanAndLoadPlugins = function(folder) {
    var plugins = { cssCode: "", jsScripts: [] };
    var pluginFolders = scanFolder(folder, false, true);

    for (let pluginFolder of pluginFolders) {
        pluginFiles = scanFolder(pluginFolder, true);
        let length = plugins.jsScripts.push('');

        for (let file of pluginFiles)
            if (file.endsWith(".js"))
                plugins.jsScripts[length - 1] += "\n" + tryRead(file);
            else if (file.endsWith(".css"))
                plugins.cssCode += "\n" + tryRead(file);
            else if (file.endsWith("settings.json"))
                plugins.jsScripts[length - 1] += "\nPepegaInject.settings = " + tryRead(file) + ";";
    }

    return plugins;
}

module.exports = {
    scanFolder,
    tryRead,
    scanAndLoadPlugins
};