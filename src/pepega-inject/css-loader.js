window.headInjectScripts = function() {
    if (window.pepegaInject.pepegaCssCode) {
        var headElement = null;
        var customCssTag = document.getElementById('pepega-inject-css');
    
        if (!customCssTag)
            headElement =  document.getElementsByTagName('head')[0];
    
        if (!customCssTag) {
            customCssTag = document.createElement("style");
            customCssTag.innerHTML = window.pepegaInject.pepegaCssCode;
            customCssTag.id = 'pepega-inject-css';
    
            headElement.appendChild(customCssTag);
        }
    }
}
window.headInjectScripts();

window.headMutationObserver = new MutationObserver(window.headInjectScripts);
window.headMutationObserver.observe(document.getElementsByTagName('head')[0], { attributes: false, childList: true, subtree: false });