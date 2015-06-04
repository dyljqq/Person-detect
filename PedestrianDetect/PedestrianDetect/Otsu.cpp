#include"Otsu.h"

int cvOtsu(IplImage *src)   
{  
	int thresholdValue=1;
	int thresholdValue_deltaT=0;
    int deltaT = 0; //光照调节参数
	uchar grayflag =1;
	IplImage* gray = NULL;
	if(src->nChannels != 1) //检查源图像是否为灰度图像
	{
		gray = cvCreateImage(cvGetSize(src), 8, 1);
		cvCvtColor(src, gray, CV_BGR2GRAY);
		grayflag = 0;
	}
	else gray = src;
	uchar* ImgData=(uchar*)(gray->imageData);   
	int thresholdValue_temp = 1;//阈值
    int ihist[256];   //图像直方图,256个点  
   
    int i, imgsize; //循环变量,图像尺寸
    int n, n1, n2;  //n 非零像素个数, n1 前景像素个数, n2 背景像素个数
    double m1, m2, sum, csum, fmax, sb;//m1前景灰度均值,m2背景灰度均值
    //对直方图置零   
    memset(ihist, 0, sizeof(ihist));   
    //生成直方图  
    imgsize = (gray->widthStep)*(gray->height);//图像数据总数 
    for (i=0; i<imgsize;i++)   
    {   
		ihist[((int)(*ImgData))&255]++;//灰度统计 '&255'防止指针溢出  
		ImgData++;//像素遍历
    }   
    // set up everything   
	sum=csum=0.0;   
	n=0;   
    for (i=0; i<255; i++)   
    {   
		sum+=(double)i*(double)ihist[i];  // x*f(x)质量矩   
		n+= ihist[i];   //f(x)质量 像素总数
    }


	deltaT = (int)(sum/imgsize); //像素平均灰度
	deltaT = deltaT>>1; //与之矫正,delatT = v*n; v=0.5
   
    if (!n)   
    {//图像全黑,输出警告
		fprintf (stderr, "NOT NORMAL thresholdValue=160\n");  
		return NULL;
    }   
    // OTSU算法
    fmax=-1.0;   
    n1=0;   
    for (i=0; i<255; i++)   
    {   
        n1+= ihist[i];   
        if (n1==0) {continue;}
        n2=n-n1;   
        if (n2==0) {break;}   
        csum += (double)i *ihist[i];   
        m1=csum/n1;   
        m2=(sum-csum)/n2;   
        sb=(double)n1*(double)n2*(m1-m2)*(m1-m2); //计算类间方差,  公式已简化  
        if (sb>fmax)   
        {   
            fmax=sb;   
            thresholdValue_temp=i;  //找到使类间方差最大的灰度值i   
        }  
    }   
   
	thresholdValue = thresholdValue_temp;
	thresholdValue_deltaT = thresholdValue_temp + deltaT;


	if(!grayflag) cvReleaseImage(&gray);
	if(thresholdValue<OTUS_THRESHHOLD_MIN) return OTUS_THRESHHOLD_MIN;
	return thresholdValue_deltaT;
 }