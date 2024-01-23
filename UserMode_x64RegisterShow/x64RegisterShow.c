// x64RegisterShow.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <stdio.h>
#include <stdint.h>
#include <Windows.h>

uint64_t GetRSI();
uint64_t GetRDI();
uint64_t GetCS();
uint64_t GetDS();
uint64_t GetCR0();
uint64_t GetCR3();

void InitRSIRDI();

int main()
{
    InitRSIRDI();
    while (1)
    {
        printf_s("RSI = %016llx, RDI = %016llx, CS = %016llx, DS = %016llx\n", GetRSI(), GetRDI(), GetCS(), GetDS());
        // check CS and DS to see if we are in Ring0
        // The privilege level of the currently running code is indicated in bits 1 and 2 of the CS register. and DS is the same as CS.
        // 00 = Ring 0, 01 = Ring 1, 10 = Ring 2, 11 = Ring 3
        switch(GetCS() & 0x3)
        {
        case 0:
            printf_s("We are Win! Ring0!\n");
            printf_s("CR0 = %016llx, CR3 = %016llx\n", GetCR0(), GetCR3());
            break;
        case 1:
            printf_s("We are In Ring1!?!\n");
            break;
        case 2:
            printf_s("We are In Ring2!?!\n");
            break;
        case 3:
            printf_s("Boring, we are in Ring3.\n");
            break;
        }
        Sleep(1000);
    }
}