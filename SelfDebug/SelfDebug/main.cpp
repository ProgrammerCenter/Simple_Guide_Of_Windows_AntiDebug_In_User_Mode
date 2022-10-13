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
		printf("附加成功！");
	}
	//注意一点，单独起线程，因为Windows会向附加进程的线程发消息（只有这个线程接收得到）
	//而且消息不处理会卡住，所以单独起线程，附加，并且注意处理调试消息
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
		printf("被调试器PID：%s\n", argv[1]);
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
			printf("调试器成功创建！\n");
	}

	if (!debug)
	{
		Sleep(400000);
	}
	else
	{
		//创建新线程
		_beginthreadex(NULL, 0, self_debug_thread, (LPVOID)processId, 0, NULL);
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
		if (hProcess)
		{
			printf("成功打开被调试器句柄，等待退出！\n");
			WaitForSingleObject(hProcess, INFINITE);
		}
	}
	system("pause");
	return 0;
}