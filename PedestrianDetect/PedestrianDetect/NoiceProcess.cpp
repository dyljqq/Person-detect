#include"NoiceProcess.h"

int SumOld,SumRow[3000];
int SumPixel;
int SlideStep;

//自定义去除均值滤波
IplImage* onBlurSmooth(IplImage* src,IplImage* dst){
	int wRadius=iFilterW>>1;
	int hRadius=iFilterH>>1;
	if(src->nChannels!=1){
		printf("输入的图像不是单通道图像\n");
		return NULL;
	}
	//滑窗的长宽以及上下两行间距
	int row,col,imgStep;
	int x,y,m,n;
	for(x=wRadius;x<src->widthStep-wRadius;x++){
		for(y=hRadius,SlideStep=-1;y<src->height-hRadius;y++,SlideStep++){
			if(y==hRadius){
				SumPixel=0;
				imgStep=0;
				for(m=-hRadius,row=0;m<=hRadius;m++,row++){
					SumRow[row]=0;
					for(n=-wRadius,col=0;n<=wRadius;n++,col++){
						SumRow[row]+=(uchar)src->imageData[imgStep+x+n];
					}
					imgStep+=src->widthStep;
					SumPixel+=SumRow[row];
				}
			}else{
				SlideStep=SlideStep%iFilterH;
				SumOld=SumRow[SlideStep];
				SumRow[SlideStep]=0;
				if(x==1)	printf("SumOld:%d	",SumOld);
				for(n=-wRadius,col=0;n<=wRadius;n++,col++){
					SumRow[SlideStep]+=(uchar)src->imageData[imgStep+x+n];
				}
				imgStep+=src->widthStep;
				SumPixel+=SumRow[SlideStep]-SumOld;
			}
			dst->imageData[src->widthStep*y+x]=SumPixel/9;
		}
	}
	printf("%d\n",iFilterLen);
	return dst;
}

//自定义中值滤波
IplImage* onMedianSmooth(IplImage* src){
	
	return NULL;
}

//mode表示选择的去噪方式
IplImage* onSmooth(IplImage* src,int mode){
	IplImage* out=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,src->nChannels);
	cvSmooth(src,out,mode,3,src->nChannels);
	return out;
}
