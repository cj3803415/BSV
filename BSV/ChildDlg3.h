#pragma once


// CChildDlg3 �Ի���

class CChildDlg3 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDlg3)

public:
	CChildDlg3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChildDlg3();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton13();
};
