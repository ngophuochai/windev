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

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify);
void OnPaint(HWND hWnd);
void OnDestroy(HWND hwnd);
LPCWSTR NumConvertStr(LONG num);
LONG StrConvertNum(LPCWSTR str);
LPCWSTR getPath(HTREEITEM hItem, HWND h_TreeView);
LPCWSTR getPath(HWND h_ListView, int iItem);
HWND createTreeView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle);
void loadMyComputerToTree(DriveHelper *drive, HWND h_TreeView);
void loadTreeviewItemAt(HTREEITEM &hParent, LPCWSTR path, HWND h_TreeView);
void loadExpandedChild(HTREEITEM hCurrSelected, HWND h_TreeView);
HWND createListView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle);
void initListviewColumn(HWND h_ListView, int type);
void loadMyComputerToListView(DriveHelper *drive, HWND h_ListView);
void loadDirItemToLisview(HWND m_hParent, HWND h_ListView, LPCWSTR path);
void loadOrExecSelected(HWND h_ListView);
LPWSTR convertTimeStampToString(const FILETIME &ftLastWrite);
LPWSTR _GetType(const WIN32_FIND_DATA &fd);
void loadStatusBar(HWND h_ListView);
void loadDesktopToListView(HWND h_ListView);

//For init lv column
#define LVCOL_DRIVE_TYPE		0
#define LVCOL_FOLDER_TYPE		1

#define MAX_PATH_LEN			10240
#define NUMBER_OF_INIT_ICON		8
#define MAX_EXPAND_ICON			3
#define DEFAULT_ICON_INDEX		0

RECT rcClient;
HWND g_hWnd;
HWND h_TreeView;
HWND h_ListView;
HWND hStatusBar;
DriveHelper* g_Drive;
WCHAR configPath[MAX_PATH_LEN];

