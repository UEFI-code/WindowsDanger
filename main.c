#include <ntddk.h>
#include <ntstrsafe.h>
//#include <windef.h>
//#include <Winternl.h>
#include <ntifs.h>
#include "mydef.h"

VOID DriverUnload(PDRIVER_OBJECT pDriverObject)
{
    DbgPrint("Driver Unload Called\n");
    pDriverObject->DriverUnload = NULL;
}

NTSTATUS WINAPI ZwQuerySystemInformation(
    _In_      SYSTEM_INFORMATION_CLASS SystemInformationClass,
    _Inout_   PVOID                    SystemInformation,
    _In_      ULONG                    SystemInformationLength,
    _Out_opt_ PULONG                   ReturnLength
);

NTSTATUS GetThreadContext(HANDLE thread_id, PKTRAP_FRAME* trap_frame) {  
    NTSTATUS status;  
    PETHREAD ethread;  
  
    status = PsLookupThreadByThreadId(thread_id, &ethread);  
    if (!NT_SUCCESS(status)) {  
        KdPrint(("PsLookupThreadByThreadId failed with status 0x%X\n", status));  
        return status;  
    }  

    PVOID kernel_stack = ((PKTHREAD)ethread)->KernelStack;
   
    *trap_frame = (PKTRAP_FRAME)((PUCHAR)kernel_stack - sizeof(KTRAP_FRAME));  
   
    ObDereferenceObject(ethread);  
  
    return STATUS_SUCCESS;  
}  

NTSTATUS GetSystemProcessInformation(OUT PSYSTEM_PROCESS_INFORMATION* process_info, OUT PULONG process_info_size) {  
    NTSTATUS status;  
    ULONG buffer_size = 0;  
    PVOID buffer = NULL;  
  
    status = ZwQuerySystemInformation(SystemProcessInformation, buffer, buffer_size, &buffer_size);  
    if (status != STATUS_INFO_LENGTH_MISMATCH) {  
        return status;  
    }  
   
    buffer = ExAllocatePoolWithTag(NonPagedPool, buffer_size, 'PINF');  
    if (!buffer) {  
        return STATUS_INSUFFICIENT_RESOURCES;  
    }  
   
    status = ZwQuerySystemInformation(SystemProcessInformation, buffer, buffer_size, &buffer_size);  
    if (!NT_SUCCESS(status)) {  
        ExFreePoolWithTag(buffer, 'PINF');  
        return status;  
    }  
  
    *process_info = (PSYSTEM_PROCESS_INFORMATION)buffer;  
    *process_info_size = buffer_size;  
    return STATUS_SUCCESS;  
} 
  
NTSTATUS EnumerateThreadIds(VOID) {  
    NTSTATUS status;  
    PSYSTEM_PROCESS_INFORMATION process_info;  
    ULONG process_info_size;  
  
    status = GetSystemProcessInformation(&process_info, &process_info_size);  
    if (!NT_SUCCESS(status)) {  
        KdPrint(("GetSystemProcessInformation failed with status 0x%X\n", status));  
        return status;  
    }  
  
    PSYSTEM_PROCESS_INFORMATION current_process_info = process_info;
    while (TRUE) {
        DbgPrint("Process Name: %wZ\n", &current_process_info->ImageName);
        PSYSTEM_THREAD_INFORMATION pThreadInfo = (PSYSTEM_THREAD_INFORMATION)(current_process_info + 1);
        PKTRAP_FRAME current_trap_frame;
        for (ULONG i = 0; i < current_process_info->NumberOfThreads; i++) {  
            HANDLE thread_id = pThreadInfo->ClientId.UniqueThread;
            DbgPrint("Thread ID: %d\n", thread_id);
            status = GetThreadContext(thread_id, &current_trap_frame);
            if (!NT_SUCCESS(status)) {  
                KdPrint(("GetThreadContext failed with status 0x%X\n", status));  
                //return status;
                continue;
            }
            DbgPrint("Trap Frame: %p\n", current_trap_frame);
            //Print RIP, CS
            //DbgPrint("RIP: %p, CS: %p\n", current_trap_frame->Rip, current_trap_frame->SegCs);
            pThreadInfo++;
        }  
        if (current_process_info->NextEntryOffset == 0) {  
            break;  
        }  
        current_process_info = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)current_process_info + current_process_info->NextEntryOffset);  
    }  
    ExFreePoolWithTag(process_info, 'PINF');  
    return STATUS_SUCCESS;  
}  

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {  
    NTSTATUS status;  
  
    status = EnumerateThreadIds();  
    if (!NT_SUCCESS(status)) {  
        KdPrint(("EnumerateThreadIds failed with status 0x%X\n", status));  
    }  
  
    driver_object->DriverUnload = DriverUnload;
    DbgPrint("Driver Loaded at RegistryPath: %wZ\n", registry_path);

    return STATUS_SUCCESS;  
}  

