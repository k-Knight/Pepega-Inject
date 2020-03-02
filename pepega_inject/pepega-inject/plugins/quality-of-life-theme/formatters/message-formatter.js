const elementProperties = [
    { fieldName: 'transition', cssName: 'transition'},
    { fieldName: 'height', cssName: 'height'},
    { fieldName: 'width', cssName: 'width'},
    { fieldName: 'paddingLeft', cssName: 'padding-left'},
    { fieldName: 'paddingRight', cssName: 'padding-right'},
    { fieldName: 'paddingTop', cssName: 'padding-top'},
    { fieldName: 'paddingBottom', cssName: 'padding-bottom'},
    { fieldName: 'marginLeft', cssName: 'margin-left'},
    { fieldName: 'marginRight', cssName: 'margin-right'},
    { fieldName: 'marginTop', cssName: 'margin-top'},
    { fieldName: 'marginBottom', cssName: 'margin-bottom'},
    { fieldName: 'borderWidth', cssName: 'border-width'},
    { fieldName: 'background', cssName: 'background'},
    { fieldName: 'overflow', cssName: 'overflow'},
    { fieldName: 'display', cssName: 'display'}
];

recordElementProperties = function(element) {
    if (!element.qolt)
        element.qolt = {};

    let compStyle = getComputedStyle(element);

    for (let property of elementProperties) {
        if (element.style[property.fieldName])
            element.qolt[property.fieldName] = {  defined: true, value: element.style[property.fieldName] };
        else
            element.qolt[property.fieldName] = {  defined: false, value: compStyle.getPropertyValue(property.cssName) };
    }
}

getElementProperty = function(element, property) {
    if (element.qolt && element.qolt[property])
        return element.qolt[property].value;

    return element.style[property];
}

unsetElementProperties = function(element) {
    if (element.qolt)
        for (let property of elementProperties)
            if (element.qolt[property.fieldName] && !element.qolt[property.fieldName].defined)
                element.style[property.fieldName] = '';
}

resetElementProperties = function(element, exclude) {
    if (!exclude)
        exclude = [];

    if (element.qolt)
        for (let property of elementProperties)
            if (element.qolt[property.fieldName] && !exclude.includes(property.cssName))
                element.style[property.fieldName] = element.qolt[property.fieldName].value;
}

setCollapedElementStyle = function(element) {
    element.style.height = '0px';
    element.style.paddingTop = '0px';
    element.style.paddingBottom = '0px';
    element.style.marginTop = '0px';
    element.style.marginBottom = '0px';
    element.style.borderWidth = '0px';
    element.style.background = 'none';
    element.style.overflow = 'hidden';
}

scrollDownBy = function(amount, mode = 'auto') {
    let messages = document.querySelector(".messages-3amgkR.scroller-3sQKXg");

    if (messages) {
        messages.scrollBy({
            left: 0,
            top: amount,
            behavior: mode
          });
        if (messages.scrollTop / (messages.scrollHeight - messages.clientHeight) > 0.995)
            messages.scrollBy({
                left: 0,
                top: messages.scrollHeight,
                behavior: mode
            });
    }
}

animateCollapseContent = function(element, transitionStyle) {
    recordElementProperties(element);
    element.qolt_scrollHeight = element.scrollHeight;
    element.style.transition = '';

    requestAnimationFrame(() => {
        resetElementProperties(element);
        if (transitionStyle)
            element.style.transition = transitionStyle;

        requestAnimationFrame(() => {
            setCollapedElementStyle(element);

            element.addEventListener('transitionend', function(e) {
                element.removeEventListener('transitionend', arguments.callee);

                element.style.setProperty("display", "none", "important");
            });
        });
    });
}

immediateCollapseContent = function(element, transitionStyle) {
    recordElementProperties(element);
    element.style.transition = '';

    setCollapedElementStyle(element);
    element.style.setProperty("display", "none", "important");

    if (transitionStyle)
        element.style.transition = transitionStyle;
}

