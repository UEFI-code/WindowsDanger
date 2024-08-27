#include<stdio.h>
#include<Windows.h>

int main()
{
    UINT8 *exe = (UINT8 *)VirtualAlloc(NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (exe == NULL)
    {
        printf("VirtualAlloc Failed!\n");
        return -1;
    }
    memset(exe, 0x90, 0x1000); // NOP
    exe[0] = 0xCD; // INT 0xXX
    exe[2] = 0xC3; // RET

    UINT8 *exe_for_ring0 = (UINT8 *)VirtualAlloc(NULL, 0x1000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (exe_for_ring0 == NULL)
    {
        printf("VirtualAlloc Failed!\n");
        return -1;
    }
    memset(exe_for_ring0, 0x90, 0x1000); // NOP
    exe_for_ring0[4] = 0xF4; // HLT
    exe_for_ring0[8] = 0xEB; // JMP SHORT 
    exe_for_ring0[9] = 0xFA; // -5 from here
    printf("Ring0 Playground: %p\n", exe_for_ring0);

    UINT8 INT_num = 0;

    while(1)
    {
        printf("Enter Interrupt Number: ");
        scanf_s("%x", &INT_num);
        printf("Triggering Interrupt 0x%X\n", INT_num);
        exe[1] = INT_num;
        ((void(*)())exe)();
        if (INT_num == 0x78)
        {
            // Goto the loop to prevent calling NT API
            ((void(*)())exe_for_ring0)();
        }
    }
}