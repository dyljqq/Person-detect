#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/video/video.hpp>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;
using namespace cv;

//常量定义
char IMG_PATH_TEXT[]="E:/INRIAPerson/color/img_path.txt";   //图像路径
//char IMG_PATH_TEXT[]="E:/研究生课件/机器视觉/Project/Data/pictures";   //图像路径
//const int START_FRAME=243;    //开始跟踪的帧243   ||   413
//const int END_FRAME=290;      //结束跟踪的帧290   ||   480
const int WINHEIGHT=180;      //图像高度
const int WINWIDTH=320;       //图像宽度
const int ROIWIDTH=160;       //ROI高度
const int ROIHEIGHT=160;      //ROI宽度

const Scalar green(0,255,0);
const Scalar red(0,0,255);
const Scalar blue(255,0,0);

const string DETECT_WINNAME="Detect Window";
const string TRACKER_WINNAME="Track Window";

Mat img(WINHEIGHT,WINWIDTH,CV_8UC3);   //the Mat storage the image
Mat blackboard(WINHEIGHT,WINWIDTH,CV_8UC3);
const cv::Rect WHOLEIMG(0,0,WINWIDTH-2,WINHEIGHT-2);
const int STATE_NUM=4;
const int MEASURE_NUM=2;