// Bmp treeview
int myComputerBmpIndex, desktopBmpIndex, driveBmpIndex, CDBmpIndex, fileBmpIndex, folderBmpIndex;
// Icon listview
int driveIconIndex, CDIconIndex, fileIconIndex, folderIconIndex, myComputerIconIndex;


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
	if (!InitInstance(hInstance, nCmdShow))
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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_HOMEWORK04FILEEXPLORER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_HOMEWORK04FILEEXPLORER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	GetCurrentDirectory(MAX_PATH_LEN, configPath);
	StrCat(configPath, L"\\appname.ini");
	RECT IpRect;
	WCHAR mainSize[11];

	GetPrivateProfileString(_T("main window size"), _T("left"), _T(""), mainSize, 11, configPath);
	if (mainSize[0] != 0) {
		IpRect.left = StrConvertNum(mainSize);
		GetPrivateProfileString(_T("main window size"), _T("right"), _T(""), mainSize, 11, configPath);
		IpRect.right = StrConvertNum(mainSize);
		GetPrivateProfileString(_T("main window size"), _T("bottom"), _T(""), mainSize, 11, configPath);
		IpRect.bottom = StrConvertNum(mainSize);
		GetPrivateProfileString(_T("main window size"), _T("top"), _T(""), mainSize, 11, configPath);
		IpRect.top = StrConvertNum(mainSize);
	}
	else {
		IpRect.top = 0;
		IpRect.left = CW_USEDEFAULT;
		IpRect.right = CW_USEDEFAULT;
		IpRect.bottom = 0;
	}

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		IpRect.left, IpRect.top, IpRect.right, IpRect.bottom, nullptr, nullptr, hInstance, nullptr);

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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT tvRect;
	GetClientRect(h_TreeView, &tvRect);
	switch (message)
	{
	case WM_NOTIFY:
	{
		int nCurSelIndex;

		//The program has started and loaded all necessary component

		NMHDR* notifyMess = (NMHDR*)lParam; //Notification Message
		LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)notifyMess; //Contains information about a tree-view notification message
		HTREEITEM currSelected;

		switch (notifyMess->code)
		{
		case TVN_ITEMEXPANDING:
			currSelected = lpnmTree->itemNew.hItem;
			loadExpandedChild(currSelected, h_TreeView);
			break;
		case NM_CLICK:
			if (notifyMess->hwndFrom == h_ListView)
				loadStatusBar(h_ListView);
			break;

		case NM_DBLCLK:
			//Get hwndFrom for window handle to the control sending the message
			//To check whether this event fire by Listview
			if (notifyMess->hwndFrom == h_ListView)
				loadOrExecSelected(h_ListView);
			break;
		}
	}
	break;
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
	g_hWnd = hWnd;
	g_Drive = new DriveHelper();
	g_Drive->getSystemDrives();

	InitCommonControls();

	//Get main parent window size
	GetClientRect(hWnd, &rcClient);
	int parentWidth = rcClient.right - rcClient.left;
	int parentHeight = rcClient.bottom - rcClient.top;

	//\Treeview
	long extStyle = 0, style = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	int x = 0, y = 0;
	int nWidth = parentWidth / 4, nHeight = parentHeight - 22;

	h_TreeView = createTreeView(extStyle, hWnd, ID_TREEVIEW, hInst, x, y, nWidth, nHeight, style);

	loadMyComputerToTree(g_Drive, h_TreeView);
	SetFocus(h_TreeView);

	//Listview
	x = nWidth + 1;
	nWidth = (rcClient.right - rcClient.left) * 3 / 4;
	extStyle = WS_EX_CLIENTEDGE;
	style = LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS;

	h_ListView = createListView(extStyle, hWnd, ID_LISTVIEW, hInst, x, y, nWidth, nHeight, style);
	loadDesktopToListView(h_ListView);

	// Create the status bar.

	HLOCAL hloc;
	PINT paParts;
	int i;
	int cParts = 3;
	InitCommonControls();

	hStatusBar = CreateWindowEx(0, STATUSCLASSNAME, (PCTSTR)NULL, SBARS_SIZEGRIP | WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hWnd, NULL, hInst, NULL);
								 
	GetClientRect(hWnd, &rcClient);

	hloc = LocalAlloc(LHND, sizeof(int) * cParts);
	paParts = (PINT)LocalLock(hloc);

	nWidth = rcClient.right / cParts;
	int rightEdge = nWidth;
	for (i = 0; i < cParts; i++) {
		paParts[i] = rightEdge;
		rightEdge += nWidth;
	}

	SendMessage(hStatusBar, SB_SETPARTS, (WPARAM)cParts, (LPARAM)paParts);
	
	LocalUnlock(hloc);
	LocalFree(hloc);

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

	RECT rcClientTV;
	GetClientRect(hWnd, &rcClient);
	GetClientRect(h_TreeView, &rcClientTV);
	int x = rcClientTV.right + 32;
	int nWidth = (rcClient.right - rcClient.left) - x - 1;
	MoveWindow(h_ListView, x, 0, nWidth, rcClient.bottom - rcClient.top - 23, TRUE);

	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hWnd)
{
	RECT IpRect;
	GetWindowRect(hWnd, &IpRect);
	IpRect.right -= IpRect.left;
	IpRect.bottom -= IpRect.top;
	
	WritePrivateProfileString(_T("main window size"), _T("left"), NumConvertStr(IpRect.left), configPath);
	WritePrivateProfileString(_T("main window size"), _T("right"), NumConvertStr(IpRect.right), configPath);
	WritePrivateProfileString(_T("main window size"), _T("bottom"), NumConvertStr(IpRect.bottom), configPath);
	WritePrivateProfileString(_T("main window size"), _T("top"), NumConvertStr(IpRect.top), configPath);

	PostQuitMessage(0);
}

LPCWSTR NumConvertStr(LONG num)
{
	if (num != 0) {
		WCHAR buffer1[11], buffer2[11];
		LONG buf = num;
		int i, j;

		for (i = 0; buf != 0; ++i) {
			buffer1[i] = buf % 10 + '0';
			buf /= 10;
		}
		buffer1[i] = 0;

		for (j = 0, --i; i >= 0; ++j, --i)
			buffer2[j] = buffer1[i];
		buffer2[j] = 0;
		return buffer2;
	}
	else
		return L"0";
}

LONG StrConvertNum(LPCWSTR str)
{
	LONG num = 0;
	for (int i = 0; i < wcslen(str); ++i)
		num = (str[i]) - '0' + num * 10;

	return num;
}

