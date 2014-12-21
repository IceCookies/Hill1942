#include <Windows.h>
#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>

#include "OpenFileDialog.h"
#include "Utils.h"


Utils::Utils(void)
{
}


Utils::~Utils(void)
{
}

void Utils::SetWindow(char* name, HWND hParent, int x, int y) 
{
	HWND hVideo = (HWND)cvGetWindowHandle(name);
	HWND hVideoParent = GetParent(hVideo);
		
	if(hVideoParent != NULL)
	{
		SetWindowLong(hVideoParent,GWL_STYLE,((GetWindowLong(hVideoParent,GWL_STYLE) & ~WS_POPUP) & ~WS_OVERLAPPEDWINDOW ) | WS_CHILD );
		//SetWindowLong(hVideoParent,GWL_EXSTYLE,GetWindowLong(hVideoParent,GWL_EXSTYLE) & ~WS_EX_CLIENTEDGE  & ~WS_EX_WINDOWEDGE  );
		POINT point;
		point.x = x;
		point.y = y;
		bool result = ScreenToClient(hParent, &point);
		if(result)
		{
			SetWindowPos(hVideoParent,HWND_TOP, point.x, point.y, 0, 0, SWP_SHOWWINDOW );

		}
		SetParent(hVideoParent, hParent);

		//SetWindowPos(hVideoParent,HWND_TOP, 0, 0, 100, 100, SWP_SHOWWINDOW);
	}
}

char* Utils::GetNativeFile() 
{
	char currentDir[100];
	GetCurrentDirectory(100, currentDir);
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
			SetCurrentDirectory(currentDir);
			return 0;
		}				
		else 
		{
			SetCurrentDirectory(currentDir);
			return fileName;
		}
	}
	else
	{
		MessageBox(NULL, "Can not Open file", "Error", MB_OK);
		SetCurrentDirectory(currentDir);
		return 0;
	}
}

BOOL Utils::SaveBitmapToFile(LPCTSTR lpszFilePath, HBITMAP hBm)
{
    //  ����λͼ�ļ���ͷ
    BITMAPFILEHEADER bmfh;
    //  ����λͼ��Ϣ��ͷ
    BITMAPINFOHEADER bmih;

    //  ��ɫ�峤��
    int nColorLen = 0;
    //  ��ɫ���С
    DWORD dwRgbQuadSize = 0;
    //  λͼ��С
    DWORD dwBmSize = 0;
    //  �����ڴ��ָ��
    HGLOBAL    hMem = NULL;

    LPBITMAPINFOHEADER     lpbi;

    BITMAP bm;

    HDC hDC;

    HANDLE hFile = NULL;

    DWORD dwWritten;

    GetObject(hBm, sizeof(BITMAP), &bm);

    bmih.biSize    = sizeof(BITMAPINFOHEADER);    // ���ṹ��ռ���ֽ�
    bmih.biWidth    = bm.bmWidth;            // λͼ��
    bmih.biHeight    = bm.bmHeight;            // λͼ��
    bmih.biPlanes    = 1;
    bmih.biBitCount        = bm.bmBitsPixel;    // ÿһͼ�ص�λ��
    bmih.biCompression    = BI_RGB;            // ��ѹ��
    bmih.biSizeImage        = 0;  //  λͼ��С
    bmih.biXPelsPerMeter    = 0;
    bmih.biYPelsPerMeter    = 0;
    bmih.biClrUsed        = 0;
    bmih.biClrImportant    = 0;

    //  ����λͼͼ����������С 
    dwBmSize = 4 * ((bm.bmWidth * bmih.biBitCount + 31) / 32) * bm.bmHeight;

    //  ���ͼ��λ <= 8bit�����е�ɫ��
    if (bmih.biBitCount <= 8)
    {
        nColorLen = (1 << bm.bmBitsPixel);
    }

    //  �����ɫ���С
    dwRgbQuadSize = nColorLen * sizeof(RGBQUAD);

    //  �����ڴ�
    hMem = GlobalAlloc(GHND, dwBmSize + dwRgbQuadSize + sizeof(BITMAPINFOHEADER));

    if (NULL == hMem)
    {
        return FALSE;
    }

    //  �����ڴ�
    lpbi = (LPBITMAPINFOHEADER)GlobalLock(hMem);

    //  ��bmih�е�����д�������ڴ���
    *lpbi = bmih;


    hDC= GetDC(NULL);

    //  ��λͼ�е�������bits����ʽ����lpData�С�
    GetDIBits(hDC, hBm, 0, (DWORD)bmih.biHeight, (LPSTR)lpbi+sizeof(BITMAPINFOHEADER)+dwRgbQuadSize, (BITMAPINFO *)lpbi, (DWORD)DIB_RGB_COLORS);
    
    bmfh.bfType = 0x4D42;  // λͼ�ļ����ͣ�BM
    bmfh.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize + dwBmSize;  // λͼ��С
    bmfh.bfReserved1 = 0;
    bmfh.bfReserved2 = 0;
    bmfh.bfOffBits    = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwRgbQuadSize;  // λͼ�������ļ�ͷ����ƫ����

    //  �����������д���ļ���

    hFile = CreateFile(lpszFilePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    
    if (INVALID_HANDLE_VALUE == hFile)
    {
        return FALSE;
    }

    //  д��λͼ�ļ�ͷ
    WriteFile(hFile, (LPSTR)&bmfh, sizeof(BITMAPFILEHEADER), (DWORD *)&dwWritten, NULL);
    //  д��λͼ����
    WriteFile(hFile, (LPBITMAPINFOHEADER)lpbi, bmfh.bfSize - sizeof(BITMAPFILEHEADER), (DWORD *)&dwWritten, NULL);

    GlobalFree(hMem);
    CloseHandle(hFile);

    return TRUE;
}



