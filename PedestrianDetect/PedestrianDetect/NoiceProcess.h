#include"cv.h"
#include"highgui.h"

using namespace cv;

#define N 1
#define iFilterW 2*N+1
#define iFilterH 2*N+1
#define iFilterLen (iFilterW)*(iFilterH)

//�Զ���ȥ����ֵ�˲�
IplImage* onBlurSmooth(IplImage *src,IplImage* dst);
//�Զ�����ֵ�˲�
IplImage* onMedianSmooth(IplImage* src);
//mode��ʾѡ���ȥ�뷽ʽ
IplImage* onSmooth(IplImage* src,int mode);