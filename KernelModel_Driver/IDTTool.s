; how to declare function of the ExAllocatePool function? 
extern ExAllocatePool2: proc
extern MmGetPhysicalAddress: proc
extern MmMapIoSpace: proc
extern DbgPrint: proc

.data

    strA:
        db "Code in INT Handler\n", 0

.code

GetIDT PROC  
    sidt fword ptr [rax]
    ret
GetIDT ENDP

SetIDT PROC  
    lidt fword ptr [rax]
    ret
SetIDT ENDP

HackIDT_FireAndForget PROC ; Retired, leave for reference
    int 03h ; Debug Break
    ; incoming parameters: INT number, pointer to the function
    push rcx ; Backup INT number
    push rdx ; Backup pointer to the function
    ; We need allocate memory size of 0x1000 bytes
    ; We need to allocate memory in non-paged pool
    mov rcx, 040h ; POOL_FLAG_NON_PAGED_EXECUTE
    mov rdx, 256 * 16 + 10 ; Another 8 bytes for LIDT
    mov r8, 0233h ; Tag is 233
    call ExAllocatePool2
    ; Now rax is pointer to allocated memory.
    sidt fword ptr [rax] ; Now the IDT memory pointer stored in rax pointed memory. Get that pointer to rbx!
    push rbx ; Backup rbx
    mov rbx, [rax + 2] ; Get IDT pointer, first 2 bytes is limit, next 8 bytes is base
    ; Then we copy the IDT to our allocated memory
    mov rcx, 0 ; let rcx as counter
    copy_process:
        mov rdx, [rbx + rcx * 8]
        mov [rax + rcx * 8], rdx
        inc rcx
        cmp rcx, 512 ; here the rdx ready 8 bytes one time, but an IDT entry is 16 bytes. So we need to copy 512 times
        jne copy_process
    ; Now we have a copy of the IDT in our allocated memory
    pop rbx ; Restore rbx, and rbx useless now
    ; Start Hack Now!
    pop rdx
    pop rcx
    lea rcx, [rcx * 4]
    lea rcx, [rcx * 4] ; Here multiple 16 is not allowed by x64.
    lea rcx, [rax + rcx] ; Get the INT indexed pointer. The rcx is useless now.
    mov [rcx], dx ; Store lower 16 bits of the pointer to the function
    ; now shift rax to right 16 bits
    shr rdx, 16
    mov byte ptr [rcx + 5], 0EEh ; Allow user mode access
    mov [rcx + 6], edx ; Store higher 32 bits of the pointer to the function
    shr rdx, 32
    mov [rcx + 10], dx ; Store higher 16 bits of the pointer to the function
    ; OK, we finished the hack!
    ; Now Load the New IDT
    mov word ptr [rax + 4096], 0FFFh ; Set the limit to 0x0FFF
    mov [rax + 4098], rax ; Store the IDT Pointer
    add rax, 4096
    lidt fword ptr [rax]
    ; Now we finished the hack! Windup to return
    ret
HackIDT_FireAndForget ENDP

HackIDT_FireAndForget2 PROC
    ; int 03h ; Debug Break
    ; incoming parameters: INT number, pointer to the function
    push rcx ; Backup INT number
    push rdx ; Backup pointer to the function

    ; now, save IDTR into stack, 10 bytes
    sub rsp, 10
    sidt fword ptr [rsp]
    ; seams Windows set the limit to 0x0FFF, so we just ignore it
    add rsp, 2
    pop rcx; Get the IDT pointer, then transfer to MmGetPhysicalAddress below ...

    ; before we play next, let's prepare the safe stack (for stupid NT API)
    push rbp
    sub rsp, 0ffh; Fly away ...
    lea rbp, [rsp + 0ffh]; ... to the safe place
    
    ; Bypass the memory protection. Hack CR0 also work!
    call MmGetPhysicalAddress
    mov rcx, rax
    mov rdx, 256 * 16
    xor r8, r8
    call MmMapIoSpace

    ; Now we may bypassed the memory protection
    ; NT API is useless now, we can restore the stack height
    add rsp, 0ffh; Dive back ...
    pop rbp;
    pop rdx
    pop rcx
    lea rcx, [rcx * 4]
    lea rcx, [rcx * 4] ; Here multiple 16 is not allowed by x64.
    lea rcx, [rax + rcx] ; Get the INT indexed pointer. The rcx is useless now.
    mov [rcx], dx ; Store lower 16 bits of the pointer to the function
    ; now shift rax to right 16 bits
    shr rdx, 16
    mov byte ptr [rcx + 5], 0EEh ; Allow user mode access
    mov [rcx + 6], edx ; Store higher 32 bits of the pointer to the function
    shr rdx, 32
    mov [rcx + 10], dx ; Store higher 16 bits of the pointer to the function
    ; OK, we finished the hack!
    ret
HackIDT_FireAndForget2 ENDP

TestINT PROC
	int 078h
    ret
TestINT ENDP

myINTHandler PROC
    cli ; Prevent Another Interrupt
    ; int 03h
    ; Before we play next, let's prepare the safe stack (for stupid NT API)
    push rbp
    sub rsp, 0ffh; Fly away ...
    lea rbp, [rsp + 0ffh]; ... to the safe place
    ; Now we can play
    mov rcx, offset strA
    call DbgPrint
    ; NT API is useless now, we can restore the stack height
    add rsp, 0ffh; Dive back ...
    pop rbp;
    sti ; Enable Interrupt
    iretq
myINTHandler ENDP

END