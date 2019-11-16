downloadItem = function(url, filename) {
    IPC.execute(`
        const { dialog, app } = require('electron');
        dialog.showSaveDialog(null, {defaultPath : \`${filename}\`}, (path) => {
            if (path) {
                try {
                    request.request(\`${url}\`, {method: 'GET'}).then((data) => {
                        fs.writeFile(path, data, (err) => {
                            if (err) {
                                console.error(err);
                                qoltSendValue(new Error('failed to write downloaded file'));
                            } else {
                                qoltSendValue(null);
                            }
                        });
                    }).catch((err) => {
                        console.error(err);
                        qoltSendValue(new Error('download request failed'));
                    });
                }
                catch (err) {
                    console.error(err);
                    qoltSendValue(new Error('failed to create download request'));
                }
            }
        });
    `, (err) => {
        if (err)
            console.error(err);
    });
}