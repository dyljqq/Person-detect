#include"HistogramEq.h"

IplImage* onHistEZ(IplImage* gray){
	IplImage *src;
	if(gray->nChannels!=1){
		printf("不是单通道图像\n");
		return NULL;
	}
	uchar* ImgData=(uchar*)(gray->imageData);
	int rows=gray->widthStep;
	int cols=gray->height;
	int gmax=0,gmin=255;//用来保存灰度范围
	int ihist[256];//图像直方图
	int nn[256];//直方图累加分布数组
	uchar T[256];//保存均衡化后的直方图
	CvMat *T_mat;
	int i=0,val=0;
	int sum;
	memset(ihist,0,sizeof(ihist));
	sum=rows*cols;
	for(i=0;i<sum;i++){
		ihist[*ImgData]++;//灰度统计
		if((int)(*ImgData)>gmax) gmax=(int)(*ImgData);
		if((int)(*ImgData)<gmin) gmin=(int)(*ImgData);
		ImgData++;
	}
	//建立灰度累加分布直方图
	for(i=0;i<256;i++){
		val=val+ihist[i];
		nn[i]=val;
	}
	//归一化直方图
	T_mat=cvCreateMatHeader(1,256,CV_8UC1);
	for(i=0;i<256;i++){
		T[i]=(uchar)(((nn[i]<<8)-nn[i])/sum);//灰度范围【0，255】
	}
	T_mat=cvCreateMatHeader(1,256,CV_8UC1);
	cvSetData(T_mat,T,0);//接上数据块
	cvLUT(gray,gray,T_mat);//反向映射

	return gray;
}