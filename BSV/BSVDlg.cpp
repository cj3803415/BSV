
// BSVDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "BSV.h"
#include "BSVDlg.h"
#include "afxdialogex.h"
#include "math.h"
#include "opencv2\core\core.hpp"
#include "opencv2\opencv.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\highgui\highgui.hpp"
//#include "Calibration.h"
#include "ConerDetect.h"
#include "Stereo.h"
#include "Cali_ParaDlg.h"
#include <iostream>
#include "global.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBSVDlg* g_pBSVDlg = NULL;
CBSVDlg* g_pBSVThread = NULL;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CBSVDlg �Ի���




CBSVDlg::CBSVDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBSVDlg::IDD, pParent),
	m_EVKillIAA(false, TRUE, NULL, NULL),
	m_EVDeadIAA(true, TRUE, NULL, NULL)
	, m_ObjectDistance(0)
	, m_ObjectHeight(0)
	, m_ObjectWidth(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	/*�Ӵ��ڳ�ʼ��*/
	//m_childdlg1 = NULL;
	//m_childdlg2 = NULL;
	//m_childdlg3 = NULL;

	/*��Ŀ·����ʼ��(���·��)*/
	ProjectPath.Format(_T("..\\BSV\\"));
	/*�����������ʼ��*/
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	g_pBSVDlg = this;
	 
	m_pLCamera = NULL;
	m_bLBufHandle = false;

	m_LeftOriDIBBits = NULL;
	m_LeftDIBBits = NULL;
	m_LeftShowDIBBits = NULL;

	m_LeftWidth = 0;      
	m_LeftHeight = 0;     
	m_LeftOriWidth = 1600;  
	m_LeftOriHeight = 1200;

	m_nLBit = 8;
	m_pLBmpInfo = NULL;//��ͼ����Ϣͷָ��
	m_pLBmpInfo = (LPBITMAPINFO) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//����ͼ����Ϣͷ
	m_pLBmpInfo->bmiHeader.biBitCount = 8;
	m_pLBmpInfo->bmiHeader.biClrImportant = 0;
	m_pLBmpInfo->bmiHeader.biClrUsed = 0;
	m_pLBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pLBmpInfo->bmiHeader.biPlanes = 1;
	m_pLBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pLBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pLBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pLBmpInfo->bmiHeader.biSizeImage = 0;
	m_pLBmpInfo->bmiHeader.biHeight = m_LeftOriHeight;
	m_pLBmpInfo->bmiHeader.biWidth = m_LeftOriWidth;
	for(int i= 0;i<256;i++)
	{
		m_pLBmpInfo->bmiColors[i].rgbBlue = i;
		m_pLBmpInfo->bmiColors[i].rgbGreen = i;
		m_pLBmpInfo->bmiColors[i].rgbRed = i;
		m_pLBmpInfo->bmiColors[i].rgbReserved = 0;
	}
	m_nLImageIndex = 0;
	/*�����������ʼ��*/
	m_pRCamera = NULL;
	m_bRBufHandle = false;

	m_RightOriDIBBits = NULL;
	m_RightDIBBits = NULL;
	m_RightShowDIBBits = NULL;

	m_RightWidth = 0;      
	m_RightHeight = 0;     
	m_RightOriWidth = 1600;  
	m_RightOriHeight = 1200;

	m_nRBit = 8;

	m_pRBmpInfo = (LPBITMAPINFO) new 
		char[sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256];		//����ͼ����Ϣͷ
	m_pRBmpInfo->bmiHeader.biBitCount = 8;
	m_pRBmpInfo->bmiHeader.biClrImportant = 0;
	m_pRBmpInfo->bmiHeader.biClrUsed = 0;
	m_pRBmpInfo->bmiHeader.biCompression = BI_RGB;
	m_pRBmpInfo->bmiHeader.biPlanes = 1;
	m_pRBmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_pRBmpInfo->bmiHeader.biXPelsPerMeter = 0;
	m_pRBmpInfo->bmiHeader.biYPelsPerMeter = 0;
	m_pRBmpInfo->bmiHeader.biSizeImage = 0;
	m_pRBmpInfo->bmiHeader.biHeight = m_RightOriHeight;
	m_pRBmpInfo->bmiHeader.biWidth = m_RightOriWidth;
	for(int i= 0;i<256;i++)
	{
		m_pRBmpInfo->bmiColors[i].rgbBlue = i;
		m_pRBmpInfo->bmiColors[i].rgbGreen = i;
		m_pRBmpInfo->bmiColors[i].rgbRed = i;
		m_pRBmpInfo->bmiColors[i].rgbReserved = 0;
	}
	m_nRImageIndex = 0;

	/*�����������ʼ��*/
	string Cascade_mode ="..\\BSV\\cascade.xml"; 
	string window_name = "Face"; 
}

CBSVDlg::~CBSVDlg()
{
	OnClose();
}

void CBSVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ObjectDistance, m_ObjectDistance);
	DDX_Text(pDX, IDC_ObjectHeight, m_ObjectHeight);
	DDX_Text(pDX, IDC_ObjectWidth, m_ObjectWidth);
}

BEGIN_MESSAGE_MAP(CBSVDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OpenCamera, &CBSVDlg::OnBnClickedOpencamera)
	ON_BN_CLICKED(IDC_CapVideo, &CBSVDlg::OnBnClickedCapvideo)
	ON_BN_CLICKED(IDC_SetCamera, &CBSVDlg::OnBnClickedSetcamera)
	ON_BN_CLICKED(IDC_CircleDetect, &CBSVDlg::OnBnClickedCircledetect)
	ON_BN_CLICKED(IDC_OpenCamera2, &CBSVDlg::OnBnClickedOpencamera2)
	ON_BN_CLICKED(IDC_CapVideo2, &CBSVDlg::OnBnClickedCapvideo2)
	ON_BN_CLICKED(IDC_SetCamera2, &CBSVDlg::OnBnClickedSetcamera2)
	ON_BN_CLICKED(IDC_StopDetect, &CBSVDlg::OnBnClickedStopdetect)
	ON_BN_CLICKED(IDC_Open2Camera, &CBSVDlg::OnBnClickedOpen2camera)
	ON_BN_CLICKED(IDC_Cap2Video, &CBSVDlg::OnBnClickedCap2video)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CircleDetect2, &CBSVDlg::OnBnClickedCircledetect2)
	ON_BN_CLICKED(IDC_StopDetect2, &CBSVDlg::OnBnClickedStopdetect2)
	ON_BN_CLICKED(IDC_Circle2Detect, &CBSVDlg::OnBnClickedCircle2detect)
	ON_BN_CLICKED(IDC_Calibration, &CBSVDlg::OnBnClickedCalibration)
	ON_BN_CLICKED(IDC_SavePic, &CBSVDlg::OnBnClickedSavepic)
	ON_BN_CLICKED(IDC_SavePic2, &CBSVDlg::OnBnClickedSavepic2)
	ON_BN_CLICKED(IDC_Calibration2, &CBSVDlg::OnBnClickedCalibration2)
	ON_BN_CLICKED(IDC_CornerDetect, &CBSVDlg::OnBnClickedCornerdetect)
	ON_BN_CLICKED(IDC_CornerDetect2, &CBSVDlg::OnBnClickedCornerdetect2)
	ON_BN_CLICKED(IDC_StereoCalibration, &CBSVDlg::OnBnClickedStereocalibration)
	ON_BN_CLICKED(IDC_StereoMatch, &CBSVDlg::OnBnClickedStereomatch)
	ON_BN_CLICKED(IDC_StereoMeasure, &CBSVDlg::OnBnClickedStereomeasure)
	ON_BN_CLICKED(IDC_StopCamera, &CBSVDlg::OnBnClickedStopcamera)
	ON_BN_CLICKED(IDC_StopCamera2, &CBSVDlg::OnBnClickedStopcamera2)
	ON_BN_CLICKED(IDC_Stop2Camera, &CBSVDlg::OnBnClickedStop2camera)
	ON_BN_CLICKED(IDC_Save2Pic, &CBSVDlg::OnBnClickedSave2pic)
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_TABBUTTON1, &CBSVDlg::OnBnClickedTabbutton1)
	ON_BN_CLICKED(IDC_TABBUTTON2, &CBSVDlg::OnBnClickedTabbutton2)
	ON_BN_CLICKED(IDC_TABBUTTON3, &CBSVDlg::OnBnClickedTabbutton3)
	ON_BN_CLICKED(IDC_FaceDetect, &CBSVDlg::OnBnClickedFacedetect)
	ON_WM_SIZE()
//	ON_BN_CLICKED(IDC_BUTTON20, &CBSVDlg::OnBnClickedButton20)
END_MESSAGE_MAP()


// CBSVDlg ��Ϣ�������

