#pragma once
#include "stdafx.h"
#include "afxwin.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/nonfree/features2d.hpp"
#include <iostream>
#include <fstream> 
#include <conio.h>
#include "global.h"
#include "Stereo.h"

using namespace std;
using namespace cv;

//˫Ŀ����궨��ز���
StereoCalib m_StereoCalib;
StereoCalib::CornerDatas m_cornersDatas;
StereoCalib::CameraParams m_cameraParams1;
StereoCalib::CameraParams m_cameraParams2;
StereoCalib::StereoParams m_stereoParams;
StereoCalib::RemapMatrixs m_remapMatrixs;

/*
 *����˵�������ݱ궨ͼƬ��д�����LCamera_calibdata.txt����
 *���������
 PicPath:�궨����ͼ���ļ���·��
 PicNum:�궨ͼƬ������
 *�������: ��
 *�� �� ֵ����
*/
void WriteTxt(string PicPath,int PicNum)
{
	fstream file(PicPath, ios::out);
	ofstream fout(PicPath);
	int k;
	for(k=1;k<=PicNum;k++)
	{
		fout<<ProjectPath<<"Ori_LImage\\chess"<<k<<".bmp"<<endl;
	}
}
/*
 *����˵������д�����LCamera_calibdata.txt����
*/
void WriteTxt2(string PicPath,int PicNum)
{
	fstream file(PicPath, ios::out);
	ofstream fout(PicPath);
	int k;
	for(k=1;k<=PicNum;k++)
	{
		fout<<ProjectPath<<"Ori_RImage\\chess"<<k<<".bmp"<<endl;
	}
}

