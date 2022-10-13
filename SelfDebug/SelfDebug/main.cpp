#include <process.h>
#include <Windows.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <stdlib.h>

unsigned __stdcall self_debug_thread(void* processId)
{
	if (DebugActiveProcess((DWORD64)processId))
	{
		printf("���ӳɹ���");
	}
	//ע��һ�㣬�������̣߳���ΪWindows���򸽼ӽ��̵��̷߳���Ϣ��ֻ������߳̽��յõ���
	//������Ϣ������Ῠס�����Ե������̣߳����ӣ�����ע�⴦�������Ϣ
	while (true)
	{
		DEBUG_EVENT e;
		if (WaitForDebugEvent(&e, 500))
		{
			ContinueDebugEvent(e.dwProcessId, e.dwThreadId, DBG_CONTINUE);
		}
	}
}

int main(int argc, char** argv)
{
	DWORD processId = 0;
	bool debug = false;
	if (argc > 1)
	{
		debug = true;
		std::stringstream ss;
		ss << argv[1];
		printf("��������PID��%s\n", argv[1]);
		ss >> processId;
	}
	else
	{
		std::stringstream ss;
		std::string cmd;
		ss << GetCurrentProcessId();
		ss >> cmd;
		cmd = " " + cmd;
		cmd = argv[0] + cmd;
		STARTUPINFOA si = {};
		si.cb = sizeof si;
		PROCESS_INFORMATION pi = {};
		BOOL callCode = CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		if (pi.hProcess)
			CloseHandle(pi.hProcess);
		if (pi.hThread)
			CloseHandle(pi.hThread);
		if (callCode)
			printf("�������ɹ�������\n");
	}

	if (!debug)
	{
		Sleep(400000);
	}
	else
	{
		//�������߳�
		_beginthreadex(NULL, 0, self_debug_thread, (LPVOID)processId, 0, NULL);
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		if (hProcess)
		{
			printf("�ɹ��򿪱�������������ȴ��˳���\n");
			WaitForSingleObject(hProcess, INFINITE);
		}
	}
	system("pause");
	return 0;
}