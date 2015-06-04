//  基于HOG特征的行人检测    
//  Author： http://blog.csdn.net/icvpr    
  
#include <iostream>  
#include"cv.h"
#include"highgui.h"
#include"NoiceProcess.h"
#include"HistogramEq.h"
#include"Otsu.h"
#include <opencv2/nonfree/features2d.hpp>
#include<fstream>

using namespace std;
using namespace cv;

const int MAX_CORNERS=500;
const float RECT_WIDTH=55;
const float RECT_HEIGHT=105;

int Abs(int A){
	return A>0?A:-A;
}

int Max(int A,int B){
	return A>B?A:B;
}

int Min(int A,int B){
	return A<B?A:B;
}

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
		 if(features_found[i]==1)
			 count++;
	 }
//	 printf("特征点相等个数：%f\n",1.0*count/corner_count);
	 if(1.0*count/corner_count>0.4)
		 return false;
	return true;
}

vector<Mat> countPedestrial(vector<Mat> total,vector<Mat> nextFrame)
{
	vector<Mat> mat=total;
	vector<Mat> nextMat=nextFrame;
//	printf("total:%d,next:%d\n",total.size(),nextFrame.size());
	for(int i=0;i<nextFrame.size();i++)
	{
		int j=0;
		for(;j<total.size();j++)
		{
			int rowMid=0,colMid=0;
//			printf("total.row:%d,total.col:%d,next:row:%d,next.col:%d\n",total[j].rows,total[j].cols,nextFrame[i].rows,nextFrame[i].cols);
			int flagRow=0,flagCol=0;
			if(Abs(nextFrame[i].rows-total[j].rows)%2==1){
				flagRow=1;
			}
			if(Abs(nextFrame[i].cols-total[j].cols)%2==1){
				flagCol=1;
			}
			if(nextFrame[i].rows>total[j].rows){
				rowMid=(nextFrame[i].rows-total[j].rows)/2;
				nextFrame[i]=nextFrame[i].rowRange(rowMid,nextFrame[i].rows-rowMid-flagRow);
			}else{
				rowMid=(total[j].rows-nextFrame[i].rows)/2;
				total[j]=total[j].rowRange(rowMid,total[j].rows-rowMid-flagRow);
			}

			if(nextFrame[i].cols>total[j].cols){
				colMid=(nextFrame[i].cols-total[j].cols)/2;
				nextFrame[i]=nextFrame[i].colRange(colMid,nextFrame[i].cols-colMid-flagCol);
			}else{
				colMid=(total[j].cols-nextFrame[i].cols)/2;
				total[j]=total[j].colRange(colMid,total[j].cols-colMid-flagCol);
			}
//			printf("total.row:%d,total.col:%d,next:row:%d,next.col:%d\n",total[j].rows,total[j].cols,nextFrame[i].rows,nextFrame[i].cols);
			if(!DiffMat(nextFrame[i],total[j])){
				break;
			}
		}
		if(j==total.size()){
			mat.push_back(nextMat[i]);
		}
	}
//	printf("mat:%d\n",mat.size());
	return mat;
}

// Normalizes a given image into a value range between 0 and 255.  
Mat norm_0_255(const Mat& src) {  
    // Create and return normalized image:  
    Mat dst;  
    switch(src.channels()) {  
    case 1:  
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);  
        break;  
    case 3:  
        cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);  
        break;  
    default:  
        src.copyTo(dst);  
        break;  
    }  
    return dst;  
} 

