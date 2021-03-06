// Dll1.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Shapes.h"
#define DLL_API __declspec(dllexport)

extern "C"
{
	DLL_API void DrawShape(Graphics *graphics, Pen *pen, Point current, Point last, int isShape)
	{
		switch (isShape) {
		case LINE:
		{
			CLine line;
			line.SetData(current, last, pen);
			line.Draw(graphics);
		}
			break;
		case RECTANGLE:
		{
			CRectangle rectangle;
			rectangle.SetData(current, last, pen);
			rectangle.Draw(graphics);
		}
			break;
		case SQUARE:
		{
			CSquare square;
			square.SetData(current, last, pen);
			square.Draw(graphics);
		}
		break;
		case ELLIPSE:
		{
			CEllipse ellipse;
			ellipse.SetData(current, last, pen);
			ellipse.Draw(graphics);
		}
		break;
		case CIRCLE:
		{
			CCircle circle;
			circle.SetData(current, last, pen);
			circle.Draw(graphics);
		}
		break;
		}
	}

	DLL_API BOOL OpenPath(WCHAR* path)
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
		ofn.lpstrFilter = L"All\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box. 

		if (GetOpenFileName(&ofn) == FALSE)
			return FALSE;

		StrCpy(path, ofn.lpstrFile);
		return TRUE;
	}
	DLL_API BOOL SavePath(WCHAR* path)
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

		StrCpy(path, ofn.lpstrFile);
		return TRUE;
	}
}