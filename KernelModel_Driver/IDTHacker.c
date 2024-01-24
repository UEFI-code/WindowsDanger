#include<ntddk.h>
#include "mydef.h"


void HackIDT_FireAndForget(UINT8 int_id, void *func);

void myINTHandler()
{
    DbgPrint("Success Enter INT Handler!\n");
    // iretq
}

void RegisterMyINTHandler()
{
    HackIDT_FireAndForget(0x80, myINTHandler);
}

