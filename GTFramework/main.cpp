#include <windows.h>
#include <GdiPlus.h>
#include <string.h>
#include <fstream>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>

#include "OpenFileDialog.h"
#include "Utils.h"
#include "Button.h"
#include "ButtonHandlers.h"
#include "Init.h"

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

WNDPROC   OriginProc;
HINSTANCE hApp;
HWND      hMainWindow;

HWND hTestBtn;
HWND hPauseBtn;
HWND hShowBtn;

HWND hMaxButton;
HWND hMinButton;
HWND hCloseButton;
HWND hTrackingButton;
HWND hBehaviorButton;
HWND hExceptionButton;
HWND hOpenVideoButton;
HWND hPauseButton;
HWND hShowButton;
HWND hBehavGetButton;
HWND hBehavDectButton;
HWND hBehavClsButton;
HWND hTrainButton;
HWND hExcepDectButton;
HWND hRedoButton;
HWND hBehaviorBoard;
HWND hExceptionBoard;

int TabState = 0;
const char* szClassName = "MainWindow";

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{

	Gdiplus::GdiplusStartupInput gdiInput;
	ULONG_PTR gdiplusStartupToken;
	Gdiplus::GdiplusStartup(&gdiplusStartupToken,&gdiInput,NULL);

	WNDCLASSEX wndclass;
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc   = WinProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szClassName;
	wndclass.hIconSm       = NULL;

	RegisterClassEx(&wndclass);

	HWND hwnd = ::CreateWindowEx(
		0,
		szClassName,
		"My First Window",
		WS_POPUP  ,
		100,
		100,
		800,
		600,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Error in Create Window", "Error", MB_OK);
		return -1;
	}

	hApp = hInstance;
	hMainWindow = hwnd;

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

	MSG msg;
	while (::GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	Gdiplus::GdiplusShutdown(gdiplusStartupToken);


	return msg.wParam;	
}

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rect;

	switch(message)
	{
	case WM_CREATE:
		{
			InitButtons(hWnd);
			InitDrawBoards(hWnd);
			InitState();
			InitTracking();
			return 0;
		}

	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);

			GetWindowRect(hWnd, &rect);

			int windowX      = rect.left;
			int windowY      = rect.top;
			int windowWidth  = rect.right  - rect.left;
			int windowHeight = rect.bottom - rect.top;

			Graphics    graphics(hdc);
			SolidBrush solidBrush1(Color(255, 69, 133, 243));
			graphics.FillRectangle(&solidBrush1, 0, windowHeight - 20, windowWidth, 20);

			Pen pen(Color(255, 69, 133, 243), 1);
			graphics.DrawRectangle(&pen, 0, 0, windowWidth - 1, windowHeight);

			SetWindowPos(hCloseButton,      NULL, windowWidth - 31, 1, 30, 25, SWP_NOSIZE);

			SetWindowPos(hTrackingButton,   NULL, 30,  30, 0, 0, SWP_NOSIZE);
			SetWindowPos(hBehaviorButton,   NULL, 90,  30, 0, 0, SWP_NOSIZE);
			SetWindowPos(hExceptionButton,  NULL, 200, 30, 0, 0, SWP_NOSIZE);

			SetWindowPos(hOpenVideoButton,  NULL, 35,  70, 0, 0, SWP_NOSIZE);
			SetWindowPos(hPauseButton,      NULL, 75,  70, 0, 0, SWP_NOSIZE);
			SetWindowPos(hShowButton,       NULL, 115, 70, 0, 0, SWP_NOSIZE);

			SetWindowPos(hBehavGetButton,   NULL, 35,  70, 0, 0, SWP_NOSIZE);							    		   
			SetWindowPos(hBehavDectButton,  NULL, 75,  70, 0, 0, SWP_NOSIZE);
			SetWindowPos(hBehavClsButton,   NULL, 115, 70, 0, 0, SWP_NOSIZE);
										    		   
			SetWindowPos(hTrainButton,      NULL, 35,  70, 0, 0, SWP_NOSIZE);
			SetWindowPos(hExcepDectButton,  NULL, 75,  70, 0, 0, SWP_NOSIZE);
			SetWindowPos(hRedoButton,       NULL, 115, 70, 0, 0, SWP_NOSIZE);

			SetWindowPos(hBehaviorBoard,    NULL, 35,  120, 0, 0, SWP_NOSIZE);
			SetWindowPos(hExceptionBoard,   NULL, 35,  120, 0, 0, SWP_NOSIZE);

			EndPaint(hWnd, &ps);
			break;
		}

	case WM_NCHITTEST:
		{
			return HTCAPTION;
		}

	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



