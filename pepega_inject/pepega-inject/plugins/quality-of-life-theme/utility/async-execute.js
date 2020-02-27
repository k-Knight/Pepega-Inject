AsyncExecute = class {
    constructor(callback) {
        this.callback = callback;
        this.returns = [];
        this.pending = 0;
        this.curId = 0;
    }

    joiner(id, returnValue) {
        this.returns[id] = returnValue;
        this.pending--;

        if (this.pending == 0)
            setTimeout(() => {
                this.callback(this.returns);
                this.curId = 0;
                this.returns = [];
            }, 0);
    }

    addTask(func, timeout) {
        this.pending++;
        let id = this.curId;
        setTimeout(() => {
            let returnValue = null;
            try {
                returnValue = func();
            } catch (err) {
                console.error(err);
            }
            this.joiner(id, returnValue);
        }, timeout ? timeout : 0);
        this.curId = this.curId + 1;
    }
}