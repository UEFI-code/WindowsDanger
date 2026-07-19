#include "mydef.h"

UINT64 original_GP_addr = 0;

UINT64 GetIDT_Item_Addr(UINT64 int_id);
void HackIDT_FireAndForget2(UINT64 int_id, void *func);

void myINTHandler_13h();
void myINTHandler_78h();
void myINTHandler_79h();

void HackIDT()
{
    // trigger breakpoint
    DbgBreakPoint();
    original_GP_addr = GetIDT_Item_Addr(0x13);
    HackIDT_FireAndForget2(0x13, myINTHandler_13h);
    HackIDT_FireAndForget2(0x78, myINTHandler_78h);
    HackIDT_FireAndForget2(0x79, myINTHandler_79h);
}