#include "mydef.h"

UINT64 original_GP_addr = 0;

UINT64 GetIDT_Item_Addr(UINT64 int_id);
void HackIDT_FireAndForget2(UINT64 int_id, void *func);

void myINTHandler_0dh();
void myINTHandler_78h();
void myINTHandler_79h();

void HackIDT()
{
    // trigger breakpoint
    DbgBreakPoint();
    original_GP_addr = GetIDT_Item_Addr(0x0d);
    HackIDT_FireAndForget2(0x0d, myINTHandler_0dh);
    HackIDT_FireAndForget2(0x78, myINTHandler_78h);
    HackIDT_FireAndForget2(0x79, myINTHandler_79h);
}

void myGPHandler_cfunc(UINT8 *arg)
{
    DbgBreakPoint();
    DbgPrint("We are in myGPHandler_cfunc, arg = %p\n", arg);
}