animateResizeContent = function($image, options, maximize) {
    if (maximize) {
        $image.qolt = {
            width: options.width,
            height: options.height,
            top: options.top,
            left: options.left
        }

        $image
            .css("width", $image.qolt.width)
            .css("height", $image.qolt.height)
            .css("top", $image.qolt.top)
            .css("left", $image.qolt.left)
            .css("transition", "all 0.15s ease")
            .css("display", "block");

        setTimeout(() => {
            $image
                .css("width", "100%")
                .css("height", "calc(100% - 22px)")
                .css("top", "22px")
                .css("left", "0px");
        }, 0);
    }
    else {
        $image
            .css("width", $image.qolt.width)
            .css("height", $image.qolt.height)
            .css("top", $image.qolt.top)
            .css("left", $image.qolt.left)

        setTimeout(() => {
            $image
                .css("display", "");
        }, 150);
    }
}

animateResizeButton = function($btn, maximize) {
    if (!$btn.qoltTop || !$btn.qoltLeft) {
        var rect = $btn[0].getBoundingClientRect();

        $btn.qoltBottom = rect.top - 27.0;
        $btn.qoltRight = 5.0 - (window.innerWidth - rect.right);
    }

    if (maximize) {
        $btn
            .css("transition", "all .15s ease")
            .css("right", 0)
            .css("bottom", 0)
            .html(resources.minimizeSvg);

        setTimeout(() => {
            $btn
                .css("right", $btn.qoltRight)
                .css("bottom", $btn.qoltBottom);
        }, 0);
    }
    else {
        $btn
            .css("right", 0)
            .css("bottom", 0)
            .html(resources.maximizeSvg);
    }
}

maximizeClickHandler = function($btn, $image, $bigImage, $link) {
    if ($btn.attr("maximized") == "true") {
        animateResizeButton($btn, false);
        animateResizeContent($bigImage, null, false);

        $image.css("visibility", "visible");
        $link
            .css("opacity", "")
            .css("pointer-events", "")
            .css("cursor", "");

        $btn.attr("maximized", false);
    }
    else {
        var boundingRect = $image[0].getBoundingClientRect();
        animateResizeContent($bigImage, {
            width: $image.css("width"),
            height: $image.css("height"),
            top: boundingRect.top,
            left: boundingRect.left,
        }, true);

        animateResizeButton($btn, true);

        $image.css("visibility", "hidden");
        $link
            .css("opacity", "0")
            .css("pointer-events", "none")
            .css("cursor", "default");

        $btn.attr("maximized", true);
    }
}

createContentMaximizer = function(imageContainer, imageNode, $originalLink, content) {
    let elementTag = content.tagName.toLowerCase();

    let src = getContentSrc(content);
    if (elementTag == 'video') {
        elementTag += ' controls';
    }

    if (imageNode) {
        let $imageNode = $(imageNode).css("margin", "auto");
        $downloadLink = $('<a>')
            .attr('class', 'anchor-3Z-8Bb anchorUnderlineOnHover-2ESHQB downloadLink-1ywL9o size14-3iUx6q')
            .attr('href', src)
            .text("Download original");
        $originalLink.remove();
        setDownloadClickHandler($downloadLink);

        let $actionContainer = $("<div>").attr("class", "qolt-image-display-actions");
        let $maximizeBtn = $("<button>").attr("maximized", false).html(resources.maximizeSvg);
        let $bigImg = $(`<${elementTag}>`).attr("class", "qolt-image-display-big").attr("src", src);

        $maximizeBtn.click(() => {
            maximizeClickHandler(
                $maximizeBtn,
                $imageNode,
                $bigImg,
                $downloadLink
            );
        });

        $actionContainer.append($downloadLink);
        $actionContainer.append($maximizeBtn);
        $(imageContainer).append($bigImg);
        $(imageContainer).append($actionContainer);
    }
}

formatNonImageDisplay = function(imageContainer, imageNode, $originalLink) {
    new Promise((resolve) => {
        if (['VIDEO'].includes(imageNode.children[0].tagName))
            resolve(imageNode.children[0]);
        else {
            let imageContentOvserver = new MutationObserver((mutationsList, observer) => {
                observer.disconnect();
                for (let mutation of mutationsList)
                    if (mutation.addedNodes.length == 1)
                        resolve(mutation.addedNodes[0]);
            });
            imageContentOvserver.observe(imageNode, {attributes: false, childList: true, subtree: false});
        }
    }).then((content) => {
        createContentMaximizer(
            imageContainer,
            imageNode,
            $originalLink,
            content
        );
    })
}

contentDisplayMenuListener = null;

formatContentDisplay = function(imageContainer) {
    let imageNode = null;

    contentDisplayMenuListener = new ContextMenuListener((contextMenu) => {
        contextMenu.appendItemGroup(1);
        contextMenu.appendItem('Copy Original Image', () => {
            copyImageFromUrl(getContentSrc(imageNode.children[0]));
            setTimeout(() => { contextMenu.close(); }, 0);
        });
        contextMenu.appendItem('Save Original Image', () => {
            downloadFromUrl(getContentSrc(imageNode.children[0]));
            setTimeout(() => { contextMenu.close(); }, 0);
        });
    });

    if (imageContainer.classList.contains('imageWrapper-2p5ogY')) {
        imageNode = imageContainer;
        imageContainer = imageContainer.parentNode;
    }
    else
        imageNode = imageContainer.querySelector(".imageWrapper-2p5ogY");

    if (imageNode && imageNode.children[0]) {
        let $originalLink = $(imageContainer.querySelector("a"));

        if (imageNode.children[0].tagName == 'IMG') {
            createContentMaximizer(
                imageContainer,
                imageNode,
                $originalLink,
                imageNode.children[0]
            );
        }
        else
            formatNonImageDisplay(imageContainer, imageNode, $originalLink);
    }
}

cancelContentDisplay = function() {
    contentDisplayMenuListener.destroy();
}

getContentSrc = function(content) {
    if (content) {
        try {
            return $(content).attr('src').split('?')[0];
        }
        catch (err) { }
    }

    return "";
}