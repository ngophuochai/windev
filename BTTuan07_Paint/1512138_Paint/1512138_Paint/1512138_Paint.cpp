// 1512138_Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512138_Paint.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
struct RGBType {
	double r;
	double g;
	double b;
};
//BOOL OpenFile();
//BOOL SaveFile();
//BOOL SaveObject(LPCWSTR szFileName);
//BOOL OpenObject(LPCWSTR szFileName);
//BOOL OpenBitmapImage(LPCWSTR szFileName, HDC hWinDC);
//BOOL SaveBitmapImage(LPCWSTR szFileName, HDC hWinDC);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	HRESULT hr = CoInitialize(NULL);
	if (FAILED(hr))
		return FALSE;

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY1512138PAINT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY1512138PAINT));

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
	CoUninitialize();

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

	wcex.style			= 0; //CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY1512138PAINT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY1512138PAINT);
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
      0, 0, 1280, 720, nullptr, nullptr, hInstance, nullptr);

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

vector<CShape*> shapes;

HWND h_Paint;

bool isDrawing = FALSE;
bool isOpening = FALSE;
bool isSaving = FALSE;
Point current;
Point last;

PROPVARIANT checkLine;
PROPVARIANT checkRectangle;
PROPVARIANT checkEllipse;
PROPVARIANT checkPenSize1;	// 1
PROPVARIANT checkPenSize2;	// 3
PROPVARIANT checkPenSize3;	// 5
PROPVARIANT checkPenSize4;	// 7

int keyShift;

