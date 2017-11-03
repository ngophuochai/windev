// 1512138_Paint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512138_Paint.h"

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

class CShape {
public:
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(int a, int b, int c, int d) = 0;
};

class CLine : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}

	CShape* Create() { return new CLine; }

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

class CRectangle : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		Rectangle(hdc, x1, y1, x2, y2);
	}

	CShape* Create() { return new CRectangle; }

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

class CSquare : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		if (x2 < x1 && y2 < y1) {
			if (x1 - x2 < y1 - y2)
				Rectangle(hdc, x1, y1, x2, y1 - (x1 - x2));
			else
				Rectangle(hdc, x1, y1, x1 - (y1 - y2), y2);
		}
		if (x2 < x1 && y2 > y1) {
			if (y2 - y1 < x1 - x2)
				Rectangle(hdc, x1, y1, x1 - (y2 - y1), y2);
			else
				Rectangle(hdc, x1, y1, x2, y1 - (x2 - x1));
		}
		if (x2 > x1 && y2 < y1) {
			if (x2 - x1 < y1 - y2)
				Rectangle(hdc, x1, y1, x2, y1 - (x2 - x1));
			else
				Rectangle(hdc, x1, y1, x1 - (y2 - y1), y2);
		}
		if (x2 > x1 && y2 > y1) {
			if (y2 - y1 < x2 - x1)
				Rectangle(hdc, x1, y1, x1 + (y2 - y1), y2);
			else
				Rectangle(hdc, x1, y1, x2, y1 + (x2 - x1));
		}
	}

	CShape* Create() { return new CSquare; }

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

class CEllipse : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		Ellipse(hdc, x1, y1, x2, y2);
	}

	CShape* Create() { return new CEllipse; }

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

class CCircle : public CShape {
public:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		if (x2 < x1 && y2 < y1) {
			if (x1 - x2 < y1 - y2)
				Ellipse(hdc, x1, y1, x2, y1 - (x1 - x2));
			else
				Ellipse(hdc, x1, y1, x1 - (y1 - y2), y2);
		}
		if (x2 < x1 && y2 > y1) {
			if (y2 - y1 < x1 - x2)
				Ellipse(hdc, x1, y1, x1 - (y2 - y1), y2);
			else
				Ellipse(hdc, x1, y1, x2, y1 - (x2 - x1));
		}
		if (x2 > x1 && y2 < y1) {
			if (x2 - x1 < y1 - y2)
				Ellipse(hdc, x1, y1, x2, y1 - (x2 - x1));
			else
				Ellipse(hdc, x1, y1, x1 - (y2 - y1), y2);
		}
		if (x2 > x1 && y2 > y1) {
			if (y2 - y1 < x2 - x1)
				Ellipse(hdc, x1, y1, x1 + (y2 - y1), y2);
			else
				Ellipse(hdc, x1, y1, x2, y1 + (x2 - x1));
		}
	}

	CShape* Create() { return new CCircle; }

	void SetData(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}
};

vector<CShape*> shapes;
vector<CShape*> prototypes;

bool isDrawing = FALSE;
int currentX;
int currentY;
int lastX;
int lastY;
int checkLine;
int checkRectangle;
int checkEllipse;
int keyShift;

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
				currentX = x;
				currentY = y;
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

			lastX = x;
			lastY = y;
			
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;
	case WM_LBUTTONUP: 
		{
			x = GET_X_LPARAM(lParam);
			y = GET_Y_LPARAM(lParam);

			if (checkLine) {
				CLine* line = new CLine;
				line->SetData(currentX, currentY, x, y);
				shapes.push_back(line);
			}
			if (checkRectangle) {
				if (keyShift) {
					CSquare* square = new CSquare;
					square->SetData(currentX, currentY, x, y);
					shapes.push_back(square);
				}
				else {
					CRectangle* rectangle = new CRectangle;
					rectangle->SetData(currentX, currentY, x, y);
					shapes.push_back(rectangle);
				}
			}
			if (checkEllipse) {
				if (keyShift) {
					CCircle* circle = new CCircle;
					circle->SetData(currentX, currentY, x, y);
					shapes.push_back(circle);
				}
				else {
					CEllipse* ellipse = new CEllipse;
					ellipse->SetData(currentX, currentY, x, y);
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
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
			HMENU hMenu = GetMenu(hWnd);
            // Parse the menu selections:
            switch (wmId)
            {
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

			for (int i = 0; i < shapes.size(); i++) {
				shapes[i]->Draw(hdc);
			}

			if (checkLine && isDrawing) {
				MoveToEx(hdc, currentX, currentY, NULL);
				LineTo(hdc, lastX, lastY);
			}

			if (checkRectangle && isDrawing) {
				if (keyShift) {
					CSquare* square = new CSquare;
					square->SetData(currentX, currentY, lastX, lastY);
					square->Draw(hdc);
					delete square;
				}
				else
					Rectangle(hdc, currentX, currentY, lastX, lastY);
			}

			if (checkEllipse && isDrawing) {
				if (keyShift) {
					CCircle* circle = new CCircle;
					circle->SetData(currentX, currentY, lastX, lastY);
					circle->Draw(hdc);
					delete circle;
				}
				else
					Ellipse(hdc, currentX, currentY, lastX, lastY);
			}

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
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
