# Hack Code Segment DPL in GDT to Ring0

**Hack Code Segment DPL in GDT only is NOT enough to elevate ring3 threads to ring0.**

The Segment Register (Like CS, DS, etc) value refering to the Segment Descriptor entries in GDT (Global Descriptor Table).

Each Segment Descriptor is 64bit.

We are interested in the 5th byte (47 - 40bits) in each Code Segment Descriptor, is Access Byte.

The 46th and 45th bits decide the DPL. Just change it to 0.

The 42th control the acceptance of lower privliage code running here. If we hacked all code segment DPL to 0, we should set it to 1, otherwise the existing ring3 threads will crash.

[GDT Info on OSDev](https://wiki.osdev.org/Global_Descriptor_Table#:~:text=The%20Global%20Descriptor%20Table%20%28GDT%29%20is%20a%20binary,Descriptor%20Table%20exists%20containing%20task%20and%20interrupt%20descriptors.)

[Segment Selector on OSDev](https://wiki.osdev.org/Segment_Selector)

**Also be noted, in x64 Long Mode, GDT is not limited the Memory Access.**

**But if you attempt to jump RIP to 'Ring3 Code Area' from Ring0 DIRECTLY, you will still got exception.**

**That is because the Paging Mechanism still effective.**

**So you might also hacking the Page Table.**

## Watch the GDT

in WinDbg Command Line:

```
db gdtr
```

Then you will found something like (Windows Server 2022):

```
kd> db gdtr
fffff804`1b4fafb0  00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00  ................
fffff804`1b4fafc0  00 00 00 00 00 9b 20 00-00 00 00 00 00 93 40 00  ...... .......@.
fffff804`1b4fafd0  ff ff 00 00 00 fb cf 00-ff ff 00 00 00 f3 cf 00  ................
fffff804`1b4fafe0  00 00 00 00 00 fb 20 00-00 00 00 00 00 00 00 00  ...... .........
fffff804`1b4faff0  67 00 00 90 4f 8b 00 1b-04 f8 ff ff 00 00 00 00  g...O...........
fffff804`1b4fb000  00 3c 00 00 00 f3 40 00-00 00 00 00 00 00 00 00  .<....@.........
fffff804`1b4fb010  00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00  ................
fffff804`1b4fb020  00 00 00 00 00 00 00 00-00 00 00 00 00 00 00 00  ................
```

Then you can change the 0xfb to 0x9f to achieve the hack manually.

## Hack the GDT Automatically

Load the "KernelModel_Driver" in this project, and run the "UserMode_Caller" in this project.

Type the control code "1" and press enter.

Then you will see the GDT is hacked.

**Pls note that you need attach the WinDbg in Kernel Mode before you run the "UserMode_Caller", or the "DbgBreakPoint" might crash the kernel.**