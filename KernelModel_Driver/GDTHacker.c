#include "ntddk.h"

UINT8 GDTDescriptorTable[10] = {0};

void GetGDT(UINT8 *Buf);
void SetGDT(UINT8 *Buf);
void DisableInterrupts();
void EnableInterrupts();
void Disable_WriteProtect();

VOID HackGDT()
{
    DisableInterrupts();
    Disable_WriteProtect();
    GetGDT(GDTDescriptorTable);
    UINT16 GDT_Len = *(UINT16 *)(GDTDescriptorTable + 0);
    UINT8 *GDT_Base = (UINT8 *)(*(UINT64 *)(GDTDescriptorTable + 2));
    DbgPrint("GDT Length: %d\n", GDT_Len);
    DbgPrint("GDT Base: %p\n", GDT_Base);
    UINT8 *Now_GDT = GDT_Base;
    for(UINT16 i = 0; i < GDT_Len; i += 1)
    {
        if(Now_GDT[5] & 0x80)
        {
            DbgPrint("This Segment %d is Enabled!\n", i);
            if (Now_GDT[5] & 0x10)
            {
                // Code or Data Segment
                if (Now_GDT[5] & 0x08)
                {
                    DbgPrint("This Segment is Code!\n");
                    if ((Now_GDT[5] & 0x60) == 0x60)
                    {
                        DbgPrint("This Code Segment is For Ring3, Start Patching!!!\n");
                        Now_GDT[5] &= 0x9F;
                        Now_GDT[5] |= 0x04; // Prevent Crash if Code run to Ring0 Segment suddenly.
                    }
                    else
                    {
                        DbgPrint("This Code Segment is For Ring0, bypass patch!\n");
                    }
                }
                else
                {
                    DbgPrint("This Segment is Data, bypass patch!\n");
                }
            }
            else
            {
                DbgPrint("This Segment is system segment, bypass patch!\n");
            }
        }
        else
        {
            DbgPrint("This Segment %d is Disabled, bypass patch!\n", i);
        }
        
        // Now chk for offset.
        if((Now_GDT[5] & 0x10) == 0)
        {
            // If CPU run in x64 and the segment is system, no matter 6th bytes...
            Now_GDT += 16;
        }
        else
        {
            Now_GDT += 8;
        }
    }
    DbgPrint("GDT Hack Done!\n");
    EnableInterrupts();
}