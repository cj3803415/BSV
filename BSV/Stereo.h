#pragma once 

#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "time.h"


using namespace std;
using namespace cv;

class StereoCalib
{
public:
	StereoCalib(void);
	~StereoCalib(void);

	/*
	*���̽ǵ����� �ṹ��
	*/
	struct CornerDatas
	{
		//int			nPoints;			// ���̽ǵ�����
		int			nImages;			// ����ͼ����
		//int			nPointsPerImage;	// ÿ�����̵Ľǵ���
		cv::Size	imageSize;			// ͼ��ֱ���
		cv::Size	boardSize;			// ���̳ߴ�
		vector<vector<cv::Point3f> >	objectPoints;	// ���̽ǵ�������������
		vector<vector<cv::Point2f> >	imagePoints1;	// ����ͼ�����̽ǵ�������������
		vector<vector<cv::Point2f> >	imagePoints2;	// ����ͼ�����̽ǵ�������������
	};

	/*
	*��Ŀ�궨���������
	*/
	struct CameraParams
	{
		cv::Size		imageSize;				// ͼ��ֱ���
		cv::Mat			cameraMatrix;			// ���������
		cv::Mat			distortionCoefficients;	// ������������
		vector<cv::Mat> rotations;				// ����ͼƬ����ת����
		vector<cv::Mat> translations;			// ����ͼƬ��ƽ������
		//int				flags;					// ��Ŀ�궨���õı�־λ
	};

	/*
	*˫Ŀ�궨���������
	*/
	struct StereoParams
	{
		//cv::Size		imageSize;		// ͼ��ֱ���
		CameraParams	cameraParams1;	// ��������궨����
		CameraParams	cameraParams2;	// ��������궨����
		cv::Mat			rotation;		// ��ת����
		cv::Mat			translation;	// ƽ������
		cv::Mat			essential;		// ���ʾ���
		cv::Mat			foundational;	// ��������
		//int				flags;			// ˫Ŀ�궨���õı�־λ
	};

	/*
	*	˫ĿУ�����������
	*/
	struct RemapMatrixs
	{
		cv::Mat		mX1;	// ����ͼ X ��������ӳ�����
		cv::Mat		mY1;	// ����ͼ Y ��������ӳ�����
		cv::Mat		mX2;	// ����ͼ X ��������ӳ�����
		cv::Mat		mY2;	// ����ͼ Y ��������ӳ�����
		cv::Mat		Q;		// ���ڼ�����ά���Ƶ� Q ����
		cv::Rect	roi1;	// ����ͼ��Ч����ľ���
		cv::Rect	roi2;	// ����ͼ��Ч����ľ���
	};

	int saveCameraData(CameraParams& CameraParams1, string filename);

	int getStereoCalibrateError(CornerDatas& cornerDatas, StereoParams& sterepParams, double& err);

	int saveCalibrationDatas(string filename, CornerDatas& cornerDatas, StereoParams& stereoParams, RemapMatrixs& remapMatrixs);

	int remapImage(cv::Mat& img1, cv::Mat& img2, cv::Mat& img1r, cv::Mat& img2r, RemapMatrixs& remapMatrixs);

	void initCorners(Size boardSize, Size imageSize, int nImages, float squareWidth, CornerDatas& cornerDatas);

	void detectCorners(string filename1, string filename2, CornerDatas& cornerDatas);

	void mstereoRectify(CameraParams& CameraParams1, CameraParams& CameraParams2, StereoParams& stereoParams, RemapMatrixs& remapMatrixs);

};