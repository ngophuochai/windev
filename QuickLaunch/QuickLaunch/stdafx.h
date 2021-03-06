// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <Windows.h>
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#define MAX_NAME				100
#define MAX_PATH				260
#define PROGRAM_FILES_PATH		L"C:\\Program Files"
#define PROGRAM_FILESx86_PATH	L"C:\\Program Files (x86)"

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <Shellapi.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <vector>
#include <fstream>
using namespace std;
#include "App.h"
#include <Shlobj.h>


// TODO: reference additional headers your program requires here