LPCWSTR getPath(HTREEITEM hItem, HWND h_TreeView)
{
	TVITEMEX tv; //Specifies or receives attributes of a tree-view item.
	tv.mask = TVIF_PARAM;
	tv.hItem = hItem;
	TreeView_GetItem(h_TreeView, &tv); //Retrieves some or all of a tree-view item's attributes. 
	return (LPCWSTR)tv.lParam;
}

LPCWSTR getPath(HWND h_ListView, int iItem)
{
	if (iItem >= 0) {
		LVITEM lv;
		lv.mask = LVIF_PARAM;
		lv.iItem = iItem;
		lv.iSubItem = 0;
		ListView_GetItem(h_ListView, &lv);
		return (LPCWSTR)lv.lParam;
	}
	return NULL;
}

HWND createTreeView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle)
{
	//Create
	HWND h_TreeView = CreateWindowEx(lExtStyle, WC_TREEVIEW, _T("Tree View"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, parentWnd,
		(HMENU)ID, hParentInst, NULL);
	
	HIMAGELIST himl;  // handle to image list 
	HBITMAP hbmp;     // handle to bitmap 

	// Create the image list. 
	himl = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK,
		NUMBER_OF_INIT_ICON, MAX_EXPAND_ICON);

	// Add the open file, closed file, and document bitmaps. 
	hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DESKTOP));
	desktopBmpIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MYCOMPUTER));
	myComputerBmpIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_DRIVE));
	driveBmpIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CD));
	CDBmpIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FILE));
	fileBmpIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDER));
	folderBmpIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	// Associate the image list with the tree-view control. 
	TreeView_SetImageList(h_TreeView, himl, TVSIL_NORMAL);

	return h_TreeView;
}

void loadMyComputerToTree(DriveHelper *drive, HWND h_TreeView)
{
	TV_INSERTSTRUCT tvInsert;

	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	//Load Desktop
	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_ROOT;
	tvInsert.item.iImage = desktopBmpIndex;
	tvInsert.item.iSelectedImage = desktopBmpIndex;
	tvInsert.item.pszText = _T("Desktop");
	tvInsert.item.lParam = (LPARAM)_T("Desktop");
	HTREEITEM hDesktop = TreeView_InsertItem(h_TreeView, &tvInsert);

	//Load My Computer
	tvInsert.hParent = hDesktop;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item.iImage = myComputerBmpIndex;
	tvInsert.item.iSelectedImage = myComputerBmpIndex;
	tvInsert.item.pszText = _T("My Computer");
	tvInsert.item.lParam = (LPARAM)_T("MyComputer");
	HTREEITEM hMyComputer = TreeView_InsertItem(h_TreeView, &tvInsert);

	//Load volume
	for (int i = 0; i < g_Drive->getCount(); ++i)
	{
		tvInsert.hParent = hMyComputer; //Add as children of My Computer
		if (StrCmp(L"Local Disk", g_Drive->getDriveType(i)) == 0) {
			tvInsert.item.iImage = driveBmpIndex;
			tvInsert.item.iSelectedImage = driveBmpIndex;
		}
		else if (StrCmp(L"CD-ROM", g_Drive->getDriveType(i)) == 0) {
			tvInsert.item.iImage = CDBmpIndex;
			tvInsert.item.iSelectedImage = CDBmpIndex;
		}
		tvInsert.item.pszText = g_Drive->getDisplayName(i); //Get volume label
		tvInsert.item.lParam = (LPARAM)g_Drive->getDriveLetter(i);
		HTREEITEM hDrive = TreeView_InsertItem(h_TreeView, &tvInsert);

		loadTreeviewItemAt(hDrive, getPath(hDrive, h_TreeView), h_TreeView);
	}

	//Expand and select My Computer
	TreeView_Expand(h_TreeView, hMyComputer, TVE_EXPAND);
	TreeView_SelectItem(h_TreeView, hMyComputer);
}

