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
#include "BehaviorDect.h"
#include "Tracking.h"

extern HWND hMainWindow;
extern HWND hPauseButton;


CvCapture*   pcapture       = NULL;
IplImage*    pFrameImg      = NULL;
IplImage*    pOldImage      = NULL;
ClipResult   clipResult     = {0, 0, 0, 0, NULL};
LPClipResult pClipResult    = NULL;



MyPoint cen_point[1000000];
int     count_center_point  = 0;

int     paused       = 0;
CvPoint pt1          = cvPoint(0,0);    
CvPoint pt2          = cvPoint(0,0);    
bool    is_selecting = false;  
bool    is_firstFram = true;

MyPoint carPostion   = {-1, -1, 0};

void TrackingRecognize(const char* trainDataName)
{
	float labels[15] = {1.0, 2.0, 3.0,
		                1.0, 2.0, 3.0,
		                1.0, 2.0, 3.0,
		                1.0, 2.0, 3.0,
		                1.0, 2.0, 3.0};
	cv::Mat labelsMat(15, 1, CV_32FC1, labels);     //1_6

	float trainingData[15][100];  

	std::ifstream trainData(trainDataName, std::ios::in);
	for(int i = 0; i < 15; i++)
	{
		std::string str;
		std::getline(trainData, str);
		std::istringstream strstream(str);
		for (int j = 0; j < 100; j++) 
		{
			strstream >> trainingData[i][j];
			char c;
			strstream >> c;
		}	
	}
	trainData.close();
	cv::Mat trainingDataMat(15, 100, CV_32FC1, trainingData);

	// step 2: 设置SVM参数
	CvSVMParams params;
	params.svm_type    = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);

	// step 3:  训练
	CvSVM SVM;
	SVM.train(trainingDataMat, labelsMat, cv::Mat(), cv::Mat(), params);

	// step 4:  识别
	std::ifstream inputFile("charactor.txt", std::ios::in);
	float charactorOfPicture[100];  //保存图片的特征
	for(int i = 0; i < 100; i++)
	{
		inputFile >> charactorOfPicture[i];
		char c;
		inputFile >> c;
	}
	inputFile.close();

	cv::Mat sampleMat(100, 1 , CV_32FC1, charactorOfPicture);
	int response = SVM.predict(sampleMat);  //1_6

	switch(response)
	{
	case 1:
		MessageBox(NULL, "自北向西转", "轨迹识别", MB_OK); 

		break;
	case 2:
		MessageBox(NULL, "直行", "轨迹识别", MB_OK); 

		break;
	case 3:
		MessageBox(NULL, "自西向南转", "轨迹识别", MB_OK); 
		break;
	}
}


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
	//检测视频是否已播放完毕
	IplImage* tempImg = cvQueryFrame(pcapture);
	if (tempImg == NULL) 
	{
		KillTimer(hMainWindow, ID_TIMER);
		return;
	}

	//把视频大小固定为600 * 360的大小
	pFrameImg = cvCreateImage(cvSize(VIDEO_WIDTH, VIDEO_HEIGHT), IPL_DEPTH_8U, tempImg->nChannels);   
	cvResize(tempImg, pFrameImg, CV_INTER_LINEAR);
	
	if (pClipResult != NULL) 
	{
		int width    = pClipResult->width;            //截图的大小
		int height   = pClipResult->height;
		int x        = pClipResult->x - width  / 2;   //截图的左上角坐标
		int y        = pClipResult->y - height / 2;
		IplImage* templateImg = pClipResult->img;     //截图的内容
		
		//得到搜索目标的左上角 x，y 坐标
		int xTarget;                                //
		int yTarget;
		if (carPostion.x != CAR_POS_INIT_X) 
		{
			xTarget = carPostion.x - TEMPLATE_INC;
			yTarget = carPostion.y - TEMPLATE_INC;
		}
		else 
		{
			xTarget = x - TEMPLATE_INC;
			yTarget = y - TEMPLATE_INC;	
		}
		//判断搜索目标是否超出视频区域
		if (xTarget <= 0 || 
			yTarget <= 0 ||
			xTarget + width  + 2 * TEMPLATE_INC  >= VIDEO_WIDTH  - 1 || 
			yTarget + height + 2 * TEMPLATE_INC  >= VIDEO_HEIGHT - 1 )
		{
			KillTimer(hMainWindow, ID_TIMER);
			return;
		}
		cvSetImageROI(pFrameImg, cvRect(xTarget, yTarget, width + 2 * TEMPLATE_INC, height + 2 * TEMPLATE_INC));  
		IplImage* target = cvCreateImage(cvSize(width + 2 * TEMPLATE_INC, height + 2 * TEMPLATE_INC),  
                                         IPL_DEPTH_8U,  
                                         pFrameImg->nChannels);  
		cvCopy(pFrameImg, target, 0);
		cvResetImageROI(pFrameImg);  

		int       resultWidth     = target->width  - templateImg->width  + 1;
		int       resultHeight    = target->height - templateImg->height + 1;
		IplImage* resultImg       = cvCreateImage(cvSize(resultWidth, resultHeight), 32, 1);
		cvMatchTemplate(target, templateImg, resultImg, CV_TM_CCOEFF);

		double    minValue;
	    double    maxValue;
		CvPoint   minPoint;
		CvPoint   maxPoint;
		cvMinMaxLoc(resultImg, &minValue, &maxValue, &minPoint, &maxPoint);

		carPostion.x                    = xTarget + maxPoint.x;
		carPostion.y                    = yTarget + maxPoint.y;
		cen_point[count_center_point].x = carPostion.x + width  / 2;
		cen_point[count_center_point].y = carPostion.y + height / 2;
		count_center_point++;
	}
	cvShowImage("video", pFrameImg);
	if (!is_firstFram) 
	{
		cvReleaseImage(&pOldImage);
		pOldImage = pFrameImg;
	}
	if (is_firstFram) 
	{
		pOldImage    = pFrameImg;
		is_firstFram = false;
	}
	//pFrameImg = NULL;
}

