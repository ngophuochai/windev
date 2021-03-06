// Midterm.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Midterm.h"
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#define MAX_LOADSTRING 100
#define MAX_BUFFER 100
#define MAX_PATH 10240

struct datax
{
	WCHAR *LoaiChiTieu;
	WCHAR *NoiDung;
	WCHAR *SoTien;
};

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
WCHAR configPath[MAX_PATH];
HWND hwnd;
HWND hwndLoaiChiTieu, hwndNoiDung, hwndSoTien, hwndTTTK, hwndTong;
HWND h_ListView;
WCHAR sumTien[6][MAX_BUFFER];				// 0: An uong
											// 1: Di chuyen
											// 2: Nha cua
											// 3: Xe co
											// 4: Nhu yeu pham
											// 5: Dich vu
vector<datax> datas;
BOOL isOpenDetail = FALSE;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
WCHAR* StrAddStr(WCHAR *s1, WCHAR *s2);
LONG StrToNum(WCHAR* s);
BOOL DrawPieChart(HWND hWnd, int x, int y, DWORD radius, float startAngle, float sweepAngle, COLORREF color);

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
    LoadStringW(hInstance, IDC_MIDTERM, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MIDTERM));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MIDTERM);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

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

   GetCurrentDirectory(MAX_PATH, configPath);
   StrCat(configPath, L"\\data.txt");

   HWND hWnd = CreateWindowW(szWindowClass, L"Quản lý chi tiêu", 
	   WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
      CW_USEDEFAULT, 0, 550, 550, nullptr, nullptr, hInstance, nullptr);

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
    switch (message)
    {
	case WM_CREATE:
		{
			LOGFONT lf;
			GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
			HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
				lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
				lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
				lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
				lf.lfPitchAndFamily, lf.lfFaceName);

			// BANG Them mot loai chi tieu
			CreateWindow(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				30, 3, 470, 100, hWnd, NULL, hInst, NULL);

			hwnd = CreateWindow(L"STATIC", L"Thêm một loại chi tiêu", WS_CHILD | WS_VISIBLE | SS_LEFT, 
				40, 4, 105, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
			
			hwnd = CreateWindow(L"STATIC", L"Loại chi tiêu:", WS_CHILD | WS_VISIBLE | SS_LEFT,
				50, 40, 70, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwndLoaiChiTieu = CreateWindow(L"COMBOBOX", NULL, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
				50, 60, 100, 20, hWnd, NULL, hInst, NULL);
			SendMessage(hwndLoaiChiTieu, WM_SETFONT, WPARAM(hFont), TRUE);

			SendMessage(hwndLoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)L"Ăn uống");
			SendMessage(hwndLoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)L"Di chuyển");
			SendMessage(hwndLoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)L"Nhà cửa");
			SendMessage(hwndLoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)L"Xe cộ");
			SendMessage(hwndLoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)L"Nhu yếu phẩm");
			SendMessage(hwndLoaiChiTieu, CB_ADDSTRING, 0, (LPARAM)L"Dịch vụ");
			
			hwnd = CreateWindow(L"STATIC", L"Nội dung:", WS_CHILD | WS_VISIBLE | SS_LEFT,
				170, 40, 70, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwndNoiDung = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
				170, 60, 100, 21, hWnd, NULL, hInst, NULL);
			SendMessage(hwndNoiDung, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Số tiền:", WS_CHILD | WS_VISIBLE | SS_LEFT,
				290, 40, 70, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwndSoTien = CreateWindow(L"EDIT", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | ES_NUMBER,
				290, 60, 100, 21, hWnd, NULL, hInst, NULL);
			SendMessage(hwndSoTien, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"BUTTON", L"Thêm", WS_CHILD | WS_VISIBLE | BS_CENTER,
				410, 60, 70, 21, hWnd, (HMENU)ID_BUTTON_THEM, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			// BANG Toan bo danh sach cac chi tieu
			CreateWindow(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
				30, 123, 470, 200, hWnd, NULL, hInst, NULL);

			hwnd = CreateWindow(L"STATIC", L"Toàn bộ danh sách các chi tiêu", WS_CHILD | WS_VISIBLE | SS_LEFT,
				40, 124, 155, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"BUTTON", L"Xóa", WS_CHILD | WS_VISIBLE | BS_CENTER,
				410, 122, 70, 21, hWnd, (HMENU)ID_BUTTON_REMOVE, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			h_ListView = CreateWindow(WC_LISTVIEW, NULL,
				WS_CHILD | WS_VISIBLE |  WS_BORDER | LVS_REPORT,
				50, 160, 430, 150, hWnd, NULL, hInst, NULL);

			LVCOLUMN lvCol;
			lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
			lvCol.fmt = LVCFMT_LEFT;

			lvCol.cx = 100;
			lvCol.pszText = L"Loại chi tiêu";
			ListView_InsertColumn(h_ListView, 0, &lvCol);

			lvCol.cx = 210;
			lvCol.pszText = L"Nội dung";
			ListView_InsertColumn(h_ListView, 1, &lvCol);

			lvCol.cx = 103;
			lvCol.pszText = L"Số tiền";
			ListView_InsertColumn(h_ListView, 2, &lvCol);

			// Load DATA
			fstream f;
			f.open(configPath, ios_base::in | ios_base::binary);

			for (int i = 0; i < 6; i++)
				StrCpy(sumTien[i], L"0");

			if (f.is_open()) {
				WCHAR length;
				datax buf;

				for (int i = 0, j; !f.eof(); i++) {
					f.read((char*)(&length), 2);
					if (f.eof())
						break;

					buf.LoaiChiTieu = new WCHAR[MAX_BUFFER];
					buf.NoiDung = new WCHAR[MAX_BUFFER];
					buf.SoTien = new WCHAR[MAX_BUFFER];

					for (j = 0; j < length; j++)
						f.read((char*)(&buf.LoaiChiTieu[j]), 2);
					buf.LoaiChiTieu[j] = 0;

					f.read((char*)(&length), 2);
					for (j = 0; j < length; j++)
						f.read((char*)(&buf.NoiDung[j]), 2);
					buf.NoiDung[j] = 0;

					f.read((char*)(&length), 2);
					for (j = 0; j < length; j++)
						f.read((char*)(&buf.SoTien[j]), 2);
					buf.SoTien[j] = 0;

					datas.push_back(buf);
				}

				f.close();

				for (int i = 0; i < datas.size(); i++) {

					if (StrCmp(L"Ăn uống", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[0], StrAddStr(sumTien[0], datas[i].SoTien));
					if (StrCmp(L"Di chuyển", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[1], StrAddStr(sumTien[1], datas[i].SoTien));
					if (StrCmp(L"Nhà cửa", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[2], StrAddStr(sumTien[2], datas[i].SoTien));
					if (StrCmp(L"Xe cộ", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[3], StrAddStr(sumTien[3], datas[i].SoTien));
					if (StrCmp(L"Nhu yếu phẩm", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[4], StrAddStr(sumTien[4], datas[i].SoTien));
					if (StrCmp(L"Dịch vụ", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[5], StrAddStr(sumTien[5], datas[i].SoTien));

					LVITEM lv;
					lv.mask = LVIF_TEXT;
					lv.iItem = 0;
					lv.iSubItem = 0;
					lv.pszText = datas[i].LoaiChiTieu;
					ListView_InsertItem(h_ListView, &lv);
					ListView_SetItemText(h_ListView, lv.iItem, 1, datas[i].NoiDung);
					ListView_SetItemText(h_ListView, lv.iItem, 2, datas[i].SoTien);
				}
			}

			// BANG thong tin thong ke
			hwndTTTK = CreateWindow(L"BUTTON", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | BS_GROUPBOX,
				30, 350, 470, 110, hWnd, NULL, hInst, NULL);

			hwnd = CreateWindow(L"STATIC", L"Thông tin thống kê", WS_CHILD | WS_VISIBLE | SS_LEFT,
				40, 344, 95, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Tổng cộng:", WS_CHILD | WS_VISIBLE | SS_LEFT,
				150, 375, 95, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			WCHAR tong[MAX_BUFFER] = L"0";
			for (int i = 0; i < datas.size(); i++)
				StrCpy(tong, StrAddStr(tong, datas[i].SoTien));

			hwndTong = CreateWindow(L"EDIT", tong, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_READONLY,
				220, 373, 100, 20, hWnd, NULL, hInst, NULL);
			SendMessage(hwndTong, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"BUTTON", L"Thoát", WS_CHILD | WS_VISIBLE | BS_CENTER,
				230, 475, 70, 21, hWnd, (HMENU)ID_BUTTON_THOAT, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"BUTTON", L"Chi tiết", WS_CHILD | WS_VISIBLE | BS_CENTER,
				410, 373, 70, 21, hWnd, (HMENU)ID_BUTTON_DETAIL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Ăn uống", WS_CHILD | WS_VISIBLE | SS_LEFT,
				600, 210, 95, 25, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Di chuyển", WS_CHILD | WS_VISIBLE | SS_LEFT,
				600, 240, 95, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Nhà cửa", WS_CHILD | WS_VISIBLE | SS_LEFT,
				600, 270, 95, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Xe cộ", WS_CHILD | WS_VISIBLE | SS_LEFT,
				600, 300, 95, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Nhu yếu phẩm", WS_CHILD | WS_VISIBLE | SS_LEFT,
				600, 330, 95, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);

			hwnd = CreateWindow(L"STATIC", L"Dịch vụ", WS_CHILD | WS_VISIBLE | SS_LEFT,
				600, 360, 95, 15, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
			case ID_BUTTON_REMOVE:
			{
				int index = ListView_GetSelectionMark(h_ListView);

				if (index >= 0) {
					vector<datax> buffer;

					for (int i = 0; i < index + 1; i++) {
						if (i != index)
							buffer.push_back(datas.back());

						datas.pop_back();
					}

					while (!buffer.empty()) {
						datas.push_back(buffer.back());
						buffer.pop_back();
					}

					for (int i = 0; i < 6; i++)
						StrCpy(sumTien[i], L"0");

					ListView_DeleteAllItems(h_ListView);
					WCHAR tong[MAX_BUFFER] = L"0";

					for (int i = 0; i < datas.size(); i++) {

						if (StrCmp(L"Ăn uống", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[0], StrAddStr(sumTien[0], datas[i].SoTien));
						if (StrCmp(L"Di chuyển", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[1], StrAddStr(sumTien[1], datas[i].SoTien));
						if (StrCmp(L"Nhà cửa", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[2], StrAddStr(sumTien[2], datas[i].SoTien));
						if (StrCmp(L"Xe cộ", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[3], StrAddStr(sumTien[3], datas[i].SoTien));
						if (StrCmp(L"Nhu yếu phẩm", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[4], StrAddStr(sumTien[4], datas[i].SoTien));
						if (StrCmp(L"Dịch vụ", datas[i].LoaiChiTieu) == 0) StrCpy(sumTien[5], StrAddStr(sumTien[5], datas[i].SoTien));

						LVITEM lv;
						lv.mask = LVIF_TEXT;
						lv.iItem = 0;
						lv.iSubItem = 0;
						lv.pszText = datas[i].LoaiChiTieu;
						ListView_InsertItem(h_ListView, &lv);
						ListView_SetItemText(h_ListView, lv.iItem, 1, datas[i].NoiDung);
						ListView_SetItemText(h_ListView, lv.iItem, 2, datas[i].SoTien);
						StrCpy(tong, StrAddStr(tong, datas[i].SoTien));
					}

					SendMessage(hwndTong, WM_SETTEXT, NULL, (LPARAM)tong);
					InvalidateRect(hWnd, NULL, TRUE);
				}
			}
				break;
			case ID_BUTTON_THOAT:
				DestroyWindow(hWnd);
				break;
			case ID_BUTTON_THEM:
			{
				int index = SendMessage(hwndLoaiChiTieu, CB_GETCURSEL, NULL, NULL);
				datax buf;
				buf.NoiDung = new WCHAR[MAX_BUFFER];
				buf.SoTien = new WCHAR[MAX_BUFFER];

				GetWindowText(hwndNoiDung, buf.NoiDung, MAX_BUFFER);
				GetWindowText(hwndSoTien, buf.SoTien, MAX_BUFFER);

				if (index < 0 || buf.NoiDung[0] == 0 || buf.SoTien[0] == 0)
					break;

				buf.LoaiChiTieu = new WCHAR[MAX_BUFFER];
				SendMessage(hwndLoaiChiTieu, CB_GETLBTEXT, index, (LPARAM)buf.LoaiChiTieu);

				if (StrCmp(L"Ăn uống", buf.LoaiChiTieu) == 0) StrCpy(sumTien[0], StrAddStr(sumTien[0], buf.SoTien));
				if (StrCmp(L"Di chuyển", buf.LoaiChiTieu) == 0) StrCpy(sumTien[1], StrAddStr(sumTien[1], buf.SoTien));
				if (StrCmp(L"Nhà cửa", buf.LoaiChiTieu) == 0) StrCpy(sumTien[2], StrAddStr(sumTien[2], buf.SoTien));
				if (StrCmp(L"Xe cộ", buf.LoaiChiTieu) == 0) StrCpy(sumTien[3], StrAddStr(sumTien[3], buf.SoTien));
				if (StrCmp(L"Nhu yếu phẩm", buf.LoaiChiTieu) == 0) StrCpy(sumTien[4], StrAddStr(sumTien[4], buf.SoTien));
				if (StrCmp(L"Dịch vụ", buf.LoaiChiTieu) == 0) StrCpy(sumTien[5], StrAddStr(sumTien[5], buf.SoTien));

				LVITEM lv;
				lv.mask = LVIF_TEXT;
				lv.iItem = 0;
				lv.iSubItem = 0;
				lv.pszText = buf.LoaiChiTieu;
				ListView_InsertItem(h_ListView, &lv);
				ListView_SetItemText(h_ListView, lv.iItem, 1, buf.NoiDung);
				ListView_SetItemText(h_ListView, lv.iItem, 2, buf.SoTien);

				WCHAR tong[MAX_BUFFER];
				GetWindowText(hwndTong, tong, MAX_BUFFER);
				StrCpy(tong, StrAddStr(tong, buf.SoTien));
				SendMessage(hwndTong, WM_SETTEXT, NULL, (LPARAM)tong);
				datas.push_back(buf);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			break;
			case ID_BUTTON_DETAIL:
			{
				RECT rect;
				GetWindowRect(hWnd, &rect);

				if (isOpenDetail == FALSE) {
					MoveWindow(hWnd, rect.left, rect.top, 550 + 200, 550, TRUE);
					isOpenDetail = TRUE;
				}
				else {
					MoveWindow(hWnd, rect.left, rect.top, 550, 550, TRUE);
					isOpenDetail = FALSE;
				}
			}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_ERASEBKGND:
		return 1;
    case WM_PAINT:
        {
			LONG sumTien0 = StrToNum(sumTien[0]);
			LONG sumTien1 = StrToNum(sumTien[1]);
			LONG sumTien2 = StrToNum(sumTien[2]);
			LONG sumTien3 = StrToNum(sumTien[3]);
			LONG sumTien4 = StrToNum(sumTien[4]);
			LONG sumTien5 = StrToNum(sumTien[5]);

			LONG sumTongTien = sumTien0 + sumTien1 + sumTien2 + sumTien3 + sumTien4 + sumTien5;

            PAINTSTRUCT ps;
			HDC hdc;
			RECT rect;

			hdc = BeginPaint(hWnd, &ps);

			GetClientRect(hWnd, &rect);
			FillRect(hdc, &rect, (HBRUSH)(COLOR_BTNFACE + 1));
			SetRect(&rect, 565, 210, 585, 225);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 0)));
			SetRect(&rect, 565, 240, 585, 255);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 128, 0)));
			SetRect(&rect, 565, 270, 585, 285);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 255)));
			SetRect(&rect, 565, 300, 585, 315);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 0)));
			SetRect(&rect, 565, 330, 585, 345);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 255)));
			SetRect(&rect, 565, 360, 585, 375);
			FillRect(hdc, &rect, CreateSolidBrush(RGB(128, 0, 255)));
			EndPaint(hWnd, &ps);

			hdc = BeginPaint(hwndTTTK, &ps);
			rect.left = 20;
			rect.top = 60;
			rect.bottom = 85;

			if (sumTongTien > 0) {
				float sect = (float)sumTien0 / sumTongTien;
				float startAngle = 0;

				rect.right = sect * 430 + 20;
				FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 0)));
				DrawPieChart(hWnd, 630, 100, 70, startAngle, sect * 360, RGB(255, 0, 0));

				rect.left += sect * 430;
				startAngle += sect * 360;
				sect = (float)sumTien1 / sumTongTien;
				rect.right += sect * 430;
				FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 128, 0)));
				DrawPieChart(hWnd, 630, 100, 70, startAngle, sect * 360, RGB(0, 128, 0));

				rect.left += sect * 430;
				startAngle += sect * 360;
				sect = ((float)sumTien2 / sumTongTien);
				rect.right += sect * 430;
				FillRect(hdc, &rect, CreateSolidBrush(RGB(0, 0, 255)));
				DrawPieChart(hWnd, 630, 100, 70, startAngle, sect * 360, RGB(0, 0, 255));

				rect.left += sect * 430;
				startAngle += sect * 360;
				sect = ((float)sumTien3 / sumTongTien);
				rect.right += sect * 430;
				FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 0)));
				DrawPieChart(hWnd, 630, 100, 70, startAngle, sect * 360, RGB(255, 255, 0));

				rect.left += sect * 430;
				startAngle += sect * 360;
				sect = ((float)sumTien4 / sumTongTien);
				rect.right += sect * 430;
				FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 0, 255)));
				DrawPieChart(hWnd, 630, 100, 70, startAngle, sect * 360, RGB(255, 0, 255));

				rect.left += sect * 430;
				startAngle += sect * 360;
				sect = ((float)sumTien5 / sumTongTien);
				rect.right += sect * 430;
				FillRect(hdc, &rect, CreateSolidBrush(RGB(128, 0, 255)));
				DrawPieChart(hWnd, 630, 100, 70, startAngle, sect * 360, RGB(128, 0, 255));
			}
			else {
				rect.right = 450;
				FillRect(hdc, &rect, NULL);
				DrawPieChart(hWnd, 630, 100, 70, 0, 360, RGB(255, 255, 255));
			}

            EndPaint(hwndTTTK, &ps);
        }
        break;
    case WM_DESTROY:
		{
			WCHAR length;
			fstream f;
			f.open(configPath, ios::out | ios::binary);

			for (int i = 0; i < datas.size(); i++) {
				length = wcslen(datas[i].LoaiChiTieu);

				f.write((char*)(&length), 2);
				for (int j = 0; j < length; j++)
					f.write((char*)(&datas[i].LoaiChiTieu[j]), 2);

				length = wcslen(datas[i].NoiDung);

				f.write((char*)(&length), 2);
				for (int j = 0; j < length; j++)
					f.write((char*)(&datas[i].NoiDung[j]), 2);

				length = wcslen(datas[i].SoTien);

				f.write((char*)(&length), 2);
				for (int j = 0; j < length; j++)
					f.write((char*)(&datas[i].SoTien[j]), 2);
			}

			datas.clear();
			f.close();

			PostQuitMessage(0);
		}
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

WCHAR* StrAddStr(WCHAR *s1, WCHAR *s2)
{
	LONG num1 = 0;
	LONG num2 = 0;
	LONG kqNum;
	WCHAR bufkq[MAX_BUFFER];
	WCHAR kq[MAX_BUFFER];

	for (int i = 0; i < wcslen(s1); i++)
		num1 = (s1[i] - '0') + num1 * 10;

	for (int i = 0; i < wcslen(s2); i++)
		num2 = (s2[i] - '0') + num2 * 10;

	kqNum = num1 + num2;

	short buf;
	int i = 0;
	
	while (kqNum != 0) {
		buf = kqNum % 10;
		kqNum /= 10;
		bufkq[i++] = buf + '0';
	}

	for (int j = 0; j < i; j++)
		kq[j] = bufkq[i - j - 1];

	kq[i] = 0;
	return kq;
}

LONG StrToNum(WCHAR* s)
{
	LONG num = 0;

	for (int i = 0; i < wcslen(s); i++)
		num = num * 10 + (s[i] - '0');

	return num;
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