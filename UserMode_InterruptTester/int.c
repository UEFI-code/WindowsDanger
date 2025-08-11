#include<stdio.h>
#include<stdint.h>
#include<Windows.h>

void trigger_int_78h();
void trigger_int_79h();

int main()
{
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