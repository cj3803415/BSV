// TEST2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BSV.h"
#include "TEST2.h"
#include "afxdialogex.h"
#include "BSVDlg.h"


// CTEST2 �Ի���

IMPLEMENT_DYNAMIC(CTEST2, CDialogEx)

CTEST2::CTEST2(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTEST2::IDD, pParent)
{

}

CTEST2::~CTEST2()
{
}

void CTEST2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTEST2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTEST2::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTEST2::OnBnClickedButton2)
END_MESSAGE_MAP()


// CTEST2 ��Ϣ�������


void CTEST2::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->ShowLeftPic();
}


void CTEST2::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CBSVDlg *pBSVDlg = (CBSVDlg*)GetParent();
	pBSVDlg->StopLeftCamera();
}
