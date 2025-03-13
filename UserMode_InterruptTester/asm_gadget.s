.code

trigger_int_78h PROC
    ; Trigger an interrupt
    int 078h
    ret
trigger_int_78h ENDP

trigger_int_79h PROC
    ; Trigger an interrupt
    int 079h
    ret
trigger_int_79h ENDP

END