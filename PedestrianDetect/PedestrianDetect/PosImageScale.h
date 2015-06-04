#include <iostream>
#include <fstream>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace  cv;

const string parent_path="E:/INRIAPerson/INRIAPerson/test/";

const string original_pos_img_lst=parent_path+"pos_compele_path.lst";

const string cap_pos_img_lst=parent_path+"cap_pos_path.lst";
	
const string img_to_save_path=parent_path+"pos_64x128/";

void PosImageScale();