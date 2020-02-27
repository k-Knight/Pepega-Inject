processVideoEmbeds = function(node) {
    let videoEmbeds = node.querySelectorAll('.embedVideo-3nf0O9');
    if (videoEmbeds && videoEmbeds.length > 0)
        for (let videoEmbed of videoEmbeds) {
            if (!videoEmbed.qoltProcessed) {
                formatVideoContainer(videoEmbed);
                videoEmbed.qoltProcessed = true;
            }
        }

    let videos = node.querySelectorAll('.wrapper-2TxpI8 video');
    if (videos && videos.length > 0)
        for (let video of videos) {
            if (!video.qoltProcessed) {
                formatVideoTag(video);
                video.qoltProcessed = true;
            }
        }
}

formatVideoContainer = function(videoContainer) {
    let videoLink = videoContainer.querySelector(".embedVideoActions-O6vR7W .anchor-3Z-8Bb.anchorUnderlineOnHover-2ESHQB");

    if (videoLink) {
        let url = $(videoLink).attr('href');
        url = url.replace(/https.*watch\?v=/, '').replace(/\//, '');
        url = 'https://www.youtube.com/embed/' + url + '?autoplay=1&auto_play=1';

        let buttonContainer = videoContainer.querySelector(".embedVideoActions-O6vR7W .wrapper-129saQ");
        if (buttonContainer)
            addOpenInWindowButton(
                buttonContainer,
                url,
                'iconWrapperActive-12kkfE iconWrapper-21idzA qolt-video-new-window'
            );
    }
}

formatVideoTag = function(videoNode) {
    let metaNode = videoNode.parentNode.querySelector('.metadata-13NcHb');
    let htmlCode = `data:text/html;charset=utf-8,
        <html>
        <head>
            <style>
                * {
                    margin: 0;
                    padding: 0;
                    background-color: #202225;
                }
                video {
                	position: fixed;
                    width: 100vw;
                    height: 100vh;
                }
            </style>
        </head>
        <body>
            <video playsinline controls
                poster="${$(videoNode).attr('poster')}"
                preload="metadata"
                src="${$(videoNode).attr('src')}">
            </video>
        </body>
        </html>
    `;
    if (metaNode)
        addOpenInWindowButton(
            metaNode,
            htmlCode,
            'anchorUnderlineOnHover-2ESHQB metadataDownload-1fk90V qolt-video-new-window'
        );
}

addOpenInWindowButton = function(buttonContainer, url, classAttr) {
    if (!url)
        return;

    if (buttonContainer) {
        let $maximizeBtn = $("<button>").html(resources.maximizeSvg);
        $maximizeBtn.click(() => {
            if (!$maximizeBtn.qoltOpened) {
                $maximizeBtn.qoltOpened = true;
                openWindow(url);

                setTimeout(() => { $maximizeBtn.qoltOpened = false; }, 1000);
            }
        });

        $('<div>')
            .attr('class', classAttr)
            .append($maximizeBtn)
            .appendTo($(buttonContainer));
    }
}