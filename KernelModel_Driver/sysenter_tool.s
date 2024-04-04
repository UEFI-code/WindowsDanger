.code

NOP_Toy PROC  
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    ret
NOP_Toy ENDP

Disable_WriteProtect PROC
    mov rax, cr0
    and rax, 0FFFEFFFFh
    mov cr0, rax
    ret
Disable_WriteProtect ENDP

END