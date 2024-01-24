; how to declare function of the ExAllocatePool function? 
extern ExAllocatePool: proc

.code

GetIDT PROC  
    sidt fword ptr [rax]
    ret
GetIDT ENDP

SetIDT PROC  
    lidt fword ptr [rax]
    ret
SetIDT ENDP

HackIDT_FireAndForget PROC
    ; incoming parameters: INT number, pointer to the function
    push rcx ; Backup INT number
    push rdx ; Backup pointer to the function
    ; We need allocate memory size of 0x1000 bytes
    ; We need to allocate memory in non-paged pool
    mov rcx, 0
    mov rdx, 256 * 16 + 8 ; Another 8 bytes for LIDT
    call ExAllocatePool
    ; Now rax is pointer to allocated memory.
    sidt fword ptr [rax] ; Now the IDT memory pointer stored in rax pointed memory. Get that pointer to rbx!
    push rbx ; Backup rbx
    mov rbx, [rax] ; Get IDT pointer
    ; Then we copy the IDT to our allocated memory
    mov rcx, 0
    cpy_process:
        mov rdx, [rbx + rcx * 8]
        mov [rax + rcx * 8], rdx
        inc rcx
        cmp rcx, 512 ; here the rdx ready 8 bytes one time, but an IDT entry is 16 bytes. So we need to copy 512 times
        jne cpy_process
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
    mov [rax + 4096], rax ; Store the IDT Pointer
    add rax, 4096
    lidt fword ptr [rax]
    ; Now we finished the hack! Windup to return
    ret
HackIDT_FireAndForget ENDP

END