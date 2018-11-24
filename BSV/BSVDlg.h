
// BSVDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "MVGigE.h"
#include "MVImage.h"
#include "opencv2\opencv.hpp"
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "FeatureDetect.h"
#include "mscomm.h"
#include "Stereo.h"
#include "StereoMatch.h"
#include "PointCloudAnalyzer.h"
#include "ChildDlg1.h"
#include "ChildDlg2.h"
#include "ChildDlg3.h"
#include "afxcmn.h"
#include "ChildDialog1.h"

using namespace std;
using namespace cv;

#define WaitOutTime 3000
#define pi 3.141592653

// CBSVDlg �Ի���
class CBSVDlg : public CDialogEx
{
// ����
public:
	CBSVDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CBSVDlg();	//��������

// �Ի�������
	enum { IDD = IDD_BSV_DIALOG };

// ����Ƕ��ʽ�Ӵ���
public:
	//CChildDlg1* m_childdlg1;
	//CChildDlg2* m_childdlg2;
	//CChildDlg3* m_childdlg3;
	
	CChildDlg1 m_childdlg1;
	CChildDlg2 m_childdlg2;
	CChildDlg3 m_childdlg3;
	CChildDialog1 m_Childdlg1;
	//���ڴ�С����
	CRect m_rect;

public:
	/*���������*/
	HANDLE m_pLCamera;			//��������
	BITMAPINFO  *m_pLBmpInfo;	//��ͼ����Ϣͷָ��
	MV_PixelFormatEnums m_LPixelFormat;	//�����ͼ���ʽ
	LPSTR  m_LeftOriDIBBits;    //ԭʼ��ͼ�񣬴�ͼ���ļ�����������ֱ�ӻ�ȡ���Ҷ�ͼ�������RGBֱ��ת��Ϊ8λ�Ҷ�ͼ
	LPSTR  m_LeftDIBBits;		//Ҫ���д������ͼ���п���������ͼ��Ҳ�п�����ԭʼͼ���һ������
	LPSTR  m_LeftShowDIBBits;   //Ҫ������ʾ����ͼ��
	LONG   m_LeftWidth;         //Ҫ���д������ͼ��Ŀ�ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG   m_LeftHeight;        //Ҫ���д������ͼ��ĸ߶ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG   m_LeftRectLeft;      //��ͼ��ѡ���������߽�
	LONG   m_LeftRectBottom;    //��ͼ��ѡ��������±߽�
	int    m_nLBit;				//��ͼ��λ��
	int    m_LeftOriWidth;      //��ͼ��ԭʼ���
	int    m_LeftOriHeight;     //��ͼ��ԭʼ�߶�
	BOOL   m_bLBufHandle;	    //��ͼ����Խ��д����ʶ,��ÿ�ε���������ص�������,�ñ����true,
							    //������ͼ����д���ʱ,�ñ����false
	CCriticalSection  m_CSectionL;	    //�������ͼ�ٽ���
	CCriticalSection  m_CSectionCopyL;	//�������ͼ�ٽ���
	int m_nLImageIndex;			//������ɼ�ͼƬ��ű�־
	string m_LImgPath;			//�����ͼƬ�洢λ��
	CArray<Blob> m_LeftBlobSeq;	//��¼�����Բ��������������
	Blob LBlob;					//������ſ����ݣ�Բ��ʶ��ʱʹ�ã�
	/*�궨��ر���*/
	BOOL   m_bLCalibDraw;		//�������ʾ�궨λ�ñ��,true��ʾ��ʾ,false��ʾ����ʾ
	CPtrArray m_LeftImgCoordSeq;//��¼����ͼ��ı������
	
	/*���������*/
	HANDLE m_pRCamera;			//��������
	BITMAPINFO  *m_pRBmpInfo;	//��ͼ����Ϣͷָ��
	MV_PixelFormatEnums m_RPixelFormat;	//�����ͼ���ʽ
	LPSTR  m_RightOriDIBBits;   //ԭʼ��ͼ�񣬴�ͼ���ļ�����������ֱ�ӻ�ȡ���Ҷ�ͼ�������RGBֱ��ת��Ϊ8λ�Ҷ�ͼ
	LPSTR  m_RightDIBBits;		//Ҫ���д������ͼ���п���������ͼ��Ҳ�п�����ԭʼͼ���һ������
	LPSTR  m_RightShowDIBBits;	//Ҫ������ʾ����ͼ��
	LONG   m_RightWidth;		//Ҫ���д������ͼ��Ŀ�ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG   m_RightHeight;		//Ҫ���д������ͼ��ĸ߶ȣ������ͼ��������д����ÿ����ԭʼͼ��Ŀ���ǲ�ͬ��
	LONG   m_RightRectLeft;		//��ͼ��ѡ���������߽�
	LONG   m_RightRectBottom;	//��ͼ��ѡ��������±߽�
	int    m_nRBit;				//��ͼ��λ��
	int    m_RightOriWidth;		//��ͼ��ԭʼ���
	int    m_RightOriHeight;	//��ͼ��ԭʼ�߶�
	BOOL   m_bRBufHandle;	    //��ͼ����Խ��д����ʶ,��ÿ�ε���������ص�������,�ñ����true,
							    //������ͼ����д���ʱ,�ñ����false
	CCriticalSection  m_CSectionR;	    //�������ͼ�ٽ���
	CCriticalSection  m_CSectionCopyR;	//�������ͼ�ٽ���
	int m_nRImageIndex;			//������ɼ�ͼƬ��ű�־
	string m_RImgPath;			//�����ͼƬ�洢λ��
	CArray<Blob> m_RightBlobSeq;//��¼�����Բ��������������
	/*�궨��ر���*/
	BOOL   m_bRCalibDraw;		//�������ʾ�궨λ�ñ��,true��ʾ��ʾ,false��ʾ����ʾ
	CPtrArray m_RightImgCoordSeq;//��¼����ͼ��ı������

