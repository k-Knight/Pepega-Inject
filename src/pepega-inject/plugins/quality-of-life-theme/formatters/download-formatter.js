setDownloadClickHandler = function($link) {
    let url = $link.attr("href");
    let fileName = url.split("/");
    fileName = fileName[fileName.length - 1];

    $link.on("click", (event) => {
        event.preventDefault();
        downloadItem(url, fileName);
    });
}

formatDownloadLinks = function(node) {
    var links = [];

    if (node.classList.contains("attachment-33OFj0")) {
        links = node.querySelectorAll(".anchor-3Z-8Bb");
    }
    else if (node.classList.contains("imageWrapper-2p5ogY")) {
        links = node.querySelectorAll(".metadata-13NcHb .anchor-3Z-8Bb");
    }

    for (let link of links) {
        setDownloadClickHandler($(link));
    }
}