#include"CountourEx.h"

vector<Rect> CountourEx(Mat image){

	Mat threshold_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	/// ʹ��Threshold����Ե
	 threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
	/// �ҵ�����
	 findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

	/// ����αƽ����� + ��ȡ���κ�Բ�α߽��
	vector<vector<Point> > contours_poly( contours.size() );
	vector<Rect> boundRect( contours.size() );
	vector<Point2f>center( contours.size() );
	vector<float>radius( contours.size() );

	for( int i = 0; i < contours.size(); i++ )
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );


	/// ����������� + ��Χ�ľ��ο� + Բ�ο�
	 Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
	for( int i = 0; i< contours.size(); i++ )
		rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), Scalar(0,0,255), 2 );

	/// ��ʾ��һ������
	namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	imshow( "Contours", drawing );

	return boundRect;
}