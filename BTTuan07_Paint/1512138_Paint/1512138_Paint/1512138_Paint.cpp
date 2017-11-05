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
BOOL OpenFile();
BOOL SaveFile();
BOOL SaveObject(LPCWSTR szFileName);
BOOL OpenObject(LPCWSTR szFileName);
BOOL OpenBitmapImage(LPCWSTR szFileName, HDC hWinDC);
BOOL SaveBitmapImage(LPCWSTR szFileName, HDC hWinDC);

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



class Point {
public:
	int x, y;

	Point() {
		x = y = 0;
	}
	Point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

class CShape {
public:
	virtual short GetType() = 0;
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(Point A, Point B) = 0;
	virtual void GetData(Point &A, Point &B) = 0;
};

class CLine : public CShape {
public:
	Point A, B;
public:
	short GetType() {
		return 0;
	}
	void Draw(HDC hdc) {
		MoveToEx(hdc, A.x, A.y, NULL);
		LineTo(hdc, B.x, B.y);
	}

	CShape* Create() { return new CLine; }

	void SetData(Point A, Point B) {
		this->A.x = A.x;
		this->A.y = A.y;
		this->B.x = B.x;
		this->B.y = B.y;
	}

	void GetData(Point &A, Point &B) {
		A.x = this->A.x;
		A.y = this->A.y;
		B.x = this->B.x;
		B.y = this->B.y;
	}
};

class CRectangle : public CShape {
public:
	Point A, B;
public:
	short GetType() {
		return 1;
	}
	void Draw(HDC hdc) {
		Rectangle(hdc, A.x, A.y, B.x, B.y);
	}

	CShape* Create() { return new CRectangle; }

	void SetData(Point A, Point B) {
		this->A.x = A.x;
		this->A.y = A.y;
		this->B.x = B.x;
		this->B.y = B.y;
	}

	void GetData(Point &A, Point &B) {
		A.x = this->A.x;
		A.y = this->A.y;
		B.x = this->B.x;
		B.y = this->B.y;
	}
};

class CSquare : public CShape {
public:
	Point A, B;
public:
	short GetType() {
		return 2;
	}
	void Draw(HDC hdc) {
		if (B.x < A.x && B.y < A.y) {
			if (A.x - B.x < A.y - B.y)
				Rectangle(hdc, A.x, A.y, B.x, A.y - (A.x - B.x));
			else
				Rectangle(hdc, A.x, A.y, A.x - (A.y - B.y), B.y);
		}
		if (B.x < A.x && B.y > A.y) {
			if (B.y - A.y < A.x - B.x)
				Rectangle(hdc, A.x, A.y, A.x - (B.y - A.y), B.y);
			else
				Rectangle(hdc, A.x, A.y, B.x, A.y - (B.x - A.x));
		}
		if (B.x > A.x && B.y < A.y) {
			if (B.x - A.x < A.y - B.y)
				Rectangle(hdc, A.x, A.y, B.x, A.y - (B.x - A.x));
			else
				Rectangle(hdc, A.x, A.y, A.x - (B.y - A.y), B.y);
		}
		if (B.x > A.x && B.y > A.y) {
			if (B.y - A.y < B.x - A.x)
				Rectangle(hdc, A.x, A.y, A.x + (B.y - A.y), B.y);
			else
				Rectangle(hdc, A.x, A.y, B.x, A.y + (B.x - A.x));
		}
	}

	CShape* Create() { return new CSquare; }

	void SetData(Point A, Point B) {
		this->A.x = A.x;
		this->A.y = A.y;
		this->B.x = B.x;
		this->B.y = B.y;
	}

	void GetData(Point &A, Point &B) {
		A.x = this->A.x;
		A.y = this->A.y;
		B.x = this->B.x;
		B.y = this->B.y;
	}
};

class CEllipse : public CShape {
public:
	Point A, B;
public:
	short GetType() {
		return 3;
	}
	void Draw(HDC hdc) {
		Ellipse(hdc, A.x, A.y, B.x, B.y);
	}

	CShape* Create() { return new CEllipse; }

	void SetData(Point A, Point B) {
		this->A.x = A.x;
		this->A.y = A.y;
		this->B.x = B.x;
		this->B.y = B.y;
	}