void loadTreeviewItemAt(HTREEITEM &hParent, LPCWSTR path, HWND h_TreeView)
{
	//Insert new item to TreeView
	TV_INSERTSTRUCT tvInsert;
	tvInsert.hParent = hParent;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvInsert.item.iImage = folderBmpIndex;
	tvInsert.item.iSelectedImage = folderBmpIndex;

	HRESULT hr = NULL;
	IShellFolder *psfChild = NULL;
	LPENUMIDLIST ppenum = NULL;
	LPITEMIDLIST pidlItems = NULL;
	WIN32_FIND_DATA fd;
	ULONG uAttr;
	LPITEMIDLIST pidl = ILCreateFromPath(path);

	SHBindToObject(NULL, pidl, NULL, IID_IShellFolder, (void**)&psfChild);
	if (psfChild == NULL)
		return;
	hr = psfChild->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &ppenum);
	if (ppenum == NULL)
		return;

	TCHAR* folderPath;

	do
	{
		hr = ppenum->Next(1, &pidlItems, NULL);
		if (hr == S_OK) {
			WCHAR buffer[255];
			STRRET strret;
			psfChild->GetDisplayNameOf(pidlItems, SHGDN_NORMAL, &strret);
			SHGetDataFromIDList(psfChild, pidlItems, SHGDFIL_FINDDATA, &fd, sizeof(WIN32_FIND_DATA));
			StrRetToBuf(&strret, pidlItems, buffer, 255);
			tvInsert.item.pszText = buffer;
			TCHAR *pszItemPath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(pszItemPath, path);

			if (wcslen(path) != 3)
				StrCat(pszItemPath, L"\\");

			StrCat(pszItemPath, buffer);
			tvInsert.item.lParam = (LPARAM)pszItemPath;

			uAttr = SFGAO_FOLDER;
			psfChild->GetAttributesOf(1, (LPCITEMIDLIST *)&pidlItems, &uAttr);

			if (uAttr & SFGAO_FOLDER)
				HTREEITEM hItem = TreeView_InsertItem(h_TreeView, &tvInsert);
		}
	} while (hr == S_OK);

	psfChild->Release();
}

void loadExpandedChild(HTREEITEM hCurrSelected, HWND h_TreeView)
{
	HTREEITEM myDesktop = TreeView_GetRoot(h_TreeView); //Return the top most or very first item of the TreeView
	HTREEITEM myComputer = TreeView_GetChild(h_TreeView, myDesktop); //Return the first child item of Desktop (MyComputer)
	if (hCurrSelected == myComputer) //If currently select My Computer -> dont load
		return;

	HTREEITEM hCurrSelectedChild = TreeView_GetChild(h_TreeView, hCurrSelected); //Get the first child of treeview item

	if (hCurrSelectedChild != NULL)
	{
		do
		{
			//Get child of this Current selected child, if result is NULL -> It never be loaded
			if (TreeView_GetChild(h_TreeView, hCurrSelectedChild) == NULL)
			{
				//Load all child of Current Selected Child	
				loadTreeviewItemAt(hCurrSelectedChild, getPath(hCurrSelectedChild, h_TreeView), h_TreeView);
			}
		} while (hCurrSelectedChild = TreeView_GetNextSibling(h_TreeView, hCurrSelectedChild));
	}
	else
	{
		//Possitively will not happen.
		loadTreeviewItemAt(hCurrSelected, getPath(hCurrSelected, h_TreeView), h_TreeView);
	}
}

HWND createListView(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle)
{
	//Create
	HWND h_ListView = CreateWindowEx(lExtStyle, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, parentWnd, (HMENU)ID, hParentInst, NULL);


	//Init 5 columns
	LVCOLUMN lvCol;

	//Let the LVCOLUMN know that we will set the format, header text and width of it
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 150;
	lvCol.pszText = _T("Name");
	ListView_InsertColumn(h_ListView, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = _T("Type");
	lvCol.cx = 125;
	ListView_InsertColumn(h_ListView, 1, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 125;
	lvCol.pszText = _T("Total Size");
	ListView_InsertColumn(h_ListView, 2, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = _T("Free Space");
	lvCol.cx = 125;
	ListView_InsertColumn(h_ListView, 3, &lvCol);

	HIMAGELIST himl;  // handle to image list 
	HICON hiconItem;

	// Create the image list. 
	himl = ImageList_Create(16, 16, ILC_MASK, NUMBER_OF_INIT_ICON, MAX_EXPAND_ICON);

	hiconItem = LoadIcon(hInst, MAKEINTRESOURCE(IDI_FILE));
	fileIconIndex = ImageList_AddIcon(himl, hiconItem);
	DestroyIcon(hiconItem);

	hiconItem = LoadIcon(hInst, MAKEINTRESOURCE(IDI_FOLDER));
	folderIconIndex = ImageList_AddIcon(himl, hiconItem);
	DestroyIcon(hiconItem);

	hiconItem = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DRIVE));
	driveIconIndex = ImageList_AddIcon(himl, hiconItem);
	DestroyIcon(hiconItem);

	hiconItem = LoadIcon(hInst, MAKEINTRESOURCE(IDI_CD));
	CDIconIndex = ImageList_AddIcon(himl, hiconItem);
	DestroyIcon(hiconItem);

	hiconItem = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MYCOMPUTER));
	myComputerIconIndex = ImageList_AddIcon(himl, hiconItem);
	DestroyIcon(hiconItem);

	ListView_SetImageList(h_ListView, himl, LVSIL_SMALL);

	return h_ListView;
}

