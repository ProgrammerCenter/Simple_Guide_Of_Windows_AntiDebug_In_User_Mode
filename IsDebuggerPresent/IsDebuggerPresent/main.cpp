#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	if (IsDebuggerPresent())
	{
		printf("���ڱ����ԣ�\n");
	}
	else
	{
		printf("û�б����ԣ�\n");
	}
	system("pause");
	return 0;
}