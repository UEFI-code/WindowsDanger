#include<ntddk.h>
#include "mydef.h"


void HackIDT_FireAndForget(UINT64 int_id, void* func);
void HackIDT_FireAndForget2(UINT64 int_id, void *func);
void TestINT();

void myINTHandler();

void RegisterMyINTHandler()
{
    // trigger breakpoint
    //DbgBreakPoint();
    //UINT8 *showMem = ExAllocatePool2(POOL_FLAG_NON_PAGED, 0x1000, '2333');
    HackIDT_FireAndForget(0x78, myINTHandler);
    TestINT();
}