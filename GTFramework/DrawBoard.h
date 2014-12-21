#pragma once

void InitDrawBoard();
HWND CreateBoard(int width, int height, HWND hParent, HINSTANCE hInstance, WNDPROC Proc);
LRESULT CALLBACK BoardOriginProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
