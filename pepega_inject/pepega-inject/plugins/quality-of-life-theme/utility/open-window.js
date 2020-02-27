openWindow = function(url) {
    PepegaInject.IPC.execute(`
        const { BrowserWindow } = require('electron')

        let win = new BrowserWindow({ width: 400, height: 300, autoHideMenuBar: true });
        win.on('closed', () => {
            win = null;
        });

        win.loadURL(\`${url}\`);
    `, null);
}