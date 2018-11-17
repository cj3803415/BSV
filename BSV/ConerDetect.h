#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"

using namespace std;
using namespace cv;


/*
 *����˵������ֵ���ص�����,������Harris�ǵ��⺯��ͬʱʹ��
*/
Mat image;
Mat imageGray;
int thresh=200;
int MaxThresh=255;
void Trackbar(int,void*)
{
	Mat dst,dst8u,dstshow,imageSource;
	dst=Mat::zeros(image.size(),CV_32FC1);  
	imageSource=image.clone();
	cornerHarris(imageGray,dst,3,3,0.04,BORDER_DEFAULT);//�ǵ���
	normalize(dst,dst8u,0,255,CV_MINMAX);  //��һ��
	convertScaleAbs(dst8u,dstshow);//ʹ�����Ա任ת����������Ԫ�س�8λ�޷�������
	/*
	dst��Ŀ��ͼ����ʾ��������һ��ת����Harris�ǵ���ͼ�񣬴�С��ԭͼ��һ�£���
	 ��ֵԽ��Խ�п����ǽǵ㡣
	image����������ɫת������˹�˲�����һ�����Լ����Ա任��Ϊdstshow��
	*/
	namedWindow("dst",WINDOW_NORMAL);
	imshow("dst",dstshow);
	for(int i=0;i<image.rows;i++)
	{
		for(int j=0;j<image.cols;j++)
		{
			if(dstshow.at<uchar>(i,j)>thresh)  //��ֵ�ж�
			{
				circle(imageSource,Point(j,i),2,Scalar(0,0,255),2); //��ע�ǵ�
			}
		}
	}
	imshow("Corner Detected",imageSource);
}
/*
 *����˵����Harris�ǵ��⺯��
*/
void CornerDetect(Mat Img)
{
	image = Img;
	cvtColor(image,imageGray,CV_RGB2GRAY);//��ɫ�ռ�ת���������ô�ת���ɻҶ�ͼ
	GaussianBlur(imageGray,imageGray,Size(5,5),1); // �˲�
	namedWindow("Corner Detected",WINDOW_NORMAL);
	createTrackbar("threshold��","Corner Detected",&thresh,MaxThresh,Trackbar);
	imshow("Corner Detected",image);
	Trackbar(0,0);
	waitKey();
}
