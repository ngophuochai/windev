// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
HWND hWnd;
HHOOK hMyHook;
HINSTANCE hInstance;
BOOL val = FALSE;

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved
					 )
{
	hInstance = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam);

INT InitMouseHook(HWND hwndYourWindow)
{
	hWnd = hwndYourWindow;

	hMyHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, hInstance, 0);
	hMyHook = SetWindowsHookEx(WH_KEYBOARD, KeyProc, hInstance, 0);

	if (hMyHook != NULL)
	{
		return 1;
	}
	return 0;
}

LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
		{
			if (val)
				return -1;
		}
		break;
		default:
			break;
		}
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

LRESULT CALLBACK KeyProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode == HC_ACTION) {
		if (GetKeyState(VK_F7) & 0x8000)
			val = (val == FALSE) ? TRUE : FALSE;
	}
	return CallNextHookEx(0, nCode, wParam, lParam);
}

INT UninstallMouseHook(HWND hWnd)
{
	if (hMyHook != NULL)
	{
		UnhookWindowsHookEx(hMyHook);
		return 1;
	}
	return 0;
}