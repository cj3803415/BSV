#pragma once


// CCali_ParaDlg �Ի���

class CCali_ParaDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCali_ParaDlg)

public:
	CCali_ParaDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCali_ParaDlg();

// �Ի�������
	enum { IDD = IDD_CaliPara_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCalistart();
	int m_row_corner_num;
	int m_column_corner_num;
	int m_grid_width;
	int m_grid_height;
	int m_PicNum;
};
