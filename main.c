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

    PVOID pStackBase = ((PKTHREAD)ethread)->StackBase;
    KdPrint(("User Stack Base: %p\n", pStackBase));
    PVOID pStackLimit = ((PKTHREAD)ethread)->StackLimit;
    KdPrint(("User Stack Limit: %p\n", pStackLimit));
    SIZE_T StackSize = (SIZE_T)pStackBase - (SIZE_T)pStackLimit;
    KdPrint(("User Stack Size: %p\n", StackSize));
    UINT8 *pKernelStack = ((PKTHREAD)ethread)->KernelStack;
    KdPrint(("Kernel Stack: %p\n", pKernelStack));
    //Allocate memory buf for kernel stack
    // PVOID pBuf = ExAllocatePoolWithTag(NonPagedPool, 2 * 1024, 'KSTK');
    // if (!pBuf) {
    //     KdPrint(("ExAllocatePoolWithTag Failed: %x\n", status));
    //     return status;
    // }
    // Raise IRQL to DISPATCH_LEVEL
    KIRQL oldIrql;
    KeRaiseIrql(DISPATCH_LEVEL, &oldIrql);
    // check memory valid before read
    if (MmIsAddressValid(pKernelStack) == FALSE) {
        KdPrint(("MmIsAddressValid Failed: %x\n", status));
        // Restore IRQL
        KeLowerIrql(oldIrql);
        // Free memory
        //ExFreePoolWithTag(pBuf, 'KSTK');
        return status;
    }

    *trap_frame = (PKTRAP_FRAME)(pKernelStack + 384);
    KdPrint(("RAX: %p, RBX: %p, RCX: %p, RDX: %p\n", (*trap_frame)->Rax, (*trap_frame)->Rbx, (*trap_frame)->Rcx, (*trap_frame)->Rdx));
    
    if (thread_id == 7656)
    {
        //Try to hack RdX to 0x6666666666666666
        (*trap_frame)->Rdx = 0x6666666666666666;
        KdPrint(("Try to hack RDX to %p\n", (*trap_frame)->Rdx));
    }
    // Read kernel stack
    //RtlCopyMemory(pBuf, pKernelStack, 2 * 1024);
    // Restore IRQL
    KeLowerIrql(oldIrql);
    // Dump stack to C:\\Thread_%llu_KernelStack.bin
    // HANDLE hFile;
    // OBJECT_ATTRIBUTES objAttr;
    // IO_STATUS_BLOCK ioStatusBlock;
    // UNICODE_STRING uniName;
    // WCHAR wcName[512];
    // swprintf(wcName, L"\\DosDevices\\C:\\Thread_%llu_KernelStack.bin", (UINT64)thread_id);
    // RtlInitUnicodeString(&uniName, wcName);
    // InitializeObjectAttributes(&objAttr, &uniName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    // status = ZwCreateFile(&hFile, GENERIC_WRITE, &objAttr, &ioStatusBlock, NULL, FILE_ATTRIBUTE_NORMAL, 0, FILE_OVERWRITE_IF, FILE_SYNCHRONOUS_IO_NONALERT, NULL, 0);
    // if (!NT_SUCCESS(status)) {
    //     KdPrint(("ZwCreateFile Failed: %x\n", status));
    //     return status;
    // }
    // status = ZwWriteFile(hFile, NULL, NULL, NULL, &ioStatusBlock, pBuf, 2 * 1024, NULL, NULL);
    // if (!NT_SUCCESS(status)) {
    //     KdPrint(("ZwWriteFile Failed: %x\n", status));
    //     return status;
    // }
    // ZwClose(hFile);
    // Free memory
    //ExFreePoolWithTag(pBuf, 'KSTK');
   
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
    // block interrupt using cli
    //DisableInterrupts();
    status = GetSystemProcessInformation(&process_info, &process_info_size);  
    if (!NT_SUCCESS(status)) {  
        KdPrint(("GetSystemProcessInformation failed with status 0x%X\n", status));  
        return status;  
    }
    PSYSTEM_PROCESS_INFORMATION current_process_info = process_info;
    while (TRUE) {
        KdPrint(("Process Name: %wZ\n", &current_process_info->ImageName));
        // check if pid > 4
        if ((UINT64)current_process_info->UniqueProcessId <= 4) {
           goto shiftNext;
        }
        PSYSTEM_THREAD_INFORMATION pThreadInfo = (PSYSTEM_THREAD_INFORMATION)(current_process_info + 1);
        PKTRAP_FRAME current_trap_frame;
        for (ULONG i = 0; i < current_process_info->NumberOfThreads; i++) {  
            HANDLE thread_id = pThreadInfo->ClientId.UniqueThread;
            KdPrint(("Thread ID: %d\n", thread_id));
            status = GetThreadContext(thread_id, &current_trap_frame);
            if (!NT_SUCCESS(status)) {  
                KdPrint(("GetThreadContext failed with status 0x%X\n", status));  
                //return status;
                pThreadInfo++;
                continue;
            }
            KdPrint(("Trap Frame: %p\n", current_trap_frame));
            //Print RIP, CS
            //KdPrint("RIP: %p, CS: %p\n", current_trap_frame->Rip, current_trap_frame->SegCs);
            pThreadInfo++;
        }

        shiftNext:

        if (current_process_info->NextEntryOffset == 0) {  
            break;  
        }  
        current_process_info = (PSYSTEM_PROCESS_INFORMATION)((PUCHAR)current_process_info + current_process_info->NextEntryOffset);  
    }  
    ExFreePoolWithTag(process_info, 'PINF');
    //EnableInterrupts();
    return STATUS_SUCCESS;  
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

