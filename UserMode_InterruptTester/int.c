#include<stdio.h>
#include<stdint.h>
#include<Windows.h>

void trigger_int_78h();
void trigger_int_79h();

uint8_t *Less_Paged_Mem = NULL;

int main()
{
    Less_Paged_Mem = (uint8_t *)VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if(!VirtualLock(Less_Paged_Mem, 4096)) {
        printf("VirtualLock failed!\n");
        return 1;
    }
    memset(Less_Paged_Mem, 0, 4096);
    printf("Allocated Less-Paged Memory at %p\n", Less_Paged_Mem);

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
                break;
            default:
                printf("Not Implemented!\n");
                break;
        }
    }
}