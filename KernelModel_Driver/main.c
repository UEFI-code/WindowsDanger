#include "mydef.h"

UNICODE_STRING DeviceName = RTL_CONSTANT_STRING(L"\\Device\\WinDanger");
UNICODE_STRING sddl = RTL_CONSTANT_STRING(L"D:P(A;;GA;;;WD)");
UNICODE_STRING DeviceGUID = RTL_CONSTANT_STRING(L"23333333-2333-2333-2333-233333333333");
PDEVICE_OBJECT g_DeviceObj = 0;
UNICODE_STRING DeviceSymbolicLinkName = RTL_CONSTANT_STRING(L"\\??\\WinDangerLink");

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    DbgPrint("Driver Unload Called\n");
    IoDeleteSymbolicLink(&DeviceSymbolicLinkName);
    IoDeleteDevice(g_DeviceObj);
    pDriverObject->DriverUnload = NULL;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {  
    NTSTATUS status;

    driver_object->DriverUnload = DriverUnload;
    KdPrint(("Driver Loaded at RegistryPath: %wZ\n", registry_path));

    status = IoCreateDeviceSecure(driver_object, 0, &DeviceName, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &sddl, (LPCGUID)&DeviceGUID, &g_DeviceObj);
    if (NT_SUCCESS(status))
    {
        DbgPrint("Create Device Success!\n");
        driver_object->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ioctl_handler;
        driver_object->MajorFunction[IRP_MJ_CREATE] = MuShi;
        driver_object->MajorFunction[IRP_MJ_CLOSE] = MuShi;
        IoCreateSymbolicLink(&DeviceSymbolicLinkName, &DeviceName);
    }

    return STATUS_SUCCESS;  
}

