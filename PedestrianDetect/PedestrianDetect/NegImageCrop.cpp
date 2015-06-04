#include"NegImageCrop.h"

void NegImageCrop(){

	Mat src;
    string ImgName;
    char saveName[256];//�ü������ĸ�����ͼƬ�ļ���
    //��ԭʼ������ͼƬ�ļ��б�


    ifstream fin("E:\\INRIAPerson\\INRIAPerson\\train_64x128_H96\\neg.lst");

    ofstream fout("E:\\INRIAPerson\\INRIAPerson\\train_64x128_H96\\neg_64x128.lst");

    //ifstream fin_pos("E:/INRIAPerson/INRIAPerson/train_64x128_H96//pos.lst");

   // ofstream fout_pos("E:/INRIAPerson/INRIAPerson/train_64x128_H96//pos_path.lst")

    cout<<"**************************************"<<endl;
    //һ��һ�ж�ȡ�ļ��б�
    while(getline(fin,ImgName))
    {

        cout<<"Processing "<<ImgName<<endl;
        //get the path of the img
        ImgName = "E:/INRIAPerson/INRIAPerson/" + ImgName;

        src = imread(ImgName);//��ȡͼƬ

        //ͼƬ��СӦ���������ٰ���һ��64*128�Ĵ���
        if(src.cols >= 64 && src.rows >= 128)
        {
            srand(time(NULL));//�������������

            //��ÿ��ͼƬ������ü�10��64*128��С�Ĳ������˵ĸ�����
            for(int i=0; i<10; i++)
            {
                int x = ( rand() % (src.cols-64) ); //���Ͻ�x����
                int y = ( rand() % (src.rows-128) ); //���Ͻ�y����
                //cout<<x<<","<<y<<endl;
                Mat imgROI = src(Rect(x,y,64,128));
                sprintf(saveName,"noperson%06d.jpg",++CropImageCount);//���ɲü����ĸ�����ͼƬ���ļ���
                string img_path="E:/INRIAPerson/INRIAPerson/train_64x128_H96/neg_64x128/"+string(saveName);
                imwrite(img_path, imgROI);//�����ļ�
                fout<<img_path<<endl;
            }
        }

    }

    fout.close();
    fin.close();
    cout<<"****************************"<<endl;
    cout<<"Done!"<<endl;
    cout<<CropImageCount<<endl;

    waitKey();
}