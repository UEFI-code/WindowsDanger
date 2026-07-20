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

typedef struct {
    UINT64 R11;
    UINT64 R10;
    UINT64 R9;
    UINT64 R8;
    UINT64 RDI;
    UINT64 RSI;
    UINT64 RDX;
    UINT64 RCX;
    UINT64 RBX;
    UINT64 RAX;
    // x64 #GP
    UINT64 ERR_CODE;
    UINT8* RIP;
    UINT64 CS;
    UINT64 RFLAGS;
    UINT8* RSP;
    UINT64 SS;
} MY_GP_FRAME;

void myGPHandler_cfunc(MY_GP_FRAME *frame)
{
    DbgBreakPoint();
    DbgPrint("We are in myGPHandler_cfunc, arg = %p\n", frame);
    DbgPrint("Usr RIP = %p\n", frame->RIP);
    // SMAP must be disabled, or we will crash
    if (frame->RIP[0] == 0x0f)
    {
        DbgPrint("Detected 0x0f prefix\n");
        frame->RIP += 3;
        DbgPrint("Attempt to skip that instruction\n");
    }
}