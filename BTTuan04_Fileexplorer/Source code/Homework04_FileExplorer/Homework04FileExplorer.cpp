// Homework04FileExplorer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Homework04FileExplorer.h"

#define MAX_LOADSTRING 100
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
DWORD WINAPI		ThreadFunc(LPVOID lpParameter);

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hWnd);
void OnDestroy(HWND hwnd);
LPWSTR GetSize(WIN32_FIND_DATA fd);
LPWSTR Convert(DWORD nSize);
LPWSTR GetType(const WIN32_FIND_DATA &fd);
LPWSTR GetDateModified(const FILETIME &ftLastWrite);
BOOL TreeView_IsChildren(WCHAR *path);

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
    LoadStringW(hInstance, IDC_HOMEWORK04FILEEXPLORER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_HOMEWORK04FILEEXPLORER));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEWORK04FILEEXPLORER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_HOMEWORK04FILEEXPLORER);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
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

HWND listview1;
HWND treeview1;
HTREEITEM hRoot;
BOOL loading = false;
WCHAR dictionary[255];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
		case WM_NOTIFY:
		{
			int wmId = LOWORD(wParam);
			WCHAR buffer[255];
			switch (wmId) {
				case ID_LISTVIEW: {
					LPNMHDR info = (LPNMHDR)lParam;
					if (NM_DBLCLK == info->code) {
						int ItemIndex = SendMessage(listview1, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
						if (ItemIndex != -1) {
							LVITEM select;
							WCHAR lpText[255];
							select.iSubItem = 0;
							select.cchTextMax = 255;
							select.pszText = lpText;
							SendMessage(listview1, LVM_GETITEMTEXTW, ItemIndex, (LPARAM)&select);

							if (!(StrCmp(select.pszText, L"..") == 0 && wcslen(dictionary) == 3)) {
								if (StrCmp(select.pszText, L"..") == 0)
									for (int i = wcslen(dictionary) - 2; i > -1; --i)
									{
										if (dictionary[i] == L'\\')
										{
											dictionary[i] = 0;
											break;
										}
									}
								else
									StrCat(dictionary, select.pszText);

								StrCat(dictionary, L"\\");

								WCHAR path[255];
								StrCpy(path, dictionary);
								StrCat(path, _T("*.*"));
								WIN32_FIND_DATA fd;
								HANDLE hFile = FindFirstFile(path, &fd);

								SendMessage(listview1, LVM_DELETEALLITEMS, 0, 0);

								LVITEM lv;
								lv.mask = LVIF_TEXT;
								lv.iItem = 0;
								lv.iSubItem = 0;
								if (dictionary[0] != 0) {
									lv.pszText = L"..";
									ListView_InsertItem(listview1, &lv);
								}
								++lv.iItem;

								while (FindNextFile(hFile, &fd) != 0) {
									if (StrCmp(fd.cFileName, L"..") != 0) {
										lv.pszText = fd.cFileName;
										ListView_InsertItem(listview1, &lv);
										ListView_SetItemText(listview1, lv.iItem, 1, GetType(fd));
										if (fd.nFileSizeLow != 0)
											ListView_SetItemText(listview1, lv.iItem, 2, GetSize(fd));
										ListView_SetItemText(listview1, lv.iItem, 3, GetDateModified(fd.ftLastWriteTime));
										lv.iItem++;
									}
								}

								FindClose(hFile);
								
							}
							else {
								LVITEM lv;
								lv.mask = LVIF_TEXT;
								lv.iItem = 0;
								lv.iSubItem = 0;
								WCHAR buffer[100];
								WCHAR pszText[3] = L"*:";
								GetLogicalDriveStrings(100, buffer);
								dictionary[0] = 0;

								SendMessage(listview1, LVM_DELETEALLITEMS, 0, 0);
									
								for (int i = 0; !((buffer[i] == 0) && (buffer[i + 1] == 0)); ++i)
								{
									if (buffer[i] == 0) {
										pszText[0] = buffer[i - 3];
										lv.pszText = pszText;
										ListView_InsertItem(listview1, &lv);
										++lv.iItem;
									}
								}
							}
						}
					}
				}
				break;
				case ID_TREEVIEW: {
					LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
					NMHDR info = pnmtv->hdr;

					if (TVN_ITEMEXPANDING == info.code) {
						if (!loading) {
							loading = true;
							CreateThread(NULL, 0, ThreadFunc, 0, 0, 0);
						}
					}
				}
				break;
			}
			break;
		}
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	INITCOMMONCONTROLSEX icex;

	// Ensure that the common control DLL is loaded. 
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES;
	InitCommonControlsEx(&icex);

	// List view
	dictionary[0] = 0;
	listview1 = CreateWindowEx(NULL, WC_LISTVIEWW, L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT | WS_BORDER,
		200, 10, 300, 400, hWnd, (HMENU)ID_LISTVIEW, hInst, NULL);

	LVCOLUMN lvCol;

	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = L"Tên";
	lvCol.cx = 100;
	ListView_InsertColumn(listview1, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 80;
	lvCol.pszText = _T("Loại");
	ListView_InsertColumn(listview1, 1, &lvCol);

	lvCol.fmt = LVCFMT_RIGHT;
	lvCol.pszText = _T("Kích thước");
	ListView_InsertColumn(listview1, 2, &lvCol);

	lvCol.pszText = _T("Mô tả");
	ListView_InsertColumn(listview1, 3, &lvCol);

	LVITEM lv;
	lv.mask = LVIF_TEXT;
	lv.iItem = 0;
	lv.iSubItem = 0;
	WCHAR buffer[100];
	WCHAR pszText[3] = L"*:";
	GetLogicalDriveStrings(100, buffer);
		
	for (int i = 0; !((buffer[i] == 0) && (buffer[i + 1] == 0)); ++i)
	{
		if (buffer[i] == 0) {
			pszText[0] = buffer[i - 3];
			lv.pszText = pszText;
			ListView_InsertItem(listview1, &lv);
			++lv.iItem;
		}
	}

	// Tree view
	treeview1 = CreateWindowEx(NULL, WC_TREEVIEWW, L"",
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		10, 10, 190, 400, hWnd, (HMENU)ID_TREEVIEW, hInst, NULL);

	TV_INSERTSTRUCT tvInsert;
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_ROOT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	tvInsert.item.pszText = L"This PC";
	tvInsert.item.cChildren = 1;
	hRoot = TreeView_InsertItem(treeview1, &tvInsert);
	
	return 1;
}
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	int wmId = LOWORD(id);
	// Parse the menu selections:
	switch (wmId)
	{
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}

LPWSTR GetSize(WIN32_FIND_DATA fd)
{
	DWORD dwSize = fd.nFileSizeLow;

	return Convert(dwSize);
}

LPWSTR Convert(DWORD nSize)
{
	int nType = 0; //Bytes

	while (nSize >= 1048576) //
	{
		nSize /= 1024;
		++nType;
	}

	DWORD nRight;

	if (nSize >= 1024)
	{
		//Lấy một chữ số sau thập phân của nSize chứa trong nRight
		nRight = nSize % 1024;

		while (nRight > 99)
			nRight /= 10;

		nSize /= 1024;
		++nType;
	}
	else
		nRight = 0;

	TCHAR *buffer = new TCHAR[11];
	_itow_s(nSize, buffer, 11, RADIX);

	if (nRight != 0 && nType > KB)
	{
		StrCat(buffer, _T("."));
		TCHAR *right = new TCHAR[3];
		_itow_s(nRight, right, 3, RADIX);
		StrCat(buffer, right);
	}

	switch (nType)
	{
	case 0://Bytes
		StrCat(buffer, _T(" bytes"));
		break;
	case KB:
		StrCat(buffer, _T(" KB"));
		break;
	case MB:
		StrCat(buffer, _T(" MB"));
		break;
	case GB:
		StrCat(buffer, _T(" GB"));
		break;
	case TB:
		StrCat(buffer, _T(" TB"));
		break;
	}

	return buffer;
}

LPWSTR GetType(const WIN32_FIND_DATA &fd)
{
	int nDotPos = StrRStrI(fd.cFileName, NULL, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);

	if (nDotPos < 0 || nDotPos >= len) //Nếu không tìm thấy
		return _T("Thư mục");

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi

	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		return _T("Web page");
	}
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Không biết");
	}
	RegCloseKey(hKey);

	return pszPath;
}

