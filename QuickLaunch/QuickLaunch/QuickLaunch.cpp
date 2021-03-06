// QuickLaunch.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuickLaunch.h"

#define MAX_LOADSTRING		100
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

void RunProgram();
void SortArrApp();
BOOL LoadFile_exe(WCHAR* path, int floor);
BOOL ListLoad(HWND hwnd);
WCHAR LowerCase(WCHAR chr);
BOOL DrawPieChart(HWND hWnd, int x, int y, DWORD radius, float startAngle, float sweepAngle, COLORREF color);
WCHAR* IntToString(int it);
BOOL LoadData();

WCHAR dataPath[MAX_PATH];
HWND g_hWnd, h_TextBox, h_ListBox;
HFONT hFont;
HHOOK hHook;
BOOL isShowing = FALSE;
vector<CApp*> arrApp;
WNDPROC oldEditProc;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUICKLAUNCH, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKLAUNCH));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKLAUNCH));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUICKLAUNCH);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 450, 520, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   /*ShowWindow(hWnd, nCmdShow);*/
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
	{
		g_hWnd = hWnd;
		// Get System Font ----------------
		LOGFONT lf;
		GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
		hFont = CreateFont(lf.lfHeight, lf.lfWidth,
			lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
			lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
			lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
			lf.lfPitchAndFamily, lf.lfFaceName);
		// --------------

		// Add icon to notification area -------------------
		NOTIFYICONDATA nid = {};
		nid.cbSize = sizeof(nid);
		nid.hWnd = hWnd;
		nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nid.uCallbackMessage = WM_APPNOTIFYICON;
		wcscpy_s(nid.szTip, 20, L"Quick Launch");
		nid.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_SMALL));
		Shell_NotifyIcon(NIM_ADD, &nid);
		// ---------------------

		// Install hook ----
		hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyHookProc, hInst, 0);
		// ------

		h_TextBox = CreateWindow(WC_EDIT, L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
			50, 30, 300, 20, hWnd, (HMENU)ID_SEARCHBOX, hInst, NULL);
		SendMessage(h_TextBox, WM_SETFONT, WPARAM(hFont), TRUE);
		oldEditProc = (WNDPROC)SetWindowLongPtr(h_TextBox, GWLP_WNDPROC, (LONG_PTR)subEditProc);

		h_ListBox = CreateWindow(WC_LISTVIEW, L"", WS_CHILD | WS_VISIBLE | WS_BORDER | LVS_NOCOLUMNHEADER | LVS_REPORT,
			50, 60, 300, 400, hWnd, (HMENU)ID_LISTBOX, hInst, NULL);
		SendMessage(h_ListBox, WM_SETFONT, WPARAM(hFont), TRUE);
		ListView_SetExtendedListViewStyle(h_ListBox, LVS_EX_FULLROWSELECT);

		LVCOLUMN lvCol;
		lvCol.mask = LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 280;
		lvCol.pszText = L"";
		ListView_InsertColumn(h_ListBox, 0, &lvCol);

		SendMessage(CreateWindow(WC_STATIC, L"OPEN", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			10, 32, 30, 20, hWnd, (HMENU)ID_ADDFOLDER, hInst, NULL),
			WM_SETFONT, WPARAM(hFont), TRUE);

		SendMessage(CreateWindow(WC_BUTTON, L"Scan", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			370, 30, 50, 20, hWnd, (HMENU)ID_ADDFOLDER, hInst, NULL),
			WM_SETFONT, WPARAM(hFont), TRUE);

		SendMessage(CreateWindow(WC_BUTTON, L"Remove", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			370, 60, 50, 25, hWnd, (HMENU)ID_REMOVE, hInst, NULL),
			WM_SETFONT, WPARAM(hFont), TRUE);

		/*LoadFile_exe(PROGRAM_FILES_PATH, 0);
		LoadFile_exe(PROGRAM_FILESx86_PATH, 0);*/

		GetCurrentDirectory(MAX_PATH, dataPath);
		StrCat(dataPath, L"\\data.ini");
		LoadData();
		SortArrApp();
		ListLoad(h_ListBox);
		SetFocus(h_TextBox);
	}
	break;
	case WM_APPNOTIFYICON:
	{
		switch (lParam) {
		case WM_LBUTTONDBLCLK:
		{
			if (isShowing == FALSE) {
				isShowing = TRUE;
				ShowWindow(hWnd, SW_SHOW);
				SetFocus(h_TextBox);
			}
		}
		break;
		case WM_RBUTTONUP:
		{
			HMENU notifyMenu = CreatePopupMenu();
			AppendMenu(notifyMenu, MF_STRING, ID_SCAN, L"Scan to build database");
			AppendMenu(notifyMenu, MF_STRING, IDM_ABOUT, L"View statitistics");
			AppendMenu(notifyMenu, MF_STRING, IDM_EXIT, L"Exit");

			// Get Position of Notify Icon to popup the menu
			POINT pCursor;
			GetCursorPos(&pCursor);
			SetForegroundWindow(hWnd);
			TrackPopupMenu(notifyMenu, TPM_RIGHTBUTTON, pCursor.x, pCursor.y, 0, hWnd, NULL);
			PostMessage(hWnd, WM_NULL, 0, 0);
		}
		break;
		}
	}
	break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_REMOVE:
			{
				LVITEM lv;
				lv.mask = LVIF_PARAM;
				lv.iItem = ListView_GetSelectionMark(h_ListBox);
				lv.iSubItem = 0;
				ListView_GetItem(h_ListBox, &lv);

				if (lv.lParam == NULL)
					break;

				for (int i = 0; i < arrApp.size(); i++) {
					if (StrCmp((LPCWSTR)lv.lParam, arrApp[i]->GetPath()) == 0)
						arrApp.erase(arrApp.begin() + i);
				}

				ListLoad(h_ListBox);
			}
			break;
			case ID_ADDFOLDER:
			{
				// Tham khao https://social.msdn.microsoft.com/Forums/en-US/6a7fbf03-2633-4c64-9d83-e99d30442aa2/open-folder-dialog?forum=vcmfcatl
				HRESULT hr;
				IFileOpenDialog *pOpenFolderDialog;
				hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pOpenFolderDialog));

				if (SUCCEEDED(hr))
				{
					pOpenFolderDialog->SetOptions(FOS_PICKFOLDERS);

					// Show the dialog
					hr = pOpenFolderDialog->Show(g_hWnd);
					if (SUCCEEDED(hr))
					{
						// Obtain the result of the user's interaction with the dialog.
						IShellItem *psiResult;
						hr = pOpenFolderDialog->GetResult(&psiResult);

						if (SUCCEEDED(hr))
						{
							// Do something with the result.
							LPWSTR pwsz = NULL;

							hr = psiResult->GetDisplayName(SIGDN_FILESYSPATH, &pwsz);

							if (SUCCEEDED(hr))
								LoadFile_exe(pwsz, 0);

							psiResult->Release();
						}
					}
					pOpenFolderDialog->Release();
				}

				SetFocus(h_TextBox);
				ListLoad(h_ListBox);
			}
			break;
			case ID_SCAN:
			{
				LoadFile_exe(PROGRAM_FILES_PATH, 0);
				LoadFile_exe(PROGRAM_FILESx86_PATH, 0);
				LoadData();
				SortArrApp();
				ListLoad(h_ListBox);
			}
			break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
	case WM_CLOSE:
	{
		isShowing = FALSE;
		ShowWindow(hWnd, SW_HIDE);
	}
	break;
    case WM_DESTROY:
	{
		// Save data ----------
		int seq;
		fstream f;
		f.open(dataPath, ios::out | ios::binary);

		for (int i = 0; i < arrApp.size(); i++) {
			seq = arrApp[i]->GetSeq();
			f.write((char*)&seq, sizeof(int));
			f.write((char*)arrApp[i]->GetPath(), sizeof(WCHAR) * MAX_PATH);
		}

		f.close();
		// -----------

		PostQuitMessage(0);
	}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Statitistics
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		return (INT_PTR)TRUE;
	}
	break;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;
		CApp buffer[6];
		hdc = BeginPaint(hDlg, &ps);

		// Display chart based on usage frequency of apps ----------------
		GetClientRect(hDlg, &rect);
		FillRect(hdc, &rect, (HBRUSH)(COLOR_BTNFACE + 1));
		SetRect(&rect, 350, 50, 380, 65);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 0)));
		SetRect(&rect, 350, 80, 380, 95);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 128, 0)));
		SetRect(&rect, 350, 110, 380, 125);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 255)));
		SetRect(&rect, 350, 140, 380, 155);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 0)));
		SetRect(&rect, 350, 170, 380, 185);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 255)));
		SetRect(&rect, 350, 200, 380, 215);
		FillRect(hdc, &rect, CreateSolidBrush(RGB(128, 0, 255)));

		if (arrApp.size() < 6) {
			for (int i = 0; i < arrApp.size(); i++)
				buffer[i].SetCApp(arrApp[i]->GetName(), arrApp[i]->GetPath(), arrApp[i]->GetSeq());
			for (int i = arrApp.size(); i < 6; i++)
				buffer[i].SetCApp(L"", L"", 0);
		}
		else {
			for (int i = 0; i < 6; i++)
				buffer[i].SetCApp(arrApp[i]->GetName(), arrApp[i]->GetPath(), arrApp[i]->GetSeq());
		}
		int seq = buffer[0].GetSeq();
		SendMessage(CreateWindow(WC_STATIC, IntToString(buffer[0].GetSeq()), WS_CHILD | WS_VISIBLE | SS_LEFT,
			390, 50, 20, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, IntToString(buffer[1].GetSeq()), WS_CHILD | WS_VISIBLE | SS_LEFT,
			390, 80, 20, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, IntToString(buffer[2].GetSeq()), WS_CHILD | WS_VISIBLE | SS_LEFT,
			390, 110, 20, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, IntToString(buffer[3].GetSeq()), WS_CHILD | WS_VISIBLE | SS_LEFT,
			390, 140, 20, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, IntToString(buffer[4].GetSeq()), WS_CHILD | WS_VISIBLE | SS_LEFT,
			390, 170, 20, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, IntToString(buffer[5].GetSeq()), WS_CHILD | WS_VISIBLE | SS_LEFT,
			390, 200, 20, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);

		SendMessage(CreateWindow(WC_STATIC, buffer[0].GetName(), WS_CHILD | WS_VISIBLE | SS_LEFT,
			410, 50, 100, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, buffer[1].GetName(), WS_CHILD | WS_VISIBLE | SS_LEFT,
			410, 80, 100, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, buffer[2].GetName(), WS_CHILD | WS_VISIBLE | SS_LEFT,
			410, 110, 100, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, buffer[3].GetName(), WS_CHILD | WS_VISIBLE | SS_LEFT,
			410, 140, 100, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, buffer[4].GetName(), WS_CHILD | WS_VISIBLE | SS_LEFT,
			410, 170, 100, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);
		SendMessage(CreateWindow(WC_STATIC, buffer[5].GetName(), WS_CHILD | WS_VISIBLE | SS_LEFT,
			410, 200, 100, 20, hDlg, NULL, hInst, NULL), WM_SETFONT, WPARAM(hFont), TRUE);

		int radius, x, y, t1, t2, t3, t4, t5, t6;
		x = 150;
		y = 150;
		radius = 120;
		t1 = buffer[0].GetSeq();
		t2 = buffer[1].GetSeq();
		t3 = buffer[2].GetSeq();
		t4 = buffer[3].GetSeq();
		t5 = buffer[4].GetSeq();
		t6 = buffer[5].GetSeq();
		int sum = t1 + t2 + t3 + t4 + t5 + t6;

		if (sum > 0) {
			float sect = (float)t1 / sum;
			float startAngle = 0;
			DrawPieChart(hDlg, x, y, radius, startAngle, sect * 360, RGB(255, 0, 0));
			startAngle += sect * 360;
			sect = (float)t2 / sum;
			DrawPieChart(hDlg, x, y, radius, startAngle, sect * 360, RGB(0, 128, 0));
			startAngle += sect * 360;
			sect = ((float)t3 / sum);
			DrawPieChart(hDlg, x, y, radius, startAngle, sect * 360, RGB(0, 0, 255));
			startAngle += sect * 360;
			sect = ((float)t4 / sum);
			DrawPieChart(hDlg, x, y, radius, startAngle, sect * 360, RGB(255, 255, 0));
			startAngle += sect * 360;
			sect = ((float)t5 / sum);
			DrawPieChart(hDlg, x, y, radius, startAngle, sect * 360, RGB(255, 0, 255));
			startAngle += sect * 360;
			sect = ((float)t6 / sum);
			DrawPieChart(hDlg, x, y, radius, startAngle, sect * 360, RGB(128, 0, 255));
		}
		else
			DrawPieChart(hDlg, x, y, radius, 0, 360, RGB(255, 255, 255));

		EndPaint(hDlg, &ps);
		// ------------------------
	}
	break;
	}

	
    return (INT_PTR)FALSE;
}

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

	if ((GetAsyncKeyState(VK_LCONTROL) < 0) && (GetAsyncKeyState('Q') < 0)) {
		if (isShowing) {
			isShowing = FALSE;
			ShowWindow(g_hWnd, SW_HIDE);
		}
		else {
			SetFocus(h_TextBox);
			isShowing = TRUE;
			ShowWindow(g_hWnd, SW_SHOW);
		}
	}

	if (GetAsyncKeyState(VK_RETURN) < 0 && isShowing) {
		RunProgram();
		ShowWindow(g_hWnd, SW_HIDE);
		isShowing = FALSE;
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}


