#include "ntddk.h"

UINT8 *BypassWP(UINT8 *Addr, UINT64 Size);

VOID HackTSS(UINT8 *TSS_Seg_RW)
{
    TSS_Seg_RW[0] = 0xFF; TSS_Seg_RW[1] = 0xFF;
    UINT8 *TSS_Base =
        ((UINT64)TSS_Seg_RW[2])        |
        ((UINT64)TSS_Seg_RW[3]  << 8)  |
        ((UINT64)TSS_Seg_RW[4]  << 16) |
        ((UINT64)TSS_Seg_RW[7]  << 24) |
        ((UINT64)TSS_Seg_RW[8]  << 32) |
        ((UINT64)TSS_Seg_RW[9]  << 40) |
        ((UINT64)TSS_Seg_RW[10] << 48) |
        ((UINT64)TSS_Seg_RW[11] << 56);
    DbgPrint("TSS Base: %p\n", TSS_Base);
    TSS_Base = BypassWP(TSS_Base, 0x68+32);
    DbgPrint("TSS Base Bypass WP: %p\n", TSS_Base);
    memset(TSS_Base+0x68, 0, 32); // Set IOPM
    DbgPrint("Hacked IOPM 0-FF!\n");
    MmUnmapIoSpace(TSS_Base, 0x68+32);
    return;
}