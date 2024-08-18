# WindowsDanger  
  
WindowsDanger is an open-source project that help developers to Learn & Hack Windows Kernel.

This driver **Dreamed** to assist developers, hardware debuggers, and reverse engineers in accessing and manipulating system resources more conveniently. By installing and launching this driver, you can elevate all threads to Ring0, allowing for full control over low-level hardware and system resources.  
  
⚠️ **Warning**: Please be aware that using WindowsDanger may result in system instability, data loss, or other unintended consequences. Use with caution and perform testing in a safe environment. 

This project is still under development. For more quick access, please use the [QEMU_Danger](https://github.com/UEFI-code/QEMU_Danger) project.

```Ke386SetIoAccessMap``` and ```Ke386IoSetAccessProcess``` is NOT available in Win10/Server 2019 and above. It's better to learn how the x86 CPU work within TSS and IOPM.

Currently achieved:
- Disable Write-Protection by modifiy CR0
- Hack Ring3 Segment in GDT to Ring0
- Insert a new IDT entry 78H

## Learning Resources

0. [Lab Experiment HandBooks](LabHandBooks): Contains Microsoft Learn Student Ambassador Workshop
1. Download the WinDbg Preview tool from the Microsoft Store or [Non Store](DownloadWinDbgPreviewNonStore)
2. Install WDK [https://learn.microsoft.com/zh-cn/windows-hardware/drivers/download-the-wdk?wt.mc_id=studentamb_188195]
3. Read Kernel Debugging Document on the Microsoft Learn [https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-up-a-network-debugging-connection?wt.mc_id=studentamb_188195]
4. Read [My Experiment log](Experiment_Record_GPT4.md) for more details

## Features  
  
- Elevate all threads to Ring0 for full control over low-level system resources  
- Facilitate hardware debugging and reverse engineering tasks  
- Support for Windows operating systems  
  
## Installation  
  
1. Download the latest version of the WindowsDanger driver.  
2. Copy the driver file to an appropriate directory (e.g., `C:\Windows\System32\drivers`).  
3. Run the following command with administrator privileges to install the driver:  
  
```C
sc create WindowsDanger type= kernel binPath= C:\Windows\System32\drivers\WindowsDanger.sys
```
  
4. Start the driver:  
```C
sc start WindowsDanger
```

  
## Uninstallation  
  
1. Stop the driver:  
```C
sc stop WindowsDanger
```

2. Remove the driver:  
```C
sc delete WindowsDanger
```

  
3. Delete the driver file.  
  
## License  
  
WindowsDanger is licensed under the [MIT License](LICENSE). Please refer to the [LICENSE](LICENSE) file for more information.  
  
## Contributing  
  
We welcome your contributions to the WindowsDanger project! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for how to get started.  
  
## Support and Feedback  
  
If you encounter any issues while using WindowsDanger, or have any suggestions and feedback, please submit your concerns on the [Issues](https://github.com/UEFI-code/WindowsDanger/issues) page.  
  
## Disclaimer  
  
WindowsDanger is intended for learning and research purposes only. The use of this driver may result in system instability, data loss, or other unintended consequences. The project authors and contributors are not responsible for any damages or losses resulting from the use of this driver. By using WindowsDanger, you agree to assume all risks.  
