#include <windows.h>
#include <GdiPlus.h>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>

#include "Utils.h"
#include "DrawBoardHandlers.h"

extern WNDPROC OriginProc;
extern HWND hBehaviorBoard;
extern HWND hExceptionBoard;

extern int TabState;

int isBehaviorDraw  = 0;
int isExceptionDraw = 0;
int isStartRecognize = 0;
POINT ptOne, ptTwo;

void BehavBoardPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hBehaviorBoard, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Image image(L"Assets/CrossRoad.png");
	graphics.DrawImage(&image, 0, 0, 600, 360);

	graphics.ReleaseHDC(hdc);
	EndPaint(hBehaviorBoard, &ps);
}

void ExceptionBoardPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hExceptionBoard, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 255, 255, 255));
	graphics.FillRectangle(&solidBrush1, 0, 0, 600, 360);

	graphics.ReleaseHDC(hdc);
	EndPaint(hExceptionBoard, &ps);
}

LRESULT CALLBACK BehabiorBoardProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			BehavBoardPaintHandler();
			return 0;
		}	

	case WM_LBUTTONDOWN:
		{
			isBehaviorDraw = 1;
			
			ptOne.x = (int)(short)LOWORD(lParam);
			ptOne.y = (int)(short)HIWORD(lParam);

		    return 0;
		}
	case WM_MOUSEMOVE:
		{
			if (isBehaviorDraw) 
			{
				ptTwo.x = (int)(short)LOWORD(lParam);
				ptTwo.y = (int)(short)HIWORD(lParam);
				
				HDC hdc = GetDC(hwnd);

				HPEN hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				SelectObject(hdc,hpen);
				MoveToEx(hdc, ptOne.x, ptOne.y, NULL);
				LineTo(hdc, ptTwo.x, ptTwo.y);
			 
				ptOne = ptTwo;
			}
				
			return 0;	
			
		}
	case WM_LBUTTONUP:
		{
			isBehaviorDraw  = 0;
			
			HDC     hdc     = GetDC(hBehaviorBoard);
            HBITMAP hBitmap = CreateCompatibleBitmap(hdc, 600, 360);
            HDC     hdcMem  = CreateCompatibleDC(hdc);
            
            SelectObject(hdcMem, hBitmap);
            BitBlt(hdcMem, 0, 0, 600, 360, hdc, 0, 0, SRCCOPY);
            Utils::SaveBitmapToFile("E:\\aaa.bmp", hBitmap);
			
		    return 0;
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK ExceptionBoardProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			ExceptionBoardPaintHandler();
			return 0;
		}	

	case WM_LBUTTONDOWN:
		{
			isExceptionDraw = 1;
			
			ptOne.x = (int)(short)LOWORD(lParam);
			ptOne.y = (int)(short)HIWORD(lParam);

		    return 0;
		}
	case WM_MOUSEMOVE:
		{
			if (isExceptionDraw) 
			{
				ptTwo.x = (int)(short)LOWORD(lParam);
				ptTwo.y = (int)(short)HIWORD(lParam);
				
				HDC hdc = GetDC(hwnd);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             

				HPEN hpen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
				SelectObject(hdc,hpen);
				MoveToEx(hdc, ptOne.x, ptOne.y, NULL);
				Rectangle(hdc, 0, 0, 600, 360);
				LineTo(hdc, ptTwo.x, ptTwo.y);
			 
				//ptOne = ptTwo;
			}
				
			return 0;	
			
		}
	case WM_LBUTTONUP:
		{
			isExceptionDraw = 0;
			
		    return 0;
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
