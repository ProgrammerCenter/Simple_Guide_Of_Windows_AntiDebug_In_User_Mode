#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	if (IsDebuggerPresent())
	{
		printf("正在被调试！\n");
	}
	else
	{
		printf("没有被调试！\n");
	}
	system("pause");
	return 0;
}