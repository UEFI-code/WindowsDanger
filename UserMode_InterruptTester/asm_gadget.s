extern Less_Paged_Mem: qword

.code

trigger_int_78h PROC
    ; Trigger an interrupt
    int 078h
    ; try in/out
    in al, 66h
    out 67, al;
    ret
trigger_int_78h ENDP

trigger_int_79h PROC
    ; While we play in Ring0, we need less-paged-rsp to reduce #DF
    mov rax, rsp
    mov rsp, Less_Paged_Mem
    add rsp, 4096
    push rax
    ; Trigger an interrupt
    int 079h
    ; Now We are Ring0, with kernel gs!
    cli;
    ; dq 0f4f4f4f4f4f4f4f4h ; HLT
    dq 9090909090909090h ; NOP
    ; Well, we already played enough, so let's do something pretty...
    ; You are right, we need back to Ring3...
    push 02bh ; SS
    push [rsp+8] ; orignal RSP
    pushfq ; RFLAGS
    push 033h ; CS
    mov rax, return_addr
    push rax ; Return address
    swapgs; swap gs back to user
    iretq
    return_addr:
    ; OK, we backed to Ring3
    int 3; Inform windbg
    ret
trigger_int_79h ENDP

END