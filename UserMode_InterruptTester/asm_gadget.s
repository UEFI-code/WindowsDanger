.code

trigger_int_78h PROC
    ; Trigger an interrupt
    int 078h
    ret
trigger_int_78h ENDP

trigger_int_79h PROC
    ; Trigger an interrupt
    int 079h
    ; Now We are Ring0!
    swapgs; WinDbg need a "right" gs to work...
    int 3; Inform windbg
    nop_start:
    dq 9090909090909090h
    jmp nop_start
    ret
trigger_int_79h ENDP

END