void RunProgram()
{
	LVITEM lv;
	lv.mask = LVIF_PARAM;
	lv.iItem = ListView_GetSelectionMark(h_ListBox);
	lv.iSubItem = 0;
	ListView_GetItem(h_ListBox, &lv);

	if (lv.lParam == NULL)
		return;

	for (int i = 0; i < arrApp.size(); i++) {
		if (StrCmp((LPCWSTR)lv.lParam, arrApp[i]->GetPath()) == 0)
			arrApp[i]->Run();
	}
}

void SortArrApp() 
{
	if (arrApp.size() != 0) {
		for (int i = 0; i < arrApp.size() - 1; i++) {
			for (int j = i + 1; j < arrApp.size(); j++) {
				if (arrApp[i]->GetSeq() < arrApp[j]->GetSeq()) {
					CApp* buf = new CApp(arrApp[i]->GetName(), arrApp[i]->GetPath(), arrApp[i]->GetSeq());
					arrApp[i]->SetCApp(arrApp[j]->GetName(), arrApp[j]->GetPath(), arrApp[j]->GetSeq());
					arrApp[j]->SetCApp(buf->GetName(), buf->GetPath(), buf->GetSeq());
					delete buf;
				}
			}
		}
	}
	return;
}

BOOL LoadFile_exe(WCHAR* path, int floor)
{
	if (floor >= 2)
		return FALSE;

	LPITEMIDLIST pidl;
	IShellFolder *psfChild = NULL;
	LPITEMIDLIST pidlItems = NULL;
	LPENUMIDLIST ppenum = NULL;
	HRESULT hr = NULL;
	WIN32_FIND_DATA fd;
	STRRET strret;
	WCHAR itemName[MAX_NAME];
	WCHAR itemPath[MAX_PATH];
	CApp* item;

	pidl = ILCreateFromPath(path);
	SHBindToObject(NULL, pidl, NULL, IID_IShellFolder, (void**)&psfChild);
	if (psfChild == NULL)
		return FALSE;

	psfChild->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &ppenum);
	if (ppenum == NULL)
		return FALSE;

	do {
		hr = ppenum->Next(1, &pidlItems, NULL);
		if (hr == S_OK) {
			psfChild->GetDisplayNameOf(pidlItems, SHGDN_NORMAL, &strret);
			StrRetToBuf(&strret, pidlItems, itemName, MAX_NAME);
			StrCpy(itemPath, path);
			StrCat(itemPath, L"\\");
			StrCat(itemPath, itemName);
			GetFileAttributesEx(itemPath, GetFileExInfoStandard, &fd);

			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				LoadFile_exe(itemPath, floor + 1);
			else {
				if ((itemName[wcslen(itemName) - 1] == 'e') && (itemName[wcslen(itemName) - 2] == 'x') &&
					(itemName[wcslen(itemName) - 3] == 'e') && (itemName[wcslen(itemName) - 4] == '.')) {
					item = new CApp(itemName, itemPath, 0);
					arrApp.push_back(item);
				}
			}
		}
	} while (hr == S_OK);

	psfChild->Release();
	return TRUE;
}

