#include <ntddk.h>
#include <ntstrsafe.h>
//#include <windef.h>
//#include <Winternl.h>
//#include <ntifs.h>
#include "mydef.h"

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    DbgPrint("Driver Unload Called\n");
    pDriverObject->DriverUnload = NULL;
}  

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {  
    NTSTATUS status;  
  
    status = EnumerateThreadIds();  
    if (!NT_SUCCESS(status)) {  
        KdPrint(("EnumerateThreadIds failed with status 0x%X\n", status));  
    }  
  
    driver_object->DriverUnload = DriverUnload;
    KdPrint(("Driver Loaded at RegistryPath: %wZ\n", registry_path));

    return STATUS_SUCCESS;  
}  

