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
    exe[0] = 0xCD; // INT 0xXX
    exe[2] = 0xC3; // RET

    UINT8 INT_num = 0;

    while(1)
    {
        printf("Enter Interrupt Number: ");
        scanf_s("%x", &INT_num);
        printf("Triggering Interrupt 0x%X\n", INT_num);
        exe[1] = INT_num;
        ((void(*)())exe)();
    }
}