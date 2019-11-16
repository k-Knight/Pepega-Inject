IPC = class {
    static execute(jsCode, callback) {
        this.callback = callback;

        window.DiscordNative.ipc.send("pepega-execute", `
            try {
                const qoltSendValue = (value) => {
                    windowWebContents.send(\`DISCORD_${this.qoltChannelName}\`, value);
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
IPC.qoltChannelName = ((channelName) =>  {
    window.DiscordNative.ipc.on(channelName, (event, value) => {
        IPC.sendValue(value)
    });

    return channelName;
})("qolt-receive-value");