BOOL ListLoad(HWND hwnd)
{
	SortArrApp();
	int size = GetWindowTextLength(h_TextBox);
	WCHAR* fileName = new WCHAR[size + 1];
	GetWindowText(h_TextBox, fileName, size + 1);

	int count = SendMessage(h_TextBox, CB_GETCOUNT, 0, 0);
	for (int i = count - 1; i >= 0; i--)
		SendMessage(h_TextBox, CB_DELETESTRING, i, 0);

	ListView_DeleteAllItems(hwnd);
	int nItemCount = 0;
	LVITEM lv;
	lv.mask = LVIF_TEXT | LVIF_PARAM;
	lv.iSubItem = 0;

	for (int i = 0; i < arrApp.size(); i++) {
		int j;
		for (j = 0; j < size; j++) {
			if (LowerCase(arrApp[i]->GetName()[j]) != LowerCase(fileName[j]))
				break;
		}

		if (j == size) {
			lv.iItem = nItemCount++;
			lv.pszText = arrApp[i]->GetName();
			lv.lParam = (LPARAM)arrApp[i]->GetPath();
			ListView_InsertItem(hwnd, &lv);
		}
	}

	return TRUE;
}

WCHAR LowerCase(WCHAR chr)
{
	if (chr >= 65 && chr <= 90)
		return chr + 32;
}

