contentFormatter = function(mutationsList, observer) {
    for(let mutation of mutationsList) {
        if (mutation.target.classList) {
            if (mutation.target.classList.contains("messages-3amgkR")) {
                if (mutation.addedNodes.length > 0) {
                    for (let message of mutation.addedNodes)
                        if (!message.qoltIsProcessing)
                            formatMessageContainer(message);

                    mutation.qolt_processed = true;
                }
            }
            if (!mutation.qolt_processed) {
                if (mutation.target.classList.contains('content-98HsJk')) {
                    for (let chat of mutation.addedNodes) {
                        if (chat.classList && chat.classList.contains('chat-3bRxxu'))
                            findAndFormatMessages(chat.querySelector('.chatContent-a9vAAp'));
                    }

                    mutation.qolt_processed = true;
                }
            }
            if (!mutation.qolt_processed) {
                if (mutation.target.classList.contains('content-yTz4x3')) {
                    for (let chatContent of mutation.addedNodes) {
                        if (chatContent.classList && chatContent.classList.contains('chatContent-a9vAAp'))
                            findAndFormatMessages(chatContent);
                    }

                    mutation.qolt_processed = true;
                }
            }
        }

        if(!mutation.qolt_processed && mutation.addedNodes.length == 1 && mutation.addedNodes[0].classList) {
            if (mutation.addedNodes[0].classList.contains("modal-3c3bKg")) {
                setTimeout(() => {
                    formatContentDisplay(mutation.addedNodes[0].querySelector(".inner-1ilYF7 > div"));
                }, 10);
                mutation.qolt_processed = true;
            }
            else if (mutation.addedNodes[0].classList.contains("layer-v9HyYc")) {
                let contextMenu = mutation.addedNodes[0].querySelector(".contextMenu-HLZMGh");
                if (contextMenu) {
                    ContextMenuManager.registerContextMenu(contextMenu);
                    mutation.qolt_processed = true;
                }
            }
        }

        if (!mutation.qolt_processed && mutation.removedNodes.length > 0) {
            if(mutation.removedNodes.length == 1 && mutation.removedNodes[0].classList) {
                if (mutation.target && mutation.target.classList.contains("container-1ov-mD") &&
                    mutation.nextSibling && mutation.nextSibling.classList.contains("qolt-content-closer")) {

                    deleteContentCloser(mutation.nextSibling);
                    mutation.qolt_processed = true;
                }
                if (!mutation.qolt_processed) {
                    if (mutation.removedNodes[0].classList.contains("modal-3c3bKg")) {
                        setTimeout(() => {
                            cancelContentDisplay();
                        }, 10);
                        mutation.qolt_processed = true;
                    }
                    else if (mutation.removedNodes[0].classList.contains("layer-v9HyYc"))
                        if (mutation.removedNodes[0].querySelector(".contextMenu-HLZMGh"))
                            ContextMenuManager.removeContextMenu();
                }
            }
        }
    }
}

customMutationObserver = new MutationObserver(contentFormatter);
customMutationObserver.observe(document.getElementsByTagName('body')[0], {
    attributes: false, childList: true, subtree: true, characterData: true
});