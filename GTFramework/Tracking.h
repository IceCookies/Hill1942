#pragma once

#define ID_TIMER         1  
#define TEMPLATE_INC     6
#define CAR_POS_INIT_X  -1
#define VIDEO_WIDTH     600
#define VIDEO_HEIGHT    360


VOID CALLBACK TimerProc(HWND, UINT, UINT, DWORD);  
void TrackingOpenHandler();
void TrackingPauseHandler();
void TrackingShowHandler();

void DestroyTracking();