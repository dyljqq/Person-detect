#include"Kalman.h"

//ʹ��HOG��imgͼ���м������λ��
//���صľ���vectorΪ����λ��
//���ؼ��ʱ��
void DetectPedestrian(vector<cv::Rect>& found_filtered,const HOGDescriptor& hog,
                      double* time=NULL)
{
    found_filtered.clear ();
    vector<cv::Rect> found;

    //hog detector get the cv::Rect with pedestrian
    if(time!=NULL)
        *time=(double)getTickCount();
    
    hog.detectMultiScale(img, found, 0, Size(8,8), Size(32,32), 1.05, 2);
    
    if(time!=NULL)
    {
        *time=(double)getTickCount()-*time;
        *time /= getTickFrequency();
    }

    //if the cv::Rect overlap
    size_t i, j;
    for( i = 0; i < found.size(); i++ )
    {
        cv::Rect r = found[i];
        for( j = 0; j < found.size(); j++ )
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            found_filtered.push_back(r);
    }
    for( i = 0; i < found_filtered.size(); i++ )
    {
        cv::Rect r = found_filtered[i];
        // the HOG detector returns slightly larger rectangles than the real objects.
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        //found_filtered[i]=r;
        cv::rectangle (img,r,green,3);
    }
	cout<<found_filtered.size ()<<endl;
    std::cout<<" Time : "<<*time<<std::endl;
}

//���غ�������img��һ���������м������λ��
//����������һ������ȷ��
//
//���صľ���vectorΪ����λ��
//���ؼ��ʱ��
void DetectPedestrian(vector<cv::Rect>& found_filtered,const HOGDescriptor& hog,
                      const cv::Rect& roi,double* time=NULL)
{
    //CV_Assert(WHOLEIMG.contains(roi.br()) && WHOLEIMG.contains(roi.tl()));
    found_filtered.clear ();
    vector<cv::Rect> found;

    //hog detector get the cv::Rect with pedestrian
    if(time!=NULL)
        *time=(double)getTickCount();

    hog.detectMultiScale(img(roi), found, 0, Size(8,8), Size(32,32), 1.05, 2);

    if(time!=NULL)
    {
        *time=(double)getTickCount()-*time;
        *time /= getTickFrequency();
    }

    Point pt=roi.tl();
    //if the cv::Rect overlap
    size_t i, j;
    for( i = 0; i < found.size(); i++ )
    {
        found[i]+=pt;
        cv::Rect r = found[i];
        for( j = 0; j < found.size(); j++ )
            if( j != i && (r & found[j]) == r)
                break;
        if( j == found.size() )
            found_filtered.push_back(r);
    }
    for( i = 0; i < found_filtered.size(); i++ )
    {
        cv::Rect r = found_filtered[i];
        // the HOG detector returns slightly larger rectangles than the real objects.
        // so we slightly shrink the rectangles to get a nicer output.
        r.x += cvRound(r.width*0.1);
        r.width = cvRound(r.width*0.8);
        r.y += cvRound(r.height*0.07);
        r.height = cvRound(r.height*0.8);
        //found_filtered[i]=r;
        cv::rectangle (img,r,green,3);
    }

    std::cout<<" Time : "<<*time<<std::endl;
}