void initListviewColumn(HWND h_ListView, int type)
{
	LVCOLUMN lvCol;
	if (type == LVCOL_DRIVE_TYPE)
	{
		lvCol.mask = LVCF_TEXT | LVCF_FMT;

		lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
		lvCol.cx = 100;
		lvCol.pszText = _T("Type");
		ListView_SetColumn(h_ListView, 1, &lvCol);

		lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
		lvCol.cx = 80;
		lvCol.pszText = _T("Total Size");
		ListView_SetColumn(h_ListView, 2, &lvCol);

		lvCol.cx = 80;
		lvCol.pszText = _T("Free Space");
		ListView_SetColumn(h_ListView, 3, &lvCol);
	}
	else if (type == LVCOL_FOLDER_TYPE)
	{
		lvCol.mask = LVCF_WIDTH;
		lvCol.cx = 180;
		ListView_SetColumn(h_ListView, 0, &lvCol);

		lvCol.mask = LVCF_TEXT | LVCF_FMT;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.pszText = _T("Date Modified");
		ListView_SetColumn(h_ListView, 1, &lvCol);


		lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 130;
		lvCol.pszText = _T("Type");
		ListView_SetColumn(h_ListView, 2, &lvCol);

		lvCol.pszText = _T("Size");
		ListView_SetColumn(h_ListView, 3, &lvCol);
	}
	else
	{
		//Reserve for initialize or create LV
	}
}

void loadMyComputerToListView(DriveHelper *drive, HWND h_ListView)
{
	//Init column of Listview
	initListviewColumn(h_ListView, LVCOL_DRIVE_TYPE);
	LV_ITEM lv;
	lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lv.iItem = 0;
	lv.iImage = folderIconIndex;
	lv.iSubItem = 0;
	lv.pszText = L"Desktop";
	lv.lParam = (LPARAM)L"Desktop";
	ListView_InsertItem(h_ListView, &lv);

	for (int i = 0; i < drive->getCount(); ++i)
	{
		//Let ListView know that we'r going to change item text, image and param
		lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;

		//Load Label name and default icon to first column
		lv.iItem = i + 1;

		if (StrCmp(drive->getDriveType(i), L"Local Disk") == 0)
			lv.iImage = driveIconIndex;
		else if (StrCmp(drive->getDriveType(i), L"CD-ROM") == 0)
			lv.iImage = CDIconIndex;

		lv.iSubItem = 0;
		lv.pszText = drive->getDisplayName(i);
		lv.lParam = (LPARAM)drive->getDriveLetter(i);
		ListView_InsertItem(h_ListView, &lv);

		//
		lv.mask = LVIF_TEXT;

		//Load Type of directory to second column
		lv.iSubItem = 1;
		lv.pszText = drive->getDriveType(i);
		ListView_SetItem(h_ListView, &lv);

		//Load total size to third column
		lv.iSubItem = 2;
		if (wcscmp(drive->getDriveType(i), CD_ROM) != 0)
			lv.pszText = drive->getTotalSize(i);
		else
			lv.pszText = NULL;
		ListView_SetItem(h_ListView, &lv);

		//Load Free Space to last column
		lv.iSubItem = 3;
		if (wcscmp(drive->getDriveType(i), CD_ROM) != 0)
			lv.pszText = drive->getFreeSpace(i);
		else
			lv.pszText = NULL;

		//Set
		ListView_SetItem(h_ListView, &lv);
	}
}

