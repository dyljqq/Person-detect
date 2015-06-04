#include"LucasKanande.h"


bool DiffMat(Mat matA,Mat matB){
	//必须转换成灰度图
	 IplImage* srcA=&(IplImage)matA;
	 IplImage* srcB=&(IplImage)matB;

	 IplImage* imgA=cvCreateImage(cvGetSize(srcA),IPL_DEPTH_8U,1);
	 IplImage* imgB=cvCreateImage(cvGetSize(srcB),IPL_DEPTH_8U,1);
	 cvCvtColor(srcA,imgA,CV_BGR2GRAY);
	 cvCvtColor(srcB,imgB,CV_BGR2GRAY);

	 CvSize img_sz=cvGetSize(imgA);
	 int win_size=10;
//	 IplImage* imgC=cv
	 IplImage* eig_image=cvCreateImage(img_sz,IPL_DEPTH_32F,1);
	 IplImage* tmp_image=cvCreateImage(img_sz,IPL_DEPTH_32F,1);

	 int corner_count=MAX_CORNERS;
	 CvPoint2D32f* cornersA=new CvPoint2D32f[MAX_CORNERS];

	 cvGoodFeaturesToTrack(
		 imgA,
		 eig_image,
		 tmp_image,
		 cornersA,
		 &corner_count,
		 0.01,
		 5.0,
		 0,
		 3,
		 0,
		 0.04
		 );

	 cvFindCornerSubPix(
		 imgA,
		 cornersA,
		 corner_count,
		 cvSize(win_size,win_size),
		 cvSize(-1,-1),
		 cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03)
		 );

	 char features_found[MAX_CORNERS];
	 float features_errors[MAX_CORNERS];

	 CvSize pyr_sz=cvSize(imgA->width+8,imgB->height/3);

	 IplImage* pyrA=cvCreateImage(pyr_sz,IPL_DEPTH_32F,1);
	 IplImage* pyrB=cvCreateImage(pyr_sz,IPL_DEPTH_32F,1);

	 CvPoint2D32f* cornersB=new CvPoint2D32f[MAX_CORNERS];


	 cvCalcOpticalFlowPyrLK(
		 imgA,
		 imgB,
		 pyrA,
		 pyrB,
		 cornersA,
		 cornersB,
		 corner_count,
		 cvSize(win_size,win_size),
		 5,
		 features_found,
		 features_errors,
		 cvTermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03),
		 0
		 );

	 int count=0;
	 for(int i=0;i<corner_count;i++){
//		 if(features_found[i]==1)
//			 count++;

		 if ( 0 == features_found[i] || features_errors[i] > 550 )
			{
				printf("error is %f \n" , features_errors[i] ) ;
				continue ;
			}

			printf("find it !\n") ;

			CvPoint pt_prev = cvPoint( cornersA[i].x , cornersA[i].y ) ;
			CvPoint pt_cur = cvPoint( cornersB[i].x , cornersB[i].y ) ;

			cvLine( imgA,pt_prev,pt_cur,CV_RGB( 255,0,0),2 );
	 }
	 printf("特征点相等个数：%f\n",1.0*count/corner_count);
	 if(1.0*count/corner_count>0.9)
		 return false;
	return true;
}

vector<Mat> countPedestrial(vector<Mat> total,vector<Mat> nextFrame)
{
	vector<Mat> mat=total;
	printf("total:%d,next:%d\n",total.size(),nextFrame.size());
	for(int i=0;i<nextFrame.size();i++)
	{
		int j=0;
		for(;j<total.size();j++)
		{
			if(!DiffMat(nextFrame[i],total[j])){
				break;
			}
		}
		if(j==total.size())
			mat.push_back(nextFrame[i]);
	}
	return mat;
}