BOOL CBSVDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	CDialogEx::SetBackgroundColor(RGB(230,230,230)); 
	//�򿪿���̨
	AllocConsole();
	freopen("CONOUT$", "a+", stdout);

	//�����Ӵ���
	//m_childdlg1 = new CChildDlg1;
	//m_childdlg1->Create(IDD_DIALOG1, GetDlgItem(IDC_MENUFORM));

	//m_childdlg2 = new CChildDlg2;
	//m_childdlg2->Create(IDD_DIALOG2, GetDlgItem(IDC_MENUFORM));

	//m_childdlg3 = new CChildDlg3;
	//m_childdlg3->Create(IDD_DIALOG3, GetDlgItem(IDC_MENUFORM));
	m_childdlg1.Create(IDD_DIALOG1, GetDlgItem(IDC_MENUFORM));
	m_childdlg2.Create(IDD_DIALOG2, GetDlgItem(IDC_MENUFORM));
	m_childdlg3.Create(IDD_DIALOG3, GetDlgItem(IDC_MENUFORM));
	m_Childdlg1.Create(IDD_ChildDlg1, GetDlgItem(IDC_MENUFORM));
	//CChildDialog1.Create(IDD_ChildDlg1, GetDlgItem(IDC_MENUFORM));
	//CRect rect;
	//GetDlgItem(IDC_MENUFORM)->GetWindowRect(&rect);
	//m_Childdlg1.MoveWindow(rect);


	//�̶�Picture Control�ؼ��Ĵ�С
	CRect rectL;  
	GetDlgItem(IDC_LeftPic)->GetWindowRect(&rectL);
	ScreenToClient(&rectL);  
	GetDlgItem(IDC_LeftPic)->MoveWindow(rectL.left, rectL.top, 520, 390, true);

	CRect rectR;  
	GetDlgItem(IDC_RightPic)->GetWindowRect(&rectR);
	ScreenToClient(&rectR);  
	GetDlgItem(IDC_RightPic)->MoveWindow(rectR.left, rectR.top, 520, 390, true);

	//��ȡ�Ի��򴴽�ʱ�Ĵ�С�����ڴ���Ŵ�߶Ȳ��䣩
	GetClientRect(&m_rect);

	//���ð�ť�ؼ���ʼ�ɲ���״̬
	GetDlgItem(IDC_CapVideo)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect)->EnableWindow(false);
	GetDlgItem(IDC_StopCamera)->EnableWindow(false);

	GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
	GetDlgItem(IDC_SetCamera2)->EnableWindow(false);
	GetDlgItem(IDC_StopDetect2)->EnableWindow(false);
	GetDlgItem(IDC_StopCamera2)->EnableWindow(false);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CBSVDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�ͷ��ڴ���Դ
	if (m_pLCamera != NULL)
	{
		MVStopGrab(m_pLCamera);
		MVCloseCam(m_pLCamera);
		m_pLCamera = NULL;
	}

	delete[] m_RightShowDIBBits;
	m_LeftShowDIBBits = NULL;	
	delete[] m_LeftDIBBits;
	m_LeftDIBBits = NULL;
	delete[] m_LeftOriDIBBits;
	m_LeftOriDIBBits = NULL;

	//�����ʱ��
	if (m_TimerFlag)
	{
		KillTimer(1);
		m_TimerFlag = NULL;
	}

	if (m_TimerFlag2)
	{
		KillTimer(2);
		m_TimerFlag2 = NULL;
	}

	if (m_pLBmpInfo  != NULL)
	{
		delete[] m_pLBmpInfo;
		m_pLBmpInfo = NULL;
	}

	Blob_ReleaseLeftBlobSeq();
	Blob_ReleaseRightBlobSeq();

	CDialogEx::OnClose();
}

void CBSVDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CBSVDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CBSVDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*
 *����˵����������ص���������������������봥��ģʽ��ʱ��ÿ���һ֡ͼ��ͻ���øú���˵����
 *�����������
 *		
 *���������
 LPBYTE pbuf       ͼ��������
 int nW            ͼ��Ŀ�
 int nH            ͼ��ĸ�
 int nBitCount     ͼ�������λ��
 int nCameraIndex  �����ţ���1��ʼ������
 FILE *pFile       ��������Ϣ���ļ�ָ��
 *����ֵ���ޡ�
