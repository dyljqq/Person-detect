#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/ml/ml.hpp>
#include <opencv2/objdetect/objdetect.hpp>

#include "MySVM.h"
#include "HogPedestrialDete.h"
using namespace std;
using namespace  cv;

const string NEG_IMAGE_LIST="E:\\INRIAPerson\\INRIAPerson\\train_64x128_H96\\neg_64x128.lst" ;

const string POS_IMAGE_LIST="E:\\INRIAPerson\\INRIAPerson\\train_64x128_H96\\pos_64x128.lst";



const int pos_num = 2410;
const int neg_num = 12180;
int hard_num = 0;

#define TRAIN false
void HogPedestrianDete()
{
    //��ⴰ��(64,128),��ߴ�(16,16),�鲽��(8,8),cell�ߴ�(8,8),ֱ��ͼbin����9
    HOGDescriptor hog(Size(64,128),Size(16,16),Size(8,8),Size(8,8),9);
    //������ά��
    int descriptorDim = 0;
    //ѵ��SVM
    MySVM svmTrain;

    ofstream foutVector("Dim.txt");
	string img_path;
	bool find_hard= (hard_num==0) ;   //hard_num=0˵����Ҫ������

BEGIN:	if(TRAIN)
    {
        ifstream fin_pos(POS_IMAGE_LIST);
        ifstream fin_neg(NEG_IMAGE_LIST);

        Mat trainFeatureMat;
        Mat trainLabelMat;

		//����������
        for(int i = 0;i<pos_num && getline(fin_pos,img_path);i++)
        {
            cout<<"Process: "<<img_path<<endl;
            Mat img = imread(img_path,1);  //read the positive img
            vector<float> descriptors;
            hog.compute(img,descriptors,Size(8,8));  //compute the descriptors

            //�����һ������ʱ��ʼ�����������������������Ϊֻ��֪��������������ά�����ܳ�ʼ��������������
            if(i == 0)
            {
                descriptorDim = descriptors.size();//HOG�����ӵ�ά��
                foutVector<<descriptorDim<<endl;
                //��ʼ������ѵ������������������ɵľ��������������������ĸ�������������HOG������ά��sampleFeatureMat
                trainFeatureMat =
                      Mat::zeros(pos_num + neg_num + hard_num, descriptorDim, CV_32FC1);
                //��ʼ��ѵ����������������������������������ĸ�������������1��1��ʾ���ˣ�0��ʾ����
                trainLabelMat = Mat::zeros(pos_num + neg_num + hard_num, 1, CV_32FC1);\
            }
            //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
            for(int j = 0;j<descriptorDim;j++)
            {
                trainFeatureMat.at<float>(i,j) = descriptors [j];
            }

            trainLabelMat.at<float>(i,0) = 1;     //positive

        }   // ��������������
		fin_pos.close();

		// ���븺����
        for(int i = 0;i<neg_num && getline(fin_neg,img_path);i++)
        {
            cout<<"Process: "<<img_path<<endl;
            Mat img = imread(img_path,1);  //read the positive img
            vector<float> descriptors;
            hog.compute(img,descriptors,Size(8,8));  //compute the descriptors
            //������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
            for(int j = 0;j<descriptorDim;j++)
            {
                trainFeatureMat.at<float>(i+pos_num,j) = descriptors [j];
            }
            trainLabelMat.at<float>(i+pos_num,0) = -1;     //positive
        }     // ���븺��������
		fin_neg.close();


		// ��������
		if (hard_num>0)
		{
			find_hard=false;   // ����Ҫ����������

			ifstream fin_hard("E:/INRIAPerson/INRIAPerson/HardExample/hard.lst");
			for (int i=0;i<hard_num && getline(fin_hard,img_path);i++)
			{
				cout<<"Process: "<<img_path<<endl;
				Mat img=imread(img_path,1);
				vector<float> descriptors;
				hog.compute(img,descriptors,Size(8,8));  //compute the descriptors
				//������õ�HOG�����Ӹ��Ƶ�������������sampleFeatureMat
				for(int j = 0;j<descriptorDim;j++)
				{
					trainFeatureMat.at<float>(i+pos_num+neg_num,j) = descriptors [j];
				}
				trainLabelMat.at<float>(i+pos_num+neg_num,0) = -1;     //
			}
			fin_hard.close();
			cout<<"Hard Example Complete!"<<endl;
		}


/*
        //���������HOG�������������ļ�
        ofstream foutDescriptor("TrainFeatureMat.txt");
        for(int i=0; i<pos_num + neg_num; i++)
        {
          foutDescriptor<<i<<endl;
          for(int j=0; j<descriptorDim; j++)
              foutDescriptor<<trainFeatureMat.at<float>(i,j)<<"  ";
          foutDescriptor<<endl;
        }
        foutDescriptor.close();
*/
        foutVector.close();
        cout<<"*********************************"<<endl;
        cout<<"Feature Extraction Done!"<<endl;
        //ѵ��SVM������
        //������ֹ��������������1000�λ����С��FLT_EPSILONʱֹͣ����
        //CvTermCriteria criteria = cvTermCriteria(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 1E7, FLT_EPSILON);
        CvTermCriteria criteria=cvTermCriteria(CV_TERMCRIT_EPS,1000,1E-7);   //mark
        //SVM������SVM����ΪC_SVC�����Ժ˺������ɳ�����C=0.01
     //int svm_type,int kernel_type,double degree,double gamma,double coef0,
     //double CValue,double nu,double p,cvMat *class_wights,CvTermCriteria criteria
        CvSVMParams param(CvSVM::C_SVC, CvSVM::LINEAR, 0, 1, 0, 0.01, 0, 0, 0, criteria);

        cout<<endl;
        cout<<"---------------------------------"<<endl;
        cout<<"Start Training....."<<endl;
        double t = (double)getTickCount();
        svmTrain.train(trainFeatureMat, trainLabelMat, Mat(), Mat(), param);//ѵ��������
        t = (double)getTickCount() - t;
        cout<<"Train Complete!"<<endl;

        svmTrain.save("SVM_HOG.xml");//��ѵ���õ�SVMģ�ͱ���Ϊxml�ļ�
        printf("train time = %gms\n", t*1000./cv::getTickFrequency());
    }

    else    //load previous xml file�����ѵ���Ļ���ֱ�Ӽ������е�XML�ļ�
    {
//        const char svm_xml_name[128]="SVM_HOG_wuless_lou_more.xml";
		const char svm_xml_name[128]="SVM_HOG.xml";
        cout<<"Load the previous XML File: "<<svm_xml_name<<endl;
        svmTrain.load(svm_xml_name);
    }

	// ��ѵ������SVM��������
    descriptorDim = svmTrain.get_var_count();
    cout<<"The dim of HOG descriptor is "<<descriptorDim<<endl;

    int supportVecNum = svmTrain.get_support_vector_count();
    cout<<"The count of support vector is "<<supportVecNum<<endl;

    //alpha���������ȵ���֧����������
    Mat alphaMat = Mat::zeros(1, supportVecNum, CV_32FC1);   //1x1
    //֧����������
    Mat supportVecMat = Mat::zeros(supportVecNum, descriptorDim, CV_32FC1);//1x3780
    //alpha��������֧����������Ľ��
    Mat resultMat = Mat::zeros(1, descriptorDim, CV_32FC1); //1x3780

    //��֧�����������ݸ��Ƶ�supportVectorMat������
    for(int i=0; i<supportVecNum; i++)
    {
        const float * pSVData = svmTrain.get_support_vector(i);//���ص�i��֧������������ָ��
        for(int j=0; j<descriptorDim; j++)
        {
            supportVecMat.at<float>(i,j) = pSVData[j];
        }
    }

    //��alpha���������ݸ��Ƶ�alphaMat��
    double * pAlphaData = svmTrain.get_alpha_vector();//����SVM�ľ��ߺ����е�alpha����
    for(int i=0; i<supportVecNum; i++)
    {
        alphaMat.at<float>(0,i) = pAlphaData[i];
    }

    //����-(alphaMat * supportVectorMat),����ŵ�resultMat��
    //gemm(alphaMat, supportVectorMat, -1, 0, 1, resultMat);//��֪��Ϊʲô�Ӹ��ţ�
    resultMat = -1 * alphaMat * supportVecMat;

    //�õ����յ�setSVMDetector(const vector<float>& detector)�����п��õļ����
    vector<float> myDetector;
    //��resultMat�е����ݸ��Ƶ�����myDetector��
    for(int i=0; i<descriptorDim; i++)
    {
        myDetector.push_back(resultMat.at<float>(0,i));
    }
    //������ƫ����rho���õ������
    myDetector.push_back(svmTrain.get_rho());
    cout<<"the dim of mydetector: "<<myDetector.size()<<endl;

    //�������Ӳ������ļ�
    ofstream foutHOG("HOGDetectorForOpenCV0425.txt");
    for(size_t i=0; i<myDetector.size(); i++)
    {
        foutHOG<<myDetector[i]<<endl;
    }
    foutHOG.close();
	
    waitKey();
}
