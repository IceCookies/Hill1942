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

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

#define ID_TIMER    1  
#define TEMPLATE_INC 3

WNDPROC OrginProc;
HINSTANCE hApp;
HWND      hMainWindow;

HWND hTestBtn;
HWND hPauseBtn;
HWND hShowBtn;

Button* pauseBtn;

CvCapture* pcapture = NULL;
IplImage* pFrameImg = NULL;
IplImage* frameImgBackup = NULL;
ClipResult clipResult = {0, 0, 0, 0, NULL};
LPClipResult pClipResult = NULL;
//IplImage* dst = NULL;
char* szClassName = "MainWindow";
int count_center_point = 0;
MyPoint cen_point[1000000];


int paused = 0;
CvPoint pt1 = cvPoint(0,0);    
CvPoint pt2 = cvPoint(0,0);    
bool is_selecting = false;  
cv::Rect selection;

MyPoint carPostion = {-1, -1, 0};

int test = 0;


int numFrames = 0;

LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);
VOID    CALLBACK TimerProc(HWND, UINT, UINT, DWORD);  

void cvMouseCallback(int mouseEvent,int x,int y,int flags,void* param)    
{    
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
        break;    
    case CV_EVENT_LBUTTONUP:    
        pt2 = cvPoint(x,y);    
        is_selecting = false;   
		cvSetImageROI(pFrameImg, cvRect(pt1.x, pt1.y, std::abs(pt2.x - pt1.x), std::abs(pt2.y - pt1.y)));  
		IplImage* dst = cvCreateImage(cvSize(std::abs(pt2.x-pt1.x), std::abs(pt2.y-pt1.y)),  
                            IPL_DEPTH_8U,  
                            pFrameImg->nChannels);  
		cvCopy(pFrameImg, dst, 0);
		clipResult.x = (pt1.x + pt2.x) / 2;
		clipResult.y = (pt1.y + pt2.y) / 2;
		clipResult.width = std::abs(pt2.x - pt1.x);
		clipResult.height = std::abs(pt2.y - pt1.y);
		clipResult.img = dst;

		pClipResult = &clipResult;

		cvResetImageROI(pFrameImg);  
		
		cvNamedWindow("newimg", 1);
		cvMoveWindow("newimg", 0, 0);
		Utils::SetWindow("newimg", hMainWindow, 800, 50);
		cvShowImage("newimg", dst);
        break;    
    }    
    return;    
}  

VOID CALLBACK TimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)  
{
	
	pFrameImg = cvQueryFrame(pcapture);
	numFrames++;
	if (pFrameImg == NULL) 
	{
		KillTimer(hMainWindow, ID_TIMER);
		return;
	}
	frameImgBackup = pFrameImg;
	if (pClipResult != NULL) 
	{

		

		int width = pClipResult->width;    //截图的大小
		int height = pClipResult->height;
		int x = pClipResult->x - width / 2;   //截图的左上角坐标
		int y = pClipResult->y - height / 2;
		IplImage* templateImg = pClipResult->img;  

		int xTemplate;
		int yTemplate;
		if (carPostion.x != -1) 
		{
			xTemplate = carPostion.x - TEMPLATE_INC;
			yTemplate = carPostion.y - TEMPLATE_INC;
		}
		else 
		{
			xTemplate = x - TEMPLATE_INC;
			yTemplate = y - TEMPLATE_INC;	
		}
		cvSetImageROI(pFrameImg, cvRect(xTemplate, yTemplate, width + 2 * TEMPLATE_INC, height + 2 * TEMPLATE_INC));  
		IplImage* target = cvCreateImage(cvSize(width + 2 * TEMPLATE_INC, height + 2 * TEMPLATE_INC),  
                                         IPL_DEPTH_8U,  
                                         pFrameImg->nChannels);  

		
		cvCopy(pFrameImg, target, 0);
		cvResetImageROI(pFrameImg);  
		int resultWidth = target->width - templateImg->width + 1;
		int resultHeight = target->height - templateImg->height + 1;
		IplImage* resultImg = cvCreateImage(cvSize(resultWidth, resultHeight), 32, 1);
		cvMatchTemplate(target, templateImg, resultImg, CV_TM_CCOEFF);
		double minValue, maxValue;
		CvPoint minPoint;
		CvPoint maxPoint;

		cvMinMaxLoc(resultImg, &minValue, &maxValue, &minPoint, &maxPoint);

		carPostion.x = xTemplate + maxPoint.x;
		carPostion.y = yTemplate + maxPoint.y;
		
		cen_point[count_center_point].x = carPostion.x + width / 2;
		cen_point[count_center_point].y = carPostion.y + height / 2;
			//cen_point[count_center_point].value = maxValue;
		count_center_point++;
		
			

			//cvShowImage("target", target);
			//cvShowImage("result", resultImg);

			//KillTimer(hMainWindow, ID_TIMER);
	}

	//frameImgBackup = cvCreateImage(cvSize(pFrameImg->width, pFrameImg->height), IPL_DEPTH_8U, pFrameImg->nChannels);
	//cvCopy(pFrameImg, frameImgBackup, 0);
	cvShowImage("video", pFrameImg);
}  

void testBtnHandler() 
{
	pcapture = cvCreateFileCapture(Utils::GetNativeFile());
	
	cvNamedWindow("video", 1);
	cvMoveWindow("video", 0, 0);
	Utils::SetWindow("video", hMainWindow, 0, 50);

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
		test = 1;
		pauseBtn->SetContent(L"Pause");
		pauseBtn->SetState(BUTTON_RELEASE);
		cvSetMouseCallback("video", NULL);
	}
	
	
}

void showBtnHandler() 
{
	IplImage* pTrackImg = cvCreateImage(cvGetSize(frameImgBackup), 8, 3);
	cvZero(pTrackImg);
	CvScalar color = {0, 255, 0};
	cvNamedWindow("Track", 1);
	for (int i = 0; i < count_center_point - 1; i++) 
	{
		cvLine(pTrackImg, cen_point[i].point, cen_point[i + 1].point, color);
		//cvWaitKey(100);
		cvShowImage("Track", pTrackImg);
	}

	std::fstream fs;
	fs.open("out.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	while (count_center_point > 0) 
	{
		int x = cen_point[count_center_point - 1].x;
		int y = cen_point[count_center_point - 1].y;
		fs << "(" << x << ", " <<  y << ")" << std::endl;
		count_center_point--;
		
	}
	fs.close();
	
	
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
			hShowBtn = (new Button(290, 10, 120, 30, L"Show", hWnd, NULL, showBtnHandler))->Create();
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



