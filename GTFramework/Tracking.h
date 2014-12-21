#pragma once

#define ID_TIMER    1  
#define TEMPLATE_INC 3

VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD);  
void TrackingOpenHandler();
void TrackingPauseHandler();
void TrackingShowHandler();

void DestroyTracking();