*/
int LeftCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_LeftOriDIBBits != NULL)
	{
		//����ͼ���ڴ�
		g_pBSVDlg->m_CSectionCopyL.Lock();

		//Ϊԭʼͼ��ֵ�������24λRGB��ʽ����ֱ��ת��
		if( g_pBSVDlg->m_LPixelFormat == PixelFormat_Mono8 )
		{
			memcpy(g_pBSVDlg->m_LeftOriDIBBits,pInfo->pImageBuffer,
				g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_LeftOriHeight);
		}
		else
		{
			int count = (((g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_nLBit)+31)/32)*4;
			MVBayerToBGR(g_pBSVDlg->m_pLCamera,pInfo->pImageBuffer,g_pBSVDlg->m_LeftOriDIBBits,count,
				g_pBSVDlg->m_LeftOriWidth,g_pBSVDlg->m_LeftOriHeight,g_pBSVDlg->m_LPixelFormat);
		}

		LPSTR itemBits;
		itemBits = new char[g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_LeftOriHeight*(g_pBSVDlg->m_nLBit/8)];
		memcpy(itemBits,g_pBSVDlg->m_LeftOriDIBBits,g_pBSVDlg->m_LeftOriWidth*g_pBSVDlg->m_LeftOriHeight*(g_pBSVDlg->m_nLBit/8)); 
		for (int i=0;i<g_pBSVDlg->m_LeftOriHeight;i++)
		{
			for (int j=0;j<g_pBSVDlg->m_LeftOriWidth;j++)
			{
				if (g_pBSVDlg->m_nLBit == 8)
				{
					g_pBSVDlg->m_LeftShowDIBBits[i*g_pBSVDlg->m_LeftOriWidth+j] = 
						itemBits[(g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j];
				}
				else if (g_pBSVDlg->m_nLBit == 24)
				{
					g_pBSVDlg->m_LeftShowDIBBits[(i*g_pBSVDlg->m_LeftOriWidth+j)*3] = 
						itemBits[((g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j)*3];
					g_pBSVDlg->m_LeftShowDIBBits[(i*g_pBSVDlg->m_LeftOriWidth+j)*3+1] = 
						itemBits[((g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j)*3+1];
					g_pBSVDlg->m_LeftShowDIBBits[(i*g_pBSVDlg->m_LeftOriWidth+j)*3+2] = 
						itemBits[((g_pBSVDlg->m_LeftOriHeight-i-1)*g_pBSVDlg->m_LeftOriWidth+j)*3+2];
				}
			}
		}
		delete []itemBits;
		g_pBSVDlg->m_CSectionCopyL.Unlock();

		g_pBSVDlg->m_bLBufHandle = true;//��ͼ����Խ��д����ʶ
		g_pBSVDlg->DrawLeftCamera();
	}
	return 1;
}

/*
 *����˵����������ص�������
 */
int RightCallbackFunction(MV_IMAGE_INFO *pInfo,long nUserVal)
	// int index, LPSTR pData, int Length, void* pUserData )
{
	if(g_pBSVDlg->m_RightOriDIBBits != NULL)//������
	{
		//����ͼ���ڴ�
		g_pBSVDlg->m_CSectionCopyR.Lock();

		//Ϊԭʼͼ��ֵ�������24λRGB��ʽ����ֱ��ת��
		if( g_pBSVDlg->m_RPixelFormat == PixelFormat_Mono8 )
		{
			memcpy(g_pBSVDlg->m_RightOriDIBBits,pInfo->pImageBuffer,
				g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_RightOriHeight);
		}
		else
		{
			int count = (((g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_nRBit)+31)/32)*4;
			MVBayerToBGR(g_pBSVDlg->m_pRCamera,pInfo->pImageBuffer,g_pBSVDlg->m_RightOriDIBBits,count,
				g_pBSVDlg->m_RightOriWidth,g_pBSVDlg->m_RightOriHeight,g_pBSVDlg->m_RPixelFormat);
		}

		LPSTR itemBits;
		itemBits = new char[g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_RightOriHeight*(g_pBSVDlg->m_nRBit/8)];
		memcpy(itemBits,g_pBSVDlg->m_RightOriDIBBits,g_pBSVDlg->m_RightOriWidth*g_pBSVDlg->m_RightOriHeight*(g_pBSVDlg->m_nRBit/8)); 
		for (int i=0;i<g_pBSVDlg->m_RightOriHeight;i++)
		{
			for (int j=0;j<g_pBSVDlg->m_RightOriWidth;j++)
			{
				if (g_pBSVDlg->m_nRBit == 8)
				{
					g_pBSVDlg->m_RightShowDIBBits[i*g_pBSVDlg->m_RightOriWidth+j] = 
						itemBits[(g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j];
				}
				else if (g_pBSVDlg->m_nRBit == 24)
				{
					g_pBSVDlg->m_RightShowDIBBits[(i*g_pBSVDlg->m_RightOriWidth+j)*3] = 
						itemBits[((g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j)*3];
					g_pBSVDlg->m_RightShowDIBBits[(i*g_pBSVDlg->m_RightOriWidth+j)*3+1] = 
						itemBits[((g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j)*3+1];
					g_pBSVDlg->m_RightShowDIBBits[(i*g_pBSVDlg->m_RightOriWidth+j)*3+2] = 
						itemBits[((g_pBSVDlg->m_RightOriHeight-i-1)*g_pBSVDlg->m_RightOriWidth+j)*3+2];
				}
			}
		}
		delete []itemBits;
		g_pBSVDlg->m_CSectionCopyR.Unlock();

		g_pBSVDlg->m_bRBufHandle = true;//��ͼ����Խ��д����ʶ
		g_pBSVDlg->DrawRightCamera();
	}
	return 1;
}

/*
 *����˵������ʾ�����ͼ��
 *�����������
 *          
 *����������ޡ�
 *�� �� ֵ���ޡ�
*/
void CBSVDlg::DrawLeftCamera()
{
	m_CSectionL.Lock();
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();//���DC
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;//����ģʽ

	//��ʾͼ��
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_LeftOriWidth,m_LeftOriHeight,
		m_LeftShowDIBBits,(LPBITMAPINFO) m_pLBmpInfo, DIB_RGB_COLORS, SRCCOPY); 

	GetDlgItem(IDC_LeftPic)->ReleaseDC(pdc);	
	m_CSectionL.Unlock();
}

/*
 *����˵������ʾ�����ͼ��
 *�����������
 *          
 *����������ޡ�
 *�� �� ֵ���ޡ�
 */
void CBSVDlg::DrawRightCamera()
{
	m_CSectionR.Lock();
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();//���DC
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;//����ģʽ

	//��ʾͼ��
	::StretchDIBits(pdc->GetSafeHdc(), 0, 0, rc.Width(),rc.Height(),
		0,0,m_RightOriWidth,m_RightOriHeight,
		m_RightShowDIBBits,(LPBITMAPINFO) m_pRBmpInfo, DIB_RGB_COLORS, SRCCOPY); 

	GetDlgItem(IDC_RightPic)->ReleaseDC(pdc);	
	m_CSectionR.Unlock();
}

/*
 *�������ܣ���Ӧ��ʱ����Ϣ������
 *����˵����
 nIDEvent ��ʱ����ʶ������һ�������ڿ���ʹ�ö����ʱ��
          ����ͬ�Ķ�ʱ������nIDEvent�����֡�
*/
void CBSVDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)//nIDEvent Ϊ��ʱ���¼�ID��1��2��3
	{
	case 1:	//�����Բ�μ��
		{
			if( m_LPixelFormat != PixelFormat_Mono8 )
				return;

			Mat LSrcImg;
			LSrcImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);

			g_pBSVDlg->m_CSectionCopyL.Lock();
			memcpy(LSrcImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
			g_pBSVDlg->m_CSectionCopyL.Unlock();
	
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
	
			//Blob_ReleaseLeftBlobSeq();
			//Detect_LeftCircleDetect(LBinImg, 0, 255, 0.6, 0.6);
			//Blob_DenoisingLeftArea(1000, 8000);
			
			if (!m_LeftBlobSeq.IsEmpty())
			{
				ShowLeftCircles();
			}
			LSrcImg.release();
			LFilImg.release();
		}
		break;
	case 2:	//�����Բ�μ��
		{
			if( m_RPixelFormat != PixelFormat_Mono8 )
				return;

			Mat RSrcImg;
			RSrcImg.create(m_RightOriHeight, m_RightOriWidth, CV_8UC1);
			g_pBSVDlg->m_CSectionCopyR.Lock();
			memcpy(RSrcImg.data,m_RightShowDIBBits,m_RightOriWidth*m_RightOriHeight*(m_nRBit/8));
			g_pBSVDlg->m_CSectionCopyR.Unlock();
	
			Mat RFilImg;
			//˫���˲�������ȥ��
			//bilateralFilter(RSrcImg, RFilImg, 25, 25*2, 25/2);
			//��˹�˲�
			GaussianBlur(RSrcImg, RFilImg, Size(7,7), 0, 0);

			Mat RBinImg;
			//����Ӧ��ֵ�ָ�ͼ��	
			//adaptiveThreshold(RFilImg, RBinImg, 255, CV_ADAPTIVE_THRESH_MEAN_C, 
			//CV_THRESH_BINARY, 7, 5);	
			//�̶���ֵ�ָ�ͼ�� lq:��ֵ��ͼ��
			threshold(RFilImg, RBinImg, 100, 255, CV_THRESH_BINARY);
			
			Blob_ReleaseRightBlobSeq();
			Detect_RightCircleDetect(RBinImg, 0, 255, 0.6, 0.6);
			Blob_DenoisingRightArea(1000, 8000);

			if (!m_RightBlobSeq.IsEmpty())
			{
				ShowRightCircles();
			}
			RSrcImg.release();
			RFilImg.release();
			RBinImg.release();
		}
		break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

/*
 *�������ܣ��ͷ�BlobSeq�е�Blob�����ڴ棬���BlobSeq�е�����
 *����˵��
 CArray<Blob> m_LeftBlobSeq	���ͷ��ڴ��������ſ���С�
 */
/*�����*/
void CBSVDlg::Blob_ReleaseLeftBlobSeq()
{
	if (!m_LeftBlobSeq.IsEmpty())
	{
		m_LeftBlobSeq.RemoveAll();
		m_LeftBlobSeq.FreeExtra();
	}
}

/*
 *�������ܣ��ͷ�BlobSeq�е�Blob�����ڴ棬���BlobSeq�е�����
 *����˵��
 CArray<Blob> m_RightBlobSeq	���ͷ��ڴ��������ſ���С�
 */
/*�����*/
void CBSVDlg::Blob_ReleaseRightBlobSeq()
{
	if (!m_RightBlobSeq.IsEmpty())
	{
		m_RightBlobSeq.RemoveAll();
		m_RightBlobSeq.FreeExtra();
	}
}

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
void CBSVDlg::Detect_LeftCircleDetect(InputArray SrcImg, double lowthresh, 
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
	m_LeftBlobSeq.SetSize(ln,10);
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

					m_LeftBlobSeq.SetAtGrow(cnt,pB);
					cnt++;

					//��ȡԲ��ֵ
					LBlob.BlobX = pB.BlobX;
					LBlob.BlobY = pB.BlobY;
				}
			}
		}
	}
	m_LeftBlobSeq.FreeExtra();
}

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
void CBSVDlg::Detect_RightCircleDetect(InputArray SrcImg, double lowthresh, 
	double highthresh, double AspectRatio, double Circularity)
{
	Mat EdgeImg;
	Canny(SrcImg, EdgeImg, lowthresh, highthresh);
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(EdgeImg, contours, hierarchy, CV_RETR_LIST,
		CV_CHAIN_APPROX_NONE, cvPoint(0,0));
	EdgeImg.release();

	double area, length, cir, width, height, asp;
	RotatedRect rect;
	int cnt = 0;
	int ln = contours.size();
	m_RightBlobSeq.SetSize(ln,10);
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

					m_RightBlobSeq.SetAtGrow(cnt,pB);
					cnt++;
				}
			}
		}
	}
	m_RightBlobSeq.FreeExtra();
}

/*
 *�������ܣ�����ſ�������������ָ����Χ�ڵ��ſ�����
 *����˵��
 CArray<Blob> m_LeftBlobSeq     ��������ſ����
 int MinArea,MaxArea    ָ�����ſ������Χ
*/
/*�����*/
void CBSVDlg::Blob_DenoisingLeftArea(int MinArea, int MaxArea)
{
	if (!m_LeftBlobSeq.IsEmpty())
	{
		int ln = m_LeftBlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = m_LeftBlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				m_LeftBlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		m_LeftBlobSeq.FreeExtra();
	}
}

/*
 *�������ܣ�����ſ�������������ָ����Χ�ڵ��ſ�����
 *����˵��
 CArray<Blob> m_RightBlobSeq     ��������ſ����
 int MinArea,MaxArea    ָ�����ſ������Χ
*/
/*�����*/
void CBSVDlg::Blob_DenoisingRightArea(int MinArea, int MaxArea)
{
	if (!m_RightBlobSeq.IsEmpty())
	{
		int ln = m_RightBlobSeq.GetSize();;
		for (int i=0; i<ln; i++)
		{
			Blob pB = m_RightBlobSeq.GetAt(i);
			if (pB.Area<MinArea || pB.Area>MaxArea)
			{
				m_RightBlobSeq.RemoveAt(i);
				i--;
				ln--;
			}
		}
		m_RightBlobSeq.FreeExtra();
	}
}

/*
 *�������ܣ���ͼ���ϻ�������Բ�α�־
 *����˵��
*/
/*�����*/
void CBSVDlg::ShowLeftCircles()
{
	CDC *pdc = GetDlgItem(IDC_LeftPic)->GetDC();		//��ãģ�
	CRect rc;
	GetDlgItem(IDC_LeftPic)->GetClientRect(&rc);		//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//����ģʽ

	// ������ɫ���ʶ���
	CBrush BrushRed(RGB(255, 0, 0));
	CBrush *pOldBrush;
	pOldBrush = pdc->SelectObject(&BrushRed);

	double dbRateX = (double)rc.Width()/(double)m_LeftOriWidth;
	double dbRateY = (double)rc.Height()/(double)m_LeftOriHeight;
	int ln = m_LeftBlobSeq.GetSize();
	for (int i=0;i<ln;i++)
	{
		Blob pB = m_LeftBlobSeq.GetAt(i);
		int x, y;
		x = (int)(pB.BlobX*dbRateX);
		y = (int)((m_LeftOriHeight - pB.BlobY)*dbRateY);
		if(x<6)
		{
			x = 6;
		}
		else if(x>rc.Width()-6)
		{
			x = rc.Width()-6;
		}
		if(y<6)
		{
			y = 6;
		}
		else if(y>rc.Height() - 6)
		{
			y = rc.Height()-6;
		}
		pdc->Ellipse(x-5, y-5, x+5, y+5);//���Բ��	
	}

	pdc->SelectObject(pOldBrush);
	BrushRed.DeleteObject();
	ReleaseDC(pdc);
}

