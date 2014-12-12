#include <windows.h>
#include <GdiPlus.h>
#include <string.h>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>

#include "OpenFileDialog.h"
#include "Utils.h"
#include "Button.h"

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

#define ID_TIMER    1  

WNDPROC OrginProc;
HINSTANCE hApp;
HWND      hMainWindow;

HWND hTestBtn;
HWND hPauseBtn;

Button* pauseBtn;

CvCapture* pcapture = NULL;
IplImage* pFrameImg = NULL;
IplImage* frameImgBackup = NULL;
IplImage* dst = NULL;
char* szClassName = "MainWindow";
int count_center_point;
CvPoint cen_point[1000000];

int paused = 0;
CvPoint pt1 = cvPoint(0,0);    
CvPoint pt2 = cvPoint(0,0);    
bool is_selecting = false;  
cv::Rect selection;

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
VOID    CALLBACK TimerProc(HWND, UINT, UINT, DWORD);  

void cvMouseCallback(int mouseEvent,int x,int y,int flags,void* param)    
{    
	/*if (is_selecting) 
	{
		selection.x = MIN(x, pt1.x);
        selection.y = MIN(y, pt1.y);
        selection.width = std::abs(x - pt1.x);
        selection.height = std::abs(y - pt1.y);
        selection &= cv::Rect(0, 0, pFrameImg.cols, pFrameImg.rows);
	}*/
    switch(mouseEvent)    
    {    
    case CV_EVENT_LBUTTONDOWN:    
        pt1 = cvPoint(x,y);    
        pt2 = cvPoint(x,y);  
		selection = cv::Rect(x ,y ,0, 0);
        is_selecting = true;    
        break;    
    case CV_EVENT_MOUSEMOVE:   
		if(is_selecting)  
            pt2 = cvPoint(x,y);  
		//cvRectangle(pFrameImg, pt1, pt2, CV_RGB(255, 0, 0));
		//cvShowImage("video", pFrameImg);
		//cvShowImage("video", frameImgBackup);
        break;    
    case CV_EVENT_LBUTTONUP:    
        pt2 = cvPoint(x,y);    
        is_selecting = false;   
		//cvRectangle(pFrameImg, pt1, pt2, CV_RGB(255, 0, 0));
		cvSetImageROI(pFrameImg, cvRect(pt1.x, pt1.y, std::abs(pt2.x - pt1.x), std::abs(pt2.y - pt1.y)));  
		//cvNot(pFrameImg, pFrameImg);
		//cvSetImageROI(pFrameImg, cvRect(0,0,0.5*pFrameImg->width,0.5*pFrameImg->height));  
		dst = cvCreateImage(cvSize(std::abs(pt2.x-pt1.x), std::abs(pt2.y-pt1.y)),  
            IPL_DEPTH_8U,  
            pFrameImg->nChannels);  
		cvCopy(pFrameImg, dst, 0);  
		cvResetImageROI(pFrameImg);  
		//cvCopy(pFrameImg, outImg);
       // cvSaveImage(save_path, img);    
       // cvResetImageROI(pFrameImg);  
		
		cvNamedWindow("newimg", 1);
		cvMoveWindow("newimg", 0, 0);
		Utils::SetWindow("newimg", hMainWindow, 400, 50);
		cvShowImage("newimg", dst);
        break;    
    }    
    return;    
}  

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)  
{
	pFrameImg = cvQueryFrame(pcapture);
	if (pFrameImg == NULL)
		KillTimer(hMainWindow, ID_TIMER);

	//frameImgBackup = cvCreateImage(cvSize(pFrameImg->width, pFrameImg->height), IPL_DEPTH_8U, pFrameImg->nChannels);
	//cvCopy(pFrameImg, frameImgBackup, 0);

	
	cvShowImage("video", pFrameImg);

}  

void testBtnHandler() 
{
	pcapture = cvCreateFileCapture(Utils::GetNativeFile());
	
	cvNamedWindow("video", 1);
	cvMoveWindow("video", 0, 0);
	Utils::SetWindow("video", hMainWindow, 5, 50);

	SetTimer(hMainWindow, ID_TIMER, 30, TimerProc);  
}

void PauseBtnHandler()
{
	if (pauseBtn->GetState() == BUTTON_RELEASE) 
	{
		paused = 1;
		KillTimer(hMainWindow, ID_TIMER);
		pauseBtn->SetContent(L"Resume");
		pauseBtn->SetState(BUTTON_PRESSED);
		cvSetMouseCallback("video", cvMouseCallback);
	}
	else 
	{
		paused = 0;
		SetTimer(hMainWindow, ID_TIMER, 30, TimerProc);  
		cvWaitKey(30);
		pauseBtn->SetContent(L"Pause");
		pauseBtn->SetState(BUTTON_RELEASE);
		cvSetMouseCallback("video", NULL);
	}
	
	
}


int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{

	Gdiplus::GdiplusStartupInput gdiInput;
	ULONG_PTR gdiplusStartupToken;
	Gdiplus::GdiplusStartup(&gdiplusStartupToken,&gdiInput,NULL);

	memset(cen_point, 0, sizeof(cen_point));


	WNDCLASSEX wndclass;
	wndclass.cbSize        = sizeof(wndclass);
	wndclass.style         = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc   = WinProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = NULL;
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szClassName;
	wndclass.hIconSm       = NULL;

	RegisterClassEx(&wndclass);

	HWND hwnd = ::CreateWindowEx(
		0,
		szClassName,
		"My First Window",
		WS_OVERLAPPEDWINDOW  ,
		100,
		100,
		640,
		480,
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
			
			hTestBtn = (new Button(10, 10, 120, 30, L"Open", hWnd, NULL, testBtnHandler))->Create();
			pauseBtn = new Button(150, 10, 120, 30, L"Pause",hWnd, NULL, PauseBtnHandler);
			hPauseBtn = pauseBtn->Create();
			return 0;
		}

	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;
			hdc = BeginPaint(hWnd, &ps);

			GetWindowRect(hWnd, &rect);


			EndPaint(hWnd, &ps);
			return 0;
		}



	case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}