WCHAR FileOpenPath[255];
WCHAR FileSavePath[255];

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x;
	int y;
	bool initSuccess;


    switch (message)
    {
	case ID_BTN_CLEAR:
		MessageBox(0, 0, 0, 0);
		break;
	case WM_CREATE:
		{
			h_Paint = CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 5, 150, 1000, 550, hWnd, NULL, hInst, NULL);

			initSuccess = InitializeFramework(hWnd);
			if (!initSuccess)
				return -1;

			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

			g_pFramework->GetUICommandProperty(ID_BTN_LINE, UI_PKEY_BooleanValue, &checkLine);
			checkLine.boolVal = -1;
			g_pFramework->SetUICommandProperty(ID_BTN_LINE, UI_PKEY_BooleanValue, checkLine);
			g_pFramework->SetUICommandProperty(ID_SIZE_ITEM1, UI_PKEY_BooleanValue, checkLine);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (!isDrawing) {
				isDrawing = TRUE;
				current.X = GET_X_LPARAM(lParam) - 5;
				current.Y = GET_Y_LPARAM(lParam) - 150;
			}

			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM1, UI_PKEY_BooleanValue, &checkPenSize1);
			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM2, UI_PKEY_BooleanValue, &checkPenSize2);
			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM3, UI_PKEY_BooleanValue, &checkPenSize3);
			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM4, UI_PKEY_BooleanValue, &checkPenSize4);

			g_pFramework->GetUICommandProperty(ID_BTN_LINE, UI_PKEY_BooleanValue, &checkLine);
			g_pFramework->GetUICommandProperty(ID_BTN_RECTANGLE, UI_PKEY_BooleanValue, &checkRectangle);
			g_pFramework->GetUICommandProperty(ID_BTN_ELLIPSE, UI_PKEY_BooleanValue, &checkEllipse);
		}
		break;
	case WM_MOUSEMOVE:
		{
			last.X = GET_X_LPARAM(lParam) - 5;
			last.Y = GET_Y_LPARAM(lParam) - 150;
			
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP: 
		{
			Point point;
			point.X = GET_X_LPARAM(lParam) - 5;
			point.Y = GET_Y_LPARAM(lParam) - 150;

			int lineWidth = 3;

			if (checkPenSize1.boolVal == -1)
				lineWidth = 1;
			if (checkPenSize2.boolVal == -1)
				lineWidth = 3;
			if (checkPenSize3.boolVal == -1)
				lineWidth = 5;
			if (checkPenSize4.boolVal == -1)
				lineWidth = 7;

			Pen* pen = new Pen(Color(255, 0, 0, 0), lineWidth);

			if (checkLine.boolVal) {
				CLine* line = new CLine;
				line->SetData(current, point, pen);
				shapes.push_back(line);
			}
			if (checkRectangle.boolVal) {
				if (keyShift) {
					CSquare* square = new CSquare;
					square->SetData(current, point, pen);
					shapes.push_back(square);
				}
				else {
					CRectangle* rectangle = new CRectangle;
					rectangle->SetData(current, point, pen);
					shapes.push_back(rectangle);
				}
			}
			if (checkEllipse.boolVal) {
				if (keyShift) {
					CCircle* circle = new CCircle;
					circle->SetData(current, point, pen);
					shapes.push_back(circle);
				}
				else {
					CEllipse* ellipse = new CEllipse;
					ellipse->SetData(current, point, pen);
					shapes.push_back(ellipse);
				}
			}

			isDrawing = FALSE;
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_KEYDOWN:
		{
			keyShift = (GetKeyState(VK_SHIFT) & 0x8000);
		}
		break;
	case WM_KEYUP:
		{
			keyShift = 0;
		}
		break;
	case WM_ERASEBKGND:
		return 1;
    case WM_COMMAND:
		{
			int wmId = LOWORD(wParam);
			
			HMENU hMenu = GetMenu(hWnd);
			// Parse the menu selections:
			switch (wmId)
			{
			case ID_BTN_CLEAR:
				MessageBox(0, 0, 0, 0);
				break;
				/*case ID_FILE_SAVEOBJECT:
					if (!SaveFile())
						break;
					SaveObject(FileSavePath);
					break;
				case ID_FILE_OPENOBJECT:
					if (!OpenFile())
						break;
					OpenObject(FileOpenPath);
					break;
				case ID_FILE_OPENIMAGE:
					if (OpenFile())
						isOpening = TRUE;
					else
						isOpening = FALSE;
					break;
				case ID_FILE_SAVEIMAGE:
					if (SaveFile())
						isSaving = TRUE;
					else
						isSaving = FALSE;
					break;
				case ID_FILE_CLEAR:
					shapes.clear();
					isOpening = FALSE;
					break;
				case ID_DRAW_LINE:
					CheckMenuItem(hMenu, ID_DRAW_LINE, MF_CHECKED);

					CheckMenuItem(hMenu, ID_DRAW_RECTANGLE, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_DRAW_ELLIPSE, MF_UNCHECKED);
					break;
				case ID_DRAW_RECTANGLE:
					CheckMenuItem(hMenu, ID_DRAW_RECTANGLE, MF_CHECKED);

					CheckMenuItem(hMenu, ID_DRAW_LINE, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_DRAW_ELLIPSE, MF_UNCHECKED);
					break;
				case ID_DRAW_ELLIPSE:
					CheckMenuItem(hMenu, ID_DRAW_ELLIPSE, MF_CHECKED);

					CheckMenuItem(hMenu, ID_DRAW_LINE, MF_UNCHECKED);
					CheckMenuItem(hMenu, ID_DRAW_RECTANGLE, MF_UNCHECKED);
					break;
				case IDM_ABOUT:
					DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
					break;
				case IDM_EXIT:
					DestroyWindow(hWnd);
					break;
				default:
					return DefWindowProc(hWnd, message, wParam, lParam);
				}*/
			}
			break;
		}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);

            // TODO: Add any drawing code that uses hdc here...
			hdc = BeginPaint(h_Paint, &ps);
			RECT win;
			GetClientRect(h_Paint, &win);
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmMem = CreateCompatibleBitmap(hdc, win.right, win.bottom);

			HANDLE hOld = SelectObject(hdcMem, hbmMem);
			FillRect(hdcMem, &win, NULL);

			/*if (isOpening)
				OpenBitmapImage(FileOpenPath, hdcMem);*/

			SelectObject(hdcMem, GetStockBrush(NULL_BRUSH));

			// Draw into hdcMem here
			Graphics* graphics = new Graphics(hdcMem);
			int lineWidth = 3;

			if (checkPenSize1.boolVal == -1)
				lineWidth = 1;
			if (checkPenSize2.boolVal == -1)
				lineWidth = 3;
			if (checkPenSize3.boolVal == -1)
				lineWidth = 5;
			if (checkPenSize4.boolVal == -1)
				lineWidth = 7;

			Pen* pen = new Pen(Color(255, 0, 0, 0), lineWidth);

			for (int i = 0; i < shapes.size(); i++)
				shapes[i]->Draw(graphics);

			/*if (isSaving) {
				SaveBitmapImage(FileSavePath, hdcMem);
				isSaving = FALSE;
			}*/

			if (checkLine.boolVal && isDrawing) {
				CLine line;
				line.SetData(current, last, pen);
				line.Draw(graphics);
			}

			if (checkRectangle.boolVal && isDrawing) {
				if (keyShift) {
					CSquare square;
					square.SetData(current, last, pen);
					square.Draw(graphics);
				}
				else {
					CRectangle rectangle;
					rectangle.SetData(current, last, pen);
					rectangle.Draw(graphics);
				}
			}

			if (checkEllipse.boolVal && isDrawing) {
				if (keyShift) {
					CCircle circle;
					circle.SetData(current, last, pen);
					circle.Draw(graphics);
				}
				else {
					CEllipse ellipse;
					ellipse.SetData(current, last, pen);
					ellipse.Draw(graphics);
				}
			}

			delete pen;
			delete graphics;

			// Transfer the off-screen DC to the screen
			BitBlt(hdc, 0, 0, win.right, win.bottom, hdcMem, 0, 0, SRCCOPY);

			// Free-up the off-screen DC
			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC(hdcMem);

			EndPaint(h_Paint, &ps);
        }
        break;
    case WM_DESTROY:
	{
		DestroyFramework();
		GdiplusShutdown(gdiplusToken);
		
		for (int i = 0; i < shapes.size(); i++) {
			delete shapes[i]->pen;
			delete shapes[i];
		}
		
		PostQuitMessage(0);
		break;
	}
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
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
//
//#define OPENFILENAMEW OPENFILENAME
//
//BOOL OpenFile()
//{
//	OPENFILENAME ofn;			// common dialog box structure
//	WCHAR szFile[260];			// buffer for file name
//	HWND hwnd = NULL;			// owner window
//	HANDLE hf;					// file handle
//
//								// Initialize OPENFILENAME
//
//	ZeroMemory(&ofn, sizeof(ofn));
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = hwnd;
//	ofn.lpstrFile = szFile;
//
//	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not use the contents of szFile to initialize itself.
//
//	ofn.lpstrFile[0] = '\0';
//	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = NULL;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//	// Display the Open dialog box. 
//
//	if (GetOpenFileName(&ofn) == FALSE)
//		return FALSE;
//
//	StrCpy(FileOpenPath, ofn.lpstrFile);
//}
//
//BOOL SaveFile()
//{
//	OPENFILENAME ofn;			// common dialog box structure
//	WCHAR szFile[260];			// buffer for file name
//	HWND hwnd = NULL;			// owner window
//	HANDLE hf;					// file handle
//
//								// Initialize OPENFILENAME
//
//	ZeroMemory(&ofn, sizeof(ofn));
//	ofn.lStructSize = sizeof(ofn);
//	ofn.hwndOwner = hwnd;
//	ofn.lpstrFile = szFile;
//
//	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not use the contents of szFile to initialize itself.
//
//	ofn.lpstrFile[0] = '\0';
//	ofn.nMaxFile = sizeof(szFile);
//	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
//	ofn.nFilterIndex = 1;
//	ofn.lpstrFileTitle = NULL;
//	ofn.nMaxFileTitle = 0;
//	ofn.lpstrInitialDir = NULL;
//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
//
//	if (GetSaveFileName(&ofn) == FALSE)
//		return FALSE;
//
//	StrCpy(FileSavePath, ofn.lpstrFile);
//}
//
//BOOL SaveObject(LPCWSTR szFileName)
//{
//	fstream fOut;
//	fOut.open(szFileName, ios::out | ios::binary);
//
//	for (int i = 0; i < shapes.size(); i++)
//	{
//		Point A, B;
//		shapes[i]->GetData(A, B);
//		short type = shapes[i]->GetType();
//
//		fOut.write((char *)&type, sizeof(short));
//		fOut.write((char *)&A.x, sizeof(int));
//		fOut.write((char *)&A.y, sizeof(int));
//		fOut.write((char *)&B.x, sizeof(int));
//		fOut.write((char *)&B.y, sizeof(int));
//	}
//
//	fOut.close();
//	return TRUE;
//}
//
//BOOL OpenObject(LPCWSTR szFileName)
//{
//	ifstream fIn(szFileName, ios_base::in | ios_base::binary);
//	shapes.clear();
//
//	while (!fIn.eof()) {
//		short type;
//		fIn.read((char *)(&type), sizeof(short));
//
//		if (fIn.eof())
//			break;
//
//		Point A, B;
//		fIn.read((char *)&A.x, sizeof(int));
//		fIn.read((char *)&A.y, sizeof(int));
//		fIn.read((char *)&B.x, sizeof(int));
//		fIn.read((char *)&B.y, sizeof(int));
//
//		switch (type)
//		{
//		case 0: {
//			CLine* line = new CLine;
//			line->SetData(A, B);
//			shapes.push_back(line);
//		}
//			break;
//		case 1: {
//			CRectangle* rectangle = new CRectangle;
//			rectangle->SetData(A, B);
//			shapes.push_back(rectangle);
//		}
//			break;
//		case 2: {
//			CSquare* square = new CSquare;
//			square->SetData(A, B);
//			shapes.push_back(square);
//		}
//			break;
//		case 3: {
//			CEllipse* ellipse = new CEllipse;
//			ellipse->SetData(A, B);
//			shapes.push_back(ellipse);
//		}
//			break;
//		case 4: {
//			CCircle* circle = new CCircle;
//			circle->SetData(A, B);
//			shapes.push_back(circle);
//		}
//			break;
//		}
//	}
//
//	fIn.close();
//	return TRUE;
//}
//
//BOOL OpenBitmapImage(LPCWSTR szFileName, HDC hWinDC)
//{
//	// Load the bitmap image file
//	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//
//	if (hBitmap == NULL)
//		return FALSE;
//
//	HDC hLocalDC = CreateCompatibleDC(hWinDC);
//
//	if (hLocalDC == NULL)
//		return FALSE;
//
//	BITMAP qBitmap;
//	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
//	if (!iReturn)
//		return FALSE;
//
//	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
//	if (hOldBmp == NULL)
//		return FALSE;
//
//	BOOL qRetBlit = BitBlt(hWinDC, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight, hLocalDC, 0, 0, SRCCOPY);
//	if (!qRetBlit)
//		return FALSE;
//
//	SelectObject(hLocalDC, hOldBmp);
//	DeleteDC(hLocalDC);
//	DeleteObject(hBitmap);
//	return TRUE;
//}
//
//BOOL SaveBitmapImage(LPCWSTR szFileName, HDC hWinDC) {
//	int dpi = 72;
//	int thisone, width, height;
//
//	if (isOpening) {
//		HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, FileOpenPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//
//		if (hBitmap == NULL)
//			return FALSE;
//
//		BITMAP qBitmap;
//		int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
//		if (!iReturn)
//			return FALSE;
//
//		width = qBitmap.bmWidth;
//		height = qBitmap.bmHeight;
//	}
//	else {
//		width = 1280;
//		height = 720;
//	}
//
//	int n = width * height;
//	RGBType *pixels = new RGBType[n];
//	COLORREF color;
//
//	for (int x = 0; x < width; x++) {
//		for (int y = 0; y < height; y++) {
//			thisone = y * width + x;
//
//			color = GetPixel(hWinDC, x, y);
//
//			pixels[thisone].r = GetRValue(color);
//			pixels[thisone].g = GetGValue(color);
//			pixels[thisone].b = GetBValue(color);
//		}
//	}
//
//	int k = width * height;
//	int s = 4 * k;
//	int filesize = 54 + s;
//
//	double factor = 39.375;
//	int m = static_cast<int>(factor);
//
//	int ppm = dpi*m;
//
//	unsigned char bmpfileheader[14] = { 'B', 'M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
//	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };
//
//	bmpfileheader[2] = (unsigned char)(filesize);
//	bmpfileheader[3] = (unsigned char)(filesize>>8);
//	bmpfileheader[4] = (unsigned char)(filesize>>16);
//	bmpfileheader[5] = (unsigned char)(filesize>>24);
//
//	bmpinfoheader[4] = (unsigned char)(width);
//	bmpinfoheader[5] = (unsigned char)(width>>8);
//	bmpinfoheader[6] = (unsigned char)(width>>16);
//	bmpinfoheader[7] = (unsigned char)(width>>24);
//
//	bmpinfoheader[8] = (unsigned char)(height);
//	bmpinfoheader[9] = (unsigned char)(height>>8);
//	bmpinfoheader[10] = (unsigned char)(height>>16);
//	bmpinfoheader[11] = (unsigned char)(height>>24);
//
//	bmpinfoheader[21] = (unsigned char)(s);
//	bmpinfoheader[22] = (unsigned char)(s>>8);
//	bmpinfoheader[23] = (unsigned char)(s>>16);
//	bmpinfoheader[24] = (unsigned char)(s>>24);
//
//	bmpinfoheader[25] = (unsigned char)(ppm);
//	bmpinfoheader[26] = (unsigned char)(ppm>>8);
//	bmpinfoheader[27] = (unsigned char)(ppm>>16);
//	bmpinfoheader[28] = (unsigned char)(ppm>>24);
//
//	bmpinfoheader[29] = (unsigned char)(ppm);
//	bmpinfoheader[30] = (unsigned char)(ppm>>8);
//	bmpinfoheader[31] = (unsigned char)(ppm>>16);
//	bmpinfoheader[32] = (unsigned char)(ppm>>24);
//
//	fstream f;
//	f.open(szFileName, ios::out | ios::binary);
//
//	f.write((const char*)bmpfileheader, 14);
//	f.write((const char*)bmpinfoheader, 40);
//
//	for (int i = 0; i < height; i++) 
//	{
//		int j = k - width * (i + 1);
//		for (; j < (k - width * i); j++) 
//		{
//			RGBType rgb = pixels[j];
//
//			double red = rgb.r;
//			double green = rgb.g;
//			double blue = rgb.b;
//
//			unsigned char color[3] = { (int)floor(blue), (int)floor(green), (int)floor(red) };
//
//			f.write((const char*)color, sizeof(color));
//		}
//	}
//
//	f.close();
//	return TRUE;
//}