==============
chrome-vertmax
==============

Chrome extension that vertical mazimize newly created windows under Windows 7.

Based on ``npsimple`` NPAPI example.

Developers
==========

Build NPAPI plugin
------------------

- Visual Studio 2008
- Download `xulrunner-sdk <https://developer.mozilla.org/en/gecko_sdk#Downloading>`_ and extract contents to ``plugin/xulrunner-sdk``
- Open solution and press *build*.

Testing the chrome extension
----------------------------

Now to test the extension use::
    
    chrome.exe --load-extension="C:\...\chrome-vertmax"

Notice that this *runs the extension* from that directory in *developer mode*, for installing use `--install-extension`.