	void GetData(Point &A, Point &B) {
		A.x = this->A.x;
		A.y = this->A.y;
		B.x = this->B.x;
		B.y = this->B.y;
	}
};

class CCircle : public CShape {
public:
	Point A, B;
public:
	short GetType() {
		return 4;
	}
	void Draw(HDC hdc) {
		if (B.x < A.x && B.y < A.y) {
			if (A.x - B.x < A.y - B.y)
				Ellipse(hdc, A.x, A.y, B.x, A.y - (A.x - B.x));
			else
				Ellipse(hdc, A.x, A.y, A.x - (A.y - B.y), B.y);
		}
		if (B.x < A.x && B.y > A.y) {
			if (B.y - A.y < A.x - B.x)
				Ellipse(hdc, A.x, A.y, A.x - (B.y - A.y), B.y);
			else
				Ellipse(hdc, A.x, A.y, B.x, A.y - (B.x - A.x));
		}
		if (B.x > A.x && B.y < A.y) {
			if (B.x - A.x < A.y - B.y)
				Ellipse(hdc, A.x, A.y, B.x, A.y - (B.x - A.x));
			else
				Ellipse(hdc, A.x, A.y, A.x - (B.y - A.y), B.y);
		}
		if (B.x > A.x && B.y > A.y) {
			if (B.y - A.y < B.x - A.x)
				Ellipse(hdc, A.x, A.y, A.x + (B.y - A.y), B.y);
			else
				Ellipse(hdc, A.x, A.y, B.x, A.y + (B.x - A.x));
		}
	}

	CShape* Create() { return new CCircle; }

	void SetData(Point A, Point B) {
		this->A.x = A.x;
		this->A.y = A.y;
		this->B.x = B.x;
		this->B.y = B.y;
	}

	void GetData(Point &A, Point &B) {
		A.x = this->A.x;
		A.y = this->A.y;
		B.x = this->B.x;
		B.y = this->B.y;
	}
};

vector<CShape*> shapes;
vector<CShape*> prototypes;

bool isDrawing = FALSE;
bool isOpening = FALSE;
bool isSaving = FALSE;
Point current;
Point last;

int checkLine;
int checkRectangle;
int checkEllipse;
int keyShift;

WCHAR FileOpenPath[255];
WCHAR FileSavePath[255];

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x;
	int y;

    switch (message)
    {
	case WM_CREATE:
		{
			prototypes.push_back(new CLine);
			prototypes.push_back(new CRectangle);
			prototypes.push_back(new CEllipse);

			HMENU hMenu = GetMenu(hWnd);
			CheckMenuItem(hMenu, ID_DRAW_LINE, MF_CHECKED);
		}
		break;
	case WM_LBUTTONDOWN:
		{
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);

			if (!isDrawing) {
				isDrawing = TRUE;
				current.x = x;
				current.y = y;
			}

			HMENU hMenu = GetMenu(hWnd);
			checkLine = GetMenuState(hMenu, ID_DRAW_LINE, MF_BYCOMMAND);
			checkRectangle = GetMenuState(hMenu, ID_DRAW_RECTANGLE, MF_BYCOMMAND);
			checkEllipse = GetMenuState(hMenu, ID_DRAW_ELLIPSE, MF_BYCOMMAND);
		}
		break;
	case WM_MOUSEMOVE:
		{
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);

