.code  

PUBLIC GetRSI
PUBLIC GetRDI
PUBLIC GetCS
PUBLIC GetDS
PUBLIC GetCR0
PUBLIC GetCR3

PUBLIC InitRSIRDI

InitRSIRDI PROC
    mov rsi, 2323232323232323h
    mov rdi, 6666666666666666h
    ret
InitRSIRDI ENDP

GetRSI PROC
    mov rax, rsi
    ret
GetRSI ENDP

GetRDI PROC
    mov rax, rdi
    ret
GetRDI ENDP

GetCS PROC
    mov rax, cs
    ret
GetCS ENDP

GetDS PROC
    mov rax, ds
    ret
GetDS ENDP

GetCR0 PROC
    mov rax, cr0
    ret
GetCR0 ENDP

GetCR3 PROC
    mov rax, cr3
    ret
GetCR3 ENDP

END