#pragma once


// CChildDlg2 �Ի���

class CChildDlg2 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlg2)

public:
	CChildDlg2(CWnd* pParent = NULL);   
	virtual ~CChildDlg2();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedCircledetect();
	afx_msg void OnBnClickedOpencamera();
	afx_msg void OnBnClickedCapvideo();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
//	afx_msg void OnBnClickedStopcamera();
};
