// ChildDlg3.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "BSV.h"
#include "ChildDlg3.h"
#include "afxdialogex.h"


// CChildDlg3 �Ի���

IMPLEMENT_DYNAMIC(CChildDlg3, CDialogEx)

CChildDlg3::CChildDlg3(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChildDlg3::IDD, pParent)
{

}

CChildDlg3::~CChildDlg3()
{
}

void CChildDlg3::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CChildDlg3, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON13, &CChildDlg3::OnBnClickedButton13)
END_MESSAGE_MAP()


// CChildDlg3 ��Ϣ�������


void CChildDlg3::OnBnClickedButton13()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
