.code  

PUBLIC DisableInterrupts  
PUBLIC GetGDT  

DisableInterrupts PROC  
    cli  
    ret  
DisableInterrupts ENDP  

EnableInterrupts PROC
    sti
    ret
EnableInterrupts ENDP

GetGDT PROC  
    sgdt [rax]
    ret
GetGDT ENDP

SetGDT PROC  
    lgdt [rax]
    ret
SetGDT ENDP
