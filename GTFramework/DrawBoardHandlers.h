#pragma once


void BehavBoardPaintHandler();
void ExceptionBoardPaintHandler();

LRESULT CALLBACK BehabiorBoardProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ExceptionBoardProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
