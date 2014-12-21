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
#include "Tracking.h"

extern HWND hMainWindow;
extern HWND hPauseButton;


CvCapture* pcapture = NULL;
IplImage* pFrameImg = NULL;
IplImage* frameImgBackup = NULL;
ClipResult clipResult = {0, 0, 0, 0, NULL};
LPClipResult pClipResult = NULL;


int count_center_point = 0;
MyPoint cen_point[1000000];

int paused = 0;
CvPoint pt1 = cvPoint(0,0);    
CvPoint pt2 = cvPoint(0,0);    
bool is_selecting = false;  

MyPoint carPostion = {-1, -1, 0};



void cvMouseCallback(int mouseEvent,int x,int y,int flags,void* param)    
{    
    switch(mouseEvent)    
    {    
    case CV_EVENT_LBUTTONDOWN:    
        pt1          = cvPoint(x, y);    
        pt2          = cvPoint(x, y);  
        is_selecting = true;    

        break;    
    case CV_EVENT_MOUSEMOVE:   
		if(is_selecting)  
            pt2      = cvPoint(x, y);  
		
        break;    
    case CV_EVENT_LBUTTONUP:    
        pt2           = cvPoint(x,y);    
        is_selecting  = false;   
		cvSetImageROI(pFrameImg, cvRect(pt1.x, pt1.y, std::abs(pt2.x - pt1.x), std::abs(pt2.y - pt1.y)));  
		IplImage* dst = cvCreateImage(cvSize(std::abs(pt2.x-pt1.x), std::abs(pt2.y-pt1.y)),  
                            IPL_DEPTH_8U,  
                            pFrameImg->nChannels);  
		cvCopy(pFrameImg, dst, 0);

		clipResult.x      = (pt1.x + pt2.x) / 2;
		clipResult.y      = (pt1.y + pt2.y) / 2;
		clipResult.width  = std::abs(pt2.x - pt1.x);
		clipResult.height = std::abs(pt2.y - pt1.y);
		clipResult.img    = dst;
		pClipResult       = &clipResult;

		cvResetImageROI(pFrameImg);  
		
		cvNamedWindow("newimg", 1);
		cvMoveWindow("newimg", 0, 0);
		cvShowImage("newimg", dst);
        break;    
    }    
    return;    
}  

VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD)
{
	IplImage* tempImg = cvQueryFrame(pcapture);
	pFrameImg = cvCreateImage(cvSize(600, 360), IPL_DEPTH_8U, tempImg->nChannels);   

	cvResize(tempImg, pFrameImg, CV_INTER_LINEAR);
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
		count_center_point++;
	}
	cvShowImage("video", pFrameImg);
}

void TrackingOpenHandler()
{
	pcapture = cvCreateFileCapture(Utils::GetNativeFile());
	
	cvNamedWindow("video",  CV_WINDOW_AUTOSIZE);
	cvMoveWindow("video",  0, 0);
	Utils::SetWindow("video", hMainWindow, 35, 120);

	SetTimer(hMainWindow, ID_TIMER, 30, TimerProc);  
}
void TrackingPauseHandler()
{
	if (paused) 
	{
		paused = 0;
		SetTimer(hMainWindow, ID_TIMER, 30, TimerProc); 
		InvalidateRect(hPauseButton, NULL, TRUE);
		cvSetMouseCallback("video", NULL);
	}
	else 
	{
		paused = 1;
		KillTimer(hMainWindow, ID_TIMER);
		InvalidateRect(hPauseButton, NULL, TRUE);
		cvSetMouseCallback("video", cvMouseCallback);
	}
}
void TrackingShowHandler()
{
	IplImage* pTrackImg = cvCreateImage(cvGetSize(frameImgBackup), 8, 3);
	cvZero(pTrackImg);
	CvScalar color = {0, 255, 0};
	cvNamedWindow("Track", 1);
	for (int i = 0; i < count_center_point - 1; i++) 
	{
		cvLine(pTrackImg, cen_point[i].point, cen_point[i + 1].point, color);
		cvShowImage("Track", pTrackImg);
	}

	std::ofstream fs;
	fs.open("out.txt", std::fstream::out);
	while (count_center_point > 0) 
	{
		int x = cen_point[count_center_point - 1].x;
		int y = cen_point[count_center_point - 1].y;
		fs << "(" << x << ", " <<  y << ")" << std::endl;
		count_center_point--;
		
	}
	fs.close();
}

void DestroyTracking()
{
	KillTimer(hMainWindow, ID_TIMER);
	memset(cen_point, 0, sizeof(cen_point));
	//cvDestroyWindow("video");
	cvDestroyAllWindows();
	pcapture           = NULL;
	pFrameImg          = NULL;
	frameImgBackup     = NULL;
	pClipResult        = NULL;

	count_center_point = 0;
	paused             = 0;
	pt1                = cvPoint(0, 0);
	pt2                = cvPoint(0, 0);
	is_selecting       = 0;
	carPostion.x       = -1;
	carPostion.y       = -1;
	carPostion.value   = 0;
	clipResult.x       = 0;
	clipResult.y       = 0;
	clipResult.width   = 0;
	clipResult.height  = 0;
	clipResult.img     = NULL;
}



