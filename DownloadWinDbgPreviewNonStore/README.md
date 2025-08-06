# Update...

U dont need WinDbg-Preview at all...
WinDbg-Legacy already has all features...

# Download WinDbg Preview without using the Microsoft Store

If you don't want to use the Microsoft Store, you can using the ```way1.ps1``` script to download WinDbg Preview.

## Acknowledgements

- [My GitHub Issue](https://github.com/microsoft/WinDbg-Samples/issues/57)

Thanks for ```lzybkr``` who provided this script to show how the Microsoft Store works.

The ```windbg.appinstaller``` is a ```XML``` file which contains the information of WinDbg Preview (Metadata).

There is ```Uri``` in the ```XML``` file which points to the download link of ```windbg.msixbundle```, and the ```windbg.msixbundle``` is a compressed file which contains the WinDbg Preview Appx package. Just open it as ```zip``` file, and you can see the ```msix``` files inside it then.

The ```msix``` file is also a compressed file, just decompress it as ```zip``` file, and you got the WinDbg Preview finally!