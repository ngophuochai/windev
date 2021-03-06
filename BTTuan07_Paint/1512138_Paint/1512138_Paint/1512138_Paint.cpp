// 1512138_Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Shapes.h"
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
BOOL SaveBitmapImage(LPCWSTR szFileName, HDC hWinDC, int width, int height);

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

struct Shape
{
	int isShape;
	Pen *pen;
	Point cur;
	Point las;
};

vector<Shape*> shapes;
HWND h_Paint;

bool isDrawing = FALSE;
bool isOpening = FALSE;
bool isSaving = FALSE;
Point current;
Point last;
PROPVARIANT check;
int isShape;
int penSize;
Color penColor;
int keyShift;

WCHAR FileOpenPath[255];
WCHAR FileSavePath[255];

GdiplusStartupInput gdiplusStartupInput;
ULONG_PTR           gdiplusToken;

typedef void (__cdecl *MYPROC)(Graphics*, Pen*, Point, Point, int);
typedef BOOL (__cdecl *OPENPATH)(WCHAR*);
typedef BOOL (__cdecl *SAVEPATH)(WCHAR*);

MYPROC DrawShape;
OPENPATH OpenPath;
SAVEPATH SavePath;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x;
	int y;
	bool initSuccess;

    switch (message)
    {
	case WM_CREATE:
		{
			h_Paint = CreateWindow(L"STATIC", NULL, WS_CHILD | WS_VISIBLE
				, 5, 150, 1252, 550, hWnd, NULL, hInst, NULL);

			initSuccess = InitializeFramework(hWnd);
			if (!initSuccess)
				return -1;

			GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

			g_pFramework->GetUICommandProperty(ID_BTN_LINE, UI_PKEY_BooleanValue, &check);
			check.boolVal = -1;
			g_pFramework->SetUICommandProperty(ID_BTN_LINE, UI_PKEY_BooleanValue, check);
			g_pFramework->SetUICommandProperty(ID_SIZE_ITEM1, UI_PKEY_BooleanValue, check);
			g_pFramework->SetUICommandProperty(ID_COLOR_BLACK, UI_PKEY_BooleanValue, check);

			isShape = LINE;
			penSize = 1;
			penColor = BLACK;

			HMODULE hinstLib;
			hinstLib = LoadLibrary(L"Dll1.dll");
			if (hinstLib != NULL) {
				DrawShape = (MYPROC)GetProcAddress(hinstLib, "DrawShape");
				OpenPath = (OPENPATH)GetProcAddress(hinstLib, "OpenPath");
				SavePath = (SAVEPATH)GetProcAddress(hinstLib, "SavePath");

				if (DrawShape == NULL || OpenPath == NULL || SavePath == NULL)
					DestroyWindow(hWnd);
			}
			else
				DestroyWindow(hWnd);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			if (!isDrawing) {
				isDrawing = TRUE;
				current.X = GET_X_LPARAM(lParam) - 5;
				current.Y = GET_Y_LPARAM(lParam) - 150;
			}

			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM1, UI_PKEY_BooleanValue, &check);
			penSize = (check.boolVal == -1) ? 1 : penSize;
			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM2, UI_PKEY_BooleanValue, &check);
			penSize = (check.boolVal == -1) ? 3 : penSize;
			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM3, UI_PKEY_BooleanValue, &check);
			penSize = (check.boolVal == -1) ? 5 : penSize;
			g_pFramework->GetUICommandProperty(ID_SIZE_ITEM4, UI_PKEY_BooleanValue, &check);
			penSize = (check.boolVal == -1) ? 7 : penSize;

			g_pFramework->GetUICommandProperty(ID_BTN_LINE, UI_PKEY_BooleanValue, &check);
			isShape = (check.boolVal == -1) ? LINE : isShape;
			g_pFramework->GetUICommandProperty(ID_BTN_RECTANGLE, UI_PKEY_BooleanValue, &check);
			isShape = (check.boolVal == -1) ? RECTANGLE : isShape;
			isShape = (check.boolVal == -1 && keyShift) ? SQUARE : isShape;
			g_pFramework->GetUICommandProperty(ID_BTN_ELLIPSE, UI_PKEY_BooleanValue, &check);
			isShape = (check.boolVal == -1) ? ELLIPSE : isShape;
			isShape = (check.boolVal == -1 && keyShift) ? CIRCLE : isShape;

			g_pFramework->GetUICommandProperty(ID_COLOR_BLACK, UI_PKEY_BooleanValue, &check);
			penColor = (check.boolVal == -1) ? BLACK : penColor;
			g_pFramework->GetUICommandProperty(ID_COLOR_RED, UI_PKEY_BooleanValue, &check);
			penColor = (check.boolVal == -1) ? RED : penColor;
			g_pFramework->GetUICommandProperty(ID_COLOR_GREEN, UI_PKEY_BooleanValue, &check);
			penColor = (check.boolVal == -1) ? GREEN : penColor;
			g_pFramework->GetUICommandProperty(ID_COLOR_BLUE, UI_PKEY_BooleanValue, &check);
			penColor = (check.boolVal == -1) ? BLUE : penColor;
			g_pFramework->GetUICommandProperty(ID_COLOR_YELLOW, UI_PKEY_BooleanValue, &check);
			penColor = (check.boolVal == -1) ? YELLOW : penColor;
			g_pFramework->GetUICommandProperty(ID_COLOR_MAGENTA, UI_PKEY_BooleanValue, &check);
			penColor = (check.boolVal == -1) ? MAGENTA : penColor;
			g_pFramework->GetUICommandProperty(ID_COLOR_CYAN, UI_PKEY_BooleanValue, &check);
			penColor = (check.boolVal == -1) ? CYAN : penColor;
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

			Shape *shape = new Shape;
			shape->cur = current;
			shape->las = last;
			shape->pen = new Pen(penColor, penSize);
			
			switch (isShape)
			{
			case LINE: shape->isShape = LINE; break;
			case RECTANGLE: shape->isShape = RECTANGLE; break;
			case SQUARE: shape->isShape = SQUARE; break;
			case ELLIPSE: shape->isShape = ELLIPSE; break;
			case CIRCLE: shape->isShape = CIRCLE; break;
			}

			shapes.push_back(shape);
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
			{
				isOpening = FALSE;
				shapes.clear();
				InvalidateRect(hWnd, NULL, TRUE);
			}
				break;
			case ID_BTN_OPEN:
			{
				if (OpenPath(FileOpenPath))
					isOpening = TRUE;
			}
				break;
			case ID_BTN_SAVE:
			{
				if (SavePath(FileSavePath))
					isSaving = TRUE;
			}
			break;
			default:
			{
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
				break;
			}
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			RECT win;
            HDC hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);

            // TODO: Add any drawing code that uses hdc here...
			hdc = BeginPaint(h_Paint, &ps);
			
			GetClientRect(h_Paint, &win);
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmMem = CreateCompatibleBitmap(hdc, win.right, win.bottom);

			HANDLE hOld = SelectObject(hdcMem, hbmMem);
			FillRect(hdcMem, &win, NULL);

			SelectObject(hdcMem, GetStockBrush(NULL_BRUSH));

			// Draw into hdcMem here
			Graphics* graphics = new Graphics(hdcMem);

			Pen* pen = new Pen(penColor, penSize);

			if (isOpening == TRUE) {
				Image myImage(FileOpenPath);
				graphics->DrawImage(&myImage, 0, 0);
			}

			for (int i = 0; i < shapes.size(); i++)
				DrawShape(graphics, shapes[i]->pen, shapes[i]->cur, shapes[i]->las, shapes[i]->isShape);

			if (isDrawing) {
				switch (isShape) {
				case LINE: DrawShape(graphics, pen, current, last, LINE); break;
				case RECTANGLE: DrawShape(graphics, pen, current, last, RECTANGLE); break;
				case SQUARE: DrawShape(graphics, pen, current, last, SQUARE); break;
				case ELLIPSE: DrawShape(graphics, pen, current, last, ELLIPSE); break;
				case CIRCLE: DrawShape(graphics, pen, current, last, CIRCLE); break;
				}
			}

			if (isSaving && isOpening) {
				Image myImage(FileOpenPath);
				SaveBitmapImage(FileSavePath, hdcMem, myImage.GetWidth(), myImage.GetHeight());
				isSaving = FALSE;
			}
			else if (isSaving) {
				SaveBitmapImage(FileSavePath, hdcMem, 1252, 550);
				isSaving = FALSE;
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
		shapes.clear();
		
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

BOOL SaveBitmapImage(LPCWSTR szFileName, HDC hWinDC, int width, int height) {
	int dpi = 72;
	int thisone;

	int n = width * height;
	RGBType *pixels = new RGBType[n];
	COLORREF color;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			thisone = y * width + x;

			color = GetPixel(hWinDC, x, y);

			pixels[thisone].r = GetRValue(color);
			pixels[thisone].g = GetGValue(color);
			pixels[thisone].b = GetBValue(color);
		}
	}

	int k = width * height;
	int s = 4 * k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = { 'B', 'M', 0,0,0,0, 0,0,0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize>>8);
	bmpfileheader[4] = (unsigned char)(filesize>>16);
	bmpfileheader[5] = (unsigned char)(filesize>>24);

	bmpinfoheader[4] = (unsigned char)(width);
	bmpinfoheader[5] = (unsigned char)(width>>8);
	bmpinfoheader[6] = (unsigned char)(width>>16);
	bmpinfoheader[7] = (unsigned char)(width>>24);

	bmpinfoheader[8] = (unsigned char)(height);
	bmpinfoheader[9] = (unsigned char)(height>>8);
	bmpinfoheader[10] = (unsigned char)(height>>16);
	bmpinfoheader[11] = (unsigned char)(height>>24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm>>8);
	bmpinfoheader[31] = (unsigned char)(ppm>>16);
	bmpinfoheader[32] = (unsigned char)(ppm>>24);

	fstream f;
	f.open(szFileName, ios::out | ios::binary);

	f.write((const char*)bmpfileheader, 14);
	f.write((const char*)bmpinfoheader, 40);

	for (int i = 0; i < height; i++) 
	{
		int j = k - width * (i + 1);
		for (; j < (k - width * i); j++) 
		{
			RGBType rgb = pixels[j];

			double red = rgb.r;
			double green = rgb.g;
			double blue = rgb.b;

			unsigned char color[3] = { (int)floor(blue), (int)floor(green), (int)floor(red) };

			f.write((const char*)color, sizeof(color));
		}
	}

	f.close();
	return TRUE;
}