#include"PosImageScale.h"

void PosImageScale(){

	string img_path;
	ifstream fin(original_pos_img_lst);
	ofstream fout(cap_pos_img_lst);
	int count=0;
	while (getline (fin,img_path))
	{
        cout<<"´¦Àí£º"<<img_path<<endl;  
		Mat src=imread (img_path,1);
		Mat roi = src(Rect(1,3,64,128));
		char buf[128];
		sprintf(buf,"pos-cap-%06d.png",count++);
		string new_path=img_to_save_path+string(buf);
		imwrite (new_path,roi);
		fout<<new_path<<endl;
	}
	fout.close ();
	fin.close ();
}