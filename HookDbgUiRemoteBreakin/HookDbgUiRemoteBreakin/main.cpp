#include <Windows.h>
#include <stdio.h>

//我们希望跳转到的函数
void MyHooker()
{
	//printf一句
	printf("想附加老子，没门！\n");
	ExitProcess(0);
}

int main()
{
	//Hook代码
	//_WIN64和_WIN32宏判断64位还是32位
	//64位时_WIN64宏为1，反之为0

	//判断DbgUiRemoteBreakin的位置
	//由于DbgUiRemoteBreakin是一个内部函数，所以我们要手动加载它
	HMODULE hNtDll = LoadLibraryW(L"ntdll.dll");
	if (!hNtDll)
	{
		printf("加载Ntdll.dll失败！\n");
		return 0;
	}
	LPVOID pFunc = GetProcAddress(hNtDll, "DbgUiRemoteBreakin");
	if (!pFunc)
	{
		printf("寻找DbgUiRemoteBreakin失败！\n");
		FreeLibrary(hNtDll);
		return 0;
	}

	//写入Hook代码，注意一般WriteProcessMemory会无视内存权限
	SIZE_T writtenBytes = 0;
#if _WIN64
	unsigned char HookCode[] = { 0x48 ,0xb8,0,0,0,0,0,0,0,0,0xff,0xe0 };
	*((LPVOID*)(HookCode + 2)) = MyHooker;
	WriteProcessMemory(GetCurrentProcess(), pFunc, HookCode, sizeof HookCode, &writtenBytes);
#else
	unsigned char HookCode[] = { 0xe9,0,0,0,0 };
	*((PUCHAR*)(HookCode + 1)) = (PUCHAR)(((PUCHAR)MyHooker - (PUCHAR)pFunc) - 5);
	WriteProcessMemory(GetCurrentProcess(), pFunc, HookCode, sizeof HookCode, &writtenBytes);
#endif
	//检查是否写入成功
	if (writtenBytes == sizeof HookCode)
	{
		printf("Hook成功！\n");
	}
	else
	{
		printf("Hook失败！\n");
	}

	FreeLibrary(hNtDll);

	//延迟加大一点，手慢
	Sleep(40000);
	return 0;
}