/*
 *����˵������ֵ���ص�����,������Harris�ǵ��⺯��ͬʱʹ��
*/
//Mat image;
//Mat imageGray;
//int thresh=200;
//int MaxThresh=255;
//void Trackbar(int,void*)
//{
//	Mat dst,dst8u,dstshow,imageSource;
//	dst=Mat::zeros(image.size(),CV_32FC1);  
//	imageSource=image.clone();
//	cornerHarris(imageGray,dst,3,3,0.04,BORDER_DEFAULT);//�ǵ���
//	normalize(dst,dst8u,0,255,CV_MINMAX);  //��һ��
//	convertScaleAbs(dst8u,dstshow);//ʹ�����Ա任ת����������Ԫ�س�8λ�޷�������
//	/*
//	dst��Ŀ��ͼ����ʾ��������һ��ת����Harris�ǵ���ͼ�񣬴�С��ԭͼ��һ�£���
//	 ��ֵԽ��Խ�п����ǽǵ㡣
//	image����������ɫת������˹�˲�����һ�����Լ����Ա任��Ϊdstshow��
//	*/
//	namedWindow("dst",WINDOW_NORMAL);
//	imshow("dst",dstshow);
//	for(int i=0;i<image.rows;i++)
//	{
//		for(int j=0;j<image.cols;j++)
//		{
//			if(dstshow.at<uchar>(i,j)>thresh)  //��ֵ�ж�
//			{
//				circle(imageSource,Point(j,i),2,Scalar(0,0,255),2); //��ע�ǵ�
//			}
//		}
//	}
//	imshow("Corner Detected",imageSource);
//}
/*
 *����˵����Harris�ǵ��⺯��
*/
//void CornerDetect(Mat Img)
//{
//	image = Img;
//	cvtColor(image,imageGray,CV_RGB2GRAY);//��ɫ�ռ�ת���������ô�ת���ɻҶ�ͼ
//	GaussianBlur(imageGray,imageGray,Size(5,5),1); // �˲�
//	namedWindow("Corner Detected",WINDOW_NORMAL);
//	createTrackbar("threshold��","Corner Detected",&thresh,MaxThresh,Trackbar);
//	imshow("Corner Detected",image);
//	Trackbar(0,0);
//	waitKey();
//}

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
void Calibration(string PicPath,string Cali_Result,int row_corner_num, 
	int column_corner_num,double grid_width,double grid_height,string ImgPath,int Flag)
{
	ifstream fin(PicPath);//���ڿհ��лᱨ��
	ofstream fout(Cali_Result);
	Size image_size;
	Size board_size=Size(row_corner_num,column_corner_num);/* �궨����ÿ�С��еĽǵ��� */
	vector<Point2f> image_points_buf;  /* ����ÿ��ͼ���ϼ�⵽�Ľǵ� */
	vector<vector<Point2f>> image_points_seq; /* �����⵽�����нǵ� */
	string filename;
	int image_count=0;  //ͼ������
	int success_count=0;	//ʶ��ɹ�ͼ������
	cout<<"***��ʼʶ��ǵ�***"<<endl;
	while (getline(fin,filename))
	{
		Mat imageInput=imread(filename);
		if (image_count == 0)  //�Զ���ĵ�һ��ͼƬ��ߴ���ͼƬ��Ŀ��
			{
				image_size.width = imageInput.cols;
				image_size.height =imageInput.rows;			
				cout<<"ͼƬ���Ϊ��"<<image_size.width<<endl;
				cout<<"ͼƬ�߶�Ϊ��"<<image_size.height<<endl;
			}
		int a = findChessboardCorners(imageInput,board_size,image_points_buf,
			CALIB_CB_ADAPTIVE_THRESH|CALIB_CB_NORMALIZE_IMAGE);
		if (a == 0)
		{
			cout<<"��"<<image_count+1<<"��ͼƬ�ǵ�ʶ��ʧ�ܣ�"<<endl;
			image_count++;
			drawChessboardCorners(imageInput,board_size,image_points_buf,false); //������ͼƬ�б�ǽǵ�
			namedWindow("����궨",WINDOW_NORMAL);
			imshow("����궨",imageInput);//��ʾͼƬ
			waitKey(500);
		}
		else
		{
			cout<<"��"<<image_count+1<<"��ͼƬ�ǵ�ʶ��ɹ���"<<endl;
			image_count++;
			success_count++;
			image_points_seq.push_back(image_points_buf);
			/* ��ͼ������ʾ�ǵ�λ�� */
			drawChessboardCorners(imageInput,board_size,image_points_buf,true); //������ͼƬ�б�ǽǵ�
			namedWindow("����궨",WINDOW_NORMAL);
			imshow("����궨",imageInput);//��ʾͼƬ
			/*lq:��֪���Ǹ�ʲô�õ�*/
			/*if (image_count == 1)
			{
				imwrite("lq��һ��ͼƬ��·��",imageInput);
			}*/
			waitKey(500);//��ͣ0.5S	
		}
	}
	if(success_count<image_count)
	{
		cout<<"�ǵ�ʶ��ɹ���ͼƬ������"<<success_count<<endl;
		cout<<"���궨ͼƬ���нǵ㲻��ȫ��ʶ�𣬳���ǿ���˳���"<<endl;
		waitKey(3000);
		exit(1);
	}
	/*��ӡ�ǵ�����*/
	int CornerNum=board_size.width*board_size.height;  //ÿ��ͼƬ���ܵĽǵ���
	int total = image_points_seq.size();
	for (int ii=0 ; ii<total ;ii++)
	{
		/*�������ÿ��ͼƬ��⵽��һ���ǵ������*/
		if (0 == ii%CornerNum)	//CornerNum��ÿ��ͼƬ�Ľǵ���������ж������Ϊ�����ͼƬ�ţ����ڿ���̨�ۿ� 
		{	
			int i = -1;
			i = ii/CornerNum;
			int j=i+1;
			cout<<"--> �� "<<j <<"ͼƬ������ --> : "<<endl;
		}
		if (0 == ii%3)	// ���ж���䣬��ʽ����������ڿ���̨�鿴
		{
			cout<<endl;
		}
		else
		{
			cout.width(10);//�����ֶεĿ��
		}
			cout<<" -->"<<image_points_seq[ii][0].x;
			cout<<" -->"<<image_points_seq[ii][0].y;
		}
		/*�������ÿ��ͼƬ��⵽�ǵ������*/
		//cout<<"<--�� "<<ii+1 <<"ͼƬ������ -->"<<endl;
		//for (int jj=0 ; jj<CornerNum ;jj++)
		//{
		//	cout<<" -->"<<image_points_seq[ii][jj].x<<"\t";
		//	cout<<" -->"<<image_points_seq[ii][jj].y<<"\t";
		//	cout<<endl;
		//}
	//}

	//������������궨
	cout<<"\n***��ʼ�궨***"<<endl;
	/*������ά��Ϣ*/
	Size square_size = Size(grid_width,grid_height);  /* ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С */
	vector<vector<Point3f>> object_points; /* ����궨���Ͻǵ����ά���� */
	/*�������*/
	Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* ������ڲ������� */
	vector<int> point_counts;  // ÿ��ͼ���нǵ������
	Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* �������5������ϵ����k1,k2,p1,p2,k3 */
	vector<Mat> tvecsMat;  /* ÿ��ͼ�����ת���� */
	vector<Mat> rvecsMat; /* ÿ��ͼ���ƽ������ */
	/* ��ʼ���궨���Ͻǵ����ά���� */
	int i,j,t;
	for (t=0;t<image_count;t++) 
	{
		vector<Point3f> tempPointSet;
		for (i=0;i<board_size.height;i++) 
		{
			for (j=0;j<board_size.width;j++) 
			{
				Point3f realPoint;
				/* ����궨�������������ϵ��z=0��ƽ���� */
				realPoint.x = i*square_size.width;
				realPoint.y = j*square_size.height;
				realPoint.z = 0;
				tempPointSet.push_back(realPoint);
			}
		}
		object_points.push_back(tempPointSet);
	}
	for (i=0;i<image_count;i++)
	{
		point_counts.push_back(board_size.width*board_size.height);
	}
	/* ��ʼ�궨 */
	calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
	cout<<"�궨��ɣ�"<<endl;
	//�Ա궨�����������
	cout<<"***��ʼ���۱궨���***\n";
	double total_err = 0.0; /* ����ͼ���ƽ�������ܺ� */
	double err = 0.0; /* ÿ��ͼ���ƽ����� */
	vector<Point2f> image_points2; /* �������¼���õ���ͶӰ�� */
	cout<<"->ÿ��ͼ��ı궨��\n";
	fout<<"->ÿ��ͼ��ı궨��\n";
	for (i=0;i<image_count;i++)
	{
		vector<Point3f> tempPointSet=object_points[i];
		/* ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ�� */
		projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);
		/* �����µ�ͶӰ��;ɵ�ͶӰ��֮������*/
		vector<Point2f> tempImagePoint = image_points_seq[i];
		Mat tempImagePointMat = Mat(1,tempImagePoint.size(),CV_32FC2);
		Mat image_points2Mat = Mat(1,image_points2.size(), CV_32FC2);
		for (int j = 0 ; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0,j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0,j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		//norm������ʵ������ͨ���ֱ�ֿ��������(X1-X2)^2��ֵ��Ȼ��ͳһ��ͣ������и���
		total_err += err/=  point_counts[i];   
		std::cout<<"��"<<i+1<<"��ͼ���ƽ����"<<err<<"����"<<endl;   
		fout<<"��"<<i+1<<"��ͼ���ƽ����"<<err<<"����"<<endl;   
	}   
	std::cout<<"����ƽ����"<<total_err/image_count<<"����"<<endl;   
	fout<<"����ƽ����"<<total_err/image_count<<"����"<<endl<<endl;   
	std::cout<<"***������ɣ�***"<<endl;
	//����궨���  	
	std::cout<<"***��ʼ����궨���***"<<endl;       
	Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0)); /* ����ÿ��ͼ�����ת���� */
	fout<<"����ڲ�������"<<endl;   
	fout<<cameraMatrix<<endl<<endl;   
	fout<<"����ϵ����\n";   
	fout<<distCoeffs<<endl<<endl<<endl;   
	for (int i=0; i<image_count; i++) 
	{ 
		fout<<"��"<<i+1<<"��ͼ�����ת������"<<endl;   
		fout<<rvecsMat[i]<<endl;   
		/* ����ת����ת��Ϊ���Ӧ����ת���� */   
		Rodrigues(rvecsMat[i],rotation_matrix);   
		fout<<"��"<<i+1<<"��ͼ�����ת����"<<endl;   
		fout<<rotation_matrix<<endl;   
		fout<<"��"<<i+1<<"��ͼ���ƽ��������"<<endl;   
		fout<<tvecsMat[i]<<endl<<endl;   
	}   
	std::cout<<"***��ɱ���***"<<endl; 
	fout<<endl;

	//��BSVDlg.cpp���ݱ����Լ���ɱ궨���ݵı���
 	m_cornersDatas.nImages = image_count;
	m_cornersDatas.boardSize = square_size;
	m_cornersDatas.imageSize = image_size;
	m_cornersDatas.objectPoints = object_points;
	if(Flag==1)
	{
		m_cameraParams1.imageSize = image_size;
		m_cameraParams1.cameraMatrix = cameraMatrix;
		m_cameraParams1.distortionCoefficients = distCoeffs;
		m_cameraParams1.rotations = rvecsMat;
		m_cameraParams1.translations = tvecsMat;
		m_cornersDatas.imagePoints1 = image_points_seq;
	}
	else 
	{
		m_cameraParams2.imageSize = image_size;
		m_cameraParams2.cameraMatrix = cameraMatrix;
		m_cameraParams2.distortionCoefficients = distCoeffs;
		m_cameraParams2.rotations = rvecsMat;
		m_cameraParams2.translations = tvecsMat;
		m_cornersDatas.imagePoints2 = image_points_seq;
	}
	/* ��ʾ�궨���  */
 	Mat mapx = Mat(image_size,CV_32FC1);
 	Mat mapy = Mat(image_size,CV_32FC1);
 	Mat R = Mat::eye(3,3,CV_32F);
 	std::cout<<"***�������ͼ��***"<<endl;
 	string imageFileName;
 	std::stringstream StrStm;
 	for (int i = 0 ; i != image_count ; i++)
 	{
 		std::cout<<"ͼƬ"<<i+1<<"����ɹ���"<<endl;
		initUndistortRectifyMap(cameraMatrix,distCoeffs,R,cameraMatrix,image_size,CV_32FC1,mapx,mapy);		
 		StrStm.clear();
 		imageFileName.clear();
		string filePath=(LPCTSTR)ProjectPath+ImgPath;
 		StrStm<<i+1;
 		StrStm>>imageFileName;
		filePath+=imageFileName;
		filePath+=".bmp";	//ԭʼͼ��·��
 		Mat imageSource = imread(filePath);
 		Mat newimage = imageSource.clone();
		//��һ�ֲ���Ҫת������ķ�ʽ
		//undistort(imageSource,newimage,cameraMatrix,distCoeffs);
 		remap(imageSource,newimage,mapx, mapy, INTER_LINEAR);
		namedWindow("ԭʼͼ��", WINDOW_NORMAL);//����ʾͼ��ɷŴ���С
		imshow("ԭʼͼ��",imageSource);
		namedWindow("������ͼ��", WINDOW_NORMAL);
		imshow("������ͼ��",newimage);
		waitKey();		//�û���������;����û�û�а��¼�,������ȴ�
 		StrStm.clear();
 		filePath.clear();
 		StrStm<<i+1;
 		StrStm>>imageFileName;
 		imageFileName += "_d.jpg";
		string A((LPCTSTR)ProjectPath);
		string SavefilePath=A+"Save_LImage\\"+imageFileName;	//����ͼ��·��
 		imwrite(SavefilePath,newimage);
 	}
 	std::cout<<"***�������***"<<endl;
	//��Txt�ļ��򿪱궨��� 
	ShellExecute(NULL, _T("open"), Cali_Result.c_str(), NULL, NULL, SW_SHOW);
	return ;
}
