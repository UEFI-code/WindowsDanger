// UserMode_Caller.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <Windows.h>

int main()
{
    std::cout << "Hello World!\n";
	int ctl_code = 0;
	DWORD ret_code;
	HANDLE device = CreateFileA("\\\\.\\WinDangerLink", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, 0);
	if (device == INVALID_HANDLE_VALUE) {
		std::cout << "CreateFile failed" << std::endl;
		goto end;
	}
	while (1)
	{
		printf("Type CTL code: ");
		scanf_s("%d", &ctl_code);
		DeviceIoControl(device, ctl_code, NULL, 0, NULL, 0, &ret_code, 0);
	}
	CloseHandle(device);
	std::cout << "ret_code: " << ret_code << std::endl;
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
