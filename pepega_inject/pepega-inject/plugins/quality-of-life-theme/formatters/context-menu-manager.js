ContextMenu = class {
    constructor(contextMenu) {
        this.contextMenu = contextMenu;
        this.$itemGroup = $(this.contextMenu.querySelector(".itemGroup-1tL0uz"));
    }

    appendItemGroup(index) {
        this.$itemGroup = $("<div>").attr('class', 'itemGroup-1tL0uz');

        if (index)
            this.contextMenu.insertBefore(
                this.$itemGroup[0],
                this.contextMenu.children[index]
            );
        else
            $(this.contextMenu).append(this.$itemGroup);

        return this;
    }

    appendItem(text, callback, ...args) {
        if (this.$itemGroup) {
            $("<div>")
                .attr('class', 'item-1Yvehc itemBase-tz5SeC clickable-11uBi-')
                .attr('tabindex', 0)
                .attr('role', 'menuitem')
                .append(
                    $('<div>')
                        .attr('class', 'label-JWQiNe')
                        .text(text ? text : "")
                ).append(
                    $('<div>')
                        .attr('class', 'hint-22uc-')
                ).on('click', () => { callback(...args); })
                .appendTo(this.$itemGroup);
        }

        return this;
    }

    close() {
        this.contextMenu.remove();
        ContextMenuManager.removeContextMenu();
    }
}

ContextMenuManager = class {
    static registerContextMenu(contextMenu) {
        this.contextMenu = new ContextMenu(contextMenu);

        for(let listener of this.listeners)
            listener.notify(this.contextMenu);
    }

    static addListener(listener) {
        this.listeners.push(listener);

        if (this.contextMenu)
            listener.notify(this.contextMenu);
    }

    static removeListener(listener) {
        let index = this.listeners.indexOf(listener);
        if (index >= 0)
            this.listeners.splice(index, 1);
    }

    static removeContextMenu() {
        this.contextMenu = null;
    }
}
ContextMenuManager.listeners = [];

ContextMenuListener = class {
    constructor(callback) {
        this.callback = callback;
        ContextMenuManager.addListener(this);
    }

    notify(contextMenu) {
        if (this.callback)
            this.callback(contextMenu);
    }

    destroy() {
        ContextMenuManager.removeListener(this);
    }
}