/*
 *�������ܣ���ͼ���ϻ�������Բ�α�־
 *����˵��
*/
/*�����*/
void CBSVDlg::ShowRightCircles()
{
	CDC *pdc = GetDlgItem(IDC_RightPic)->GetDC();		//��ãģ�
	CRect rc;
	GetDlgItem(IDC_RightPic)->GetClientRect(&rc);		//���ͼ����ʾ����
	SetStretchBltMode(pdc->GetSafeHdc(), COLORONCOLOR ) ;	//����ģʽ

	// ������ɫ���ʶ���
	CBrush BrushRed(RGB(255, 0, 0));
	CBrush *pOldBrush;
	pOldBrush = pdc->SelectObject(&BrushRed);

	double dbRateX = (double)rc.Width()/(double)m_RightOriWidth;
	double dbRateY = (double)rc.Height()/(double)m_RightOriHeight;
	int ln = m_RightBlobSeq.GetSize();
	for (int i=0;i<ln;i++)
	{
		Blob pB = m_RightBlobSeq.GetAt(i);
		int x, y;
		x = (int)(pB.BlobX*dbRateX);
		y = (int)((m_RightOriHeight - pB.BlobY)*dbRateY);
		if(x<6)
		{
			x = 6;
		}
		else if(x>rc.Width()-6)
		{
			x = rc.Width()-6;
		}
		if(y<6)
		{
			y = 6;
		}
		else if(y>rc.Height() - 6)
		{
			y = rc.Height()-6;
		}
		pdc->Ellipse(x-5, y-5, x+5, y+5);			
	}

	pdc->SelectObject(pOldBrush);
	BrushRed.DeleteObject();
	ReleaseDC(pdc);
}

/*
 *����˵�������½�Ϊ�ؼ���Ϣ��Ӧ����
 *�����������       
 *����������ޡ�
 *�� �� ֵ���ޡ�
*/
void CBSVDlg::OnBnClickedOpencamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������	
	MVSTATUS_CODES hr;
	if (m_pLCamera == NULL)	//�������
	{
		hr = MVOpenCamByIndex(0, &m_pLCamera );		
		if (hr != MVST_SUCCESS)
		{
			MessageBox("δ�ҵ��������ȷ���豸���Ӻ�IP���ã�","BSV",MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(IDC_OpenCamera)->SetWindowText("�ر����");
			GetDlgItem(IDC_CapVideo)->EnableWindow(true);
			GetDlgItem(IDC_SetCamera)->EnableWindow(true);
			GetDlgItem(IDC_StopCamera)->EnableWindow(false);
		}
	}
	else	//�ر����
	{
		hr = MVStopGrab(m_pLCamera);		 
		hr = MVCloseCam(m_pLCamera);	
		m_pLCamera = NULL;

		GetDlgItem(IDC_OpenCamera)->SetWindowText("�������");
		GetDlgItem(IDC_CapVideo)->EnableWindow(false);
		GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	}
}


void CBSVDlg::OnBnClickedCapvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pLCamera != NULL)
	{
		MVGetWidth(m_pLCamera, &m_LeftOriWidth);  //���ͼ�񳤿�
		MVGetHeight(m_pLCamera,&m_LeftOriHeight);
		MVGetPixelFormat(m_pLCamera,&m_LPixelFormat);  //������ݸ�ʽ
		if( m_LPixelFormat == PixelFormat_Mono8 )
		{
			m_nLBit = 8;
		}
		else
		{
			m_nLBit = 24;
		}
		
		MVSTATUS_CODES hr;
		hr =  MVStartGrab(m_pLCamera, LeftCallbackFunction, (long)this); //���ûص�����
		if (hr == MVST_SUCCESS)
		{	
			//ԭʼͼ��
			m_pLBmpInfo->bmiHeader.biBitCount = m_nLBit;
			m_pLBmpInfo->bmiHeader.biWidth = m_LeftOriWidth;
			m_pLBmpInfo->bmiHeader.biHeight = m_LeftOriHeight;

			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
			delete[] m_LeftShowDIBBits;	
			m_LeftShowDIBBits = NULL;	
			m_LeftShowDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			delete[] m_LeftOriDIBBits;	
			m_LeftOriDIBBits = NULL;						
			m_LeftOriDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			//Ϊ���д����ͼ��ֵ
			m_LeftRectLeft=0;  
			m_LeftRectBottom=0;
			m_LeftWidth=m_LeftOriWidth; 
			m_LeftHeight=m_LeftOriHeight;

			delete[] m_LeftDIBBits;	
			m_LeftDIBBits = NULL;					
			m_LeftDIBBits = new char[m_LeftWidth * m_LeftHeight * (m_nLBit/8)];

			//��ʼ�ɼ�ͼ��󣬲��ٽ��ܸ��µġ������ɼ���ָ��
			GetDlgItem(IDC_CapVideo)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}


void CBSVDlg::OnBnClickedSetcamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pLCamera == NULL )
	{
		MessageBox("��δ����������޷����ã�","BSV",MB_ICONWARNING);
	}
	else
	{
		//SetCameraDlg Dlg;	//�½�������öԻ���
		//if (Dlg.DoModal())
		//{
		//}
		MessageBox("���޴˹���");
	}
}


void CBSVDlg::OnBnClickedCircledetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_LeftShowDIBBits == NULL)
	{
		MessageBox("��ͼ�����ݣ�","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(1, 300, NULL);//���ö�ʱ��1
		m_TimerFlag = true;

		GetDlgItem(IDC_CircleDetect)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect)->EnableWindow(true);
	}
}


void CBSVDlg::OnBnClickedStopdetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_TimerFlag)
	{
		KillTimer(1);
		m_TimerFlag = false;
	}

	GetDlgItem(IDC_CircleDetect)->EnableWindow(true);
	GetDlgItem(IDC_StopDetect)->EnableWindow(false);
}


void CBSVDlg::OnBnClickedOpencamera2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//���������	
	MVSTATUS_CODES hr;
	if (m_pRCamera == NULL)	//�������
	{
		hr = MVOpenCamByIndex(1, &m_pRCamera );		
		if (hr != MVST_SUCCESS)
		{
			MessageBox("δ�ҵ��������ȷ���豸���Ӻ�IP���ã�","BSV",MB_ICONWARNING);
			return;
		}
		else if (hr == MVST_SUCCESS)
		{
			GetDlgItem(IDC_OpenCamera2)->SetWindowText("�ر����");
			GetDlgItem(IDC_CapVideo2)->EnableWindow(true);
			GetDlgItem(IDC_SetCamera2)->EnableWindow(true);
			GetDlgItem(IDC_StopCamera2)->EnableWindow(false);
		}
	}
	else	//�ر����
	{
		hr = MVStopGrab(m_pRCamera);		 
		hr = MVCloseCam(m_pRCamera);	
		m_pRCamera = NULL;

		GetDlgItem(IDC_OpenCamera2)->SetWindowText("�������");
		GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
		GetDlgItem(IDC_SetCamera2)->EnableWindow(false);
	}
}


void CBSVDlg::OnBnClickedCapvideo2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_pRCamera != NULL)
	{
		MVGetWidth(m_pRCamera, &m_RightOriWidth);  //���ͼ�񳤿�
		MVGetHeight(m_pRCamera,&m_RightOriHeight);
		MVGetPixelFormat(m_pRCamera,&m_RPixelFormat);  //������ݸ�ʽ
		if( m_RPixelFormat == PixelFormat_Mono8 )
		{
			m_nRBit = 8;
		}
		else
		{
			m_nRBit = 24;
		}
		
		MVSTATUS_CODES hr2;
		hr2 =  MVStartGrab(m_pRCamera, RightCallbackFunction, (long)this); //���ûص�����
		if (hr2 == MVST_SUCCESS)
		{	
			//ԭʼͼ��
			m_pRBmpInfo->bmiHeader.biBitCount = m_nRBit;
			m_pRBmpInfo->bmiHeader.biWidth = m_RightOriWidth;
			m_pRBmpInfo->bmiHeader.biHeight = m_RightOriHeight;

			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
			delete[] m_RightShowDIBBits;	
			m_RightShowDIBBits = NULL;	
			m_RightShowDIBBits = new char[m_RightOriWidth * m_RightOriHeight * (m_nRBit/8)];

			delete[] m_RightOriDIBBits;	
			m_RightOriDIBBits = NULL;						
			m_RightOriDIBBits = new char[m_RightOriWidth * m_RightOriHeight * (m_nRBit/8)];

			//Ϊ���д����ͼ��ֵ
			m_RightRectLeft=0;
			m_RightRectBottom=0;
			m_RightWidth=m_RightOriWidth; 
			m_RightHeight=m_RightOriHeight;

			delete[] m_RightDIBBits;	
			m_RightDIBBits = NULL;					
			m_RightDIBBits = new char[m_RightWidth * m_RightHeight * (m_nRBit/8)];

			//��ʼ�ɼ�ͼ��󣬲��ٽ��ܸ��µġ������ɼ���ָ��
			GetDlgItem(IDC_CapVideo2)->EnableWindow(false);
			//GetDlgItem(IDC_Btn_SetCamera)->EnableWindow(false);
		}
	}
}


