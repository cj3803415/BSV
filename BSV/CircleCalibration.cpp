#include "stdafx.h"
#include "CircleCalibration.h"

using namespace std;
using namespace cv;

//�ſ��������
CArray<Blob> BlobSeq;
//˫Ŀ����궨��ز�������
StereoCalib stereoCalib;
StereoCalib::CornerDatas cornersDatas;
StereoCalib::CameraParams cameraParams1;
StereoCalib::CameraParams cameraParams2;
StereoCalib::StereoParams stereoParams;
StereoCalib::RemapMatrixs remapMatrixs;

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
	Size board_size=Size(column_corner_num,row_corner_num);/* �궨����ÿ�С��е�Բ���� */
	vector<Point2f> image_points_buf;  /* ����ÿ��ͼ���ϼ�⵽��Բ�� */
	vector<Point2f> new_points_buf; /* ����ÿ��ͼ���������Ĵ�ԲԲ�� */
	vector<Point2f> image_bigpoints_buf; /* ����ÿ��ͼ���ϼ�⵽�Ĵ�ԲԲ�� */
	vector<vector<Point2f>> image_points_seq; /* �����⵽������Բ�� */
	vector<vector<Point2f>> image_bigpoints_seq; /* �����⵽�����д�ԲԲ�� */
	while (getline(fin,filename))
	{
		//2��Բ�μ�Ⲣ����
		//����ͼ��
		Mat imageInput=imread(filename);
		if (image_count == 0)  //�Զ���ĵ�һ��ͼƬ��ߴ���ͼƬ��Ŀ��
			{
				image_size.width = imageInput.cols;
				image_size.height =imageInput.rows;			
				cout<<"ͼƬ���Ϊ��"<<image_size.width<<endl;
				cout<<"ͼƬ�߶�Ϊ��"<<image_size.height<<endl;
			}
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
		//�̶���ֵ�ָ�ͼ�񣬶�ֵ��ͼ��
		threshold(LFilImg, LBinImg, 100, 255, CV_THRESH_BINARY);
			
		//Բ�γ�ʼ��⣬δ����
		Blob_ReleaseBlobSeq();
		Detect_CircleDetect(LBinImg, 0, 255, 0.8, 0.8);
		Blob_DenoisingArea(600, 8000);
		int ln = BlobSeq.GetSize();
		for (int i=0;i<ln;i+=1)
		{
			Blob pB = BlobSeq.GetAt(i);
			Point2f center;
			center.x = pB.BlobX;
			center.y = pB.BlobY;
			image_points_buf.push_back(center);
		}
		if (image_points_buf.size()!=99)
		{
			cout<<"��"<<image_count<<"��ͼƬ��ȡ�궨��������ƥ�䣡"<<endl;
			image_count++;	
			drawChessboardCorners(LBinImg,board_size,image_points_buf,false); 
			namedWindow("����궨",WINDOW_NORMAL);
			imshow("����궨",LBinImg);//��ʾͼƬ
			waitKey(500);
			image_points_buf.clear();
		}
		else
		{
			//Բ�α�־������
			cout<<"��"<<image_count+1<<"��ͼƬ��ȡ�궨��ɹ���"<<endl;
			image_count++;
			success_count++;
			//�ڴ˴��������ʾδ����ǰ��Բ����ͼƬ�Ĵ���
			
			//ɸѡ����ԲԲ��
			Blob_DenoisingArea(1500, 8000);
			int bn = BlobSeq.GetSize();
			for (int i=0;i<bn;i+=1)
			{
				Blob pB = BlobSeq.GetAt(i);
				Point2f center;
				center.x = pB.BlobX;
				center.y = pB.BlobY;
				image_bigpoints_buf.push_back(center);	
			}
			new_points_buf = CircleSort(image_points_buf,image_bigpoints_buf); //image_points_buf���ܴ��ڱ���յ�����
			//�����м�⵽��Բ�ΰ�˳���ʶ����
			drawChessboardCorners(LBinImg,board_size,new_points_buf,true); 
			namedWindow("Բ�μ�Ⲣ����",WINDOW_NORMAL);
			imshow("Բ�μ�Ⲣ����",LBinImg);
			waitKey(500);
			image_bigpoints_seq.push_back(image_bigpoints_buf);
		}
		image_points_seq.push_back(new_points_buf);
		//ÿ��ͼƬ�����֮�����ͼƬ����
		image_bigpoints_buf.clear();
		image_points_buf.clear();
		new_points_buf.clear();
	}

	if(success_count<image_count)
	{
		AfxMessageBox(_T("ͼƬ��Բ��ʶ������������ǿ���˳���"));
		exit(1);//�����˳�
	}

	//3��������궨
	cout<<"\n***��ʼ�궨***"<<endl;
	Size square_size = Size(grid_width,grid_height);  //ʵ�ʲ����õ��ı궨����ÿ�����̸�Ĵ�С
	vector<vector<Point3f>> object_points; //����궨���Ͻǵ����ά����
	Mat cameraMatrix=Mat(3,3,CV_32FC1,Scalar::all(0)); //������ڲ������� 
	vector<int> point_counts;  // ÿ��ͼ����Բ�������
	Mat distCoeffs=Mat(1,5,CV_32FC1,Scalar::all(0)); //�������5������ϵ����k1,k2,p1,p2,k3 
	vector<Mat> tvecsMat;  //ÿ��ͼ�����ת����
	vector<Mat> rvecsMat; //ÿ��ͼ���ƽ������
	//��ʼ���궨���Ͻǵ����ά����
	int i,j,t;
	for (t=0;t<image_count;t++) 
	{
		vector<Point3f> tempPointSet;
		for (i=0;i<board_size.height;i++) 
		{
			for (j=0;j<board_size.width;j++) 
			{
				Point3f realPoint;
				// ����궨�������������ϵ��z=0��ƽ����
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
	calibrateCamera(object_points,image_points_seq,image_size,cameraMatrix,distCoeffs,rvecsMat,tvecsMat,0);
	cout<<"�궨��ɣ�"<<endl;
	//4���Ա궨�����������
	cout<<"***��ʼ���۱궨���***\n";
	double total_err = 0.0; //����ͼ���ƽ�������ܺ�
	double err = 0.0; // ÿ��ͼ���ƽ�����
	vector<Point2f> image_points2; //�������¼���õ���ͶӰ��
	cout<<"->ÿ��ͼ��ı궨��\n";
	fout<<"->ÿ��ͼ��ı궨��\n";
	for (i=0;i<image_count;i++)
	{
		vector<Point3f> tempPointSet=object_points[i];
		//ͨ���õ������������������Կռ����ά���������ͶӰ���㣬�õ��µ�ͶӰ��
		projectPoints(tempPointSet,rvecsMat[i],tvecsMat[i],cameraMatrix,distCoeffs,image_points2);
		//�����µ�ͶӰ��;ɵ�ͶӰ��֮������
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

	//5������궨���  	
	std::cout<<"***��ʼ����궨���***"<<endl;       
	Mat rotation_matrix = Mat(3,3,CV_32FC1, Scalar::all(0));//����ÿ��ͼ�����ת����
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

	//6��ͼƬ��������ʾ
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
		waitKey();	//�û���������;����û�û�а��¼�,������ȴ�
 		StrStm.clear();
 		filePath.clear();
 		StrStm<<i+1;
 		StrStm>>imageFileName;
 		imageFileName += "_d.jpg";
		string A((LPCTSTR)ProjectPath);
		string SavefilePath=A+"Save_LImage\\"+imageFileName;//����ͼ��·��
 		imwrite(SavefilePath,newimage);
 	}
 	std::cout<<"***�������***"<<endl;
	//��Txt�ļ��򿪱궨��� 
	ShellExecute(NULL, _T("open"), Cali_Result.c_str(), NULL, NULL, SW_SHOW);

	//7����BSVDlg.cpp���ݱ����Լ���ɵĵ�����궨����
 	cornersDatas.nImages = image_count;
	cornersDatas.boardSize = square_size;
	cornersDatas.imageSize = image_size;
	cornersDatas.objectPoints = object_points;
	if(Flag==1)
	{
		cameraParams1.imageSize = image_size;
		cameraParams1.cameraMatrix = cameraMatrix;
		cameraParams1.distortionCoefficients = distCoeffs;
		cameraParams1.rotations = rvecsMat;
		cameraParams1.translations = tvecsMat;
		cornersDatas.imagePoints1 = image_points_seq;
	}
	else 
	{
		cameraParams2.imageSize = image_size;
		cameraParams2.cameraMatrix = cameraMatrix;
		cameraParams2.distortionCoefficients = distCoeffs;
		cameraParams2.rotations = rvecsMat;
		cameraParams2.translations = tvecsMat;
		cornersDatas.imagePoints2 = image_points_seq;
	}

	return ;
}

void StereoCircleCalibration(CString LCalibdataPath,CString RCalibdataPath,CString SCaliFile)
{
	//1����������궨����cameraParams1��cameraParams2������.yml�ļ���
	CString ymlFile1,ymlFile2;
	ymlFile1.Format(_T("%s\\data\\cameraParams1.yml"),ProjectPath);
	ymlFile2.Format(_T("%s\\data\\cameraParams2.yml"),ProjectPath);
	stereoCalib.saveCameraData(cameraParams1, (LPCTSTR)ymlFile1);
	stereoCalib.saveCameraData(cameraParams2, (LPCTSTR)ymlFile2);

	//2����ʼ˫Ŀ�궨
	stereoParams.cameraParams1 = cameraParams1;
	stereoParams.cameraParams2 = cameraParams2;
	stereoCalibrate(cornersDatas.objectPoints, cornersDatas.imagePoints1, 
		cornersDatas.imagePoints2,cameraParams1.cameraMatrix, 
		cameraParams1.distortionCoefficients, cameraParams2.cameraMatrix,
		cameraParams2.distortionCoefficients, cornersDatas.imageSize, 
		stereoParams.rotation,stereoParams.translation, 
		stereoParams.essential, stereoParams.foundational,
		TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-5), 
		CV_CALIB_FIX_INTRINSIC);

	//3��˫Ŀ����
	stereoCalib.mstereoRectify(cameraParams1, cameraParams2,
		stereoParams, remapMatrixs);
	//ͼƬ��������ʾ
	ifstream fin11(LCalibdataPath);
	ifstream fin12(RCalibdataPath);
	string str11, str22;
	Mat img1r, img2r;
	CString name11,name22;
	int cnt = 0;
	while (getline(fin11, str11) && getline(fin12, str22))
	{
		cnt++;
		Mat image1 = imread(str11);
		Mat image2 = imread(str22);
		stereoCalib.remapImage(image1, image2, img1r, img2r, remapMatrixs);
		name11.Format(_T("%s\\Save_LImage\\Left_%d.jpg"),ProjectPath,cnt);
		name22.Format(_T("%s\\Save_RImage\\Right_%d.jpg"),ProjectPath,cnt);
		string Name11((LPCTSTR)name11);
		string Name22((LPCTSTR)name22);
		imwrite(Name11, img1r);
		imwrite(Name22, img2r);
	}
	//��У�����ͼ�񻭳�����
	string Name11((LPCTSTR)name11);
	string Name22((LPCTSTR)name22);
	Mat imgLr = imread(Name11);
	Mat imgRr = imread(Name22);
	Mat img(cornersDatas.imageSize.height*0.5, cornersDatas.imageSize.width, 
		CV_8UC3);//����IMG���߶�һ�������˫��
	Mat imgPart1 = img(Rect(0, 0, cornersDatas.imageSize.width*0.5, 
		cornersDatas.imageSize.height*0.5));//ǳ����
	Mat imgPart2 = img(Rect(cornersDatas.imageSize.width*0.5, 0, 
		cornersDatas.imageSize.width*0.5, cornersDatas.imageSize.height*0.5));//ǳ����
	resize(imgLr, imgPart1, imgPart1.size(), 0, 0, CV_INTER_AREA);
	resize(imgRr, imgPart2, imgPart2.size(), 0, 0, CV_INTER_AREA);//�ı�ͼ��ߴ磬����0,0��
	for (int i = 0; i < img.rows; i += 16) //������
		line(img, Point(0, i), Point(img.cols, i), Scalar(0, 255, 0), 1, 8);
	namedWindow("У�����ͼ��", WINDOW_NORMAL);
	imshow("У�����ͼ��", img);

	//4�����ü���Լ������˫Ŀ�궨���
	double err;//�������ص�Լ�������ƽ��ֵ
	stereoCalib.getStereoCalibrateError(cornersDatas, stereoParams, err);
	CString error;
	error.Format(_T("���ص�Լ�������ƽ��ֵΪ��%f"),err);
	AfxMessageBox(error);

	//5������˫Ŀ�궨����
	//a)������txt�ļ���
	ofstream StereoCaliResult(SCaliFile);
	StereoCaliResult<<"������ڲ�������\n";	
	StereoCaliResult<<cameraParams1.cameraMatrix<<'\n';	
	StereoCaliResult<<"���������ϵ����\n";
	StereoCaliResult<<cameraParams1.distortionCoefficients<<'\n';
	StereoCaliResult<<"������ڲ�������\n";	
	StereoCaliResult<<cameraParams2.cameraMatrix<<'\n';	
	StereoCaliResult<<"���������ϵ����\n";
	StereoCaliResult<<cameraParams2.distortionCoefficients<<'\n';
	StereoCaliResult<<"���������ת����\n";	
	StereoCaliResult<<stereoParams.rotation<<'\n';	
	StereoCaliResult<<"�������ƽ�ƾ���\n";
	StereoCaliResult<<stereoParams.translation<<'\n';
	StereoCaliResult<<"��������\n";
	StereoCaliResult<<stereoParams.essential<<'\n';
	StereoCaliResult<<"��������\n";
	StereoCaliResult<<stereoParams.foundational<<'\n';

	//b)������yml�ļ���
	CString ymlFile;
	ymlFile.Format(_T("%s\\data\\CalibrationDatas.yml"),ProjectPath);
	stereoCalib.saveCalibrationDatas((LPCTSTR)ymlFile, cornersDatas, stereoParams, 
		remapMatrixs);
	
	//c)������xml�ļ���
	CString xmlFile;
	xmlFile.Format(_T("%s\\data\\CalibrationDatas.xml"),ProjectPath);
	stereoCalib.saveCalibrationDatas((LPCTSTR)xmlFile, cornersDatas, stereoParams, remapMatrixs);
}

//������Բ��������
vector<Point2f> CircleSort(vector<Point2f> buff,vector<Point2f> Bigbuff)
{
	//ʶ�������Բ��λ��
	int d, min_d,max_d,count=0,n;
	n = Bigbuff.size();
	vector<int> distance; //�����Բ֮������о���
	vector<Point2f> BigCircle; //�����Ĵ�Բ���ο��궨���Բ���
	for(int i=0;i<n;i++)
	{
		for (int j=0;j<n,j!=i;j++)
		{
			d = sqrt((Bigbuff[i].x-Bigbuff[j].x)*(Bigbuff[i].x-Bigbuff[j].x)
				+(Bigbuff[i].y-Bigbuff[j].y)*(Bigbuff[i].y-Bigbuff[j].y));
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
			d = CalDistance(Bigbuff,i,j);
			if(d == max_d) //������������Բ
			{
				aa=i;
				bb=j;
			}
			if(d == min_d)//������С������Բ
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
			ee=i; //ʣ�µĴ�Բ
		}
	}
	//Bigbuff[ee]; //b4Բ������
	//�ֱ�b1��b2Բ
	int judge;
	if(CalDistance(Bigbuff,ee,cc)>CalDistance(Bigbuff,ee,dd))
	{
		BigCircle.push_back(Bigbuff[dd]);//b1Բ
		BigCircle.push_back(Bigbuff[cc]);//b2Բ
		judge = cc;//��b2Բ���ο����ֱ�b3,b5Բ
	}
	else
	{
		BigCircle.push_back(Bigbuff[cc]);//b1Բ
		BigCircle.push_back(Bigbuff[dd]);//b2Բ
		judge = dd;//��b2Բ���ο����ֱ�b3,b5Բ
	}
	//�ֱ�b3��b5Բ
	if(CalDistance(Bigbuff,judge,aa)>CalDistance(Bigbuff,judge,bb))
	{
		BigCircle.push_back(Bigbuff[bb]);//b3Բ
		BigCircle.push_back(Bigbuff[ee]);//b4Բ
		BigCircle.push_back(Bigbuff[aa]);//b5Բ
	}
	else
	{
		BigCircle.push_back(Bigbuff[aa]);//b3Բ
		BigCircle.push_back(Bigbuff[ee]);//b4Բ
		BigCircle.push_back(Bigbuff[bb]);//b5Բ
	}
	//��Բֱ�����
	std::vector<cv::Point> Lpoint1,Lpoint2;//cv::point����洢�����������ݣ����ܻ�Ӱ�쾫��
	cv::Vec4f Line1,Line2;
	Lpoint1.push_back (BigCircle[0]);//�����0��ʼ��0����b1Բ��������4����b5Բ
	Lpoint1.push_back (BigCircle[3]);
	fitLine(Lpoint1,Line1,CV_DIST_L2,0,0.01,0.01);//�Դ�Բ1��4ֱ�����,��Ϊ����ο���
	Lpoint2.push_back (BigCircle[2]);
	Lpoint2.push_back (BigCircle[4]);
	fitLine(Lpoint2,Line2,CV_DIST_L2,0,0.01,0.01);//�Դ�Բ3��5ֱ�����,��Ϊ����ο���
	
	//ȷ��ÿ�����λ�ò���������
	vector<Point2f> NewPointSeq;//����Ҫ���ڲ����б���������
	vector<int> distance14,distance35; 
	int d1,d2,an,distance14_2,distance35_2;
	an = buff.size();
	distance14_2 = 
		(Line1[1]*BigCircle[1].x-Line1[0]*BigCircle[1].y+Line1[0]*Line1[3]-Line1[1]*Line1[2])
		/sqrt(Line1[1]*Line1[1]+Line1[0]*Line1[0]); //�ο���2��1��4���߾��룬ȷ������
	distance35_2 = 
		(Line2[1]*BigCircle[1].x-Line2[0]*BigCircle[1].y+Line2[0]*Line2[3]-Line2[1]*Line2[2])
		/sqrt(Line2[1]*Line2[1]+Line2[0]*Line2[0]); //�ο���2��3��5���߾��룬ȷ������
	
	for(int i=0;i<an;i++)//���е㵽ֱ��1��4�ľ��룬��������
	{
		d1 = 
			(Line1[1]*buff[i].x-Line1[0]*buff[i].y+Line1[0]*Line1[3]-Line1[1]*Line1[2])
			/sqrt(Line1[1]*Line1[1]+Line1[0]*Line1[0]); 
		distance14.push_back(d1);
	}

	for(int j=0;j<an;j++)//���е㵽ֱ��3��5�ľ��룬��������
	{
		d2 = 
			(Line2[1]*buff[j].x-Line2[0]*buff[j].y+Line2[0]*Line2[3]-Line2[1]*Line2[2])
			/sqrt(Line2[1]*Line2[1]+Line2[0]*Line2[0]); 
		distance35.push_back(d2);
	}

	if (distance14_2 >0)
	{
		sort(distance14.begin(),distance14.end());//��������
	}
	else
	{
		sort(distance14.rbegin(),distance14.rend());//��������
	}
	if (distance35_2 >0)
	{
		sort(distance35.begin(),distance35.end());//��������
	}
	else
	{
		sort(distance35.rbegin(),distance35.rend());//��������
	}

	for(int i=0;i<an;i++)
	{
		int distance14_i,distance35_i,row,column,NewPosition;
		distance14_i = 
		(Line1[1]*buff[i].x-Line1[0]*buff[i].y+Line1[0]*Line1[3]-Line1[1]*Line1[2])
		/sqrt(Line1[1]*Line1[1]+Line1[0]*Line1[0]); //�ο���i��1��4���߾��룬ȷ������
		vector<int>::iterator result1 = 
			find(distance14.begin(),distance14.end(),distance14_i);
		if (result1!=distance14.end())//��������distance14���ҵ���distance14_i
		{ 
			column = (result1 - distance14.begin())/9+1; //ȷ������
		}
		distance35_i = 
		(Line2[1]*buff[i].x-Line2[0]*buff[i].y+Line2[0]*Line2[3]-Line2[1]*Line2[2])
			/sqrt(Line2[1]*Line2[1]+Line2[0]*Line2[0]);  //�ο���i��3��5���߾��룬ȷ������
		vector<int>::iterator result2 = 
			find(distance35.begin(),distance35.end(),distance35_i);
		if (result2!=distance35.end())//��������distance35���ҵ���distance35_i
		{ 
			row = (result2 - distance35.begin())/11+1; //ȷ������,iterator - vector.begin()
		}											   //����iteratorָ���λ��
		NewPosition = (row-1)*11+column;
		NewPointSeq.resize(an); //��NewPointSeq�Ĵ�С����Ϊan
		NewPointSeq[NewPosition-1] = buff[i];//����Ӧ��ÿ��λ�ý��и�ֵ
	}
	return NewPointSeq;
}

int CalDistance(vector<Point2f> buff,int a,int b)
{
	int dis;
	dis = sqrt((buff[a].x-buff[b].x)*(buff[a].x-buff[b].x)
		+(buff[a].y-buff[b].y)*(buff[a].y-buff[b].y));
	return dis;
}

void Blob_ReleaseBlobSeq()
{
	if (!BlobSeq.IsEmpty())
	{
		BlobSeq.RemoveAll();
		BlobSeq.FreeExtra();
	}
}

void Blob_DenoisingArea(int MinArea, int MaxArea)
{
	if (!BlobSeq.IsEmpty())
	{
		int ln = BlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = BlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				BlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		BlobSeq.FreeExtra();
	}
}

void Detect_CircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_EXTERNAL,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	
	//������⵽��ͼ������
	//Scalar color = Scalar(rand() % 255, rand() % 255, rand() % 255);
	//drawContours ( EdgeImg, contours,-1,color);
	//namedWindow("�������",WINDOW_NORMAL);
	//imshow("�������",EdgeImg);
	//waitKey(500);
	//EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	BlobSeq.SetSize(ln,10);
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

					BlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	BlobSeq.FreeExtra();
}
