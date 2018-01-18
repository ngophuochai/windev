#pragma once
#include "stdafx.h"

class CApp
{
private:
	WCHAR name[MAX_NAME];
	WCHAR path[MAX_PATH];
	int seq;
public:
	CApp();
	CApp(WCHAR* name, WCHAR* path, int seq);

	void SetCApp(WCHAR* name, WCHAR* path, int seq);
	WCHAR* GetName();
	WCHAR* GetPath();
	int GetSeq();
	void SetSeq(int seq);
	void Run();
};