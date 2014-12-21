#include <windows.h>
#include <GdiPlus.h>
#include "Button.h"


extern WNDPROC OriginProc;
HWND CreateButton(int width, int height, HWND hParent, HINSTANCE hInstance, WNDPROC Proc)
{
	HWND hwnd = CreateWindow(TEXT("Button"),
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
	OriginProc = (WNDPROC)SetWindowLong(hwnd, GWL_WNDPROC, (LONG)Proc);
	return hwnd;
}





