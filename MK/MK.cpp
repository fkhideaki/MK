
#include "pch.h"
#include <stdio.h>
#include <windows.h>
#include <locale.h>
#include <iostream>
#include <time.h>
#include <direct.h>
#include <stdlib.h>
using namespace std;

enum class ExistsState
{
	Directory,
	File,
	Nothing,
};

ExistsState getExistsState(const wchar_t* objPath)
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

bool makeFileIfNotExists(const wchar_t* fn)
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

bool makeDirIfNotExists(const wchar_t* fn)
{
	ExistsState exs = getExistsState(fn);
	if (exs != ExistsState::Nothing) {
		wcout << fn << L" is exists" << endl;
		return false;
	}

	return _wmkdir(fn) == 0;
}

std::wstring getToday()
{
	wchar_t s[100];

	time_t t = time(NULL);
	struct tm* tp = localtime(&t);
	size_t rc = wcsftime(s, sizeof(s) / 2, L"%Y%m%d", tp);
	return s;
}

bool makeFileMain(wchar_t* fn)
{
	std::wstring s = fn;
	if (s.length() >= 3)
	{
		if (s.substr(0, 3) == L"/tt")
		{
			std::wstring today = getToday();
			today += L".txt";
			return makeFileIfNotExists(today.c_str());
		}
		else if (s.substr(0, 3) == L"/td")
		{
			std::wstring today = getToday();
			return makeDirIfNotExists(today.c_str());
		}
		else if (s.substr(0, 2) == L"/t")
		{
			std::wstring today = getToday();
			today += s.substr(2);
			return makeFileIfNotExists(today.c_str());
		}
	}
	return makeFileIfNotExists(fn);
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