void InitialKalmanFilter(KalmanFilter& kf,double x,double y,
                          double delta_x,double delta_y)
{
    kf.transitionMatrix=(Mat_<float>(STATE_NUM, STATE_NUM) <<
                                            1,0,1,0,
                                            0,1,0,1,
                                            0,0,1,0,
                                            0,0,0,1 );
    kf.statePost=(Mat_<float>(STATE_NUM,1)<<
                                            x,
                                            y,
                                            delta_x,
                                            delta_y );
    kf.statePre=(Mat_<float>(STATE_NUM,1)<<
                                            x,
                                            y,
                                            delta_x,
                                            delta_y );
    //setIdentity: ���ŵĵ�λ�ԽǾ���;
    // !< measurement matrix (H) �۲�ģ��
    setIdentity(kf.measurementMatrix);
    // !< process noise covariance matrix (Q)
    // wk �ǹ������������ٶ�����Ͼ�ֵΪ�㣬Э�������ΪQk(Q)�Ķ�Ԫ��̬�ֲ�;
    setIdentity(kf.processNoiseCov,Scalar::all(25));
    // !< measurement noise covariance matrix (R)
    //vk �ǹ۲����������ֵΪ�㣬Э�������ΪRk,�ҷ�����̬�ֲ�;
    setIdentity(kf.measurementNoiseCov,Scalar::all(25));
    // !< priori error estimate covariance matrix (P'(k)): P'(k)=A*P(k-1)*At + Q)
    // A����F: transitionMatrix
    //Ԥ�����Э�������;
    setIdentity(kf.errorCovPost,Scalar::all(25));
}

//��þ��ε���������
Point2f GetCentroid(const cv::Rect& r)
{
    Point tl=r.tl();
    Point br=r.br();
    return Point2f((float)((tl.x+br.x)/2.0),(float)((tl.y+br.y)/2.0));
}

string DescripRect(const cv::Rect r)
{
    char buf[128];
    sprintf(buf,"This rectangle width: %d , height %d , tl: (%d,%d) ,br: (%d,%d)",
            r.width,r.height,r.tl ().x,r.tl ().y,r.br ().x,r.br ().y);
    
    return string(buf);
}

cv::Rect GetROI(const Point2f& centroid)
{
    Point tl((int)(centroid.x-ROIWIDTH/2),(int)(centroid.y-ROIHEIGHT/2));	
    return (cv::Rect(tl.x,tl.y,ROIWIDTH,ROIHEIGHT) & WHOLEIMG);
}


