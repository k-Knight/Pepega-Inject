formatVideoContainer = function(videoContainer) {
    let videoLink = videoContainer.querySelector(".embedVideoActions-O6vR7W .anchor-3Z-8Bb.anchorUnderlineOnHover-2ESHQB");

    if (videoLink) {
        let url = $(videoLink).attr('href');
        url = url.replace(/https.*watch\?v=/, '').replace(/\//, '');
        url = 'https://www.youtube.com/embed/' + url + '?autoplay=1&auto_play=1';

        addOpenInWindowButton(videoContainer, url);
    }
}

addOpenInWindowButton = function(videoContainer, url) {
    let buttons = videoContainer.querySelector(".embedVideoActions-O6vR7W .wrapper-129saQ");

    if (buttons) {
        let $maximizeBtn = $("<button>").html(resources.maximizeSvg);
        $maximizeBtn.click(() => {
            if (!$maximizeBtn.qoltOpened) {
                $maximizeBtn.qoltOpened = true;
                openWindow(url);

                setTimeout(() => { $maximizeBtn.qoltOpened = false; }, 1000);
            }
        });

        $('<div>')
            .attr('class', 'iconWrapperActive-12kkfE iconWrapper-21idzA qolt-video-new-window')
            .append($maximizeBtn)
            .appendTo($(buttons));
    }
}