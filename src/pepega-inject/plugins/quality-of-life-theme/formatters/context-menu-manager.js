ContextMenu = class {
    constructor(contextMenu) {
        this.contextMenu = contextMenu;
        this.$itemGroup = $(this.contextMenu.querySelector(".itemGroup-1tL0uz"));
    }

    appendItemGroup() {
        this.$itemGroup = $("<div>").attr('class', 'itemGroup-1tL0uz');
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
}

ContextMenuManager = class {
    static registerContenxtMenu(contextMenu) {
        this.contextMenu = new ContextMenu(contextMenu);

        if (this.resolvePromise) {
            this.resolvePromise(this.contextMenu);
            this.rejectPromise = null
            this.resolvePromise = null;
        }
    }

    static getContextMenu() {
        return new Promise((resolve, reject) => {
            if (this.contextMenu)
                resolve(this.contextMenu);
            else {
                if (this.resolvePromise)
                    this.rejectPromise(new Error("Another context menu request was made"));

                this.resolvePromise = resolve;
                this.rejectPromise = reject;
            }
        });
    }
}