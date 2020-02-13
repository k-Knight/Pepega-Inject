downloadItem = function(url, filename) {
    PepegaInject.IPC.execute(`
        const { dialog, app } = require('electron');
        dialog.showSaveDialog(null, {defaultPath : \`${filename}\`}, (path) => {
            if (path) {
                try {
                    request.request(\`${url}\`, {method: 'GET'}).then((data) => {
                        fs.writeFile(path, data, (err) => {
                            if (err) {
                                console.error(err);
                                pepegaSendValue(new Error('failed to write downloaded file'));
                            } else {
                                pepegaSendValue(null);
                            }
                        });
                    }).catch((err) => {
                        console.error(err);
                        pepegaSendValue(new Error('download request failed'));
                    });
                }
                catch (err) {
                    console.error(err);
                    pepegaSendValue(new Error('failed to create download request'));
                }
            }
        });
    `, (err) => {
        if (err)
            console.error(err);
    });
}

downloadFromUrl = function(url) {
    let fileName = url.split("/");
    fileName = fileName[fileName.length - 1];

    downloadItem(url, fileName);
}