#include <process.h>
#include <Windows.h>
#include <stdio.h>

//����ָ�������
typedef BOOL(__stdcall* PNtSetInformationThread)(HANDLE, DWORD, LPVOID, ULONG);

unsigned __stdcall my_thread(void*)
{
	//����NtSetInformationThread��һ���ڲ���������������Ҫ�ֶ�������
	HMODULE hNtDll = LoadLibraryW(L"ntdll.dll");
	if (!hNtDll)
	{
		printf("����Ntdll.dllʧ�ܣ�\n");
		return 0;
	}
	PNtSetInformationThread pFunc = (PNtSetInformationThread)GetProcAddress(hNtDll, "NtSetInformationThread");
	if (!pFunc)
	{
		printf("Ѱ��NtSetInformationThreadʧ�ܣ�\n");
		FreeLibrary(hNtDll);
		return 0;
	}
	//����NtSetInformationThread
	if (!pFunc(GetCurrentThread(), 0x11, 0, 0))
	{
		printf("�����߳����ص��Գɹ���\n");
		return 0;
	}
	else
	{
		printf("�����߳����ص���ʧ�ܣ�\n");
		return 0;
	}
	FreeLibrary(hNtDll);
	while (true)
	{
		printf("���ֶ����¶ϵ㣡\n");
	}
}

int main()
{
	//�������߳�
	_beginthreadex(NULL, 0, my_thread, NULL, 0, NULL);
	Sleep(10000);
	return 0;
}