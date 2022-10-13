#include <process.h>
#include <Windows.h>
#include <stdio.h>

//函数指针的声明
typedef BOOL(__stdcall* PNtSetInformationThread)(HANDLE, DWORD, LPVOID, ULONG);

unsigned __stdcall my_thread(void*)
{
	//由于NtSetInformationThread是一个内部函数，所以我们要手动加载它
	HMODULE hNtDll = LoadLibraryW(L"ntdll.dll");
	if (!hNtDll)
	{
		printf("加载Ntdll.dll失败！\n");
		return 0;
	}
	PNtSetInformationThread pFunc = (PNtSetInformationThread)GetProcAddress(hNtDll, "NtSetInformationThread");
	if (!pFunc)
	{
		printf("寻找NtSetInformationThread失败！\n");
		FreeLibrary(hNtDll);
		return 0;
	}
	//调用NtSetInformationThread
	if (!pFunc(GetCurrentThread(), 0x11, 0, 0))
	{
		printf("设置线程隐藏调试成功！\n");
		return 0;
	}
	else
	{
		printf("设置线程隐藏调试失败！\n");
		return 0;
	}
	FreeLibrary(hNtDll);
	while (true)
	{
		printf("有种对我下断点！\n");
	}
}

int main()
{
	//创建新线程
	_beginthreadex(NULL, 0, my_thread, NULL, 0, NULL);
	Sleep(10000);
	return 0;
}