animateExpandContent = function(element, transitionStyle) {
    element.style.display = getElementProperty(element, 'display');

    requestAnimationFrame(() => {
        let exclude = ['overflow'];
        if (transitionStyle) {
            exclude.push('transition');
            element.style.transition = transitionStyle;
        }

        resetElementProperties(element, exclude);

        element.addEventListener('transitionend', function(e) {
            element.removeEventListener('transitionend', arguments.callee);

            resetElementProperties(element);
            unsetElementProperties(element);
        });
    });
}

createCloser = function(node) {
    const transitionStyle = 'height 0.2s ease, margin 0.2s ease, padding 0.2s ease, border 0.2s ease';
    node.style.setProperty("box-sizing", "border-box", "important");

    let closer = document.createElement("span");
    closer.setAttribute("class", "qolt-content-closer");
    closer.qoltContentVisible = true;

    let closerBtn = document.createElement("button");
    closerBtn.setAttribute("class", "qolt-content-closer-btn");
    closerBtn.innerHTML = resources.arrowSvg;
    closer.appendChild(closerBtn);

    let arrowSvg = closerBtn.children[0];

    closer.addEventListener("click", (event) => {
        try {
            if (closer.qoltContentVisible) {
                animateCollapseContent(node, transitionStyle);
                closer.style.setProperty("margin-top", "8px", "important");
                arrowSvg.style.setProperty("transform", "translateX(-50%) translateY(-50%) rotate(90deg)", "important");
            }
            else {
                animateExpandContent(node, transitionStyle);
                closer.style.setProperty("margin-top", "3px", "important");
                arrowSvg.style.setProperty("transform", "translateX(-50%) translateY(-50%) rotate(-90deg)", "important");
            }

            closer.qoltContentVisible = !closer.qoltContentVisible;
        } catch (err) {
            console.error(err);
        }

        event.preventDefault();
    });

    if (PepegaInject.settings['autocollapse']) {
        immediateCollapseContent(node, transitionStyle);
        closer.style.setProperty("margin-top", "8px", "important");
        arrowSvg.style.setProperty("transform", "translateX(-50%) translateY(-50%) rotate(90deg)", "important");
        closer.qoltContentVisible = false;
    }

    closer.style.setProperty("transition", "background-color .15s ease, fill .15s ease, margin .2s ease", "important");
    arrowSvg.style.setProperty("transition", "transform 0.3s", "important");

    return closer;
}

appendCloser = function(parent, node, asyncExecute) {
    asyncExecute.addTask(() => {
        if (node.classList && !node.classList.contains("reactions-1xb2Ex") && !node.classList.contains("qolt-content-closer")) {
            parent.qoltIsProcessing = true;
            formatDownloadLinks(node);
            if (node.qoltCloserExists != true) {
                node.qoltCloserExists = true;
                let closer = createCloser(node);
                parent.insertBefore(closer, node.nextSibling);
                scrollDownBy(closer.scrollHeight, 'auto');
            }
        }
    }, 10);
}

deleteContentCloser = function(node) {
    node.remove();
}

formatEmdeds = function(container, asyncExecute) {
    for (let childNode of container.children)
        if (childNode.classList.contains('attachment-33OFj0'))
            formatDownloadLinks(childNode);
        else if (childNode.classList.contains('embedWrapper-lXpS3L') || childNode.classList.contains('spoilerContainer-331r0R')) {
            appendCloser(container, childNode, asyncExecute);
            processVideoEmbeds(childNode);
        }
}

formatMessageContainer = function(node) {
    node.qoltIsProcessing = true;
    let asyncExecute = new AsyncExecute(() => {
        node.qoltIsProcessing = false;
    });

    for(closer of node.querySelectorAll(".qolt-content-closer"))
        closer.remove();

    if (node) {
        if (node.children) {
            let container =  node.querySelector('.container-1ov-mD');

            if (container) {
                formatEmdeds(container, asyncExecute);

                let contentObserver = new MutationObserver((mutationsList, observer) => {
                    formatEmdeds(container, asyncExecute);
                });

                contentObserver.observe(container, {attributes: false, childList: true, subtree: false});
                ObserverArray.push(contentObserver);
            }
        }
    }
    node.qoltProcessed = true;
}

findAndFormatMessages = function(chatNode) {
    if (chatNode) {
        let messages = chatNode.querySelector('.messages-3amgkR');
        
        if (messages)
            for (let message of messages.children)
                formatMessageContainer(message);
    }
}