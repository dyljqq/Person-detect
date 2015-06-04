#include"DifficultTrain.h"

void DifficultTrain(){

	Mat src;  
	char saveName[256];//���ó�����hard exampleͼƬ���ļ���  
	string ImgName;  
	ifstream fin_detector("HOGDetectorForOpenCV_2400PosINRIA_12000Neg.txt");//���Լ�ѵ����SVM������ļ�  
	ifstream fin_imgList("INRIANegativeImageList.txt");//��ԭʼ������ͼƬ�ļ��б�  
	//ifstream fin_imgList("subset.txt");  

	//���ļ��ж����Լ�ѵ����SVM����  
	float temp;  
	vector<float> myDetector;//3781ά�ļ��������  
	while(!fin_detector.eof())  
	{  
		fin_detector >> temp;  
		myDetector.push_back(temp);//������������  
	}  

	cout<<"�����ά����"<<myDetector.size()<<endl;  

	//namedWindow("src",0);  
	HOGDescriptor hog;//HOG���������  
	hog.setSVMDetector(myDetector);//���ü��������Ϊ�Լ�ѵ����SVM����  

	//һ��һ�ж�ȡ�ļ��б�  
	while(getline(fin_imgList,ImgName))  
	{  
		cout<<"����"<<ImgName<<endl;  
		string fullName = "D:\\DataSet\\INRIAPerson\\INRIAPerson\\Train\\neg\\" + ImgName;//����·����  
		src = imread(fullName);//��ȡͼƬ  
		Mat img = src.clone();//����ԭͼ  

		vector<Rect> found;//���ο�����  
		//�Ը�����ԭͼ���ж�߶ȼ�⣬�����Ķ�����  
		hog.detectMultiScale(src, found, 0, Size(8,8), Size(32,32), 1.05, 2);  

		//������ͼ���м������ľ��ο򣬵õ�hard example  
		for(int i=0; i < found.size(); i++)  
		{  
			//�������ĺܶ���ο򶼳�����ͼ��߽磬����Щ���ο�ǿ�ƹ淶��ͼ��߽��ڲ�  
			Rect r = found[i];  
			if(r.x < 0)  
				r.x = 0;  
			if(r.y < 0)  
				r.y = 0;  
			if(r.x + r.width > src.cols)  
				r.width = src.cols - r.x;  
			if(r.y + r.height > src.rows)  
				r.height = src.rows - r.y;  

			//�����ο򱣴�ΪͼƬ������Hard Example  
			Mat hardExampleImg = src(r);//��ԭͼ�Ͻ�ȡ���ο��С��ͼƬ  
			resize(hardExampleImg,hardExampleImg,Size(64,128));//�����ó�����ͼƬ����Ϊ64*128��С  
			sprintf(saveName,"hardexample%09d.jpg",hardExampleCount++);//����hard exampleͼƬ���ļ���  
			imwrite(saveName, hardExampleImg);//�����ļ�  


			//�����ο���Ϊhog�����ľ��ο��ʵ�������Ҫ��΢��Щ,����������Ҫ��һЩ����  
			//r.x += cvRound(r.width*0.1);  
			//r.width = cvRound(r.width*0.8);  
			//r.y += cvRound(r.height*0.07);  
			//r.height = cvRound(r.height*0.8);  
			rectangle(img, r.tl(), r.br(), Scalar(0,255,0), 3);  

		}  
		//imwrite(ImgName,img);  
		//imshow("src",src);  
		//waitKey(100);//ע�⣺imshow֮��һ��Ҫ��waitKey�������޷���ʾͼ��  

	}  

	system("pause");  
}  

//���ȶ���
/*
#include <iostream>
#include <queue>
#include <string>
using namespace std;

struct Human
{
	string name;
	int age;
	Human(string _name,int _age):name(_name),age(_age) {}
};

class compare_human
{
public:
	bool operator() (const Human& h1,const Human& h2)
	{
		return h1.age<h2.age;
	}
};

typedef priority_queue<Human,vector<Human>,compare_human>				  human_pariority_queue  ;
int main()
{
	string names[]={"xiao","messi","zhao","C","zhang","Lilu"};
	human_pariority_queue q;
	for(int i=0;i<6;i++)
	{
		q.push(Human(names[i],i*10));
	}
	for (int i=0;i<6;i++)
	{
		cout<<q.top().name<<"   "<<q.top().age<<endl;
		q.pop();
	}
	return 0;
}
*/