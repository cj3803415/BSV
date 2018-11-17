#include "stdafx.h"
#include "CircleCalibration.h"

using namespace std;
using namespace cv;

CArray<Blob> LeftBlobSeq;

void CircleCalibration(string PicPath,string Cali_Result,int row_corner_num, 
	int column_corner_num,double grid_width,double grid_height,string ImgPath,int Flag)
{
	//1�����ȶ���ͼƬ����Բ�ν��м��
	ifstream fin(PicPath);//���ڿհ��лᱨ��
	ofstream fout(Cali_Result);
	string filename;/* �ļ�������*/
	int image_count=0;  /* ͼ������*/
	int success_count=0;	/* ʶ��ɹ�ͼ������*/
	Size image_size;
	Size board_size=Size(row_corner_num,column_corner_num);/* �궨����ÿ�С��е�Բ���� */
	vector<Point2f> image_points_buf;  /* ����ÿ��ͼ���ϼ�⵽��Բ�� */
	vector<Point2f> image_bigpoints_buf; /* ����ÿ��ͼ���ϼ�⵽�Ĵ�ԲԲ�� */
	vector<vector<Point2f>> image_points_seq; /* �����⵽������Բ�� */
	vector<vector<Point2f>> image_bigpoints_seq; /* �����⵽�����д�ԲԲ�� */
	while (getline(fin,filename))
	{
		Mat imageInput=imread(filename);
		if (image_count == 0)  /* �Զ���ĵ�һ��ͼƬ��ߴ���ͼƬ��Ŀ��*/
			{
				image_size.width = imageInput.cols;
				image_size.height =imageInput.rows;			
				cout<<"ͼƬ���Ϊ��"<<image_size.width<<endl;
				cout<<"ͼƬ�߶�Ϊ��"<<image_size.height<<endl;
			}
		
		//Բ�μ��
		Mat LSrcImg = imageInput;
		Mat LFilImg;
		//˫���˲�������ȥ��
		//bilateralFilter(LSrcImg, LFilImg, 25, 25*2, 25/2);
		//��˹�˲�
		GaussianBlur(LSrcImg, LFilImg, Size(7,7), 0, 0);
			
		Mat LBinImg;
		//����Ӧ��ֵ�ָ�ͼ��	
		//adaptiveThreshold(LFilImg, LBinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, 
		//CV_THRESH_BINARY, 7, 5);	
		//�̶���ֵ�ָ�ͼ�� lq:��ֵ��ͼ��
		threshold(LFilImg, LBinImg, 100, 255, CV_THRESH_BINARY);
			
		Blob_ReleaseLeftBlobSeq();
		Detect_LeftCircleDetect(LBinImg, 0, 255, 0.8, 0.8);
		Blob_DenoisingLeftArea(600, 8000);
		int ln = LeftBlobSeq.GetSize();
		for (int i=0;i<ln;i+=1)
		{
			Blob pB = LeftBlobSeq.GetAt(i);
			Point2f center;
			center.x = pB.BlobX;
			center.y = pB.BlobY;
			image_points_buf.push_back(center);
		}

		if (image_points_buf.size()!=99)
		{
			cout<<"��%d��ͼƬ��ȡ�궨��������ƥ�䣡"<<image_count<<endl;
			image_count++;	
			drawChessboardCorners(LBinImg,board_size,image_points_buf,false); 
			namedWindow("����궨",WINDOW_NORMAL);
			imshow("����궨",LBinImg);//��ʾͼƬ
			waitKey(500);
			image_points_buf.clear();
		}
		else
		{
			//������ͼƬ�б��Բ��
			cout<<"��%d��ͼƬ��ȡ�궨��ɹ���"<<image_count<<endl;
			image_count++;
			success_count++;
			drawChessboardCorners(LBinImg,board_size,image_points_buf,true); 
			namedWindow("����궨",WINDOW_NORMAL);
			imshow("����궨",LBinImg);//��ʾͼƬ
			waitKey(500);
			/*��Բ�����������ٸ�ֵ*/
			image_points_seq.push_back(image_points_buf);
			image_points_buf.clear();
		}

		//ɸѡ����ԲԲ��
		Blob_DenoisingLeftArea(1500, 8000);
		int bn = LeftBlobSeq.GetSize();
		for (int i=0;i<bn;i+=1)
		{
			Blob pB = LeftBlobSeq.GetAt(i);
			Point2f center;
			center.x = pB.BlobX;
			center.y = pB.BlobY;
			image_bigpoints_buf.push_back(center);	
		}
		CircleSort(image_bigpoints_buf);
		image_bigpoints_seq.push_back(image_bigpoints_buf);
		image_bigpoints_buf.clear();
	}
	if(success_count<image_count)
	{
		AfxMessageBox(_T("ͼƬ��Բ��ʶ������������ǿ���˳���"));
		exit(1);//�����˳�
	}
}

