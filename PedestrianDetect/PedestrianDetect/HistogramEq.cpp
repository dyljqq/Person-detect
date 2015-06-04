#include"HistogramEq.h"

IplImage* onHistEZ(IplImage* gray){
	IplImage *src;
	if(gray->nChannels!=1){
		printf("���ǵ�ͨ��ͼ��\n");
		return NULL;
	}
	uchar* ImgData=(uchar*)(gray->imageData);
	int rows=gray->widthStep;
	int cols=gray->height;
	int gmax=0,gmin=255;//��������Ҷȷ�Χ
	int ihist[256];//ͼ��ֱ��ͼ
	int nn[256];//ֱ��ͼ�ۼӷֲ�����
	uchar T[256];//������⻯���ֱ��ͼ
	CvMat *T_mat;
	int i=0,val=0;
	int sum;
	memset(ihist,0,sizeof(ihist));
	sum=rows*cols;
	for(i=0;i<sum;i++){
		ihist[*ImgData]++;//�Ҷ�ͳ��
		if((int)(*ImgData)>gmax) gmax=(int)(*ImgData);
		if((int)(*ImgData)<gmin) gmin=(int)(*ImgData);
		ImgData++;
	}
	//�����Ҷ��ۼӷֲ�ֱ��ͼ
	for(i=0;i<256;i++){
		val=val+ihist[i];
		nn[i]=val;
	}
	//��һ��ֱ��ͼ
	T_mat=cvCreateMatHeader(1,256,CV_8UC1);
	for(i=0;i<256;i++){
		T[i]=(uchar)(((nn[i]<<8)-nn[i])/sum);//�Ҷȷ�Χ��0��255��
	}
	T_mat=cvCreateMatHeader(1,256,CV_8UC1);
	cvSetData(T_mat,T,0);//�������ݿ�
	cvLUT(gray,gray,T_mat);//����ӳ��

	return gray;
}