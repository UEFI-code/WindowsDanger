#include<ntddk.h>
#include "mydef.h"

void HackIDT_FireAndForget2(UINT64 int_id, void *func);
void TestINT();

void myINTHandler();

void RegisterMyINTHandler()
{
    // trigger breakpoint
    DbgBreakPoint();
    HackIDT_FireAndForget2(0x78, myINTHandler);
    TestINT();
}