void loadDirItemToLisview(HWND m_hParent, HWND h_ListView, LPCWSTR path)
{
	initListviewColumn(h_ListView, LVCOL_FOLDER_TYPE);

	HRESULT hr = NULL;
	IShellFolder *psfChild = NULL;
	LPENUMIDLIST ppenum = NULL;
	LPITEMIDLIST pidlItems = NULL;
	WIN32_FIND_DATA fd;
	ULONG uAttr;
	LPITEMIDLIST pidl = ILCreateFromPath(path);

	SHBindToObject(NULL, pidl, NULL, IID_IShellFolder, (void**)&psfChild);
	if (psfChild == NULL)
		return;
	hr = psfChild->EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_NONFOLDERS, &ppenum);
	if (ppenum == NULL)
		return;
	int count = 0;
	for (int i = 0; i < wcslen(path); ++i)
	{
		if (path[i] == '\\')
			count++;
	}

	LV_ITEM lv;
	lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lv.iItem = 0;
	lv.iImage = folderIconIndex;
	lv.iSubItem = 0;
	lv.pszText = L"..";
	if (count >= 1) {
		if (wcslen(path) == 3)
			lv.lParam = (LPARAM)L"This PC";
		else {
			TCHAR* buffer = new TCHAR[10240];
			StrCpy(buffer, path);
 			int length = wcslen(buffer);

			int i;
			for (i = length - 1; i >= 0; i--) {
				if (buffer[i] == '\\')
					break;
			}
			buffer[i] = 0;
			if (wcslen(buffer) == 2)
				StrCat(buffer, L"\\");

			lv.lParam = (LPARAM)buffer;
		}
	}
	ListView_InsertItem(h_ListView, &lv);

	do
	{
		hr = ppenum->Next(1, &pidlItems, NULL);
		if (hr == S_OK) {
			WCHAR buffer[255];
			STRRET strret;
			psfChild->GetDisplayNameOf(pidlItems, SHGDN_NORMAL, &strret);
			SHGetDataFromIDList(psfChild, pidlItems, SHGDFIL_FINDDATA, &fd, sizeof(WIN32_FIND_DATA));
			StrRetToBuf(&strret, pidlItems, buffer, 255);
			lv.pszText = buffer;
			TCHAR *pszItemPath = new TCHAR[255];

			StrCpy(pszItemPath, path);
			if (count >= 1 && wcslen(path) != 3)
				StrCat(pszItemPath, L"\\");

			StrCat(pszItemPath, buffer);
			lv.lParam = (LPARAM)pszItemPath;
			lv.iItem++;
			lv.iSubItem = 0;

			uAttr = SFGAO_FOLDER;
			psfChild->GetAttributesOf(1, (LPCITEMIDLIST *)&pidlItems, &uAttr);

			if (uAttr & SFGAO_FOLDER)
				lv.iImage = folderIconIndex;
			else
				lv.iImage = fileIconIndex;

			ListView_InsertItem(h_ListView, &lv);
			
			if (uAttr & SFGAO_FOLDER) {
				lv.iImage = folderIconIndex;
				ListView_SetItemText(h_ListView, lv.iItem, 2, _T("File folder"));
			}
			else {
				ListView_SetItemText(h_ListView, lv.iItem, 3, CDriveSize::convertByteToStringSize(fd.nFileSizeLow));
				ListView_SetItemText(h_ListView, lv.iItem, 2, _GetType(fd));
			}
			
			ListView_SetItemText(h_ListView, lv.iItem, 1, convertTimeStampToString(fd.ftLastWriteTime));
		}
	} while (hr == S_OK);

	psfChild->Release();
}

