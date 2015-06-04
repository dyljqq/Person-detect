#include"NoiceProcess.h"

int SumOld,SumRow[3000];
int SumPixel;
int SlideStep;

//�Զ���ȥ����ֵ�˲�
IplImage* onBlurSmooth(IplImage* src,IplImage* dst){
	int wRadius=iFilterW>>1;
	int hRadius=iFilterH>>1;
	if(src->nChannels!=1){
		printf("�����ͼ���ǵ�ͨ��ͼ��\n");
		return NULL;
	}
	//�����ĳ����Լ��������м��
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

//�Զ�����ֵ�˲�
IplImage* onMedianSmooth(IplImage* src){
	
	return NULL;
}

//mode��ʾѡ���ȥ�뷽ʽ
IplImage* onSmooth(IplImage* src,int mode){
	IplImage* out=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,src->nChannels);
	cvSmooth(src,out,mode,3,src->nChannels);
	return out;
}
