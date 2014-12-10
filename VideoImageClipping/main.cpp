#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <GdiPlus.h>
#include <string.h>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>
#include "OpenFileDialog.h"

#include "Button.h"

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;
using namespace std;

WNDPROC OrginProc;
HINSTANCE hApp;
HWND      hMainWindow;

HWND hTestBtn;
HWND hPauseBtn;

Button* pauseBtn;

CvPoint pt1 = cvPoint(0,0);    
CvPoint pt2 = cvPoint(0,0);    
bool is_selecting = false;   

int g_slider_position = 0;

CvCapture* g_Capture = NULL;

char* szClassName = "MainWindow";
int count_center_point;
CvPoint cen_point[1000000];

int paused = 0;

IplImage* outImg = NULL;
IplImage* pFrameImg = NULL;


cv::Rect selection;


LRESULT CALLBACK WinProc(HWND, UINT, WPARAM, LPARAM);

char* GetNativeFile() 
{
	char* fileName;
	OpenFileDialog* openFileDialog = new OpenFileDialog();		
	openFileDialog->FilterIndex = 1;

	openFileDialog->Title = "Open Video File";

	if (openFileDialog->ShowDialog())
	{
		fileName = openFileDialog->FileName;
		if (fileName == 0)
		{
			MessageBox(NULL, "Can not Open file", "Warning", MB_OK);
			return 0;
		}				
		else 
		{
			return fileName;
		}
	}
	else
	{
		MessageBox(NULL, "Can not Open file", "Error", MB_OK);
		return 0;
	}
}

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
        break;    
    case CV_EVENT_LBUTTONUP:    
        pt2 = cvPoint(x,y);    
        is_selecting = false;   
		cvSetImageROI(pFrameImg, cvRect(pt1.x, pt1.y, std::abs(pt2.x-pt1.x), std::abs(pt2.y-pt1.y)));  
		//cvSetImageROI(pFrameImg, cvRect(0,0,0.5*pFrameImg->width,0.5*pFrameImg->height));  
		IplImage* dst = cvCreateImage(cvSize(std::abs(pt2.x-pt1.x), std::abs(pt2.y-pt1.y)),  
            IPL_DEPTH_8U,  
            pFrameImg->nChannels);  
		cvCopy(pFrameImg,dst,0);  
		cvResetImageROI(pFrameImg);  
		//cvCopy(pFrameImg, outImg);
       // cvSaveImage(save_path, img);    
       // cvResetImageROI(pFrameImg);  

		cvShowImage("new image", dst);
        break;    
    }    
    return;    
}  



void TestBtnHandler()
{
	cvNamedWindow( "Video", CV_WINDOW_AUTOSIZE );
	cvSetMouseCallback("Video", cvMouseCallback);    
	g_Capture = cvCreateFileCapture(GetNativeFile());
	//CvCapture* pCapture = cvCaptureFromFile(GetNativeFile());

	//IplImage* pFrameImg = NULL;

	cvShowImage("Video", pFrameImg);

	while(1) {
        pFrameImg = cvQueryFrame(g_Capture);
        if(pFrameImg == NULL)
			break;

        cvShowImage("Video", pFrameImg);

		
		

		if (paused)
		{
			cvWaitKey(0);
		}
		else
		{
			cvWaitKey(30);
		}

		
    }
}

void PauseBtnHandler()
{
	paused = 1;
	//pauseBtn-
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
			hTestBtn = (new Button(10, 10, 120, 30, L"GetVideo",hWnd, NULL, TestBtnHandler))->Create();
			Button* pauseBtn = new Button(10, 300, 120, 30, L"Pause",hWnd, NULL, PauseBtnHandler);
			pauseBtn->Create();
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