// Tham khao https://stackoverflow.com/questions/15711311/edit-control-capture-enter-key
LRESULT CALLBACK subEditProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN: case VK_UP:
			SetFocus(h_ListBox);
			ListView_SetItemState(h_ListBox, 0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
			break;
		}
		break;
	case WM_KEYUP:
		ListLoad(h_ListBox);
		break;
	default:
		return CallWindowProc(oldEditProc, wnd, msg, wParam, lParam);
	}
	return 0;
}

BOOL DrawPieChart(HWND hWnd, int x, int y, DWORD radius, float startAngle, float sweepAngle, COLORREF color)
{
	HDC hdc;
	hdc = GetDC(hWnd);
	BeginPath(hdc);
	SelectObject(hdc, GetStockObject(DC_BRUSH));
	SetDCBrushColor(hdc, color);
	MoveToEx(hdc, x, y, (LPPOINT)NULL);
	AngleArc(hdc, x, y, radius, startAngle, sweepAngle);
	EndPath(hdc);
	StrokeAndFillPath(hdc);
	ReleaseDC(hWnd, hdc);

	return TRUE;
}

WCHAR* IntToString(int it)
{
	int buf = it, length = 0;

	while (buf != 0) {
		buf /= 10;
		length++;
	}

	if (length != 0) {
		buf = it;
		WCHAR buffer[MAX_LOADSTRING];
		buffer[length] = 0;
		for (int i = length - 1; i >= 0; i--) {
			buffer[i] = (buf % 10) + '0';
			buf /= 10;
		}

		return buffer;
	}
	else
		return L"0";
}

BOOL LoadData()
{
	fstream f;
	f.open(dataPath, ios::in | ios::binary);

	if (f.is_open()) {
		int seq;
		WCHAR* filePath = new WCHAR[MAX_PATH];

		while (!f.eof()) {
			f.read((char*)&seq, sizeof(int));
			if (f.eof()) break;

			f.read((char*)filePath, sizeof(WCHAR) * MAX_PATH);

			for (int i = 0; i < arrApp.size(); i++)
				if (StrCmp(filePath, arrApp[i]->GetPath()) == 0) {
					arrApp[i]->SetSeq(seq);
					break;
				}
		}

		delete filePath;
	}

	f.close();
	return TRUE;
}