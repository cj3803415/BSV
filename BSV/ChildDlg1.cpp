// ChildDlg1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BSV.h"
#include "ChildDlg1.h"
#include "afxdialogex.h"


// CChildDlg1 �Ի���

IMPLEMENT_DYNAMIC(CChildDlg1, CDialogEx)

CChildDlg1::CChildDlg1(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlg1::IDD, pParent)
{

}

CChildDlg1::~CChildDlg1()
{
}

void CChildDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlg1, CDialogEx)
//	ON_BN_CLICKED(IDC_BUTTON12, &CChildDlg1::OnBnClickedButton12)
END_MESSAGE_MAP()


// CChildDlg1 ��Ϣ�������


//void CChildDlg1::OnBnClickedButton12()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������B
//	AfxMessageBox(_T("TRY!"));
//}
