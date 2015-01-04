#pragma once

void CreateBoard();

void BehavGetHandler();
void BehavDectHandler();
void BehavClsHandler();

void recognize(const char* trainDataName);
int  training(const char* bgName, const char* targetName, IplImage* targetImage);
void reverseImgColor(IplImage* item);
