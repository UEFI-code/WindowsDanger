#include "ntddk.h"
#include "mydef.h"

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
                // DbgBreakPoint();
				KeIpiGenericCall(Disable_WriteProtect, NULL);
				DbgPrint("We hacked to Disable Write-Protection!\n");
                break;
            case 1:
				DbgPrint("Here is 1!\n");
                // DbgBreakPoint();
				HackGDT();
				DbgPrint("We hacked GDT to elevate!\n");
                break;
			case 2:
				DbgPrint("Here is 2!\n");
                // DbgBreakPoint();
				PHYSICAL_ADDRESS theCR3 = { 0 };
				theCR3.QuadPart = (UINT64)GetCR3Value() & 0xFFFFFFFFFFFFFF00;
				void* cr3_mapped_p = MmMapIoSpace(theCR3, 0xfff * 8, MmNonCached);
				if (cr3_mapped_p != NULL)
				{
					DbgPrint("Successing Mapped CR3 %p -> %p\n", (void *)theCR3.QuadPart, cr3_mapped_p);
				}
				break;
			case 3:
				DbgPrint("Here is 3!\n");
				// DbgBreakPoint();
				RegisterMyINTHandler();
				DbgPrint("We registered INT 0x78 and 0x79!\n");
				break;
			case 4:
				DbgPrint("Here is 4!\n");
				// DbgBreakPoint();
				KeIpiGenericCall(Disable_SMAP_SMEP, NULL);
				DbgPrint("We disabled SMAP and SMEP!\n");
				break;
			default:
				DbgPrint("Here is default!\n");
                // DbgBreakPoint();
				PKTHREAD pCurrentThread = KeGetCurrentThread();
				//PKTRAP_FRAME pTrap_frame = (PKTRAP_FRAME)((UINT8 *)pCurrentThread->KernelStack + 384);
				//DbgPrint("RSI=%llX, RDI = %llX, CS = %x\n", pTrap_frame->Rsi, pTrap_frame->Rdi, pTrap_frame->SegCs);
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