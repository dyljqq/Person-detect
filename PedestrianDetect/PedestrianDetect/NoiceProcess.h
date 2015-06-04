#include"cv.h"
#include"highgui.h"

using namespace cv;

#define N 1
#define iFilterW 2*N+1
#define iFilterH 2*N+1
#define iFilterLen (iFilterW)*(iFilterH)

//自定义去除均值滤波
IplImage* onBlurSmooth(IplImage *src,IplImage* dst);
//自定义中值滤波
IplImage* onMedianSmooth(IplImage* src);
//mode表示选择的去噪方式
IplImage* onSmooth(IplImage* src,int mode);