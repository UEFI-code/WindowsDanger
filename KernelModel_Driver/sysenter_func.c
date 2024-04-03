#include "ntddk.h"
#include "mydef.h"

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\WinDanger");
UNICODE_STRING sddl = RTL_CONSTANT_STRING(L"D:P(A;;GA;;;WD)");
UNICODE_STRING DeviceGUID = RTL_CONSTANT_STRING(L"23333333-2333-2333-2333-233333333333");
PDEVICE_OBJECT g_DeviceObj = 0;
UNICODE_STRING DeviceSymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\WinDangerLink");

NTSTATUS sysenter_handler(PDEVICE_OBJECT DeviceObj, PIRP myIRP)
{
	if (DeviceObj != 0 && myIRP != 0)
	{
		DbgPrint("On DeviceCTL DeviceObj and myIRP Valid 233!");
		if (DeviceObj != g_DeviceObj)
		{
			DbgPrint("Go wrong dispatch place!");
			return STATUS_BAD_DEVICE_TYPE;
		}
		PIO_STACK_LOCATION myIRPsp = IoGetCurrentIrpStackLocation(myIRP);
		DbgPrint("IOCTL code is 0x%X", myIRPsp->Parameters.DeviceIoControl.IoControlCode);
		switch (myIRPsp->Parameters.DeviceIoControl.IoControlCode)
		{
		    case 0:
                DbgPrint("Here is 0!");
                break;
            case 1:
                DbgPrint("Here is 1!");
                break;
		}
		myIRP->IoStatus.Status = STATUS_SUCCESS;
		IoCompleteRequest(myIRP, IO_NO_INCREMENT); //No this will cause bug in usermode!
		return STATUS_SUCCESS;
	}

	return STATUS_ILLEGAL_INSTRUCTION;
}