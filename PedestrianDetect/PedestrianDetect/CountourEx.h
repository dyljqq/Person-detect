#include"cv.h"
#include"highgui.h"
#include<iostream>

using namespace std;
using namespace cv;

Mat src; Mat src_gray;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);

/// º¯ÊıÉùÃ÷
vector<Rect> CountourEx(Mat image);