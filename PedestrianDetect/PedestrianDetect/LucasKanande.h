#include"total.h"
#include"cxcore.h"

using namespace cv;

const int MAX_CORNERS=500;

vector<Mat> countPedestrial(vector<Mat> total,vector<Mat> nextFrame);
bool DiffMat(Mat matA,Mat matB)