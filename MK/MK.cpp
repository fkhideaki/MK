#include "pch.h"
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <iostream>
using namespace std;

enum class ExistsState
{
	Directory,
	File,
	Nothing,
};

ExistsState getExistsState(wchar_t* objPath)
{
	WIN32_FIND_DATA data;
	HANDLE hFile = FindFirstFile(objPath, &data);

	if (hFile == INVALID_HANDLE_VALUE)
		return ExistsState::Nothing;

	FindClose(hFile);
	if ((data.dwFileAttributes & 0x10) != 0)
		return ExistsState::Directory;

	return ExistsState::File;
}

bool makeFileMain(wchar_t* fn)
{
	ExistsState exs = getExistsState(fn);
	if (exs != ExistsState::Nothing) {
		wcout << fn << L" is exists" << endl;
		return false;
	}

	FILE* fp;
	_wfopen_s(&fp, fn, L"w+");
	if (fp == NULL)
	{
		printf("file open error\n");
		return false;
	}

	fclose(fp);

	return true;
}

int wmain(int argc, wchar_t* argv[])
{
	setlocale(LC_CTYPE, "");

	for (int i = 1; i < argc; ++i)
	{
		makeFileMain(argv[i]);
	}

	return 0;
}
