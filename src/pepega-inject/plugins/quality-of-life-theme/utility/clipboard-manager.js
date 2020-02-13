copyImageFromUrl = function(url) {
    PepegaInject.IPC.execute(`
        const electron = require('electron');
        const nativeImage = electron.nativeImage;

        request.request(\`${url}\`, {method: 'GET'}).then((data) => {
            try {
                const nativeImg = nativeImage.createFromBuffer(data);
                electron.clipboard.write({ html: \`<img src="${url}">\`, image: nativeImg });

                pepegaSendValue(null);
            } catch (err) {
                console.error(err);
                pepegaSendValue(new Error('copy request failed'));
            }
        }).catch((err) => {
            console.error(err);
            pepegaSendValue(new Error('copy request failed at download stage'));
        });
    `, (err) => {
        if (err)
            console.error(err);
    });
}