#include <ntddk.h>
#include <wdmsec.h>

extern UNICODE_STRING DeviceName;
extern UNICODE_STRING sddl;
extern UNICODE_STRING DeviceGUID;
extern PDEVICE_OBJECT g_DeviceObj;
extern UNICODE_STRING DeviceSymbolicLinkName;

NTSTATUS ioctl_handler(PDEVICE_OBJECT DeviceObj, PIRP myIRP);
void MuShi(); void NOP_Toy();

void Disable_WriteProtect();
UINT8 *BypassWP(UINT8 *Addr, UINT64 Size);
void Disable_SMAP_SMEP();
void DisableInterrupts(); void EnableInterrupts();

void* GetCR3Value();

void GetGDT(UINT8 *Buf); void SetGDT(UINT8 *Buf);
void HackGDT();
void HackTSS(UINT8 *TSS_Seg_RW);

void HackIDT();