void CBSVDlg::OnBnClickedSetcamera2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pRCamera == NULL )
	{
		MessageBox("��δ����������޷����ã�","BSV",MB_ICONWARNING);
	}
	else
	{
		//SetCameraDlg Dlg;	//�½�������öԻ���
		//if (Dlg.DoModal())
		//{
		//}
		MessageBox("���޴˹���");
	}
}


void CBSVDlg::OnBnClickedOpen2camera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedOpencamera();
	OnBnClickedOpencamera2();
}


void CBSVDlg::OnBnClickedCap2video()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//OnBnClickedCapvideo();
	OnBnClickedCapvideo2();
}


void CBSVDlg::OnBnClickedCircledetect2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_RightShowDIBBits == NULL)
	{
		MessageBox("��ͼ�����ݣ�","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		SetTimer(2, 300, NULL);//lq:���ö�ʱ��2
		m_TimerFlag2 = true;

		GetDlgItem(IDC_CircleDetect2)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect2)->EnableWindow(true);
	}
}


void CBSVDlg::OnBnClickedStopdetect2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_TimerFlag2)
	{
		KillTimer(2);
		m_TimerFlag2 = false;
	}

	GetDlgItem(IDC_CircleDetect2)->EnableWindow(true);
	GetDlgItem(IDC_StopDetect2)->EnableWindow(false);
}


void CBSVDlg::OnBnClickedCircle2detect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedCircledetect();
	OnBnClickedCircledetect2();
}


/*��δ���ò�ƿ���ȥ��*/
CString format_fraction(double data)
{
	CString result;     //��ʽ��֮���������Чλ��Ϊ6
	
	//�ݴ��������ݲ���Ϊ����
	int tmp;
	tmp = abs((int)data);

	if (tmp/10000>=1)
	{
		result.Format("%.0f", data);
		return result;
	}
	if (tmp/1000>=1)
	{
		result.Format("%.1f", data);
		return result;
	}
	if (tmp/100>=1)
	{
		result.Format("%.2f", data);
		return result;
	}
	if (tmp/10>=1)
	{
		result.Format("%.3f", data);
		return result;
	}
    else
	{
		result.Format("%.4f", data);
		return result;
	}
}


void CBSVDlg::OnBnClickedCalibration()
{
	 //TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString a, b ;
	//a.Format(_T("%s\\LCamera_calibdata.txt"),ProjectPath);
	//b.Format(_T("%s\\LCamera_caliberation_result.txt"),ProjectPath);
	//string A((LPCTSTR)a);
	//string B((LPCTSTR)b);

	//row_corner_num = 0;
	//column_corner_num = 0;
	//grid_width = 0;
	//grid_height = 0;
	//PicNum==0;
	//CCali_ParaDlg dlg;
	//dlg.DoModal();
	//if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0|PicNum==0)
	//{
	//	AfxMessageBox(_T("�뽫������д������"));
	//}
	//else
	//{
	//	WriteTxt(A,PicNum);
	//	m_LImgPath="Ori_LImage\\chess";
	//	Calibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,m_LImgPath,1);
	//}
}


void CBSVDlg::OnBnClickedCalibration2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString a,b;
	//a.Format(_T("%s\\RCamera_calibdata.txt"),ProjectPath);
	//b.Format(_T("%s\\RCamera_caliberation_result.txt"),ProjectPath);
	//string A((LPCTSTR)a);
	//string B((LPCTSTR)b);

	//row_corner_num = 0;
	//column_corner_num = 0;
	//grid_width = 0;
	//grid_height = 0;
	//PicNum==0;
	//CCali_ParaDlg dlg;
	//dlg.DoModal();
	//if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0|PicNum==0)
	//{
	//	AfxMessageBox(_T("�뽫������д������"));
	//}
	//else
	//{
	//	WriteTxt2(A,PicNum);
	//	m_RImgPath="Ori_RImage\\chess";
	//	Calibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,m_RImgPath,2);
	//}
}


void CBSVDlg::OnBnClickedSavepic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ͼ����Ori_LImage�У���ΪͼƬ����Lchess*.bmp
	if(m_pLCamera == NULL)
	{
		AfxMessageBox("û�д������");
		return;
	}
	m_nLImageIndex ++;

	if( m_LPixelFormat != PixelFormat_Mono8 )
		return;
	Mat LImg;
	LImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);

	g_pBSVDlg->m_CSectionCopyL.Lock();
	memcpy(LImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
	g_pBSVDlg->m_CSectionCopyL.Unlock();

	std::stringstream StrStm;
	string imageFileName;
	StrStm.clear();
 	imageFileName.clear();
	string LfilePath=ProjectPath+"Ori_LImage\\chess";
	StrStm<<m_nLImageIndex;
 	StrStm>>imageFileName;
	LfilePath += imageFileName;
	LfilePath+= ".bmp";
	imwrite(LfilePath,LImg);
}


void CBSVDlg::OnBnClickedSavepic2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_pRCamera == NULL)
	{
		AfxMessageBox("û�д������");
		return;
	}
	m_nRImageIndex ++;

	if( m_RPixelFormat != PixelFormat_Mono8 )
				return;
	Mat RImg;
	RImg.create(m_RightOriHeight, m_RightOriWidth, CV_8UC1);

	g_pBSVDlg->m_CSectionCopyR.Lock();
	memcpy(RImg.data,m_RightShowDIBBits,m_RightOriWidth*m_RightOriHeight*(m_nRBit/8));
	g_pBSVDlg->m_CSectionCopyR.Unlock();

	std::stringstream StrStm;
	string imageFileName;
	StrStm.clear();
 	imageFileName.clear();
	string RfilePath=ProjectPath+"Ori_RImage\\chess";
	StrStm<<m_nRImageIndex;
 	StrStm>>imageFileName;
	RfilePath += imageFileName;
	RfilePath+= ".bmp";
	imwrite(RfilePath,RImg);
}


void CBSVDlg::StopLeftCamera()
{
	MVSTATUS_CODES status;
	if (m_pLCamera != NULL)
	{
		status = MVStopGrab(m_pLCamera);		 
        status = MVCloseCam(m_pLCamera);	
		m_pLCamera = NULL;
	}
}


void CBSVDlg::StopRightCamera()
{
	MVSTATUS_CODES status;
	if (m_pRCamera != NULL)
	{
		status = MVStopGrab(m_pRCamera);
		status = MVCloseCam(m_pRCamera);
		m_pRCamera = NULL;
	}	
}


void CBSVDlg::OnBnClickedCornerdetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_LeftShowDIBBits == NULL)
	{
		MessageBox("��ͼ�����ݣ�","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		if( m_LPixelFormat != PixelFormat_Mono8 )
			return;
		Mat LSrcImg;
		LSrcImg.create(m_LeftOriHeight, m_LeftOriWidth, CV_8UC1);
		g_pBSVDlg->m_CSectionCopyL.Lock();
		memcpy(LSrcImg.data,m_LeftShowDIBBits,m_LeftOriWidth*m_LeftOriHeight*(m_nLBit/8));
		g_pBSVDlg->m_CSectionCopyL.Unlock();

		string LfilePath=ProjectPath+"LCornerDetect.bmp";
		imwrite(LfilePath,LSrcImg);
		Mat LCatchImg = imread(LfilePath);
		CornerDetect(LCatchImg);
	}
}


void CBSVDlg::OnBnClickedCornerdetect2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_RightShowDIBBits == NULL)
	{
		MessageBox("��ͼ�����ݣ�","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		if( m_RPixelFormat != PixelFormat_Mono8 )
			return;
		Mat RSrcImg;
		RSrcImg.create(m_RightOriHeight, m_RightOriWidth, CV_8UC1);
		g_pBSVDlg->m_CSectionCopyR.Lock();
		memcpy(RSrcImg.data,m_RightShowDIBBits,m_RightOriWidth*m_RightOriHeight*(m_nRBit/8));
		g_pBSVDlg->m_CSectionCopyR.Unlock();

		string RfilePath=ProjectPath+"RCornerDetect.bmp";
		imwrite(RfilePath,RSrcImg);
		Mat RCatchImg = imread(RfilePath);
		CornerDetect(RCatchImg);
	}
}


