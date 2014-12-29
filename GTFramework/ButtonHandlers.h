#pragma once


void MinButtonPaintHandler();
void MinButtonMouseOverHandler();
void MinButtonMouseLeaveHandler();
void MinButtonMouseMoveHandler();
void MinButtonLButtonUphandler();

void MaxButtonPaintHandler();

void CloseButtonPaintHandler();
void CloseButtonMouseOverHandler();
void CloseButtonMouseLeaveHandler();
void CloseButtonMouseMoveHandler();
void CloseButtonLButtonUphandler();

void TrackingButtonPaintHandler();
void TrackingButtonLButtonHandler();

void BehaviorButtonPaintHandler();
void BehaviorButtonLButtonHandler();

void ExceptionButtonPaintHandler();
void ExceptionButtonLButtonHandler();

void OpenVideoButtonPaintHandler();
void OpenVideoButtonLButtonHandler();

void PauseButtonPaintHandler();
void PauseButtonLButtonHandler();

void ShowButtonPaintHandler();
void ShowButtonLButtonHandler();

void BehavGetButtonPaintHandler();
void BehavGetButtonLButtonHandler();

void BehavDectButtonPaintHandler();
void BehavDectButtonLButtonHandler();

void BehavClsButtonPaintHandler();
void BehavClsButtonLButtonHandler();

void TrainButtonPaintHandler();
void TrainButtonLButtonHandler();

void ExcepDectButtonPaintHandler();
void ExcepDectButtonLButtonHandler();

void RedoButtonPaintHandler();
void RedoButtonLButtonHandler();

LRESULT CALLBACK CloseButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TrackingButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BehaviorButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ExceptionButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OpenVidoeButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PauseButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ShowButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BehavGetButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BehavDectButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK BehavClsButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TrainButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ExcepDectButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RedoButtonProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);