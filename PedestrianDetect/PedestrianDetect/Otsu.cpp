#include"Otsu.h"

int cvOtsu(IplImage *src)   
{  
	int thresholdValue=1;
	int thresholdValue_deltaT=0;
    int deltaT = 0; //���յ��ڲ���
	uchar grayflag =1;
	IplImage* gray = NULL;
	if(src->nChannels != 1) //���Դͼ���Ƿ�Ϊ�Ҷ�ͼ��
	{
		gray = cvCreateImage(cvGetSize(src), 8, 1);
		cvCvtColor(src, gray, CV_BGR2GRAY);
		grayflag = 0;
	}
	else gray = src;
	uchar* ImgData=(uchar*)(gray->imageData);   
	int thresholdValue_temp = 1;//��ֵ
    int ihist[256];   //ͼ��ֱ��ͼ,256����  
   
    int i, imgsize; //ѭ������,ͼ��ߴ�
    int n, n1, n2;  //n �������ظ���, n1 ǰ�����ظ���, n2 �������ظ���
    double m1, m2, sum, csum, fmax, sb;//m1ǰ���ҶȾ�ֵ,m2�����ҶȾ�ֵ
    //��ֱ��ͼ����   
    memset(ihist, 0, sizeof(ihist));   
    //����ֱ��ͼ  
    imgsize = (gray->widthStep)*(gray->height);//ͼ���������� 
    for (i=0; i<imgsize;i++)   
    {   
		ihist[((int)(*ImgData))&255]++;//�Ҷ�ͳ�� '&255'��ָֹ�����  
		ImgData++;//���ر���
    }   
    // set up everything   
	sum=csum=0.0;   
	n=0;   
    for (i=0; i<255; i++)   
    {   
		sum+=(double)i*(double)ihist[i];  // x*f(x)������   
		n+= ihist[i];   //f(x)���� ��������
    }


	deltaT = (int)(sum/imgsize); //����ƽ���Ҷ�
	deltaT = deltaT>>1; //��֮����,delatT = v*n; v=0.5
   
    if (!n)   
    {//ͼ��ȫ��,�������
		fprintf (stderr, "NOT NORMAL thresholdValue=160\n");  
		return NULL;
    }   
    // OTSU�㷨
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
        sb=(double)n1*(double)n2*(m1-m2)*(m1-m2); //������䷽��,  ��ʽ�Ѽ�  
        if (sb>fmax)   
        {   
            fmax=sb;   
            thresholdValue_temp=i;  //�ҵ�ʹ��䷽�����ĻҶ�ֵi   
        }  
    }   
   
	thresholdValue = thresholdValue_temp;
	thresholdValue_deltaT = thresholdValue_temp + deltaT;


	if(!grayflag) cvReleaseImage(&gray);
	if(thresholdValue<OTUS_THRESHHOLD_MIN) return OTUS_THRESHHOLD_MIN;
	return thresholdValue_deltaT;
 }