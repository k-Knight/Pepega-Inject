contentFormatter = function(mutationsList, observer) {
    for(let mutation of mutationsList) {
        if (mutation.target.classList) {
            if (mutation.target.classList.contains("content-98HsJk")) {
                mutation.qolt_processed = findAndFormatMessages(mutation.addedNodes);
            }
            else if (mutation.target.classList.contains("containerCozy-B4noqO") && mutation.target.classList.contains("container-1e22Ot")) {
                if (mutation.addedNodes.length > 0 && !mutation.target.qoltIsProcessing) {
                    formatMessageContainer(mutation.target);
                    mutation.qolt_processed = true;
                }
            }
        }

        if (!mutation.qolt_processed) {
            var contentContainer = document.querySelector(".content-98HsJk");

            if (contentContainer && contentContainer.contains(mutation.target)) {
                mutation.qolt_processed = findAndFormatMessages(mutation.addedNodes);
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
                    ContextMenuManager.registerContenxtMenu(contextMenu);
                    mutation.qolt_processed = true;
                }
            }
        }
    }
}

customMutationObserver = new MutationObserver(contentFormatter);
customMutationObserver.observe(document.getElementsByTagName('body')[0], {
    attributes: false, childList: true, subtree: true, characterData: true
});