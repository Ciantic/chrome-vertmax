
    var plugin = document.getElementById("pluginId");
    var enabled = true;
    
    // Maximize new instance of chrome too
    plugin.onWindowCreated();
    
    chrome.browserAction.onClicked.addListener(function (tab) {
        enabled = !enabled;
        
        if (enabled) {
            chrome.browserAction.setIcon({
                "path" : "icon.png"
            });
        } else {
            chrome.browserAction.setIcon({
                "path" : "icon_disabled.png"
            });
        }
    });
    
    chrome.windows.onCreated.addListener(function (win) {
        // Vertical maximize only normal windows
        if (enabled && win.type == 'normal')
            plugin.onWindowCreated();
    });