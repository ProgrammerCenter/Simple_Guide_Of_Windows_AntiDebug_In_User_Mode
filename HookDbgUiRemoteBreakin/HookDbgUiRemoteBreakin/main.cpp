#include <Windows.h>
#include <stdio.h>

//����ϣ����ת���ĺ���
void MyHooker()
{
	//printfһ��
	printf("�븽�����ӣ�û�ţ�\n");
	ExitProcess(0);
}

int main()
{
	//Hook����
	//_WIN64��_WIN32���ж�64λ����32λ
	//64λʱ_WIN64��Ϊ1����֮Ϊ0

	//�ж�DbgUiRemoteBreakin��λ��
	//����DbgUiRemoteBreakin��һ���ڲ���������������Ҫ�ֶ�������
	HMODULE hNtDll = LoadLibraryW(L"ntdll.dll");
	if (!hNtDll)
	{
		printf("����Ntdll.dllʧ�ܣ�\n");
		return 0;
	}
	LPVOID pFunc = GetProcAddress(hNtDll, "DbgUiRemoteBreakin");
	if (!pFunc)
	{
		printf("Ѱ��DbgUiRemoteBreakinʧ�ܣ�\n");
		FreeLibrary(hNtDll);
		return 0;
	}

	//д��Hook���룬ע��һ��WriteProcessMemory�������ڴ�Ȩ��
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
	//����Ƿ�д��ɹ�
	if (writtenBytes == sizeof HookCode)
	{
		printf("Hook�ɹ���\n");
	}
	else
	{
		printf("Hookʧ�ܣ�\n");
	}

	FreeLibrary(hNtDll);

	//�ӳټӴ�һ�㣬����
	Sleep(40000);
	return 0;
}