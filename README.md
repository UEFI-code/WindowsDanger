# WindowsDanger

WindowsDanger is an open-source project that help developers to Learn Windows Kernel and other x86 Features.

**But, We are NOT modify/patch Windows Code**

**We Directly Modify x86 Context Instead**

**So, We will not bypass PatchGuard, Using Official Test-Mode Instead**

This driver **Dreamed** to assist developers accessing and manipulating system resources more conveniently. By installing and launching this driver, you can elevate all threads to Ring0, allowing for full control over low-level hardware and system resources.  
  
⚠️ **Warning**: Please be aware that using WindowsDanger may result in system instability, data loss, or other unintended consequences. Use with caution and perform testing in a safe environment.

This project is still under development. For more quick access, please use the [QEMU_Danger_x86](https://github.com/UEFI-code/QEMU_Danger_x86) project.

```Ke386SetIoAccessMap``` and ```Ke386IoSetAccessProcess``` is NOT available in Win10/Server 2019 and above. It's better to learn how the x86 CPU work within TSS and IOPM.

Currently achieved:
- Disable Write-Protection by modifiy CR0
- Hack Ring3 Segment in GDT to Ring0
- Hack TSS IOPM (0~FF)
- Insert new user-callable IDT entries 78H and 79H, 78H will hack IOPL -> 3, while 79H will hack thread's CS & SS to Ring0
- Disable SMAP/SMEP by modify CR4
- Adapt Multi-Processor System

Known Issues:
- After EXE entered Ring0, the nt!SwapContext may not working properly, and result in #DF.
```
04 fffff801`103eed10 fffff801`11036efb     nt!KiBugCheckDispatch+0x69
05 fffff801`103eee50 fffff801`110316d2     nt!KiDoubleFaultAbort+0x2fb
06 000001c1`78380cd0 fffff801`11031484     nt!SwapContext+0x1b2
07 000001c1`78380d10 fffff801`11030b35     nt!KxDispatchInterrupt+0x144
08 000001c1`78380e50 fffff801`1102ba11     nt!KiDpcInterruptBypass+0x25
09 000001c1`78380e60 00007ff6`cabf1b2c     nt!KiInterruptDispatchNoLockNoEtw+0xb1
0a 000001c1`78380ff8 0000002d`df37fc08     0x00007ff6`cabf1b2c
0b 000001c1`78381000 00000000`00000000     0x0000002d`df37fc08
```
- This problem might caused by NT lazy-mem-allocation
- U may use less-paged-rsp & `cli` to reduce #DF

Ideas:
- Use soft ```int``` instruct from Ring3, and hack stack for return CS RPL -> 0, then ```iretq```.
- Hack IOPL is the same

## Features

- Ring3 thread can touch IO (0~FF) directly after `int 078h`
- Elevate any threads to Ring0 for full control over low-level system resources by simply `int 079h`
- Facilitate hardware debugging and kernel-hack study
- Tested on Windows Server 2022 x64, on Hyper-V

### Hint

- I recommend u play on QEMU / Physical PC, as Hyper-V may acting strange.
- I found Azure Hypervisor may strict on vmexit, if u hack CR0/GDT, u'll recieve undef exception. 
```
Unknown exception - code c0000096 (!!! second chance !!!)
WindowsDanger!Disable_WriteProtect+0x9:
fffff802`662c112e 0f22c0          mov     cr0,rax
```
- If u buy PC / Network-Card, make sure it support NT-Kernel-Dbg. U can search those info on MS-Learn.

## Installation

**MUST Attach Kernel Debugger to prevent BSOD**, because ```int 3``` will trigger BSOD without Kernel Debugger.

1. Download the latest version of the WindowsDanger driver.  
2. Copy the driver file to an appropriate directory (e.g., `C:\kd`).  
3. Run the following command with administrator privileges to install the driver:

```C
sc create WindowsDanger type= kernel binPath= C:\kd\WindowsDanger.sys
```

4. Start the driver:  
```C
sc start WindowsDanger
```

## Play

- [IoCTL_Caller](UserMode_Caller): Send Requests to our Kernel Driver. Supported Commands:
    - 0 : Disable Write-Protection by modifiy CR0
    - 1 : Hack Ring3 Segment in GDT to Ring0 & Hack TSS_IOPM
    - 2 : Mapping CR3 to a Virtual Address
    - 3 : Insert new user-callable IDT entries 78H and 79H
    - 4 : Disable SMAP/SMEP by modify CR4

- [InterruptTester](UserMode_InterruptTester): Test our new IDT entries 78H and 79H
    - 78H : Hack IOPL -> 3
    - 79H : Hack current thread's CS & SS to Ring0 !!

**Please note, before you use `int 078h/079h`, you must Hack the GDT, Disable SMAP/SMEP, and Insert new IDT entries by (1, 4, 3).**

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

## Learning Resources

0. [Lab Experiment HandBooks](LabHandBooks): Contains Microsoft Learn Student Ambassador Workshop
1. Download WinDbg
2. Install [WDK](https://learn.microsoft.com/zh-cn/windows-hardware/drivers/download-the-wdk?wt.mc_id=studentamb_188195)
3. Read [Kernel Debugging Document](https://learn.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-up-a-network-debugging-connection?wt.mc_id=studentamb_188195) on the Microsoft Learn
4. Read [My Experiment log](Experiment_Record_GPT4.md) for more details
5. Cource: [Arch2001_x86-64_OS_Internals](https://apps.p.ost2.fyi/learning/course/course-v1:OpenSecurityTraining2+Arch2001_x86-64_OS_Internals+2021_v1/home)
  
## License

WindowsDanger is licensed under the [MIT License](LICENSE). Please refer to the [LICENSE](LICENSE) file for more information.

## Contributing

We welcome your contributions to the WindowsDanger project! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for how to get started.

## Support and Feedback

If you encounter any issues while using WindowsDanger, or have any suggestions and feedback, please submit your concerns on the [Issues](https://github.com/UEFI-code/WindowsDanger/issues) page.

## Disclaimer

WindowsDanger is intended for learning and research purposes only. The use of this driver may result in system instability, data loss, or other unintended consequences. The project authors and contributors are not responsible for any damages or losses resulting from the use of this driver. By using WindowsDanger, you agree to assume all risks.