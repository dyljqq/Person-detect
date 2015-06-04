#include"BackgroundSub.h"

Mat BackgroundSub(Mat image,int nFrmNum){

	IplImage* pFrame = &(IplImage)image;

	IplImage* pFrImg = NULL;
	IplImage* pBkImg = NULL;
	IplImage* preImg=NULL;
	IplImage* sBuf=NULL;
	IplImage* pHImg=NULL;
	IplImage* pSMask=NULL;
	IplImage* pHmat=NULL;

	CvMat* pFrameMat = NULL;
	CvMat* pFrMat = NULL;
	CvMat* pBkMat = NULL;
	CvMat* preMat=NULL;

	//CvMat* pHmat=NULL;
	int threshold=60;


	//逐帧读取视频
/*	while(pFrame = cvQueryFrame( pCapture ))
	{
		nFrmNum++;*/
		//如果是第一帧，需要申请内存，并初始化
	if(nFrmNum == 1)
	{
		sBuf=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
		sBuf->origin=1;
		pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);//size,位深度，通道
		pBkImg->origin =1;//如果图像角度不对则要用这一步来解决
		pFrImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);
		pFrImg->origin =1;
		preImg=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
		preImg->origin=1;
		pHImg=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
		pHImg->origin=1;
		pSMask=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
		pSMask->origin=1;
		pHmat=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
		pHmat->origin=1;

		pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);//行数，列数，矩阵元素类型
		pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		preMat=cvCreateMat(pFrame->height,pFrame->width,CV_32FC1);
// pSMask=cvCreateMat(pFrame->height,pFrame->width,CV_32FC1);
// pHmat=cvCreateMat(pFrame->height,pFrame->width,CV_32FC1);
//转化成单通道图像再处理
		cvCvtColor(pFrame, pBkImg, CV_BGR2GRAY);
		cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
		cvCvtColor(pFrame,sBuf,CV_BGR2GRAY);

		cvConvert(pFrImg, pFrameMat);
		cvConvert(pFrImg, pFrMat);
		cvConvert(pBkImg, pBkMat);
	}
	else
	{
		cvConvert(sBuf,preMat);
		cvCvtColor(pFrame, pFrImg, CV_BGR2GRAY);
		cvConvert(pFrImg, pFrameMat);
		cvAbsDiff(pFrameMat,preMat,pFrMat);
		cvThreshold(pFrMat,pSMask,cvOtsu(pFrImg),255.0,CV_THRESH_BINARY);//得到二 值图像

		cvAbsDiff(pFrameMat,pBkMat,pFrMat);
		if(nFrmNum%15==0)
		{
			cvConvert(pFrMat,pFrImg);
			threshold=cvOtsu(pFrImg);
		}
		cvThreshold(pFrMat,pFrMat,threshold,255.0,CV_THRESH_BINARY_INV);
		//更新背景
//		cvRunningAvg(pFrameMat, pBkMat, 0.003, pSMask);
	}
	cvCvtColor(pFrame,sBuf,CV_BGR2GRAY);
	cvShowImage("video",pFrame);

	cvConvert(pBkMat,pBkImg);

	return pFrMat;
}