Mat Surf(Mat src){

	 vector<KeyPoint> keypoints;
          
    SurfFeatureDetector surf(2500.);
    surf.detect(src,keypoints);
    drawKeypoints(src,keypoints,src,Scalar(255,0,0),
        DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	return src;
}

//通过矩形框的大小来大致判断行人数量
int Add(float width,float height){

	int add=0;
	float scaleW=1.0*width/RECT_WIDTH;
	float scaleH=1.0*height/RECT_HEIGHT;
//	printf("w:%f,H:%f\n",scaleW,scaleH);
	if(scaleW<1.5||scaleH<1.5)
		add=0;
	else if((scaleW>1.5&&scaleW<=2.0)||(scaleH>1.5&&scaleH<=2.0))
		add=1;
	else if((scaleW>2.0&&scaleW<=2.5)||(scaleH>2.0&&scaleH<=2.5))
		add=2;
	else if((scaleW>2.5&&scaleW<=3.0)||(scaleH>2.5&&scaleH<=3.0))
		add=3;
	else if((scaleW>3.0&&scaleW<=3.5)||(scaleH>3.0&&scaleH<=3.5))
		add=4;
	else
		add=5;
	return add;
}

int main(int argc, char** argv)  
{  
	ofstream out("aa.txt");
    cv::Mat image = cv::imread("f:\\test.jpg");  

	// 1. 定义HOG对象  
    cv::HOGDescriptor hog; // 采用默认参数        
    // 2. 设置SVM分类器  
    hog.setSVMDetector(cv::HOGDescriptor::getDefaultPeopleDetector());   // 采用已经训练好的行人检测分类器 
    if (image.empty())  
    {  
        std::cout<<"read image failed"<<std::endl;  
    }
	
	vector<Rect>regions,found_filtered; //矩形框数组
	VideoCapture video("F:\\movie\\p.avi");
	if(!video.isOpened()){
		printf("视频文件不存在\n");
		return 0;
	}
	long totalFrame=video.get(CV_CAP_PROP_FRAME_COUNT);
	cout<<"一共："<<totalFrame<<"帧"<<endl;
	double rate=video.get(CV_CAP_PROP_FPS);
	cout<<"rate:"<<rate<<endl;
	int curFrame=0;
	Mat frame,srcFrame;
	vector<Mat> total,nextFrame;
	int add=0,gross=0,totalGross=0;
	while(curFrame<1440){
		if(!video.read(srcFrame)){
			printf("视频读取失败\n");
			break;
		}

//		printf("%d %d\n",srcFrame.rows,srcFrame.cols);
		line(srcFrame,Point(0,60),Point((int)srcFrame.cols,60),Scalar(0,0,255),2);
		line(srcFrame,Point(0,srcFrame.rows-60),Point(srcFrame.cols,srcFrame.rows-60),Scalar(0,0,255),2);
//		srcFrame=imread("f:\\test.jpg");
		//gama校正
		Mat X;
		IplImage* src=&(IplImage)srcFrame;
		IplImage* dst=cvCreateImage(cvGetSize(src),IPL_DEPTH_8U,1);
		cvCvtColor(src,dst,CV_BGR2GRAY);
		dst=onSmooth(dst,CV_BLUR);
		frame=Mat(dst,false);
		frame.convertTo(X,CV_32FC1);
		float gamma = 1/2.2;
		Mat I;
		pow(X, gamma, I); 
		frame=norm_0_255(I);

//		frame=srcFrame;

		frame = frame(Rect(80,60,480,360));
//		frame=frame.rowRange(60,420).colRange(80,560);
//		printf("%d %d\n",frame.rows,frame.cols);
		if(curFrame%10==0){
			out<<"第"<<curFrame<<"帧"<<endl;
			gross=0;
			regions.clear();
			found_filtered.clear();
			nextFrame.clear();
			hog.detectMultiScale(frame, regions, 0, cv::Size(8,8), cv::Size(32,32), 1.05, 1);

			for(int i=0; i < regions.size(); i++)
			{
				Rect r = regions[i];
				int j=0;
				for(; j < regions.size(); j++)
					if(j != i && (r & regions[j]) == r)
						break;
				if( j == regions.size())
					found_filtered.push_back(r);
			}

			for(int i=0;i<found_filtered.size();i++){
				Rect r=found_filtered[i];
				r.x += cvRound(r.width*0.1);
				r.width = cvRound(r.width*0.8);
				r.y += cvRound(r.height*0.07);
				r.height = cvRound(r.height*0.8);
				int width=r.x+r.width;
				int height=r.y+r.height;
//				printf("width:%d,height:%d\n",r.width,r.height);
//				printf("col:%d,row:%d,x+width:%d,y+height%d\n",frame.cols,frame.rows,width,height);

				Mat mat=srcFrame.colRange(r.x+80,width+80).rowRange(r.y+60,height+60);
				if(curFrame==0){
					total.push_back(mat);
				}else{
					nextFrame.push_back(mat);
				}
			}

			total=countPedestrial(total,nextFrame);
			out<<"total:"<<total.size()<<endl;
			printf("total:%d\n",total.size());
//			gross=total.size();
			for(int i=0;i<total.size();i++){
//				printf("Col:%d,Row:%d,Add:%d\n",total[i].cols,total[i].rows,Add(total[i].cols,total[i].rows));
				out<<"Add:"<<Add(total[i].cols,total[i].rows)<<endl;
				gross+=Add(total[i].cols,total[i].rows)+1;
			}

		}

//		found_filtered=regions;

		//画矩形框，因为hog检测出的矩形框比实际人体框要稍微大些,所以这里需要做一些调整
		for(int i=0; i<found_filtered.size(); i++)
		{
			Rect r = found_filtered[i];
			r.x += cvRound(r.width*0.1+80);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07+60);
			r.height = cvRound(r.height*0.8);
			rectangle(srcFrame, r.tl(), r.br(), Scalar(0,255,0), 2);
		}
  
		cv::imshow("hog", srcFrame);  
		char c=cv::waitKey(1000/rate);
		if(c==27)
			break;
		printf("第%d帧\n",curFrame);
		out<<"行人数："<<gross<<endl;
/*		if(curFrame%200==0&&curFrame!=0){
			totalGross+=gross;
			total.clear();
		}*/
		printf("行人数：%d\n",gross);
		curFrame++;
	}
    return 0;  
}  