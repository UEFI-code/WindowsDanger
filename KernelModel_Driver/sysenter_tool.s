extern MmGetPhysicalAddress: proc
extern MmMapIoSpace: proc

.code

NOP_Toy PROC  
    dd 90909090h
    dd 90909090h
    dd 90909090h
    dd 90909090h
    dd 90909090h
    ret
NOP_Toy ENDP

Disable_WriteProtect PROC
    mov rax, cr0
    and rax, 0FFFEFFFFh
    mov cr0, rax
    ret
Disable_WriteProtect ENDP

Disable_SMAP_SMEP PROC
    mov rax, cr4
    and rax, 0FFCFFFFFh
    mov cr4, rax
    ret
Disable_SMAP_SMEP ENDP

GetCR3Value PROC
    mov rax, cr3
    ret
GetCR3Value ENDP

BypassWP PROC
    ; rcx = addr, rdx = size
    push rdx; Backup rdx

    ; before we play next, let's prepare the safe stack (for stupid NT API)
    push rbp
    mov rbp, rsp
    sub rsp, 0ffh; Fly away ...

    ; Do real things
    call MmGetPhysicalAddress
    mov rcx, rax
    mov rdx, [rsp + 0ffh + 8] ; get orignal rdx value
    xor r8, r8
    ;int 3; trigger a debug signal
    call MmMapIoSpace

    ; Now we've done
    ; NT API is useless now, we can restore the stack height
    add rsp, 0ffh; Dive back ...
    pop rbp;

    ; clean up stack and go back
    pop rdx;
    ret
BypassWP ENDP

END