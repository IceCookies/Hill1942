#include <windows.h>
#include <GdiPlus.h>

#include <opencv\highgui.h>
#include <opencv\cv.h>
#include <opencv\cvaux.h>

#include "Utils.h"
#include "Button.h"
#include "ButtonHandlers.h"
#include "DrawBoard.h"
#include "DrawBoardHandlers.h"
#include "Init.h"

extern HWND hMainWindow;
extern HWND hMaxButton;
extern HWND hMinButton;
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

extern HWND hTrackGetButton;
extern HWND hTrackDectButton;
extern HWND hTrackRedoButton;

extern MyPoint cen_point[1000000];

void InitButtons(HWND hWnd)
{
	hCloseButton     = CreateButton(30,  25, hWnd, NULL, CloseButtonProc);
	hTrackingButton  = CreateButton(60,  40, hWnd, NULL, TrackingButtonProc);
	hBehaviorButton  = CreateButton(110, 40, hWnd, NULL, BehaviorButtonProc);
	hExceptionButton = CreateButton(110, 40, hWnd, NULL, ExceptionButtonProc);
	hOpenVideoButton = CreateButton(30,  20, hWnd, NULL, OpenVidoeButtonProc);
	hPauseButton     = CreateButton(30,  20, hWnd, NULL, PauseButtonProc);
	hShowButton      = CreateButton(30,  20, hWnd, NULL, ShowButtonProc);

	hBehavGetButton  = CreateButton(30,  20, hWnd, NULL, BehavGetButtonProc);
	hBehavDectButton = CreateButton(30,  20, hWnd, NULL, BehavDectButtonProc);
	hBehavClsButton  = CreateButton(30,  20, hWnd, NULL, BehavClsButtonProc);

	hTrainButton     = CreateButton(30,  20, hWnd, NULL, TrainButtonProc);
	hExcepDectButton = CreateButton(30,  20, hWnd, NULL, ExcepDectButtonProc);
	hRedoButton      = CreateButton(30,  20, hWnd, NULL, RedoButtonProc);

	//hTrackGetButton  = CreateButton(30,  20, hWnd, NULL, TrainButtonProc);
	//hTrackDectButton = CreateButton(30,  20, hWnd, NULL, TrainButtonProc);
	//hTrackRedoButton = CreateButton(30,  20, hWnd, NULL, TrainButtonProc);
}

void InitDrawBoards(HWND hWnd)
{
	//InitDrawBoard();
	hBehaviorBoard   = CreateBoard(600, 360, hWnd, NULL, BehabiorBoardProc);
	hExceptionBoard  = CreateBoard(600, 360, hWnd, NULL, ExceptionBoardProc);
}

void InitState()
{
	ShowWindow(hBehavGetButton,    FALSE);
	ShowWindow(hBehavDectButton,   FALSE);
	ShowWindow(hBehavClsButton,    FALSE);
	ShowWindow(hTrainButton,       FALSE);
	ShowWindow(hExcepDectButton,   FALSE);
	ShowWindow(hRedoButton,        FALSE);
	ShowWindow(hBehaviorBoard,     FALSE);
	ShowWindow(hExceptionBoard,    FALSE);

	EnableWindow(hBehavGetButton,  FALSE);
	EnableWindow(hBehavDectButton, FALSE);
	EnableWindow(hBehavClsButton,  FALSE);
	EnableWindow(hTrainButton,     FALSE);
	EnableWindow(hExcepDectButton, FALSE);
    EnableWindow(hRedoButton,      FALSE);
	EnableWindow(hBehaviorBoard,   FALSE);
	EnableWindow(hExceptionBoard,  FALSE);
}

void InitTracking()
{
	memset(cen_point, 0, sizeof(cen_point));
}

