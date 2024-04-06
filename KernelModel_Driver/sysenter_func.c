#include "ntddk.h"
#include "mydef.h"

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\WinDanger");
UNICODE_STRING sddl = RTL_CONSTANT_STRING(L"D:P(A;;GA;;;WD)");
UNICODE_STRING DeviceGUID = RTL_CONSTANT_STRING(L"23333333-2333-2333-2333-233333333333");
PDEVICE_OBJECT g_DeviceObj = 0;
UNICODE_STRING DeviceSymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\WinDangerLink");
UINT8 *pIOPM = NULL;

//void Ke386SetIoAccessMap();
//void Ke386IoSetAccessProcess();

NTSTATUS sysenter_handler(PDEVICE_OBJECT DeviceObj, PIRP myIRP)
{
	if (DeviceObj != 0 && myIRP != 0)
	{
		DbgPrint("On DeviceCTL DeviceObj and myIRP Valid 233!\n");
		if (DeviceObj != g_DeviceObj)
		{
			DbgPrint("Go wrong dispatch place!\n");
			return STATUS_BAD_DEVICE_TYPE;
		}
		PIO_STACK_LOCATION myIRPsp = IoGetCurrentIrpStackLocation(myIRP);
		DbgPrint("IOCTL code is 0x%X\n", myIRPsp->Parameters.DeviceIoControl.IoControlCode);
		switch (myIRPsp->Parameters.DeviceIoControl.IoControlCode)
		{
		    case 0:
                DbgPrint("Here is 0!\n");
                DbgBreakPoint();
                Disable_WriteProtect();
				DbgPrint("We hacked to Disable Write-Protection!\n");
                break;
            case 1:
				DbgPrint("Here is 1!\n");
                DbgBreakPoint();
				HackGDT();

				// if (pIOPM == NULL)
				// {
				// 	pIOPM = MmAllocateNonCachedMemory(65536 / 8);
				// 	RtlZeroMemory(pIOPM, 65536 / 8);
				// }
				// Ke386SetIoAccessMap(1, pIOPM); // Copy IOPM to TSS
				// Ke386IoSetAccessProcess(IoGetCurrentProcess(), 1);
				// IOPL setting func not work on Win10/Server2019, should manully do it.
				
                break;
			default:
				DbgPrint("Here is default!\n");
                DbgBreakPoint();
				NOP_Toy();
				break;
		}
		myIRP->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(myIRP, IO_NO_INCREMENT); //No this will cause bug in usermode!
		return STATUS_SUCCESS;
	}

	return STATUS_ILLEGAL_INSTRUCTION;
}

void MuShi()
{
    DbgPrint("MuShi Called!");
}