			last.x = x;
			last.y = y;
			
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP: 
		{
			Point point;
			point.x = GET_X_LPARAM(lParam);
			point.y = GET_Y_LPARAM(lParam);

			if (checkLine) {
				CLine* line = new CLine;
				line->SetData(current, point);
				shapes.push_back(line);
			}
			if (checkRectangle) {
				if (keyShift) {
					CSquare* square = new CSquare;
					square->SetData(current, point);
					shapes.push_back(square);
				}
				else {
					CRectangle* rectangle = new CRectangle;
					rectangle->SetData(current, point);
					shapes.push_back(rectangle);
				}
			}
			if (checkEllipse) {
				if (keyShift) {
					CCircle* circle = new CCircle;
					circle->SetData(current, point);
					shapes.push_back(circle);
				}
				else {
					CEllipse* ellipse = new CEllipse;
					ellipse->SetData(current, point);
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
			case ID_FILE_SAVEOBJECT:
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
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			RECT win;
			GetClientRect(hWnd, &win);
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmMem = CreateCompatibleBitmap(hdc, win.right, win.bottom);

			HANDLE hOld = SelectObject(hdcMem, hbmMem);
			FillRect(hdcMem, &win, NULL);

			if (isOpening)
				OpenBitmapImage(FileOpenPath, hdcMem);

			SelectObject(hdcMem, GetStockBrush(NULL_BRUSH));
			// Draw into hdcMem here

			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->Draw(hdcMem);
			}

			if (isSaving) {
				SaveBitmapImage(FileSavePath, hdcMem);
				isSaving = FALSE;
			}

			if (checkLine && isDrawing) {
				MoveToEx(hdcMem, current.x, current.y, NULL);
				LineTo(hdcMem, last.x, last.y);
			}

			if (checkRectangle && isDrawing) {
				if (keyShift) {
					CSquare* square = new CSquare;
					square->SetData(current, last);
					square->Draw(hdcMem);
					delete square;
				}
				else
					Rectangle(hdcMem, current.x, current.y, last.x, last.y);
			}

			if (checkEllipse && isDrawing) {
				if (keyShift) {
					CCircle* circle = new CCircle;
					circle->SetData(current, last);
					circle->Draw(hdcMem);
					delete circle;
				}
				else
					Ellipse(hdcMem, current.x, current.y, last.x, last.y);
			}

			// Transfer the off-screen DC to the screen
			BitBlt(hdc, 0, 0, win.right, win.bottom, hdcMem, 0, 0, SRCCOPY);

			// Free-up the off-screen DC
			SelectObject(hdcMem, hOld);
			DeleteObject(hbmMem);
			DeleteDC(hdcMem);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
	{
		
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

#define OPENFILENAMEW OPENFILENAME

BOOL OpenFile()
{
	OPENFILENAME ofn;			// common dialog box structure
	WCHAR szFile[260];			// buffer for file name
	HWND hwnd = NULL;			// owner window
	HANDLE hf;					// file handle

								// Initialize OPENFILENAME

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not use the contents of szFile to initialize itself.

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn) == FALSE)
		return FALSE;

	StrCpy(FileOpenPath, ofn.lpstrFile);
}

BOOL SaveFile()
{
	OPENFILENAME ofn;			// common dialog box structure
	WCHAR szFile[260];			// buffer for file name
	HWND hwnd = NULL;			// owner window
	HANDLE hf;					// file handle

								// Initialize OPENFILENAME

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;

	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not use the contents of szFile to initialize itself.

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All\0*.*\0Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == FALSE)
		return FALSE;

	StrCpy(FileSavePath, ofn.lpstrFile);
}

BOOL SaveObject(LPCWSTR szFileName)
{
	fstream fOut;
	fOut.open(szFileName, ios::out | ios::binary);

	for (int i = 0; i < shapes.size(); i++)
	{
		Point A, B;
		shapes[i]->GetData(A, B);
		short type = shapes[i]->GetType();

		fOut.write((char *)&type, sizeof(short));
		fOut.write((char *)&A.x, sizeof(int));
		fOut.write((char *)&A.y, sizeof(int));
		fOut.write((char *)&B.x, sizeof(int));
		fOut.write((char *)&B.y, sizeof(int));
	}

	fOut.close();
	return TRUE;
}

BOOL OpenObject(LPCWSTR szFileName)
{
	ifstream fIn(szFileName, ios_base::in | ios_base::binary);
	shapes.clear();

	while (!fIn.eof()) {
		short type;
		fIn.read((char *)(&type), sizeof(short));

		if (fIn.eof())
			break;

		Point A, B;
		fIn.read((char *)&A.x, sizeof(int));
		fIn.read((char *)&A.y, sizeof(int));
		fIn.read((char *)&B.x, sizeof(int));
		fIn.read((char *)&B.y, sizeof(int));

		switch (type)
		{
		case 0: {
			CLine* line = new CLine;
			line->SetData(A, B);
			shapes.push_back(line);
		}
			break;
		case 1: {
			CRectangle* rectangle = new CRectangle;
			rectangle->SetData(A, B);
			shapes.push_back(rectangle);
		}
			break;
		case 2: {
			CSquare* square = new CSquare;
			square->SetData(A, B);
			shapes.push_back(square);
		}
			break;
		case 3: {
			CEllipse* ellipse = new CEllipse;
			ellipse->SetData(A, B);
			shapes.push_back(ellipse);
		}
			break;
		case 4: {
			CCircle* circle = new CCircle;
			circle->SetData(A, B);
			shapes.push_back(circle);
		}
			break;
		}
	}

	fIn.close();
	return TRUE;
}

BOOL OpenBitmapImage(LPCWSTR szFileName, HDC hWinDC)
{
	// Load the bitmap image file
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if (hBitmap == NULL)
		return FALSE;

	HDC hLocalDC = CreateCompatibleDC(hWinDC);

	if (hLocalDC == NULL)
		return FALSE;

	BITMAP qBitmap;
	int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
	if (!iReturn)
		return FALSE;

	HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
	if (hOldBmp == NULL)
		return FALSE;

	BOOL qRetBlit = BitBlt(hWinDC, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight, hLocalDC, 0, 0, SRCCOPY);
	if (!qRetBlit)
		return FALSE;

	SelectObject(hLocalDC, hOldBmp);
	DeleteDC(hLocalDC);
	DeleteObject(hBitmap);
	return TRUE;
}

BOOL SaveBitmapImage(LPCWSTR szFileName, HDC hWinDC) {
	int dpi = 72;
	int thisone, width, height;

	if (isOpening) {
		HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, FileOpenPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		if (hBitmap == NULL)
			return FALSE;

		BITMAP qBitmap;
		int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP), reinterpret_cast<LPVOID>(&qBitmap));
		if (!iReturn)
			return FALSE;

		width = qBitmap.bmWidth;
		height = qBitmap.bmHeight;
	}
	else {
		width = 1280;
		height = 720;
	}

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