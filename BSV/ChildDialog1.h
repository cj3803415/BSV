#pragma once


// CChildDialog1 �Ի���

class CChildDialog1 : public CDialogEx
{
	DECLARE_DYNAMIC(CChildDialog1)

public:
	CChildDialog1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CChildDialog1();

// �Ի�������
	enum { IDD = IDD_ChildDlg1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();

public:
	int row_corner_num;			//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int column_corner_num;		//�궨ͼ���ڽǵ㣨���ͱ�Ե�Ӵ��Ľǵ㣩����
	int grid_width;				//ʵ�ʲ����õ��ı궨����ÿ�����̸�ĳ�
	int grid_height;			//ʵ�ʲ����õ��ı궨����ÿ�����̸�Ŀ�
	int PicNum;					//��Ҫ�궨ͼƬ������
	
	afx_msg void OnBnClickedButton9();
};