LPWSTR GetDateModified(const FILETIME &ftLastWrite)
{

	//Chuyển đổi sang local time
	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&ftLastWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	TCHAR *buffer = new TCHAR[50];
	wsprintf(buffer, _T("%02d/%02d/%04d %02d:%02d %s"),
		stLocal.wDay, stLocal.wMonth, stLocal.wYear,
		(stLocal.wHour>12) ? (stLocal.wHour / 12) : (stLocal.wHour),
		stLocal.wMinute,
		(stLocal.wHour>12) ? (_T("Chiều")) : (_T("Sáng")));

	return buffer;
}

DWORD WINAPI ThreadFunc(LPVOID lpParameter)
{
	WCHAR buffer[255];
	TV_INSERTSTRUCT tvInsert;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.mask = TVIF_TEXT | TVIF_CHILDREN;
	tvInsert.hParent = hRoot;
	tvInsert.item.cChildren = 2;
	WCHAR pszText[3] = L"*:";
	GetLogicalDriveStrings(100, buffer);

	for (int i = 0; !((buffer[i] == 0) && (buffer[i + 1] == 0)); ++i)
	{
		if (buffer[i] == 0) {
			pszText[0] = buffer[i - 3];
			tvInsert.item.pszText = pszText;
			TreeView_InsertItem(treeview1, &tvInsert);
		}
	}



	TreeView_Expand(treeview1, hRoot, TVE_EXPAND);

	return 0;
}

BOOL TreeView_IsChildren(WCHAR *path)
{
	WIN32_FIND_DATA fd;
	HANDLE hFile = FindFirstFile(path, &fd);

	while (FindNextFile(hFile, &fd) != 0)
	{
		if (StrCmp(GetType(fd), L"Thư mục") == 0)
		{
			FindClose(hFile);
			return true;
		}
	}
	FindClose(hFile);
	return false;
}