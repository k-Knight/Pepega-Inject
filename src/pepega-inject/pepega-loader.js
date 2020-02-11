const PepegaInject = {};
PepegaInject.IPC = class {
    static execute(jsCode, callback) {
        this.callback = callback;

        window.DiscordNative.ipc.send("pepega-execute", `
            try {
                const pepegaSendValue = (value) => {
                    windowWebContents.send(\`DISCORD_${this.pepegaChannelName}\`, value);
                };
                ${jsCode}
            } catch (err) {
                console.log(err);
            }
        `);
    }

    static sendValue(value) {
        if (this.callback)
            this.callback(value);
    }
}
PepegaInject.IPC.pepegaChannelName = ((channelName) =>  {
    window.DiscordNative.ipc.on(channelName, (event, value) => {
        PepegaInject.IPC.sendValue(value)
    });

    return channelName;
})("pepega-receive-value");