void loadOrExecSelected(HWND h_ListView)
{
	LPCWSTR filePath = getPath(h_ListView, ListView_GetSelectionMark(h_ListView));

	if (filePath == NULL)
		return;

	if (StrCmp(filePath, L"This PC") == 0) {
		ListView_DeleteAllItems(h_ListView);
		loadMyComputerToListView(g_Drive, h_ListView);
		return;
	}
	else if (StrCmp(filePath, L"Desktop") == 0) {
		ListView_DeleteAllItems(h_ListView);
		loadDesktopToListView(h_ListView);
		return;
	}
	else {

		WIN32_FIND_DATA fd;

		//Retrieves attributes for a specified file or directory.
		if (GetFileAttributesEx(filePath, GetFileExInfoStandard, &fd) != 0)
		{
			//Check whether it's folder or directory
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				//Delete and reload item in Listview
				ListView_DeleteAllItems(h_ListView);
				loadDirItemToLisview(g_hWnd, h_ListView, filePath);
			}
			else
			{
				//If it's file -> run it
				//ShellExecute is a function to Open specified file or folder with lpOperation _T("open")
				//specify "SW_SHOWNORMAL" flag for displaying the window for the first time
				ShellExecute(NULL, _T("open"), filePath, NULL, NULL, SW_SHOWNORMAL);
			}
		}
	}
}

LPWSTR convertTimeStampToString(const FILETIME &ftLastWrite)
{
	TCHAR *buffer = new TCHAR[50];

	//The SYSTEMTIME structure represents a date and time using individual members 
	//for the month, day, year, weekday, hour, minute, second, and millisecond. 
	SYSTEMTIME st;

	char szLocalDate[255], szLocalTime[255];

	//The FILETIME Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
	//To display the FILETIME in a meaningful way, you first need to convert it to a SYSTEMTIME
	FileTimeToSystemTime(&ftLastWrite, &st);
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_AUTOLAYOUT, &st, NULL,
		(LPWSTR)szLocalDate, 255);
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, (LPWSTR)szLocalTime, 255);

	//Concat to string
	wsprintf(buffer, L"%s %s", szLocalDate, szLocalTime);

	return buffer;
}

LPWSTR _GetType(const WIN32_FIND_DATA &fd)
{
	int nDotPos = StrRStrI(fd.cFileName, NULL, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);

	if (nDotPos < 0 || nDotPos >= len) //If not found
		return _T("Unknown");

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //The character ends the string

	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		return _T("Web page");
	}
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Search for the corresponding extension handle in the registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}
	RegCloseKey(hKey);

	//Describe the information of the extension through the handle of the corresponding key in the registry
	TCHAR pszPath[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unknown");
	}

	delete[] szExtension;
	RegCloseKey(hKey);

	return pszPath;
}

void loadStatusBar(HWND h_ListView)
{
	LPCWSTR filePath = getPath(h_ListView, ListView_GetSelectionMark(h_ListView));

	if (filePath == NULL)
		return;

	WIN32_FIND_DATA fd;

	if (GetFileAttributesEx(filePath, GetFileExInfoStandard, &fd) != 0) {
		if (((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN)) {
			LPWSTR buffer = _GetType(fd);
			DWORD fileSizeLow = fd.nFileSizeLow; //The low-order DWORD value of the file size, in bytes
			SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM)CDriveSize::convertByteToStringSize(fileSizeLow));
		}
		else
			SendMessage(hStatusBar, SB_SETTEXT, 1, (LPARAM) "");
	}
}

void loadDesktopToListView(HWND h_ListView)
{
	LPSHELLFOLDER psfDesktop = NULL;
	SHGetDesktopFolder(&psfDesktop);

	LPENUMIDLIST penumIDL = NULL;
	psfDesktop->EnumObjects(NULL, SHCONTF_FOLDERS, &penumIDL);

	LPITEMIDLIST pidl = NULL;
	HRESULT hr = NULL;
	LV_ITEM lv;
	lv.iItem = -1;
	WCHAR path[10240];

	do {
		hr = penumIDL->Next(1, &pidl, NULL);
		SHGetPathFromIDList(pidl, path);
		if (hr == S_OK) {
			WCHAR buffer[1024];
			STRRET strret;
			psfDesktop->GetDisplayNameOf(pidl, SHGDN_NORMAL, &strret);
			StrRetToBuf(&strret, pidl, buffer, 1024);

			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iImage = folderIconIndex;
			lv.iItem++;
			lv.iSubItem = 0;
			lv.pszText = buffer;

			if (StrCmp(buffer, L"This PC") == 0) {
				lv.iImage = myComputerIconIndex;
				lv.lParam = (LPARAM)L"This PC";
			}
			else
				lv.lParam = (LPARAM)path;
			ListView_InsertItem(h_ListView, &lv);
		}
	} while (hr == S_OK);

	psfDesktop->Release();
}