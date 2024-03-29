// RibbonPaint.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "RibbonPaint.h"
#include <Objbase.h>
#pragma comment(lib, "Ole32.lib")
#include "RibbonFramework.h"
#include "RibbonIDs.h"

#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#include <iostream>
#include <vector>
#include <windowsX.h>
using namespace std;

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
	CoInitialize(0);
	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_RIBBONPAINT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_RIBBONPAINT));

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

	wcex.style = 0; // CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDC_RIBBONPAINT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_RIBBONPAINT);
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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
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
	virtual CShape* Create(int a, int b, int c, int d) = 0;
};

class CLine : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		MoveToEx(hdc, x1, y1, NULL);
		LineTo(hdc, x2, y2);
	}

	CLine(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CLine(a, b, c, d);
	}

	CLine() {}
};

class CRectangle : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		Rectangle(hdc, x1, y1, x2, y2);
	}

	CRectangle(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CRectangle(a, b, c, d);
	}

	CRectangle() {}
};
class CSquare : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		int temp = abs(x2 - x1);
		Rectangle(hdc, x1, y1, x1 + temp, y1 + temp);
	}

	CSquare(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CSquare(a, b, c, d);
	}

	CSquare() {}
};

class CEllipse : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		Ellipse(hdc, x1, y1, x2, y2);
	}

	CEllipse(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CEllipse(a, b, c, d);
	}

	CEllipse() {}
};
class CCircle : public CShape {
private:
	int x1;
	int y1;
	int x2;
	int y2;
public:
	void Draw(HDC hdc) {
		int temp = abs(x2 - x1);
		Ellipse(hdc, x1, y1, x1 + temp, y1 + temp);
	}

	CCircle(int a, int b, int c, int d) {
		x1 = a;
		y1 = b;
		x2 = c;
		y2 = d;
	}

	CShape* Create(int a, int b, int c, int d) {
		return new CCircle(a, b, c, d);
	}

	CCircle() {}
};

vector<CShape*> shapes;
vector<CShape*> prototypes;
int startX;
int startY;
int lastX;
int lastY;
bool isDrawing = false;
enum ModeShape {
	LINE,
	RECTANGLE,
	ELLIPSE
};

ModeShape mode;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		startX = x;
		startY = y;
		isDrawing = true;
	}
	break;
	case WM_MOUSEMOVE: {
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		WCHAR buffer[200];
		wsprintf(buffer, L"%d, %d", x, y);
		SetWindowText(hWnd, buffer);

		if (isDrawing) {
			lastX = x;
			lastY = y;

			InvalidateRect(hWnd, NULL, TRUE);
		}
	}
					   break;
	case WM_LBUTTONUP: 
	{
		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);

		
		switch (mode)
		{
		case LINE: {
			shapes.push_back(new CLine(startX, startY, x, y));
			break; 
		}
		case RECTANGLE: {
			if (GetKeyState(VK_LSHIFT)<0)
			{
				shapes.push_back(new CSquare(startX, startY, startX + abs(startX - x), startY + abs(startY - y)));
			}
			else
			{
				shapes.push_back(new CRectangle(startX, startY, x, y));
			}
			break;
		}
		case ELLIPSE: {
			if (GetKeyState(VK_LSHIFT)<0)
			{
				shapes.push_back(new CCircle(startX, startY, startX + abs(startX - x), startY + abs(startY - y)));
			}
			else
			{
				shapes.push_back(new CEllipse(startX, startY, x, y));
			}
			break;
		}

		}	
		
		
	}
	isDrawing = false;

	InvalidateRect(hWnd, NULL, TRUE);
	break;

	case WM_CREATE:
	{
		InitializeFramework(hWnd);
		prototypes.push_back(new CLine);
		prototypes.push_back(new CRectangle);
		prototypes.push_back(new CEllipse);
		prototypes.push_back(new CSquare);
		prototypes.push_back(new CCircle);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case ID_DRAW_LINE:
		{
			mode = LINE;
			CShape* shape = prototypes[0]->Create(0, 0, 0, 0);
			shapes.push_back(shape);
			InvalidateRect(hWnd, NULL, TRUE);
		}
			break;

		case ID_DRAW_RECTANGLE:
		{
			mode = RECTANGLE;
			if (GetKeyState(VK_LSHIFT)<0)
			{
				CShape* shape = prototypes[3]->Create(0, 0, 0, 0);
				shapes.push_back(shape);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else
			{
				CShape* shape = prototypes[1]->Create(0, 0, 0, 0);
				shapes.push_back(shape);
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
			break;

		case ID_DRAW_ELLIPSE:
		{
			mode = ELLIPSE;
			if (GetKeyState(VK_LSHIFT)<0)
			{
				CShape* shape = prototypes[4]->Create(0, 0, 0, 0);
				shapes.push_back(shape);
				InvalidateRect(hWnd, NULL, TRUE);
			}
			else
			{
				CShape* shape = prototypes[2]->Create(0, 0, 0, 0);
				shapes.push_back(shape);
				InvalidateRect(hWnd, NULL, TRUE);
			}
		}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

		break;
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		for (int i = 0; i < shapes.size(); i++) {
			shapes[i]->Draw(hdc);
		}

		if (isDrawing) 
		{
			switch (mode)
			{
			case LINE: {
				MoveToEx(hdc, startX, startY, NULL);
				LineTo(hdc, lastX, lastY);
				break;
			}
			case RECTANGLE:
			{
				if (GetKeyState(VK_LSHIFT)<0)
				{
					int temp = abs(startX - lastX);
					Rectangle(hdc, startX, startY, startX + temp, startY + temp);
				}
				else
				{
					Rectangle(hdc, startX, startY, lastX, lastY);
				}
			}
			break;
			case ELLIPSE:
			{
				if (GetKeyState(VK_LSHIFT)<0)

				{
					int temp = abs(startX - lastX);
					Ellipse(hdc, startX, startY, startX + temp, startY + temp);
				}
				else
				{
					Ellipse(hdc, startX, startY, lastX, lastY);
				}
				break;
			}
			}
		}

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DestroyFramework();
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

