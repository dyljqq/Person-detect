#include <iostream>
#include <stdio.h>
#include <vector>
#include <fstream>
#include"highgui.h"
#include"cv.h"
#include"cxcore.h"

using namespace std;
using namespace  cv;

struct annotation//annotation:×¢ÊÍ
{
	int _x;
	int _y;
	int _w;
	int _h;
	annotation(int x,int y,int w,int h):
		_x(x),_y(y),_w(w),_h(h){}
};

void readAnnotation(const char* filename,vector<annotation>& v);
void DrawRectangle(Mat& img,int x,int y,int w,int h);
void DrawRectangle(Mat& img,const annotation& annota);
void CCVFlagImage();