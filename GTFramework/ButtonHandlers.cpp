#include <WinSock2.h>
#include <windows.h>
#include <GdiPlus.h>
#include <string>

#include "Button.h"
#include "ButtonHandlers.h"
#include "Tracking.h"
#include "BehaviorDect.h"
#include "ExceptionDect.h"

extern HWND hMainWindow;
extern HWND hMinButton;
extern HWND hMaxButton;
extern HWND hCloseButton;

extern HWND hTrackingButton;
extern HWND hBehaviorButton;
extern HWND hExceptionButton;
extern HWND hOpenVideoButton;
extern HWND hPauseButton;
extern HWND hShowButton;
extern HWND hBehavGetButton;
extern HWND hBehavDectButton;
extern HWND hBehavClsButton;
extern HWND hTrainButton;
extern HWND hExcepDectButton;
extern HWND hRedoButton;
extern HWND hBehaviorBoard;
extern HWND hExceptionBoard;

extern HWND WINPROC;
extern WNDPROC OriginProc;

extern int TabState;
extern int paused;

//////////////////////////
///  Handlers For CloseButton 
/////////////////////////////////
void CloseButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hCloseButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::Image image(L"Assets/close.png");
	graphics.DrawImage(&image, 0, 0, 30, 25);
	graphics.ReleaseHDC(hdc);

	EndPaint(hCloseButton, &ps);
}
void CloseButtonMouseOverHandler()
{
	Gdiplus::Graphics graphics(GetDC(hCloseButton));
	Gdiplus::Image image(L"Assets/close_on.png");
	graphics.DrawImage(&image, 0, 0, 30, 25);
	graphics.ReleaseHDC(GetDC(hMinButton));
}
void CloseButtonMouseLeaveHandler()
{
	Gdiplus::Graphics graphics(GetDC(hCloseButton));
	Gdiplus::Image image(L"Assets/close.png");
	graphics.DrawImage(&image, 0, 0, 30, 25);
	graphics.ReleaseHDC(GetDC(hMinButton));
}
void CloseButtonMouseMoveHandler()
{
	TRACKMOUSEEVENT tme; 
	tme.cbSize = sizeof(tme); 
	tme.dwFlags = TME_HOVER|TME_LEAVE; 
	tme.dwHoverTime = 1;
	tme.hwndTrack = hCloseButton;
	TrackMouseEvent(&tme); 
}
void CloseButtonLButtonUphandler()
{
	SendMessage(hMainWindow, WM_CLOSE, 0, 0);
}

//////////////////////////
///  Handlers For TabButtons 
/////////////////////////////////
void TrackingButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hTrackingButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 60, 40);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 4.0f);
	
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 225, 225, 225));
	Gdiplus::SolidBrush  solidBrush3(Gdiplus::Color(255, 96, 96, 96));
	if (TabState == 0) 
	{
		graphics.DrawString(L"跟踪", -1, &font, pointF, &solidBrush2);
	}
	else 
	{
		graphics.DrawString(L"跟踪", -1, &font, pointF, &solidBrush3);
	}
	graphics.ReleaseHDC(hdc);
	EndPaint(hTrackingButton, &ps);
}
void TrackingButtonLButtonHandler()
{
	TabState = 0;
	

	InvalidateRect(hTrackingButton,  NULL,  TRUE);
	InvalidateRect(hBehaviorButton,  NULL,  TRUE);
	InvalidateRect(hExceptionButton, NULL, TRUE);
	
	ShowWindow(hOpenVideoButton, TRUE);
	ShowWindow(hPauseButton,     TRUE);
	ShowWindow(hShowButton,      TRUE);
	ShowWindow(hBehavGetButton,  FALSE);
	ShowWindow(hBehavDectButton, FALSE);
	ShowWindow(hBehavClsButton,  FALSE);
	ShowWindow(hTrainButton,     FALSE);
	ShowWindow(hExcepDectButton, FALSE);
	ShowWindow(hRedoButton,      FALSE);
	ShowWindow(hBehaviorBoard,   FALSE);
	ShowWindow(hExceptionBoard,  FALSE);

	EnableWindow(hOpenVideoButton, TRUE);
	EnableWindow(hPauseButton,     TRUE);
	EnableWindow(hShowButton,      TRUE);
	EnableWindow(hBehavGetButton,  FALSE);
	EnableWindow(hBehavDectButton, FALSE);
	EnableWindow(hBehavClsButton,  FALSE);
	EnableWindow(hTrainButton,     FALSE);
	EnableWindow(hExcepDectButton, FALSE);
	EnableWindow(hRedoButton,      FALSE);
	EnableWindow(hBehaviorBoard,   FALSE);
	EnableWindow(hExceptionBoard,  FALSE);
}

void BehaviorButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hBehaviorButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 110, 40);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 4.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 225, 225, 225));
	Gdiplus::SolidBrush  solidBrush3(Gdiplus::Color(255, 96, 96, 96));
	if (TabState == 1) 
	{
		graphics.DrawString(L"行为识别", -1, &font, pointF, &solidBrush2);
	}
	else 
	{
		graphics.DrawString(L"行为识别", -1, &font, pointF, &solidBrush3);
	}
	graphics.ReleaseHDC(hdc);
	EndPaint(hBehaviorButton, &ps);
}
void BehaviorButtonLButtonHandler()
{
	TabState = 1;
	DestroyTracking();

	InvalidateRect(hTrackingButton, NULL, TRUE);
	InvalidateRect(hBehaviorButton, NULL, TRUE);
	InvalidateRect(hExceptionButton, NULL, TRUE);

	ShowWindow(hOpenVideoButton, FALSE);
	ShowWindow(hPauseButton,     FALSE);
	ShowWindow(hShowButton,      FALSE);
	ShowWindow(hBehavGetButton,  TRUE);
	ShowWindow(hBehavDectButton, TRUE);
	ShowWindow(hBehavClsButton,  TRUE);
	ShowWindow(hTrainButton,     FALSE);
	ShowWindow(hExcepDectButton, FALSE);
	ShowWindow(hRedoButton,      FALSE);
	ShowWindow(hBehaviorBoard,   TRUE);
	ShowWindow(hExceptionBoard,  FALSE);

	EnableWindow(hOpenVideoButton, FALSE);
	EnableWindow(hPauseButton,     FALSE);
	EnableWindow(hShowButton,      FALSE);
	EnableWindow(hBehavGetButton,  TRUE);
	EnableWindow(hBehavDectButton, TRUE);
	EnableWindow(hBehavClsButton,  TRUE);
	EnableWindow(hTrainButton,     FALSE);
	EnableWindow(hExcepDectButton, FALSE);
	EnableWindow(hRedoButton,      FALSE);
	EnableWindow(hBehaviorBoard,   TRUE);
	EnableWindow(hExceptionBoard,  FALSE);
}

void ExceptionButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hExceptionButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 110, 40);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 4.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 225, 225, 225));
	Gdiplus::SolidBrush  solidBrush3(Gdiplus::Color(255, 96, 96, 96));
	if (TabState == 2) 
	{
		graphics.DrawString(L"异常检测", -1, &font, pointF, &solidBrush2);
	}
	else 
	{
		graphics.DrawString(L"异常检测", -1, &font, pointF, &solidBrush3);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hExceptionButton, &ps);
}
void ExceptionButtonLButtonHandler()
{
	TabState = 2;
	DestroyTracking();

	InvalidateRect(hTrackingButton, NULL, TRUE);
	InvalidateRect(hBehaviorButton, NULL, TRUE);
	InvalidateRect(hExceptionButton, NULL, TRUE);

	ShowWindow(hOpenVideoButton, FALSE);
	ShowWindow(hPauseButton,     FALSE);
	ShowWindow(hShowButton,      FALSE);
	ShowWindow(hBehavGetButton,  FALSE);
	ShowWindow(hBehavDectButton, FALSE);
	ShowWindow(hBehavClsButton,  FALSE);
	ShowWindow(hTrainButton,     TRUE);
	ShowWindow(hExcepDectButton, TRUE);
	ShowWindow(hRedoButton,      TRUE);
	ShowWindow(hBehaviorBoard,   FALSE);
	ShowWindow(hExceptionBoard,  TRUE);

	EnableWindow(hOpenVideoButton, FALSE);
	EnableWindow(hPauseButton,     FALSE);
	EnableWindow(hShowButton,      FALSE);
	EnableWindow(hBehavGetButton,  FALSE);
	EnableWindow(hBehavDectButton, FALSE);
	EnableWindow(hBehavClsButton,  FALSE);
	EnableWindow(hTrainButton,     TRUE);
	EnableWindow(hExcepDectButton, TRUE);
	EnableWindow(hRedoButton,      TRUE);
	EnableWindow(hBehaviorBoard,   FALSE);
	EnableWindow(hExceptionBoard,  TRUE);
}


//////////////////////////
///  Handlers For Buttons In Tracking Page
/////////////////////////////////
void OpenVideoButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hOpenVideoButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 0) 
	{
		graphics.DrawString(L"打开", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hOpenVideoButton, &ps);
}
void OpenVideoButtonLButtonHandler()
{
	TrackingOpenHandler();
	//MessageBox(NULL, "open", "dd", MB_OK);
}

void PauseButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hPauseButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);

	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 0) 
	{
		if (paused) 
		{
			graphics.DrawString(L"播放", -1, &font, pointF, &solidBrush2);
		}
		else
		{
			graphics.DrawString(L"暂停", -1, &font, pointF, &solidBrush2);
		}	
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hPauseButton, &ps);
}
void PauseButtonLButtonHandler()
{
	TrackingPauseHandler();
	//MessageBox(NULL, "pause", "dd", MB_OK);
}

void ShowButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hShowButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 0) 
	{
		graphics.DrawString(L"显示", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hShowButton, &ps);
}
void ShowButtonLButtonHandler()
{
	TrackingShowHandler();
	//MessageBox(NULL, "show", "dd", MB_OK);
}

//////////////////////////
///  Handlers For Buttons In Behavior Page
/////////////////////////////////
void BehavGetButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hBehavGetButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 1) 
	{
		graphics.DrawString(L"提取", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hBehavGetButton, &ps);
}
void BehavGetButtonLButtonHandler()
{
	BehavGetHandler();
	//MessageBox(NULL, "behde", "dd", MB_OK);
}

void BehavDectButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hBehavDectButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 1) 
	{
		graphics.DrawString(L"检测", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hBehavDectButton, &ps);
}
void BehavDectButtonLButtonHandler()
{
	BehavDectHandler();
	//MessageBox(NULL, "behde", "dd", MB_OK);
}

void BehavClsButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hBehavClsButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 1) 
	{
		graphics.DrawString(L"清除", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hBehavClsButton, &ps);
}
void BehavClsButtonLButtonHandler()
{
	BehavClsHandler();
	//MessageBox(NULL, "behcls", "dd", MB_OK);
}

//////////////////////////
///  Handlers For Buttons In Exception Page
/////////////////////////////////
void TrainButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hTrainButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 2) 
	{
		graphics.DrawString(L"训练", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hTrainButton, &ps);
}
void TrainButtonLButtonHandler()
{
	TrainHandler();
	//MessageBox(NULL, "train", "dd", MB_OK);
}

void ExcepDectButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hExcepDectButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 2) 
	{
		graphics.DrawString(L"检测", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hExcepDectButton, &ps);
}
void ExcepDectButtonLButtonHandler()
{
	ExcepDectHandler();
	//MessageBox(NULL, "exce", "dd", MB_OK);
}

void RedoButtonPaintHandler()
{
	HDC hdc;
	PAINTSTRUCT ps;
	hdc = BeginPaint(hRedoButton, &ps);

	Gdiplus::Graphics graphics(hdc);
	Gdiplus::SolidBrush solidBrush1(Gdiplus::Color(255, 0, 0, 0));
	graphics.FillRectangle(&solidBrush1, 0, 0, 35, 20);


	Gdiplus::FontFamily  fontFamily(L"Microsoft YaHei");
	Gdiplus::Font        font(&fontFamily, 12, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
	Gdiplus::PointF      pointF(0.0f, 2.0f);
	Gdiplus::SolidBrush  solidBrush2(Gdiplus::Color(255, 255, 255, 255));
	if (TabState == 2) 
	{
		graphics.DrawString(L"重来", -1, &font, pointF, &solidBrush2);
	}
	graphics.ReleaseHDC(hdc);

	EndPaint(hRedoButton, &ps);
}
void RedoButtonLButtonHandler()
{
	RedoHandler();
	//MessageBox(NULL, "redo", "dd", MB_OK);
}

LRESULT CALLBACK CloseButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			CloseButtonPaintHandler();
			return 0;
		}	
	case WM_MOUSEMOVE:
		{
			CloseButtonMouseMoveHandler();
			return 0;
		}
	case WM_MOUSELEAVE:
		{
			CloseButtonMouseLeaveHandler();
			return 0;
		}
	case WM_MOUSEHOVER:
		{
			CloseButtonMouseOverHandler();
			return 0;
		}
	case WM_LBUTTONUP:
		{
			CloseButtonLButtonUphandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK TrackingButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			TrackingButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			TrackingButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK BehaviorButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			BehaviorButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			BehaviorButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK ExceptionButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			ExceptionButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			ExceptionButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK OpenVidoeButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			OpenVideoButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			OpenVideoButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK PauseButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			PauseButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			PauseButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK ShowButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			ShowButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			ShowButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK BehavGetButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			
			BehavGetButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			BehavGetButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK BehavDectButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			
			BehavDectButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			BehavDectButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK BehavClsButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			BehavClsButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			BehavClsButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK TrainButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			TrainButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			TrainButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK ExcepDectButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			ExcepDectButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			ExcepDectButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}
LRESULT CALLBACK RedoButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{	
	case WM_PAINT:
		{
			RedoButtonPaintHandler();
			return 0;
		}	

	case WM_LBUTTONUP:
		{
			RedoButtonLButtonHandler();
		}

	default:
		return CallWindowProc(OriginProc, hwnd, message, wParam, lParam);
	}
}