#ifndef _STEREO_MATCH_H_
#define _STEREO_MATCH_H_

#pragma once

#include <vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/calib3d/calib3d.hpp"

using namespace std;

class StereoMatch
{
public:
	StereoMatch(void);
	virtual ~StereoMatch(void);

	int StereoMatch::bmMatch(cv::Mat& frameLeft, cv::Mat& frameRight, cv::Mat& disparity, cv::Mat& imageLeft, cv::Mat& imageRight);
	int StereoMatch::sgbmMatch(cv::Mat& frameLeft, cv::Mat& frameRight, cv::Mat& disparity, cv::Mat& imageLeft, cv::Mat& imageRight);
	int StereoMatch::init(int imgWidth, int imgHeight, const char* xmlFilePath);
	int StereoMatch::loadCalibData(const char* xmlFilePath);
	int StereoMatch::getDisparityImage(cv::Mat& disparity, cv::Mat& disparityImage, bool isColor);
	int StereoMatch::getPointClouds(cv::Mat& disparity, cv::Mat& pointClouds);
	void StereoMatch::savePointClouds(cv::Mat& pointClouds, const char* filename);

	cv::StereoBM	m_BM;				// ����ƥ�� BM ����
	cv::StereoSGBM	m_SGBM;				// ����ƥ�� SGBM ����
	double			m_FL;				// �������У����Ľ���ֵ

private:
	bool	m_Calib_Data_Loaded;		// �Ƿ�ɹ����붨�����
	cv::Mat m_Calib_Mat_Q;				// Q ����
	cv::Mat m_Calib_Mat_Remap_X_L;		// ����ͼ����У����������ӳ����� X
	cv::Mat m_Calib_Mat_Remap_Y_L;		// ����ͼ����У����������ӳ����� Y
	cv::Mat m_Calib_Mat_Remap_X_R;		// ����ͼ����У����������ӳ����� X
	cv::Mat m_Calib_Mat_Remap_Y_R;		// ����ͼ����У����������ӳ����� Y
	cv::Mat m_Calib_Mat_Mask_Roi;		// ����ͼУ�������Ч����
	cv::Rect m_Calib_Roi_L;				// ����ͼУ�������Ч�������
	cv::Rect m_Calib_Roi_R;				// ����ͼУ�������Ч�������

	int m_frameWidth;					// ֡��
	int m_frameHeight;					// ֡��
	int m_numberOfDisparies;			// �Ӳ�仯��Χ
};

#endif