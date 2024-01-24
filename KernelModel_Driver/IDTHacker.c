#include<ntddk.h>
#include "mydef.h"


void HackIDT_FireAndForget(UINT8 int_id, void* func);
void HackIDT_FireAndForget2(UINT8 int_id, void *func);

void myINTHandler()
{
    DbgBreakPoint();
    DbgPrint("Success Enter INT Handler!\n");
    // iretq
}

void RegisterMyINTHandler()
{
    // trigger breakpoint
    //DbgBreakPoint();
    //UINT8 *showMem = ExAllocatePool2(POOL_FLAG_NON_PAGED, 0x1000, '2333');
    HackIDT_FireAndForget2(0x00, myINTHandler);
}