/*�öδ����б�����1�������������������2�����������*/
void CBSVDlg::OnBnClickedStereocalibration()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CString a, b, c, d, SCaliFile;
	//a.Format(_T("%s\\LCamera_calibdata.txt"),ProjectPath);
	//b.Format(_T("%s\\LCamera_caliberation_result.txt"),ProjectPath);
	//string A((LPCTSTR)a);
	//string B((LPCTSTR)b);
	//c.Format(_T("%s\\RCamera_calibdata.txt"),ProjectPath);
	//d.Format(_T("%s\\RCamera_caliberation_result.txt"),ProjectPath);
	//string C((LPCTSTR)c);
	//string D((LPCTSTR)d);
	//SCaliFile.Format(_T("%s\\Stereo_caliberation_result.txt"),ProjectPath);

	//double err = 0;
	//row_corner_num = 0;
	//column_corner_num = 0;
	//grid_width = 0;
	//grid_height = 0;
	//PicNum==0;
	//CCali_ParaDlg dlg;
	//dlg.DoModal();
	//if(row_corner_num==0|column_corner_num==0|grid_width==0|grid_height==0|PicNum==0)
	//{
	//	AfxMessageBox(_T("�뽫������д������"));
	//}
	//else
	//{
	//	cout<<"***��ʼ������궨***"<<endl;
	//	cout<<endl;
	//	WriteTxt(A,PicNum);
	//	m_LImgPath="Ori_LImage\\chess";
	//	Calibration(A,B,row_corner_num,column_corner_num,grid_width,grid_height,m_LImgPath,1);
	//	
	//	cout<<endl;
	//	cout<<"***��ʼ������궨���Ժ�2��***"<<endl;
	//	cout<<endl;
	//	waitKey(2000);

	//	WriteTxt2(C,PicNum);
	//	m_RImgPath="Ori_RImage\\chess";
	//	Calibration(C,D,row_corner_num,column_corner_num,grid_width,grid_height,m_RImgPath,2);
	//}

	////���浥�������
	//CString ymlFile1,ymlFile2;
	//ymlFile1.Format(_T("%s\\data\\cameraParams1.yml"),ProjectPath);
	//ymlFile1.Format(_T("%s\\data\\cameraParams2.yml"),ProjectPath);
	//m_StereoCalib.saveCameraData(m_cameraParams1, (LPCTSTR)ymlFile1);
	//m_StereoCalib.saveCameraData(m_cameraParams2, (LPCTSTR)ymlFile2);

	////��ʼ˫Ŀ�궨
	//m_stereoParams.cameraParams1 = m_cameraParams1;
	//m_stereoParams.cameraParams2 = m_cameraParams2;

	//stereoCalibrate(m_cornersDatas.objectPoints, m_cornersDatas.imagePoints1, m_cornersDatas.imagePoints2,
	//	m_cameraParams1.cameraMatrix, m_cameraParams1.distortionCoefficients, m_cameraParams2.cameraMatrix,
	//	m_cameraParams2.distortionCoefficients, m_cornersDatas.imageSize, m_stereoParams.rotation,
	//	m_stereoParams.translation, m_stereoParams.essential, m_stereoParams.foundational,
	//	TermCriteria(cv::TermCriteria::COUNT + cv::TermCriteria::EPS, 30, 1e-5), CV_CALIB_FIX_INTRINSIC
	//	);

	////����˫Ŀ�궨���
	//m_StereoCalib.getStereoCalibrateError(m_cornersDatas, m_stereoParams, err);
	//CString error;
	//error.Format(_T("˫Ŀ�궨���Ϊ��%f"),err);
	//AfxMessageBox(error);

	////����˫Ŀ����
	//m_StereoCalib.mstereoRectify(m_cameraParams1, m_cameraParams2, m_stereoParams, m_remapMatrixs);

	////����ͼƬ����
	//ifstream fin11(a);
	//ifstream fin12(c);
	//string str11, str22;
	//Mat img1r, img2r;
	//CString name11,name22;
	//int cnt = 0;
	//while (getline(fin11, str11) && getline(fin12, str22))
	//{
	//	cnt++;
	//	Mat image1 = imread(str11);
	//	Mat image2 = imread(str22);
	//	m_StereoCalib.remapImage(image1, image2, img1r, img2r, m_remapMatrixs);
	//	name11.Format(_T("%s\\Save_LImage\\Left_%d.jpg"),ProjectPath,cnt);
	//	name22.Format(_T("%s\\Save_RImage\\Right_%d.jpg"),ProjectPath,cnt);
	//	string Name11((LPCTSTR)name11);
	//	string Name22((LPCTSTR)name22);
	//	imwrite(Name11, img1r);
	//	imwrite(Name22, img2r);
	//}

	////��У�����ͼ����л���
	//string Name11((LPCTSTR)name11);
	//string Name22((LPCTSTR)name22);
	//Mat imgLr = imread(Name11);
	//Mat imgRr = imread(Name22);
	//Mat img(m_cornersDatas.imageSize.height*0.5, m_cornersDatas.imageSize.width, CV_8UC3);//����IMG���߶�һ�������˫��
	//Mat imgPart1 = img(Rect(0, 0, m_cornersDatas.imageSize.width*0.5, m_cornersDatas.imageSize.height*0.5));//ǳ����
	//Mat imgPart2 = img(Rect(m_cornersDatas.imageSize.width*0.5, 0, m_cornersDatas.imageSize.width*0.5, m_cornersDatas.imageSize.height*0.5));//ǳ����
	//resize(imgLr, imgPart1, imgPart1.size(), 0, 0, CV_INTER_AREA);
	//resize(imgRr, imgPart2, imgPart2.size(), 0, 0, CV_INTER_AREA);//�ı�ͼ��ߴ磬����0,0��
	//for (int i = 0; i < img.rows; i += 16) //������
	//	line(img, Point(0, i), Point(img.cols, i), Scalar(0, 255, 0), 1, 8);
	//namedWindow("У�����ͼ��", WINDOW_NORMAL);
	//imshow("У�����ͼ��", img);

	////����˫Ŀ�궨����
	///*������txt�ļ���*/
	//ofstream StereoCaliResult(SCaliFile);
	//StereoCaliResult<<"������ڲ�������\n";	
	//StereoCaliResult<<m_cameraParams1.cameraMatrix<<'\n';	
	//StereoCaliResult<<"���������ϵ����\n";
	//StereoCaliResult<<m_cameraParams1.distortionCoefficients<<'\n';
	//StereoCaliResult<<"������ڲ�������\n";	
	//StereoCaliResult<<m_cameraParams2.cameraMatrix<<'\n';	
	//StereoCaliResult<<"���������ϵ����\n";
	//StereoCaliResult<<m_cameraParams2.distortionCoefficients<<'\n';
	//StereoCaliResult<<"���������ת����\n";	
	//StereoCaliResult<<m_stereoParams.rotation<<'\n';	
	//StereoCaliResult<<"�������ƽ�ƾ���\n";
	//StereoCaliResult<<m_stereoParams.translation<<'\n';
	//StereoCaliResult<<"��������\n";
	//StereoCaliResult<<m_stereoParams.essential<<'\n';
	//StereoCaliResult<<"��������\n";
	//StereoCaliResult<<m_stereoParams.foundational<<'\n';

	///*������yml�ļ���*/
	////CString ymlFile;
	////ymlFile.Format(_T("%s\\data\\CalibrationDatas.yml"),ProjectPath);
	////m_StereoCalib.saveCalibrationDatas((LPCTSTR)ymlFile, m_cornersDatas, m_stereoParams, m_remapMatrixs);
	//
	///*������xml�ļ���*/
	//CString xmlFile;
	//xmlFile.Format(_T("%s\\data\\CalibrationDatas.xml"),ProjectPath);
	//m_StereoCalib.saveCalibrationDatas((LPCTSTR)xmlFile, m_cornersDatas, m_stereoParams, m_remapMatrixs);
}


