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

animateCollapseContent = function(element, transitionStyle) {
    recordElementProperties(element);
    element.style.transition = '';

    requestAnimationFrame(() => {
        resetElementProperties(element);
        if (transitionStyle)
            element.style.transition = transitionStyle;

        requestAnimationFrame(() => {
            element.style.height = '0px';
            element.style.paddingTop = '0px';
            element.style.paddingBottom = '0px';
            element.style.marginTop = '0px';
            element.style.marginBottom = '0px';
            element.style.borderWidth = '0px';
            element.style.background = 'none';
            element.style.overflow = 'hidden';

            element.addEventListener('transitionend', function(e) {
                element.removeEventListener('transitionend', arguments.callee);

                element.style.setProperty("display", "none", "important")
            });
        });
    });
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

    var closer = document.createElement("span");
    closer.setAttribute("class", "qolt-content-closer");
    closer.style.setProperty("transition", "background-color .15s ease, fill .15s ease, margin .2s ease", "important");
    closer.qoltContentVisible = true;

    var closerBtn = document.createElement("button");
    closerBtn.setAttribute("class", "qolt-content-closer-btn");
    closerBtn.innerHTML = resources.arrowSvg;
    closer.appendChild(closerBtn);

    var arrowSvg = closerBtn.children[0];
    arrowSvg.style.setProperty("transition", "transform 0.3s", "important");

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

    return closer;
}

appendCloser = function(parent, node, asyncExecute) {
    asyncExecute.addTask(() => {
        if (node.classList && !node.classList.contains("reactions-1xb2Ex") && !node.classList.contains("qolt-content-closer")) {
            parent.qoltIsProcessing = true;
            formatDownloadLinks(node);
            parent.insertBefore(createCloser(node), node.nextSibling);
        }
    }, 10);
}

formatMessageContainer = function(node) {
    node.qoltIsProcessing = true;
    let asyncExecute = new AsyncExecute(() => {
        node.qoltIsProcessing = false;
    });

    for(closer of node.querySelectorAll(".qolt-content-closer"))
        closer.remove();

    node.style.setProperty("width", "100%", "important");

    if (node) {
        if (node.children)
            for (let childNode of node.children)
                appendCloser(node, childNode, asyncExecute);

        let contentObserver = new MutationObserver((mutationsList, observer) => {
            for (let mutation of mutationsList)
                for (let childNode of mutation.addedNodes)
                    appendCloser(node, childNode, asyncExecute);
        });
        contentObserver.observe(node, {attributes: false, childList: true, subtree: false});
        ObserverArray.push(contentObserver);
    }
    node.qoltProcessed = true;
}

findAndFormatMessages = function(nodes) {
    for (let node of nodes) {
        try {
            var messages = node.querySelectorAll(".containerCozy-B4noqO.container-1e22Ot");
            messages.forEach((message) => {
                if (!message.qoltProcessed)
                    formatMessageContainer(message);
            });
            if (messages && messages.length > 0)
                return true;
            else
                return false;
        }
        catch (err) {
            return false;
        }
    }
}