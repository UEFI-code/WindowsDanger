// UserMode_Caller.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//
#include<stdio.h>
#include<stdint.h>
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
	int ctl_code = 0;
	DWORD ret_code;
	HANDLE device = CreateFileA("\\\\.\\WinDangerLink", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (device == INVALID_HANDLE_VALUE) {
		printf("CreateFile failed!\n");
		goto end;
	}
	InitRSIRDI();
	while (1)
	{
		printf("Type CTL code: ");
		scanf_s("%d", &ctl_code);
		DeviceIoControl(device, ctl_code, NULL, 0, NULL, 0, &ret_code, 0);
		printf_s("RSI = %016llx, RDI = %016llx, CS = %016llx, DS = %016llx\n", GetRSI(), GetRDI(), GetCS(), GetDS());
	}
	CloseHandle(device);
end:
	system("pause");
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
