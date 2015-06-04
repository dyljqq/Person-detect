#include <iostream>
#include <fstream>
#include <stdlib.h> //srand()和rand()函数
#include <time.h> 
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/ml/ml.hpp>

using namespace std;
using namespace cv;

int CropImageCount = 0; //裁剪出来的负样本图片个数

void NegImageCrop();