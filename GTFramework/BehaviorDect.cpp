#include <windows.h>
#include <GdiPlus.h>
#include <string>
#include <fstream>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>

#include "OpenFileDialog.h"
#include "Utils.h"
#include "BehaviorDect.h"

extern HINSTANCE hApp;
extern HWND hMainWindow;
extern HWND hBehaviorBoard;

extern POINT startBehaviorPoint;
extern POINT endBehaviorPoint;

void CreateBoard()
{

}


/**
* 图片颜色反转
* @param item是要处理的图片
*
* @return
*/
void reverseImgColor(IplImage* item)
{
	CvScalar cs;             //声明像素变量  
	for(int i = 0; i < item->height; i++) 
	{
		for (int j = 0; j < item->width; j++)  
		{  
			cs = cvGet2D(item, i, j);   //获取像素  
			cs.val[0] = 255 - cs.val[0];             //对指定像素的RGB值进行重新设定  
			cs.val[1] = 255 - cs.val[1]; 
			cs.val[2] = 255 - cs.val[2];  
			cvSet2D(item, i, j, cs);    //将改变的像素保存到图片中 
		}
	}
}

/*提取特征*/
int training(const char* bgName, const char* targetName)
{
	int    flag;
	int    sample[10][10];

	int bottom, top, left, right;

	IplImage *bg    = cvLoadImage(bgName);
	IplImage *model = cvLoadImage(targetName);

	IplImage *src = cvCreateImage(cvSize(bg->width, bg->height), IPL_DEPTH_8U, bg->nChannels);
	cvAbsDiff(model, bg, src);  //获取轨迹
	reverseImgColor(src);       //反转图片颜色

	IplImage *img_binary = cvCreateImage(cvGetSize(src), src->depth, 1);
	IplImage *img_gray   = cvCreateImage(cvGetSize(src), src->depth, 1);
	int       height     = src->height;
	int       width      = src->width;

	cvCvtColor(src, img_gray, CV_BGR2GRAY);
	cvThreshold(img_gray, img_binary, 145, 255, CV_THRESH_BINARY);

	uchar*    ptr        = (uchar*)img_binary->imageData;

	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			flag = 0;
			if(ptr[i * img_binary->widthStep + j]== 0)
			{
				flag = 1;
				top  = i;
				break;
			}
		}
		if(flag)
		{
			break;
		}
	}

	for(int i = height - 1; i > 0; i--)
	{
		for(int j = 0 ; j < width; j++)
		{
			flag = 0;
			if(ptr[i * img_binary->widthStep + j] == 0)
			{
				flag   = 1;
				bottom = i;
				break;
			}
		}
		if(flag)
		{
			break;
		}
	}

	for(int i = 0; i < width; i++)
	{
		for(int j = 0; j < height; j++)
		{
			flag = 0;
			if(ptr[j * img_binary->widthStep + i] == 0)
			{
				flag = 1;
				left = i;
				break;
			}
		}
		if(flag)
		{
			break;
		}
	}

	for(int i = width - 1; i > 0; i--)
	{
		for(int j = 0; j < height; j++)
		{
			flag = 0;
			if(ptr[j * img_binary->widthStep + i] == 0)
			{
				flag  = 1;
				right = i;
				break;
			}
		}
		if(flag)
		{
			break;
		}
	}

	int w = (right  - left) / 10;
	int h = (bottom - top)  / 10;

	for(int p = 0; p < 10; p++)
	{
		for(int k = 0; k < 10; k++)
		{
			int count = 0;
			for(int t = top + k * h; t < top + (k + 1) * h; t++)
			{
				for(int r = left + p * w; r < left + (p + 1) * w; r++)
				{
					if(ptr[t * img_binary->widthStep + r] == 0)
					{
						count++;
					}
				}
			}
			if(count > 0)
			{
				sample[p][k] = 1;
			}
			else
			{
				sample[p][k] = 0;
			}
		}
	}

	//提取特征向量
	std::ofstream outFile("charactor.txt", std::ios::out);
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			outFile << sample[i][j] << ",";
		}
	}
	outFile << std::endl;
	outFile.close();

	return 1;
}

void recognize()
{
	float labels[30] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
		                1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
		                1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
		                1.0, 2.0, 3.0, 4.0, 5.0, 6.0,
		                1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
	cv::Mat labelsMat(30, 1, CV_32FC1, labels);     //1_6

	float trainingData[30][100];  

	std::ifstream trainData("train.txt", std::ios::in);
	for(int i = 0; i < 30; i++)
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
	cv::Mat trainingDataMat(30, 100, CV_32FC1, trainingData);

	// step 2: 设置SVM参数
	CvSVMParams params;
	params.svm_type = CvSVM::C_SVC;
	params.kernel_type = CvSVM::LINEAR;
	params.term_crit = cvTermCriteria(CV_TERMCRIT_ITER, 1000, 1e-6);

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
		if (startBehaviorPoint.y < endBehaviorPoint.y) 
		{
			MessageBox(NULL, "自北向西转", "轨迹识别", MB_OK); 
		}
		else
		{
			MessageBox(NULL, "自西向北转", "轨迹识别", MB_OK); 
		}

		break;
	case 2:
		if (startBehaviorPoint.y < endBehaviorPoint.y) 
		{
			MessageBox(NULL, "直行, 自北向南", "轨迹识别", MB_OK); 
		}
		else
		{
			MessageBox(NULL, "直行, 自南向北", "轨迹识别", MB_OK); 
		}

		break;
	case 3:
		if (startBehaviorPoint.y < endBehaviorPoint.y) 
		{
			MessageBox(NULL, "自北向东转", "轨迹识别", MB_OK); 
		}
		else
		{
			MessageBox(NULL, "自东向北转", "轨迹识别", MB_OK); 
		}
		break;
	case 4:
		if (startBehaviorPoint.y > endBehaviorPoint.y) 
		{
			MessageBox(NULL, "自南向西转", "轨迹识别", MB_OK); 
		}
		else
		{
			MessageBox(NULL, "自西向南转", "轨迹识别", MB_OK); 
		}

		break;
	case 5:
		if (startBehaviorPoint.x > endBehaviorPoint.x) 
		{
			MessageBox(NULL, "直行, 自东向西", "轨迹识别", MB_OK); 
		}
		else
		{
			MessageBox(NULL, "直行, 自西向东", "轨迹识别", MB_OK); 
		}
		break;
	case 6:	
		if (startBehaviorPoint.y > endBehaviorPoint.y) 
		{
			MessageBox(NULL, "自南向东转", "轨迹识别", MB_OK); 
		}
		else
		{
			MessageBox(NULL, "自东向南转", "轨迹识别", MB_OK); 
		}
		break;
	}
}

//提取按钮
void BehavGetHandler()
{
	if (training("Assets\\CrossRoad.png", "target.bmp")) 
	{
		MessageBox(NULL, "提取成功", "结果", MB_OK);
	}

}

//检测按钮
void BehavDectHandler()
{
	recognize();
}

//清除按钮
void BehavClsHandler()
{
	startBehaviorPoint.x = 0;
	startBehaviorPoint.y = 0;
	endBehaviorPoint.x   = 0;
	endBehaviorPoint.y   = 0;
	InvalidateRect(hBehaviorBoard, NULL, TRUE);
}

