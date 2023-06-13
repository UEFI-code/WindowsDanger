#include "ntddk.h"

UINT8 *GDTDescriptorTable = 0;
UINT8 *GDTBackup = 0;

void GetGDT(UINT8 *Buf);
void SetGDT(UINT8 *Buf);
void DisableInterrupts();
void EnableInterrupts();

VOID HackGDT()
{
    // Alloc memory for GDTDescriptorTable, x64 is 10 bytes
    GDTDescriptorTable = ExAllocatePool(NonPagedPool, 10);
    if (!GDTDescriptorTable)
    {
        DbgPrint("Failed to allocate memory for GDTDescriptorTable\n");
        return STATUS_UNSUCCESSFUL;
    }
    DisableInterrupts();
    GetGDT(GDTDescriptorTable);
    DbgPrint("GDTDescriptorTable: %p\n", GDTDescriptorTable);
    DbgPrint("GDT Length: %x\n", *(UINT16 *)(GDTDescriptorTable + 0));
    DbgPrint("GDT Base: %x\n", *(UINT64 *)(GDTDescriptorTable + 2));
    GDTBackup = ExAllocatePool(NonPagedPool, *(UINT16 *)(GDTDescriptorTable + 0));
    if (!GDTBackup)
    {
        DbgPrint("Failed to allocate memory for GDTBackup\n");
        return STATUS_UNSUCCESSFUL;
    }
    RtlCopyMemory(GDTBackup, (UINT8 *)*(UINT64 *)(GDTDescriptorTable + 2), *(UINT16 *)(GDTDescriptorTable + 0));
    DbgPrint("GDTBackup: %p\n", GDTBackup);
    UINT8 *NewGDT = ExAllocatePool(NonPagedPool, *(UINT16 *)(GDTDescriptorTable + 0));
    if (!NewGDT)
    {
        DbgPrint("Failed to allocate memory for NewGDT\n");
        return STATUS_UNSUCCESSFUL;
    }
    DbgPrint("NewGDT: %p\n", NewGDT);
    RtlCopyMemory(NewGDT, GDTBackup, *(UINT16 *)(GDTDescriptorTable + 0));
    for(UINT16 i = 0; i < *(UINT16 *)(GDTDescriptorTable + 0); i += 8)
    {
        NewGDT[i + 5] &= 0x9F;
    }
    SetGDT(NewGDT);
    DbgPrint("GDT Hack Done!\n");
    EnableInterrupts();
}