int main1()
{
    double time;
    ofstream fout("data_recorder.txt");
    int count=0;      //frame count
    string img_path;                // img path
    ifstream fin(IMG_PATH_TEXT);    //the text storage the image path
    vector<cv::Rect> pedestrian_location;
    
    HOGDescriptor hog;    //HoG detector
    hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());
    
    cv::Rect preFrame;
    cv::Rect curFrame;
    int count_tmp=-1;
    bool start_track=false;    //�Ƿ�ʼ���٣�

    int loss_frame=0;
    KalmanFilter kf(STATE_NUM,MEASURE_NUM);
    vector<Point2f> pedestrian_centroid;     //�洢���ı仯
    vector<Point2f> pedestrian_centroid_pre;    //Ԥ������ı仯
    vector<Point2f> state_post;
    //while(getline (fin,img_path) && count<=END_FRAME)
    
 //   const string video_name = "E:/�о����μ�/�����Ӿ�/Project/Data/VID_20141228_151213.mp4";
	const string video_name ="F:\\movie\\1.avi";
    VideoCapture video(video_name);
    Mat frame;

    VideoWriter videowriter;
    videowriter.open("Test_LX.avi",CV_FOURCC('M','J','P','G'),
                      10,Size(WINWIDTH,WINHEIGHT),true);
   
    VideoWriter boardwriter;
    boardwriter.open("Test_LX_board.avi",CV_FOURCC('M','J','P','G'),
                      10,Size(WINWIDTH,WINHEIGHT),true);
    while(1)
    {		
        //BEGIN !
        //Read the image from disk
        //img=imread (img_path,1);
        video >> frame;
        
        if(frame.empty ())
            break;

        cv::resize (frame, img, Size(WINWIDTH,WINHEIGHT));
        blackboard.setTo (0);

        //������������
        for (size_t k=0;k<pedestrian_centroid.size ();k++)
        {
            cv::circle(blackboard ,pedestrian_centroid[k],3,green);
            if(k!=0)
                cv::line(blackboard ,pedestrian_centroid[k],pedestrian_centroid[k-1],
                        green,2);
        }
        
        for (size_t k=0;k<pedestrian_centroid_pre.size ();k++)
        {
            cv::circle(blackboard ,pedestrian_centroid_pre[k],3,red);
            if(k!=0)
                cv::line(blackboard ,pedestrian_centroid_pre[k],
                                     pedestrian_centroid_pre[k-1],red,2);
        }

        for (size_t k=0;k<state_post.size ();k++)
        {
            cv::circle(blackboard ,state_post[k],4,blue,-1);
            if(k!=0)
                cv::line(blackboard ,state_post[k],state_post[k-1],blue,2);
        }
        
        char buf[128];
        sprintf(buf,"frame: %3d",count);
/*
        putText (blackboard ,string(buf),cv::Point(10,20),
            FONT_HERSHEY_SIMPLEX,0.2,Scalar::all(255));
        putText (blackboard ,"Red Rectangle: ROI from Kalman",cv::Point(10,40),
                FONT_HERSHEY_SIMPLEX,0.2,Scalar::all(255));
        putText(blackboard , "Red Circle: Centroid from Kalman",cv::Point(10,60),
                FONT_HERSHEY_SIMPLEX,0.2,Scalar::all(255));
        putText(blackboard , "Green Rectangle: Pedestrian Rect from HOG",
                                                 cv::Point(10,80),
            FONT_HERSHEY_SIMPLEX,0.2,Scalar::all(255));
        putText(blackboard , "Green Circle: Centroid from HoG",cv::Point(10,100),
            FONT_HERSHEY_SIMPLEX,0.2,Scalar::all(255));
        putText(blackboard , "Blue Circle: posteriori state estimate",cv::Point(10,120),
            FONT_HERSHEY_SIMPLEX,0.2,Scalar::all(255));
        */
        if (!start_track && (count_tmp==-1))   //û�п�ʼ����   
        {
            // HoG detection 
            //����ʱ����������أ���
            //���û�����ˣ��ͽ��Ŷ���һ֡�ɣ�ֱ���ҵ�
            //��������ˣ���Ŀ�꽨���������˲���
            DetectPedestrian(pedestrian_location,hog,&time);
            if (pedestrian_location.size ()==0)    //�ڸ�֡û���ҵ�����
            {
                cv::imshow (DETECT_WINNAME,img);
                waitKey(2);
                printf("Frame %d cannot find pedestrian! \n",count);
                count++;
                continue;
            }
            else    //����֡�ҵ�������
            {
                preFrame=pedestrian_location[0];
                //����ж�����ο�Ļ����Ƚ���ȡ��������
                for (size_t j=1;j<pedestrian_location.size ();j++)
                {
                    if (preFrame.area ()<pedestrian_location[j].area ())
                    {
                        preFrame=pedestrian_location[j];
                    }
                    //preFrame |= pedestrian_location[j];
                }
                
                count_tmp = count;       ///tmp ��ֵ 
                printf("Frame %d find pedestrian! \n",count);

            }	
            cv::imshow (DETECT_WINNAME,img);
        }
        else   //��ʼ�˸���
        {
            if (count==(count_tmp+1))    //����������֡���ݽ����������˲���
            {
                cv::Rect roi(WINWIDTH/2,WINHEIGHT/2,WINWIDTH/2,WINHEIGHT/2);
                roi=(roi&WHOLEIMG); 
                std::cout<<roi.tl ().x<<"   "<<roi.tl ().y<<endl;
                std::cout<<roi.br ().x<<"   "<<roi.br ().y<<endl;
                DetectPedestrian(pedestrian_location,hog,WHOLEIMG,&time);	

                if(pedestrian_location.size ()==0)   //û���ҵ�
                {
                    start_track = 0;
                    count_tmp = -1;
                    continue;
                }

                curFrame = pedestrian_location[0];   //�����t+1֡��λ����Ϣ
                Point2f pt1=GetCentroid (preFrame);
                Point2f pt2=GetCentroid (curFrame);

                // 1. ��ʼ��
                InitialKalmanFilter (kf,pt2.x,pt2.y,pt2.x-pt1.x,pt2.y-pt1.y);
                
                state_post.push_back (pt2);
                pedestrian_centroid.push_back (pt2);
                pedestrian_centroid_pre.push_back (pt2);
                
                cout<<"Start!"<<endl;
                fout<<"Kalman Initial Complete!"<<endl;
                cv::imshow (DETECT_WINNAME,img);
            }	
            else
            {
                // 2.Ԥ��
                Mat predict=kf.predict ();
                Point2f predictPt(predict.at<float>(0),predict.at<float>(1));

                fout<<"This is the "<<count<<" frame, and the predict point is ( "<<
                    predictPt.x<<" , "<<predictPt.y<<" )"<<endl;

                pedestrian_centroid_pre.push_back (predictPt);
                
                //cv::circle(blackboard,predictPt,5,red);
                cv::rectangle (blackboard,GetROI (predictPt),red,3);
                
                cv::circle (img,predictPt,5,red,5);
                

                // 3.����Update
                // 1) ʹ��HoG���
                cv::Rect curroi=GetROI (predictPt);
                //cv::Rect curroi=WHOLEIMG;
                //fout<<"This is the "<<count<<" frame, and "<<DescripRect(curroi);

                DetectPedestrian (pedestrian_location,hog,curroi,&time);

                // 2)����λ�ø���
                if (pedestrian_location.size ()!=0)
                {
                    loss_frame=0;

                    int len=pedestrian_location.size ();
                    double *dis=new double[len];
                    int _min=0;
                    for (int j=0;j<len;j++)
                    {
                        Rect r=pedestrian_location[j];
                        Point cen=GetCentroid (r);
                        double distan=(cen.x-predictPt.x)*(cen.x-predictPt.x)+
                                        (cen.y-predictPt.y)*(cen.y-predictPt.y);
                        dis[j]=distan;
                        if (distan<dis[_min])
                            _min=j;
                    }
                    delete[] dis;   //�ͷ��ڴ�

                    cv::Rect detected=pedestrian_location[_min];
#if 0
                    for (size_t j=1;j<pedestrian_location.size ();j++)
                    {
                        if (detected.area ()<pedestrian_location[j].area ())
                        {
                            detected=pedestrian_location[j];
                        }
                        //detected |= pedestrian_location[j];
                    }			
#endif
                    Point2f curCentroid=GetCentroid (detected);

                    pedestrian_centroid.push_back (curCentroid);
                    
                    cv::circle (blackboard,curCentroid,5,green);
                    
                    cv::rectangle (blackboard ,detected,green,3);
                    
                    cv::circle (img,curCentroid,5,green,5);

                    fout<<"This is the "<<count<<" frame, and the true point is ( "<<
                        curCentroid.x<<" , "<<curCentroid.y<<" )"<<endl;

                    Mat measure=*(Mat_<float>(2,1)<<curCentroid.x,curCentroid.y);
                    kf.correct (measure);
                    Mat state=kf.statePost;
                    Point2f stateP=Point2f(state.at<float>(0),state.at<float>(1));
                    //circle (blackboard ,sta
                    state_post.push_back (stateP);
                }
                else
                    loss_frame++;

                if (loss_frame>=3)
                {
                    printf("Lose the obj in frame %d\n.",count);
                    
                    // mark
                    start_track = 0;
                    count_tmp = -1;
                    continue;
                }

                cv::imshow (DETECT_WINNAME,img);
                
            }
            cv::imshow (TRACKER_WINNAME,blackboard );
        }

        count++;
    
        boardwriter << blackboard;
        videowriter << img;
        
        /*
        int c=waitKey();
        while (c!=27)
        {
            c=waitKey ();
        }
        */      
    }
    std::cout<<"-----------------------------"<<endl;
    std::cout<<"Complete!"<<endl;
    fout.close ();
    cv::waitKey();
    return 0;
}