#include"CountourEx.h"

vector<Rect> CountourEx(Mat image){

	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// 使用Threshold检测边缘
	 threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
	/// 找到轮廓
	 findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// 多边形逼近轮廓 + 获取矩形和圆形边界框
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	for( int i = 0; i < contours.size(); i++ )
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );


	/// 画多边形轮廓 + 包围的矩形框 + 圆形框
	 Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(0,0,255), 2 );

	/// 显示在一个窗口
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );

	return boundRect;
}