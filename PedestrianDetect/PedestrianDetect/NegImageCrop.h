#include <iostream>
#include <fstream>
#include <stdlib.h> //srand()��rand()����
#include <time.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

int CropImageCount = 0; //�ü������ĸ�����ͼƬ����

void NegImageCrop();