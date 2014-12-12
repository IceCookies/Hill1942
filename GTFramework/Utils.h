#pragma once
class Utils
{
public:
	Utils(void);
	~Utils(void);

public:
	static void SetWindow(char* name, HWND hParent, int x, int y);
	static char* GetNativeFile();
};