	/*ϵͳ����*/
	bool    m_TimerFlag;	    //��ʱ����־
	bool    m_TimerFlag2;       //��ʱ��2��־
	CWinThread *m_IAAThread;
	CEvent m_EVKillIAA;			//ֹͣ���������߳�
	CEvent m_EVDeadIAA;			//���������߳̽���
	bool   m_ThreadFlag;		//�궨�߳����б��,TRUE��ʾ����,FALSE��ʾֹͣ

	//����궨����
	BOOL m_bDMeasureDraw;		//˫�����ʾ����������,true��ʾ��ʾ,false��ʾ����ʾ
	int m_n2ImageIndex;			//�ɼ�ͼ����
	int row_corner_num;			//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int column_corner_num;		//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int grid_width;				//ʵ�ʲ����õ��ı궨����ÿ�����̸�ĳ�
	int grid_height;			//ʵ�ʲ����õ��ı궨����ÿ�����̸�Ŀ�
	int PicNum;					//��Ҫ�궨ͼƬ������

	//˫Ŀƥ�����
	StereoMatch m_stereoMatcher;

	//����ʶ�����
	string Cascade_mode;
	CascadeClassifier Mycascade;
	string window_name;

	protected:	
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public://lq:��afx_msg��ͷ������Ϣ������
	afx_msg void OnBnClickedOpencamera();
	afx_msg void OnBnClickedCapvideo();
	afx_msg void OnBnClickedSetcamera();
	afx_msg void OnBnClickedCircledetect();
	void DrawLeftCamera();
	void DrawRightCamera();
	afx_msg void OnBnClickedOpencamera2();
	afx_msg void OnBnClickedCapvideo2();
	afx_msg void OnBnClickedSetcamera2();
	afx_msg void OnBnClickedStopdetect();
	afx_msg void OnBnClickedOpen2camera();
	afx_msg void OnBnClickedCap2video();
	void Blob_ReleaseLeftBlobSeq();
	void Blob_ReleaseRightBlobSeq();
	void Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
		double highthresh, double AspectRatio, double Circularity);
	void Detect_RightCircleDetect(InputArray SrcImg, double lowthresh, 
		double highthresh, double AspectRatio, double Circularity);
	void Blob_DenoisingLeftArea(int MinArea, int MaxArea);
	void Blob_DenoisingRightArea(int MinArea, int MaxArea);
	void ShowLeftCircles();
	void ShowRightCircles();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedCircledetect2();
	afx_msg void OnBnClickedStopdetect2();
	afx_msg void OnBnClickedCircle2detect();
	BOOL CalibFinishLeftCamera();
	BOOL CalibFinishRightCamera();

	public:
	CRichEditCtrl	m_static_FinishText;
	CListCtrl	m_list_Finish;
	void StopLeftCamera();
	void StopRightCamera();
	afx_msg void OnBnClickedCalibration();
	afx_msg void OnBnClickedSavepic();
	afx_msg void OnBnClickedSavepic2();
	afx_msg void OnBnClickedCalibration2();
	afx_msg void OnBnClickedCornerdetect();
	afx_msg void OnBnClickedCornerdetect2();
	afx_msg void OnBnClickedStereocalibration();
	afx_msg void OnBnClickedStereomatch();
	double m_ObjectDistance;
	double m_ObjectHeight;
	double m_ObjectWidth;
	afx_msg void OnBnClickedStereomeasure();
	afx_msg void OnBnClickedStopcamera();
	afx_msg void OnBnClickedStopcamera2();
	afx_msg void OnBnClickedStop2camera();
	afx_msg void OnBnClickedSave2pic();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnBnClickedTabbutton1();
	afx_msg void OnBnClickedTabbutton2();
	afx_msg void OnBnClickedTabbutton3();
	void ShowLeftPic(void);
	Mat FaceDetect(Mat frame);
	afx_msg void OnBnClickedFacedetect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ChangeSize(CWnd *pWnd, int cx, int cy);
//	afx_msg void OnBnClickedButton20();
//	afx_msg void OnBnClickedCircleStereocalibration();
};
