# Hack Code Segment DPL in GDT to Ring0

**Hack Code Segment only is NOT enough to elevate ring3 code to ring0.**

The Segment Register (Like CS, DS, etc) value refering to the Segment Descriptor in GDT (Global Descriptor Table).

Each Segment Descriptor is 64bit.

We are Interested in 5th byte (47 - 40bits), is Access Byte.

The 46th and 45th bits decide the DPL. Just change it to 0.

The 42th control the acceptance of lower level code. If we hacked all code segment DPL to 0, we should set it to 1, otherwise the existing ring3 code will crash.

[GDT Info on OSDev](https://wiki.osdev.org/Global_Descriptor_Table#:~:text=The%20Global%20Descriptor%20Table%20%28GDT%29%20is%20a%20binary,Descriptor%20Table%20exists%20containing%20task%20and%20interrupt%20descriptors.)