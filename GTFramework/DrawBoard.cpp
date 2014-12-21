#include <Windows.h>

#include "DrawBoard.h"

extern HINSTANCE hApp;
extern WNDPROC   OriginProc;

void InitDrawBoard()
{
	WNDCLASSEX wndclass;
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc   = BoardOriginProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hApp;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = "DrawBoard";
	wndclass.hIconSm       = NULL;

	RegisterClassEx(&wndclass);
}
HWND CreateBoard(int width, int height, HWND hParent, HINSTANCE hInstance, WNDPROC Proc)
{
	HWND hwnd = CreateWindow("Button",
			            NULL,
						WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,
						1,
						1,
						width,
						height,
						hParent,
						NULL,
						hInstance,
						NULL);
	if (hwnd == NULL) 
	{
		MessageBox(NULL, "Error", "Can't not create board", MB_OK);
	}
	SetWindowLong(hwnd, GWL_WNDPROC, (LONG)Proc);
	return hwnd;
}

LRESULT CALLBACK BoardOriginProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CREATE:
		{
			return 0;
		}

	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			break;
		}


	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}