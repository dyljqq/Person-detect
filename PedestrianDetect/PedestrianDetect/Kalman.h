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

//��������
char IMG_PATH_TEXT[]="E:/INRIAPerson/color/img_path.txt";   //ͼ��·��
//char IMG_PATH_TEXT[]="E:/�о����μ�/�����Ӿ�/Project/Data/pictures";   //ͼ��·��
//const int START_FRAME=243;    //��ʼ���ٵ�֡243   ||   413
//const int END_FRAME=290;      //�������ٵ�֡290   ||   480
const int WINHEIGHT=180;      //ͼ��߶�
const int WINWIDTH=320;       //ͼ����
const int ROIWIDTH=160;       //ROI�߶�
const int ROIHEIGHT=160;      //ROI���

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