const ObserverArray = class {
    static init() {
        if (!(this.observers))
            this.observers = [];
    }

    static push(observer) {
        this.observers.push(observer);
    }

    static clear() {
        for (let observer of this.observers) {
            try {
                observer.disconnect();
            } catch (err) { }
        }

        this.observers = [];
    }
}
ObserverArray.init();