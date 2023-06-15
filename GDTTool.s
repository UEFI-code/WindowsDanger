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
    lgdt fword ptr [rax]
    ret
GetGDT ENDP

SetGDT PROC  
    lgdt fword ptr [rax]
    ret
SetGDT ENDP

END