//������Բ��������
void CircleSort(vector<Point2f> buff)
{
	int d, min_d,max_d,count=0,n;
	n = buff.size();
	vector<int> distance; //�����Բ֮������о���
	for(int i=0;i<n;i++)
	{
		for (int j=0;j<n,j!=i;j++)
		{
			d = sqrt((buff[i].x-buff[j].x)*(buff[i].x-buff[j].x)
				+(buff[i].y-buff[j].y)*(buff[i].y-buff[j].y));
			distance.push_back(d);
			count++;
		}
	}
	sort(distance.begin(),distance.end());
	max_d = distance[count-1];
	min_d = distance[0];
	int aa,bb,cc,dd,ee;
	for(int i=0;i<n;i++)
	{
		for (int j=0;j<n,j!=i;j++)
		{
			d = CalDistance(buff,i,j);
			//d = sqrt((buff[i].x-buff[j].x)*(buff[i].x-buff[j].x)
			//	+(buff[i].y-buff[j].y)*(buff[i].y-buff[j].y));
			if(d == max_d)
			{
				aa=i;
				bb=j;
			}
			if(d == min_d)
			{
				cc=i;
				dd=j;
			}
		}
	}
	for (int i=0;i<5;i++)
	{
		if(i!=aa&&i!=bb&&i!=cc&&i!=dd)
		{
			ee=i;
		}
	}
	buff[ee]; //b4Բ
	//�ֱ�b1��b2Բ
	int judge;
	if(CalDistance(buff,ee,cc)>CalDistance(buff,ee,dd))
	{
		buff[ee];
		judge = ee;
	}
	else
	{
		buff[dd];
		judge = dd;
	}
	//�ֱ�b3��b5Բ
	if(CalDistance(buff,judge,aa)>CalDistance(buff,judge,bb))
	{
		buff[aa];
	}
	else
	{
		buff[bb];
	}
}

int CalDistance(vector<Point2f> buff,int a,int b)
{
	int dis;
	dis = sqrt((buff[a].x-buff[b].x)*(buff[a].x-buff[b].x)
		+(buff[a].y-buff[b].y)*(buff[a].y-buff[b].y));
	return dis;
}
//
////3������Բ����������ϵ
//	/*������ά��Ϣ*/
//	Size square_size = Size(grid_width,grid_height);  /* ʵ�ʲ����õ��ı궨����Բ��֮��ľ���*/
//	vector<vector<Point3f>> object_points; /* ����궨�������۵���ά���� */
//	int i,j,t;
//	for (t=0;t<image_count;t++) 
//	{
//		vector<Point3f> tempPointSet;
//		for (i=0;i<board_size.height;i++) 
//		{
//			for (j=0;j<board_size.width;j++) 
//			{
//				Point3f realPoint;
//				/* ����궨�������������ϵ��z=0��ƽ���� */
//				realPoint.x = i*square_size.width;
//				realPoint.y = j*square_size.height;
//				realPoint.z = 0;
//				tempPointSet.push_back(realPoint);
//			}
//		}
//		object_points.push_back(tempPointSet);
//	}
//
////4������������궨
//	cout<<"\n***��ʼ�궨***"<<endl;
//	/*�������*/
//	Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); /* ������ڲ������� */
//	vector<int> point_counts;  /* ÿ��ͼ����Բ�������*/
//	Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); /* �������5������ϵ����k1,k2,p1,p2,k3 */
//	vector<Mat> tvecsMat;  /* ÿ��ͼ�����ת���� */
//	vector<Mat> rvecsMat; /* ÿ��ͼ���ƽ������ */
//	/* ��ʼ���궨���Ͻǵ����ά���� */
//	for (i=0;i<image_count;i++)
//	{
//		point_counts.push_back(board_size.width*board_size.height);
//	}
//	/* ��ʼ�궨 */
//	calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
//	cout<<"�궨��ɣ�"<<endl;

void Blob_ReleaseLeftBlobSeq()
{
	if (!LeftBlobSeq.IsEmpty())
	{
		LeftBlobSeq.RemoveAll();
		LeftBlobSeq.FreeExtra();
	}
}

void Blob_DenoisingLeftArea(int MinArea, int MaxArea)
{
	if (!LeftBlobSeq.IsEmpty())
	{
		int ln = LeftBlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = LeftBlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				LeftBlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		LeftBlobSeq.FreeExtra();
	}
}

void Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	
	//������⵽����������
	Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
	drawContours ( EdgeImg, contours,-1,color);
	namedWindow("�������",WINDOW_NORMAL);
	imshow("�������",EdgeImg);
	waitKey(500);
	EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	LeftBlobSeq.SetSize(ln,10);
	for (int i=0; i<ln; i++)
	{
		if (contours[i].size() > 10)
		{
			area = contourArea(contours[i]);		//��i�����������
			length = arcLength(contours[i],true);	//��i���������ܳ�
			cir = (12.56*area)/(length*length);		//��i��������Բ�ζ�
			if (cir > Circularity)
			{
				rect = fitEllipse(contours[i]);    
				width = rect.size.width;	//��Ӿ��εĿ��
				height = rect.size.height;	//��Ӿ��εĸ߶�
				asp = height/width;			//�ݺ��
				if (asp>AspectRatio && asp<(1.0/AspectRatio))
				{
					Blob pB;
					pB.BlobWidth = width;
					pB.BlobHeight = height;
					pB.Area = area;
					pB.Perimeter = length;
					pB.AspectRatio = asp;
					pB.Circularity = cir;
					pB.BlobX = rect.center.x;
					pB.BlobY = rect.center.y;

					LeftBlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	LeftBlobSeq.FreeExtra();
}
