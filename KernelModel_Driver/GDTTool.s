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
    sgdt fword ptr [rcx]
    ret
GetGDT ENDP

SetGDT PROC  
    lgdt fword ptr [rcx]
    ret
SetGDT ENDP

END