void CBSVDlg::OnBnClickedStereomatch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Mat img1, img2, img1p, img2p, disp, disp8u,pointCloud;
	//��ѡ�ɼ��ĵ�һ��ͼƬ����˫Ŀƥ��
	CString LImg, RImg;
	LImg.Format(_T("%s\\Ori_LImage\\chess1.bmp"),ProjectPath);
	RImg.Format(_T("%s\\Ori_RImage\\chess1.bmp"),ProjectPath);
	img1 = imread((LPCTSTR)LImg);
	img2 = imread((LPCTSTR)RImg);

	
	int stereoMethod = 0;//˫Ŀƥ�䷽����־λ,0ΪBM������ΪSGBM
	if(stereoMethod==0)
	{
		//ʹ��BM�㷨����˫Ŀƥ��
		m_stereoMatcher.bmMatch(img1, img2, disp, img1p, img2p);
	}
	else
	{
		//ʹ��SGBM�㷨����˫Ŀƥ��
		m_stereoMatcher.sgbmMatch(img1, img2, disp, img1p, img2p);
	}

	// �����Ⱦ�����������ߣ��Աȶ��ж�׼���
	int j = 0;
	for( j = 0; j < img1p.rows; j += 32 )		
	{
		line( img1p, cvPoint(0,j),	cvPoint(img1p.cols,j), CV_RGB(0,255,0)); 
		line( img2p, cvPoint(0,j),	cvPoint(img2p.cols,j), CV_RGB(0,255,0)); 
	}

	imshow("����ͼ",img1p);
	imshow("����ͼ",img2p);

	bool showColorDisparity = false;//�Ƿ���ʾ��ɫ�Ӳ�ͼ,�ô�Ĭ�ϲ���ʾ
	if(showColorDisparity)
	{
		m_stereoMatcher.getDisparityImage(disp, disp8u, true);
		imshow("α��ɫ�Ӳ�ͼ",disp8u);
	}
	else
	{
		m_stereoMatcher.getDisparityImage(disp, disp8u, false);
		imshow("�Ӳ�ͼ",disp8u);
	}

	//����������ͷ��������壨lq�������⵽��������嶼��ʾΪ(0,0,0)��
	bool generatePointCloud = true;	//�Ƿ�������ά���Ʊ�־λ
	PointCloudAnalyzer pointCloudAnalyzer;
	clock_t tStart = clock();
	if (generatePointCloud)
	{
		m_stereoMatcher.getPointClouds(disp, pointCloud);

		vector<PointCloudAnalyzer::ObjectInfo> objectInfos;
		pointCloudAnalyzer.detectNearObject(img1p, pointCloud, objectInfos);

		if (!objectInfos.empty() && (clock()-tStart) > 500)
		{
			tStart = clock();
			double fl = m_stereoMatcher.m_FL;
			m_ObjectDistance = objectInfos[0].distance; m_ObjectDistance = (int)(m_ObjectDistance * 10000) / 10000.;
			m_ObjectHeight = objectInfos[0].boundRect.height * objectInfos[0].distance / fl; m_ObjectHeight = (int)(m_ObjectHeight * 10000) / 10000.;
			m_ObjectWidth = objectInfos[0].boundRect.width * objectInfos[0].distance / fl; m_ObjectWidth = (int)(m_ObjectWidth * 10000) / 10000.;
			UpdateData(FALSE);
		}
	}
	// ���澰��ͼ��ĵ�������
	if ( generatePointCloud )
	{
		CString PtsFile;
		PtsFile.Format(_T("%s\\data\\points.txt"),ProjectPath);
		m_stereoMatcher.savePointClouds(pointCloud, PtsFile);
	}
	//���ͼ�����������ص��Ӧ����ά����
	//const double max_z = 1.0e4;
	//for(int y = 0; y < pointCloud.rows; y++)
	//{
	//	for(int x = 0; x < pointCloud.cols; x++)
	//	{
	//		cv::Vec3f point = pointCloud.at<cv::Vec3f>(y, x);
	//		if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z)
	//			printf("%d��%d��:%d %d %d\n", y,x,0, 0, 0);
	//		else
	//			printf("%d��%d��:%f %f %f \n", y,x,point[0], point[1], point[2]);
	//	}
	//}

	//������ص㣨46��271����Ӧ�Ŀռ���ά����
	int x = 271;
	int y = 46;
	const double max_z = 1.0e4;
	cv::Vec3f point = pointCloud.at<cv::Vec3f>(y, x);
	if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z)
		printf("%d��%d��:%d %d %d\n", y,x,0, 0, 0);
	else
		printf("%d��%d��:%f %f %f \n", y,x,point[0], point[1], point[2]);
}

/*
 *�������ܣ�ͨ�����ͼ��Բ�ĵ�����ֵ��ȷ��Բ�ĵ������Ϣ
 *���⣺Ŀǰλ�ÿռ�����ϵԭ��λ�ã�δ����������֤�Ƿ����
*/
void CBSVDlg::OnBnClickedStereomeasure()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	cv::Mat img1, img2, img1p, img2p, disp, disp8u,pointCloud;
	//ͼƬ���룺��ѡ�ɼ��ĵ�һ��ͼƬ����˫Ŀƥ�䣨�ɸĳɲɼ��ĵ�ǰͼƬ��
	CString LImg, RImg;
	LImg.Format(_T("%s\\Ori_LImage\\Left0003.bmp"),ProjectPath);
	RImg.Format(_T("%s\\Ori_RImage\\Right0003.bmp"),ProjectPath);
	img1 = imread((LPCTSTR)LImg);
	img2 = imread((LPCTSTR)RImg);
	//ʹ��BM�㷨����˫Ŀƥ��
	m_stereoMatcher.bmMatch(img1, img2, disp, img1p, img2p);
	//��ȡ�Ӳ�ͼ
	m_stereoMatcher.getDisparityImage(disp, disp8u, false);
	//��һ�������Ӳ�ͼ����Բ�μ���ȡ��Բ����������
	//��˹�˲�
	Mat disp8uFilImg;
	GaussianBlur(disp8u, disp8uFilImg, Size(7,7), 0, 0);
	//�̶���ֵ�ָ�ͼ��
	Mat disp8uBinImg;
	threshold(disp8uFilImg, disp8uBinImg, 100, 255, CV_THRESH_BINARY);

	//��������ı���Ϊ�������д���
	Blob_ReleaseLeftBlobSeq();
	Detect_LeftCircleDetect(disp8uBinImg, 0, 255, 0.6, 0.6);
	Blob_DenoisingLeftArea(1000, 8000);


	//��һ����������Բ����������ӳ��Ϊ�ռ���ά����
	double x = LBlob.BlobX;//��취��ȡ��m_LeftBlobSeq�б�ʾԲ�������BlobX��BlobY
	double y = LBlob.BlobY;//����ռ��ʱ��������������int���ʹ洢�ģ�����ʹ��double
	                       //��һ������
	//��һ�����ռ���ά�������ʾ�򱣴�,Ŀǰ�Խ�����ֵ���������̨Ϊ��
	const double max_z = 1.0e4;
	cv::Vec3f point = pointCloud.at<cv::Vec3f>(y, x);
	if(fabs(point[2] - max_z) < FLT_EPSILON || fabs(point[2]) > max_z)
		printf("%d��%d��:%d %d %d\n", y,x,0, 0, 0);
	else
		printf("%d��%d��:%f %f %f \n", y,x,point[0], point[1], point[2]);
}


void CBSVDlg::OnBnClickedStopcamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(g_pBSVDlg->m_pLCamera != NULL)
	{
		g_pBSVDlg->StopLeftCamera();
	}
}


void CBSVDlg::OnBnClickedStopcamera2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
		if(g_pBSVDlg->m_pRCamera != NULL)
	{
		g_pBSVDlg->StopRightCamera();
	}
}


void CBSVDlg::OnBnClickedStop2camera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedStopcamera();
	OnBnClickedStopcamera2();
}


void CBSVDlg::OnBnClickedSave2pic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnBnClickedSavepic();
	OnBnClickedSavepic2();
}


void CBSVDlg::OnBnClickedTabbutton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_childdlg1.ShowWindow(SW_SHOW);
	m_Childdlg1.ShowWindow(SW_HIDE);
	m_childdlg3.ShowWindow(SW_HIDE);
}


void CBSVDlg::OnBnClickedTabbutton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_childdlg1.ShowWindow(SW_HIDE);
	m_Childdlg1.ShowWindow(SW_SHOW);
	m_childdlg3.ShowWindow(SW_HIDE);
}


void CBSVDlg::OnBnClickedTabbutton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_childdlg1.ShowWindow(SW_HIDE);
	m_Childdlg1.ShowWindow(SW_HIDE);
	m_childdlg3.ShowWindow(SW_SHOW);
}


void CBSVDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CDC buttonDC;
	CBitmap bitmapTrans;
	BITMAP bmp;
	CDC mem;
	CRect rc;
	buttonDC.Attach(lpDrawItemStruct->hDC);//�õ����ڻ��ư�ť��DC
	mem.CreateCompatibleDC(&buttonDC);//׼����ť������ͼ��    

	if (lpDrawItemStruct->CtlID == IDC_TABBUTTON1)
	{
		rc = lpDrawItemStruct->rcItem;//��ȡ��ť��ռ�ľ��δ�С
		UINT state  = lpDrawItemStruct->itemState;//��ȡ��ť��ǰ��״̬����ͬ״̬���Ʋ�ͬ�İ�ť    
		if (state & ODS_FOCUS)//�����ť�Ѿ�ȡ�ý��㣬����ѡ��״̬�µİ�ť
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP1);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap * old = mem.SelectObject(&bitmapTrans);
			//��ť����λͼ��ʹ��stretcnblt����ʹͼƬ�水ť��С���ı�
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old);
			bitmapTrans.DeleteObject();

			//���İ�ť���� 
			CString btnCaption = _T("˫Ŀ����");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem)); //�õ�ԭButton�ľ��δ�С  
			CRect textRect;//����һ��CRect���ڻ����ı�
			textRect.CopyRect(&drawRect);//������������ 
			CSize sz = pDC->GetTextExtent(btnCaption);//����ַ����ߴ�
			textRect.top += (textRect.Height()- sz.cy)/2;//�����ı�λ�þ���
			pDC->SetBkMode(TRANSPARENT);//�����ı�����͸�� 
			pDC->SetTextColor(RGB(100,100,100)); //�����ı���ɫ
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);//�����ı�����
		}
		else //�����ťδȡ�ý��㣬����ѡ��״̬�µİ�ť
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP2);
			CBitmap *old2 = mem.SelectObject(&bitmapTrans);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap *old=mem.SelectObject(&bitmapTrans);
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old2);
			bitmapTrans.DeleteObject();

			//���button���� 
			CString btnCaption = _T("˫Ŀ����");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
	}

		if (lpDrawItemStruct->CtlID == IDC_TABBUTTON2)
	{
		rc = lpDrawItemStruct->rcItem;//��ȡ��ť��ռ�ľ��δ�С
		UINT state  = lpDrawItemStruct->itemState;//��ȡ��ť��ǰ��״̬����ͬ״̬���Ʋ�ͬ�İ�ť    
		if (state & ODS_FOCUS)//�����ť�Ѿ�ȡ�ý��㣬����ѡ��״̬�µİ�ť
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP1);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap * old = mem.SelectObject(&bitmapTrans);
			//��ť����λͼ��ʹ��stretcnblt����ʹͼƬ�水ť��С���ı�
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old);
			bitmapTrans.DeleteObject();

			//���İ�ť����
			CString btnCaption = _T("���������");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(100,100,100)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
		else //�����ť�Ѿ�ȡ�ý��㣬����ѡ��״̬�µİ�ť
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP2);
			CBitmap *old2 = mem.SelectObject(&bitmapTrans);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap *old=mem.SelectObject(&bitmapTrans);
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old2);
			bitmapTrans.DeleteObject();

			//���button���� 
			CString btnCaption = _T("���������");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
	}

	if (lpDrawItemStruct->CtlID == IDC_TABBUTTON3)
	{
		rc = lpDrawItemStruct->rcItem;//��ȡ��ť��ռ�ľ��δ�С
		UINT state  = lpDrawItemStruct->itemState;//��ȡ��ť��ǰ��״̬����ͬ״̬���Ʋ�ͬ�İ�ť    
		if (state & ODS_FOCUS)//�����ť�Ѿ�ȡ�ý��㣬����ѡ��״̬�µİ�ť
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP1);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap * old = mem.SelectObject(&bitmapTrans);
			//��ť����λͼ��ʹ��stretcnblt����ʹͼƬ�水ť��С���ı�
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old);
			bitmapTrans.DeleteObject();

			//���İ�ť����
			CString btnCaption = _T("���������");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2;
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(100,100,100)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}
		else //�����ť�Ѿ�ȡ�ý��㣬����ѡ��״̬�µİ�ť
		{
			bitmapTrans.LoadBitmap(IDB_BITMAP2);
			CBitmap *old2 = mem.SelectObject(&bitmapTrans);
			bitmapTrans.GetBitmap(&bmp);
			CBitmap *old=mem.SelectObject(&bitmapTrans);
			buttonDC.StretchBlt(rc.left,rc.top,rc.right,rc.bottom,&mem,0,0,bmp.bmWidth,bmp.bmHeight,SRCCOPY);
			mem.SelectObject(old2);
			bitmapTrans.DeleteObject();

			//���İ�ť����
			CString btnCaption = _T("���������");
			CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
			CRect drawRect;
			drawRect.CopyRect(&(lpDrawItemStruct->rcItem));   		
			CRect textRect;
			textRect.CopyRect(&drawRect);
			CSize sz = pDC->GetTextExtent(btnCaption);
			textRect.top += (textRect.Height()- sz.cy)/2; 
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(0,0,0)); 
			pDC->DrawText(btnCaption,&textRect,DT_RIGHT|DT_CENTER|DT_BOTTOM);
		}

	CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);
	}
}



void CBSVDlg::ShowLeftPic(void)
{
	MVSTATUS_CODES hr;
	if (m_pLCamera != NULL)
	{
		MessageBox("����Ѵ򿪣�");
		return;
	}
	 
	hr = MVOpenCamByIndex( 0, &m_pLCamera ); //�����
	if (hr != MVST_SUCCESS)
	{
		MessageBox("�������ʧ�ܣ�");
		return;
	}
	else if (hr == MVST_SUCCESS)
	{
		m_nLBit = 24;
		MVGetWidth(m_pLCamera, &m_LeftOriWidth);  //�õ�ͼ�񳤿�
		MVGetHeight(m_pLCamera,&m_LeftOriHeight);
		
		MVGetPixelFormat(m_pLCamera,&m_LPixelFormat);     //�õ����ݸ�ʽ
		if( m_LPixelFormat == PixelFormat_Mono8 )
		{
			m_nLBit = 8;
		}
		else
		{
			m_nLBit = 24;
		}
	//	MVSetStrobeSource(m_pLCamera,LineSource_ExposureActive);
		
		
		hr =  MVStartGrab(m_pLCamera, LeftCallbackFunction, (long)this); //���ûص�����
		if (hr == MVST_SUCCESS)
		{	
			//ԭʼͼ��
			m_pLBmpInfo->bmiHeader.biBitCount = m_nLBit;
			m_pLBmpInfo->bmiHeader.biWidth = m_LeftOriWidth;
			m_pLBmpInfo->bmiHeader.biHeight = m_LeftOriHeight;
			
			//Ϊͼ����ʾ��ͼ�����ݷ����ڴ�ռ�
		    delete[] m_LeftShowDIBBits;	
		    m_LeftShowDIBBits = NULL;	
			m_LeftShowDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];

			delete[] m_LeftOriDIBBits;	
			m_LeftOriDIBBits = NULL;						
			m_LeftOriDIBBits = new char[m_LeftOriWidth * m_LeftOriHeight * (m_nLBit/8)];
			
			//Ϊ���д����ͼ��ֵ
			m_LeftRectLeft=0;  
			m_LeftRectBottom=0;
			m_LeftWidth=m_LeftOriWidth; 
			m_LeftHeight=m_LeftOriHeight;

			delete[] m_LeftDIBBits;	
			m_LeftDIBBits = NULL;					
			m_LeftDIBBits = new char[m_LeftWidth * m_LeftHeight * (m_nLBit/8)];
		}
	}
}


Mat CBSVDlg::FaceDetect(Mat frame)
{
	std::vector<Rect> face;  
	Mat frame_gray(frame.size(),CV_8U);  
    cvtColor( frame, frame_gray, CV_BGR2GRAY );  
    equalizeHist( frame_gray, frame_gray );  
  
    Mycascade.detectMultiScale( frame_gray, face, 1.1, 2, 0, Size(64, 128) );  
  
    for( int i = 0; i < face.size(); i++ )  
	{    
        rectangle(frame,                   //ͼ��.  
			      face[i],
                  Scalar(0, 255, 0),     //������ɫ (RGB) �����ȣ��Ҷ�ͼ�� ��(grayscale image��  
                  1);                   //��ɾ��ε������Ĵ�ϸ�̶ȡ�ȡ��ֵʱ���� CV_FILLED���������������ɫ�ʵľ��� 
    }  
 
	imshow( window_name, frame );  
	return frame;
}


void CBSVDlg::OnBnClickedFacedetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Mat image,ROI;  
	string Cascade_mode = "..\\BSV\\cascade.xml";
    if(!Mycascade.load( Cascade_mode ))  
	{ 
		printf("[error] �޷����ؼ����������ļ���\n");   
		/*return -1;*/    
	} 
	CString ImgPath;
	ImgPath.Format (_T("%s\\face.jpg"),ProjectPath);
    image= imread((LPCTSTR)ImgPath);//��ȡͼƬ  
    if(!image.data)  
	{  
		printf("[error] û��ͼƬ\n");   
		/*return -1;*/  
	}  
    ROI= FaceDetect(image);  
    waitKey(0);   
    //return 4;  
}

/*
 *�������ܣ�����ȫ���ؼ�����Ӧ�����Ŵ�
 *���⣺����ɿؼ��Ų����ң�Ŀǰֻ������һ���ֿؼ�ִ���˸ù���
 *ԭ�ģ�https://blog.csdn.net/chw1989/article/details/7488711 
*/
void CBSVDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//�ռ��С�洰��仯
	//ԭ�ģ�https://blog.csdn.net/chw1989/article/details/7488711 
	if(nType==1) return;//��С����ʲô������
	CWnd *pWnd; 
	pWnd = GetDlgItem(IDC_STATIC);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_LeftPic);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_RightPic);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_TABBUTTON1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_TABBUTTON2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_TABBUTTON3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_MENUFORM);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Open2Camera);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Cap2Video);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_StereoCalibration);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Circle2Detect);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_StereoMatch);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_StereoMeasure);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_BUTTON8);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Stop2Camera);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_Save2Pic);
	ChangeSize(pWnd, cx, cy);
	GetClientRect(&m_rect);// ���仯��ĶԻ����С��Ϊ�ɴ�С
}

/*
 *�������ܣ��ı�ؼ���С
 *���⣺����
 *ԭ�ģ�https://blog.csdn.net/chw1989/article/details/7488711 
*/
void CBSVDlg::ChangeSize(CWnd *pWnd, int cx, int cy)
{
    if(pWnd)  //�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����	
	{
		CRect rect;   //��ȡ�ؼ��仯ǰ�Ĵ�С  
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
 
		//    cx/m_rect.Width()Ϊ�Ի����ں���ı仯����
		rect.left=rect.left*cx/m_rect.Width();//�����ؼ���С
		rect.right=rect.right*cx/m_rect.Width();
		rect.top=rect.top*cy/m_rect.Height();
		rect.bottom=rect.bottom*cy/m_rect.Height();
		pWnd->MoveWindow(rect);//���ÿؼ���С
	}
}

