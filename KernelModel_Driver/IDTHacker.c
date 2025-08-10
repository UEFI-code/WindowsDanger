#include "mydef.h"

void HackIDT_FireAndForget2(UINT64 int_id, void *func);

void myINTHandler_78h();
void myINTHandler_79h();

void HackIDT()
{
    // trigger breakpoint
    // DbgBreakPoint();
    HackIDT_FireAndForget2(0x78, myINTHandler_78h);
    HackIDT_FireAndForget2(0x79, myINTHandler_79h);
}