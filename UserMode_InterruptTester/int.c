#include<stdio.h>
#include<Windows.h>

void trigger_int_78h();
void trigger_int_79h();

int main()
{
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
        switch (INT_num)
        {
            case 0x78:
                trigger_int_78h();
                break;
            case 0x79:
                trigger_int_79h();
                ((void(*)())exe_for_ring0)();
                break;
            default:
                printf("Not Implemented!\n");
                break;
        }
    }
}