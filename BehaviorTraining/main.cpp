#include <Windows.h>
#include <fstream>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>


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
               cs.val[0] = 255-cs.val[0];             //对指定像素的RGB值进行重新设定  
               cs.val[1] = 255-cs.val[1]; 
               cs.val[2] = 255-cs.val[2];  
               cvSet2D(item, i, j, cs);    //将改变的像素保存到图片中 
           }
    }
}

/*提取特征*/
void training(const char* bgName, const char* targetName)
{
    int    flag;
	int    sample[10][10];

    int bottom, top, left, right;
	
	IplImage *bg    = cvLoadImage(bgName);
    IplImage *model = cvLoadImage(targetName);

	IplImage *src = cvCreateImage(cvSize(bg->width, bg->height), IPL_DEPTH_8U, bg->nChannels);
	cvAbsDiff(model, bg, src);  //获取轨迹
	reverseImgColor(src);       //反转图片颜色
	
	//cvShowImage("轨迹图片", src);
	//cvWaitKey(2000);

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

    for(int i = width - 1;i > 0; i--)
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
	std::ofstream outFile("train.txt", std::ios::out | std::ios::app);
	for(int i = 0; i < 10; i++)
	{
		for(int j = 0; j < 10; j++)
		{
			outFile << sample[i][j] << ",";
		}
	}
	outFile << std::endl;
	outFile.close();
}


int main(int argc, char** argv)
{
	WIN32_FIND_DATA ffd;

	if(argc != 3)
	{
		printf(TEXT("\nUsage: %s <bg-image name> <directory name>\n"), argv[0]);
		return -1;
	}

	std::string szBgName   = argv[1];
	std::string szDirName1 = argv[2];
	std::string szDirName = szDirName1 + "\\*";
	std::string trainDataName[4096];

	HANDLE hFind = FindFirstFile(szDirName.c_str(), &ffd);

	int i = 0;
	do
	{
		if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			std::string temp = ffd.cFileName;
			trainDataName[i] = szDirName1 + "\\" + temp;
			i++;
		}	
	}
	while (FindNextFile(hFind, &ffd) != 0);
	FindClose(hFind);



	for (int j = 0; j < i; j++) 
	{
		training(szBgName.c_str(), trainDataName[j].c_str());
	}

	std::cout << "Trainning End!" << std::endl;
	system("pause");

	return 0;
}




