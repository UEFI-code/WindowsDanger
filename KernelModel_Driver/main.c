#include <ntddk.h>
#include <ntstrsafe.h>
//#include <windef.h>
//#include <Winternl.h>
//#include <ntifs.h>
#include "mydef.h"

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    DbgPrint("Driver Unload Called\n");
    IoDeleteSymbolicLink(&DeviceSymbolicLinkName);
    IoDeleteDevice(g_DeviceObj);
    pDriverObject->DriverUnload = NULL;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {  
    NTSTATUS status;  
  
    //status = EnumerateThreadIds();  
    /*if (!NT_SUCCESS(status)) {  
        KdPrint(("EnumerateThreadIds failed with status 0x%X\n", status));  
    } */ 
  
    driver_object->DriverUnload = DriverUnload;
    KdPrint(("Driver Loaded at RegistryPath: %wZ\n", registry_path));

    // RegisterMyINTHandler(); Modify Interrupt Descriptor Table will cause Kernel Crash, But you can try this to Learn!!
    // Let's just create a device and let Ring3 exe to call it, then our function will be exeecute under that process! To avoid BSOD
    status = IoCreateDeviceSecure(driver_object, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &sddl, (LPCGUID)&DeviceGUID, &g_DeviceObj);
    if (NT_SUCCESS(status))
    {
        DbgPrint("Create Device Success!\n");
        driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = sysenter_handler;
        driver_object->MajorFunction[IRP_MJ_CREATE] = MuShi;
        driver_object->MajorFunction[IRP_MJ_CLOSE] = MuShi;
        IoCreateSymbolicLink(&DeviceSymbolicLinkName, &DeviceName);
    }

    return STATUS_SUCCESS;  
}

