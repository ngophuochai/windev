// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include <Windows.h>
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windowsx.h>
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <vector>
using namespace std;
#include <fstream>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")
#include "RibbonFramework.h"
#include "RibbonIDs.h"

#define LINE 0
#define RECTANGLE 1
#define SQUARE 2
#define ELLIPSE 3
#define CIRCLE 4

#define BLACK Color(0, 0, 0)
#define RED Color(255, 0, 0)
#define BLUE Color(0, 0, 255)
#define GREEN Color(0, 255, 0)
#define YELLOW Color(255, 255, 0)
#define MAGENTA Color(255, 0, 255)
#define CYAN Color(0, 255, 255)


// TODO: reference additional headers your program requires here