void TrackingOpenHandler()
{
	char* filename = Utils::GetNativeFile();
	if (filename == 0)
	{
		return;
	}
	pcapture = cvCreateFileCapture(filename);
	
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
	KillTimer(hMainWindow, ID_TIMER);

	CvScalar color = {0, 0, 255};
	for (int i = 0; i < count_center_point - 1; i++) 
	{
		cvLine(pFrameImg, cen_point[i].point, cen_point[i + 1].point, color);
		cvShowImage("video", pFrameImg);
	}

	IplImage* pTrackImg = cvLoadImage("Assets\\TrackingRoad.png");
	CvScalar color2 = {0, 0, 0};
	for (int i = 0; i < count_center_point - 1; i++) 
	{
		cvLine(pTrackImg, cen_point[i].point, cen_point[i + 1].point, color2, 2);
	}
	training("Assets\\TrackingRoad.png", 0, pTrackImg);
	TrackingRecognize("TrackingTrain.txt");
	//cvSaveImage("TrackingImage.png", pTrackImg);

/*
	std::ofstream fs;
	fs.open("out.txt", std::fstream::out);
	while (count_center_point > 0) 
	{
		int x = cen_point[count_center_point - 1].x;
		int y = cen_point[count_center_point - 1].y;
		fs << "(" << x << ", " <<  y << ")" << std::endl;
		count_center_point--;
		
	}
	fs.close();*/
}

void DestroyTracking()
{
	KillTimer(hMainWindow, ID_TIMER);
	memset(cen_point, 0, sizeof(cen_point));
	cvDestroyAllWindows();
	if (pFrameImg) 
		cvReleaseImage(&pFrameImg);
	/*if (pOldImage) 
		cvReleaseImage(&pOldImage);*/
	cvReleaseCapture(&pcapture);
	pcapture           = NULL;
	pFrameImg          = NULL;
	pOldImage          = NULL;
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



