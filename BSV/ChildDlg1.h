#pragma once
#include "resource.h"
// CChildDlg1 �Ի���

class CChildDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlg1)

public:
	CChildDlg1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChildDlg1();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton1();
//	afx_msg void OnBnClickedButton12();
};
