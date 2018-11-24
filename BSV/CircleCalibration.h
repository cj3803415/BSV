#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include <iostream>
#include "FeatureDetect.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include "stereo.h"
#include "global.h"
#include <iostream>
#include <fstream> 
#include <conio.h>
#include <cmath>

using namespace cv;
using namespace std;

/*
 *����˵�����������궨
 *���������
 PicPath:�궨����ͼ���ļ���·��
 Cali_Result������궨�����.txt�ļ���
 row_corner_num���궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
 column_corner_num���궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
 grid_width:ʵ�ʲ����õ��ı궨����ÿ�����̸�ĳ�
 grid_height:ʵ�ʲ����õ��ı궨����ÿ�����̸�Ŀ�
 Flag:�����־��1Ϊ�����������Ϊ�����
 *�� �� ֵ����
 */
void CircleCalibration(string PicPath,string Cali_Result,int row_corner_num, 
	int column_corner_num,double grid_width,double grid_height,string ImgPath,int Flag);

/*
 *�������ܣ��ͷ�BlobSeq�е�Blob�����ڴ棬���BlobSeq�е�����
 *����˵��
 CArray<Blob> BlobSeq	���ͷ��ڴ��������ſ���С�
 */
/*�����*/
void Blob_ReleaseBlobSeq();

/*
 *�������ܣ�����ſ�������������ָ����Χ�ڵ��ſ�����
 *����˵��
 CArray<Blob> BlobSeq     ��������ſ����
 int MinArea,MaxArea    ָ�����ſ������Χ
*/
/*�����*/
void Blob_DenoisingArea(int MinArea, int MaxArea);

/*
 *�������ܣ����Բ������
 *����˵��
 const CvArr* SrcImage         ������ĻҶ�ͼ����ֵ��ͼ��
 double lowthresh, highthresh  ����ӦCanny��Ե���ĸߵ���ֵ
 int LeastPixelNum             Ŀ��������������С���ص���Ŀ
 double AspectRatio            �ݺ��
 double Circularity            Բ�ζ�
*/
/*�����*/
void Detect_CircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity);

/*
 *�������ܣ���Բ�α�־���������
 *����˵��
 vector<Point2f> buff         �������Բ������
 �������أ�������Բ������
*/
vector<Point2f> CircleSort(vector<Point2f> buff,vector<Point2f> Bigbuff);

/*
 *�������ܣ���������֮��ľ���
 *����˵��
 vector<Point2f> buff         �������Բ�����꼯
 int a					      �������
 int b                        �������
*/
int CalDistance(vector<Point2f> buff,int a,int b);

/*
 *�������ܣ�ִ��˫Ŀ�궨��ע�⣺˫Ŀ�궨��Ҫ��ִ������������ı궨�����
 *����˵��
 CString LCalibdataPath			�����ͼƬ����
 CString RCalibdataPath			�����ͼƬ����
 CString SCaliFile				˫Ŀ�궨���ݱ����ļ���
*/
void StereoCircleCalibration(CString LCalibdataPath,CString RCalibdataPath,CString SCaliFile);