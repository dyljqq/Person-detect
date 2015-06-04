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


	//��֡��ȡ��Ƶ
/*	while(pFrame = cvQueryFrame( pCapture ))
	{
		nFrmNum++;*/
		//����ǵ�һ֡����Ҫ�����ڴ棬����ʼ��
	if(nFrmNum == 1)
	{
		sBuf=cvCreateImage(cvSize(pFrame->width,pFrame->height),IPL_DEPTH_8U,1);
		sBuf->origin=1;
		pBkImg = cvCreateImage(cvSize(pFrame->width, pFrame->height), IPL_DEPTH_8U,1);//size,λ��ȣ�ͨ��
		pBkImg->origin =1;//���ͼ��ǶȲ�����Ҫ����һ�������
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

		pBkMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);//����������������Ԫ������
		pFrMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		pFrameMat = cvCreateMat(pFrame->height, pFrame->width, CV_32FC1);
		preMat=cvCreateMat(pFrame->height,pFrame->width,CV_32FC1);
// pSMask=cvCreateMat(pFrame->height,pFrame->width,CV_32FC1);
// pHmat=cvCreateMat(pFrame->height,pFrame->width,CV_32FC1);
//ת���ɵ�ͨ��ͼ���ٴ���
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
		cvThreshold(pFrMat,pSMask,cvOtsu(pFrImg),255.0,CV_THRESH_BINARY);//�õ��� ֵͼ��

		cvAbsDiff(pFrameMat,pBkMat,pFrMat);
		if(nFrmNum%15==0)
		{
			cvConvert(pFrMat,pFrImg);
			threshold=cvOtsu(pFrImg);
		}
		cvThreshold(pFrMat,pFrMat,threshold,255.0,CV_THRESH_BINARY_INV);
		//���±���
//		cvRunningAvg(pFrameMat, pBkMat, 0.003, pSMask);
	}
	cvCvtColor(pFrame,sBuf,CV_BGR2GRAY);
	cvShowImage("video",pFrame);

	cvConvert(pBkMat,pBkImg);

	return pFrMat;
}