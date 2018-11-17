// ChildDlg2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BSV.h"
#include "BSVDlg.h"
#include "ChildDlg2.h"
#include "afxdialogex.h"


// CChildDlg2 �Ի���

IMPLEMENT_DYNAMIC(CChildDlg2, CDialogEx)

CChildDlg2::CChildDlg2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlg2::IDD, pParent)
{

}

CChildDlg2::~CChildDlg2()
{
}

void CChildDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlg2, CDialogEx)
	ON_BN_CLICKED(IDC_CircleDetect, &CChildDlg2::OnBnClickedCircledetect)
	ON_BN_CLICKED(IDC_OpenCamera, &CChildDlg2::OnBnClickedOpencamera)
	ON_BN_CLICKED(IDC_CapVideo, &CChildDlg2::OnBnClickedCapvideo)
	ON_WM_CREATE()
//	ON_BN_CLICKED(IDC_StopCamera, &CChildDlg2::OnBnClickedStopcamera)
END_MESSAGE_MAP()

BOOL CChildDlg2::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}


void CChildDlg2::OnBnClickedCircledetect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	if (pBSVDlg->m_LeftShowDIBBits == NULL)
	{
		MessageBox("��ͼ�����ݣ�","BSV",MB_ICONWARNING);
		return;
	}
	else
	{
		pBSVDlg->SetTimer(1, 300, NULL);//���ö�ʱ��1
		pBSVDlg->m_TimerFlag = true;

		GetDlgItem(IDC_CircleDetect)->EnableWindow(false);
		GetDlgItem(IDC_StopDetect)->EnableWindow(true);
	}
}


void CChildDlg2::OnBnClickedOpencamera()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->OnBnClickedOpencamera();
	//���������	
	//MVSTATUS_CODES hr;
	//if (pBSVDlg->m_pLCamera == NULL)	//�������
	//{
	//	hr = MVOpenCamByIndex(0, &pBSVDlg->m_pLCamera );		
	//	if (hr != MVST_SUCCESS)
	//	{
	//		MessageBox("δ�ҵ��������ȷ���豸���Ӻ�IP���ã�","BSV",MB_ICONWARNING);
	//		return;
	//	}
	//	else if (hr == MVST_SUCCESS)
	//	{
	//		GetDlgItem(IDC_OpenCamera)->SetWindowText("�ر����");
	//		//GetDlgItem(IDC_CapVideo)->EnableWindow(true);
	//		//GetDlgItem(IDC_SetCamera)->EnableWindow(true);
	//		//GetDlgItem(IDC_StopCamera)->EnableWindow(false);
	//	}
	//}
	//else	//�ر����
	//{
	//	hr = MVStopGrab(pBSVDlg->m_pLCamera);		 
	//	hr = MVCloseCam(pBSVDlg->m_pLCamera);	
	//	pBSVDlg->m_pLCamera = NULL;

	//	//GetDlgItem(IDC_OpenCamera)->SetWindowText("�������");
	//	GetDlgItem(IDC_CapVideo)->EnableWindow(false);
	//	GetDlgItem(IDC_SetCamera)->EnableWindow(false);
	//}
}


void CChildDlg2::OnBnClickedCapvideo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->ShowLeftPic();
	CDialogEx::OnCancel();
}


int CChildDlg2::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	return 0;
}


//void CChildDlg2::OnBnClickedStopcamera()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//}
