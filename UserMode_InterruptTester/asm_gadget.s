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
    ; Trigger an interrupt
    int 079h
    ; Now We are Ring0, with kernel gs!
    dq 9090909090909090h ; NOP
    ; Well, we already played enough, so let's do something pretty...
    ; You are right, we need back to Ring3...
    mov rax, rsp ; Remember the current stack pointer
    push 02bh ; SS
    push rax ; RSP
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