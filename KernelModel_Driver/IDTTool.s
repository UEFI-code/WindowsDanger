extern MmGetPhysicalAddress: proc
extern MmMapIoSpace: proc
extern DbgPrint: proc

.data

    str_78h:
        db "Code in INT 78h Handler, will ret back normally\n", 0
    str_79h:
        db "Code in INT 79h Handler, will hack CS and SS to Ring0\n", 0

.code

GetIDT PROC  
    sidt fword ptr [rax]
    ret
GetIDT ENDP

SetIDT PROC  
    lidt fword ptr [rax]
    ret
SetIDT ENDP

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
    mov byte ptr [rcx + 2], 010h ; Select Ring0 Kernel Segment!
    mov byte ptr [rcx + 5], 0EEh ; Allow user mode access
    mov [rcx + 6], edx ; Store higher 32 bits of the pointer to the function
    shr rdx, 32
    mov [rcx + 10], dx ; Store higher 16 bits of the pointer to the function
    ; OK, we finished the hack!
    ret
HackIDT_FireAndForget2 ENDP

myINTHandler_78h PROC
    ; Stack Layout:
    ; RSP + 0 : return address
    ; RSP + 8 : orignal CS
    ; RSP + 16 : orignal RFLAGS
    ; RSP + 24 : orignal RSP
    ; RSP + 32 : orignal SS

    swapgs; swap to kernel gs

    ; Gap 16 bytes for you to play
    dq 9090909090909090h ; NOP

    ; try to call DbgPrint
    ; before that we need to prepare the stack
    push rbp
    mov rbp, rsp
    sub rsp, 0ffh

    ; let's do it
    lea rcx, str_78h
    call DbgPrint

    ; restore the stack
    add rsp, 0ffh
    pop rbp

    ; Gap 16 bytes for you to play
    dq 9090909090909090h ; NOP

    swapgs; swap back to user gs
    iretq
myINTHandler_78h ENDP

myINTHandler_79h PROC
    ; Stack Layout:
    ; RSP + 0 : return address
    ; RSP + 8 : orignal CS
    ; RSP + 16 : orignal RFLAGS
    ; RSP + 24 : orignal RSP
    ; RSP + 32 : orignal SS

    swapgs; swap to kernel gs

    ; Gap 16 bytes for you to play
    dq 9090909090909090h ; NOP

    ; try to call DbgPrint
    ; before that we need to prepare the stack
    push rbp
    mov rbp, rsp
    sub rsp, 0ffh

    ; let's do it
    lea rcx, str_79h
    call DbgPrint

    ; restore the stack
    add rsp, 0ffh
    pop rbp

    ; let's Hack CS and SS !
    mov [rsp + 8], cs ; Just use the kernel CS !
    ;mov [rsp + 16], rax ; Just use the kernel RFLAGS, but this opcode doesnt exist...
    add rsp, 24 ; trick pushfq...
    pushfq ; Overwrite the RFLAGS
    sub rsp, 16 ; restore stack
    mov [rsp + 32], ss ; Just use the kernel SS !
    int 3 ; Debug Break
    ; Gap 16 bytes for you to play
    dq 9090909090909090h ; NOP

    ;swapgs; swap back to user gs, but...windbg need a "right" gs to work...
    iretq
myINTHandler_79h ENDP

END