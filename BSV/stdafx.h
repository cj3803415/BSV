
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#define Chinesse
//����������öԻ���
#ifdef    Chinesse
//����������öԻ���
#define   SCP_Static_Shut_STR                   "�ع�"
#define   SCP_Static_Gain_STR                   "����"
#define   SCP_Button_HightSet_STR               "�߼�����"
#define   SCP_Button_Reset_STR                  "����"
#define   SCP_Button_OK_STR                     "ȷ��"
#define   SCP_Radio_Continue_STR                "����ģʽ"
#define   SCP_Radio_Trigger_STR                 "����ģʽ"
//��������������öԻ���

//���Ի���
#define   BMS_Title_LeftCamParam_STR             "���������"
#define   BMS_Title_RightCamParam_STR            "���������"
////������궨��ɰ�ť�¼�
#define   	STR_LCalibFinishText_1    "������궨���:"
#define   	STR_LCalibFinishText_2    "����ϵ��:"
#define   	STR_LCalibFinishText_3    "������ڲξ���A:"
#define   	STR_LCalibFinishText_4    "������������:"
#define   	STR_LCalibFinishList_1    "���|�в�"
#define   	STR_LCalibFinishList_2    "��ͼƬ"
////������궨��ɰ�ť�¼�
#define   	STR_RCalibFinishText_1    "������궨���:"
#define   	STR_RCalibFinishText_2    "����ϵ��:"
#define   	STR_RCalibFinishText_3    "������ڲξ���A:"
#define   	STR_RCalibFinishText_4    "������������:"
#define   	STR_RCalibFinishList_1    "���|�в�"
#define   	STR_RCalibFinishList_2    "��ͼƬ"
////˫����궨��ɰ�ť�¼�	
#define   	STR_DCalibFinishText_1    "������궨���:                   ������궨���:                   ˫Ŀ�궨���:\n"
#define   	STR_DCalibFinishText_2    "����ϵ��:                               ����ϵ��:                               ƽ������T:\n"
#define   	STR_DCalibFinishText_3    "������ڲξ���A:                 ������ڲξ���A:                 ��ת����R:\n"
#define   	STR_DCalibFinishText_4    "������������:"
#define   	STR_DCalibFinishText_5    "\n������������:"
#define   	STR_DCalibFinishText_6    "\n\n��������:\n"
#define   	STR_DCalibFinishList_1    "���|�в�"
#define   	STR_DCalibFinishList_2    "��ͼƬ"
#define   	STR_DCalibFinishList_3    "��ͼƬ"

#define   	STR_CalibMeasureMessageBox       "���ڽ��б궨�����,�밴ֹͣ���ڽ������±궨","��ʾ"
#define   	STR_CalibFailText         "�궨ʧ��"
#define     STR_LUnUse                "�����������","��ʾ"
#define     STR_LNoOpen               "�����û�й���","��ʾ"
#define     STR_RUnUse                "�����������","��ʾ"
#define     STR_RNoOpen               "�����û�й���","��ʾ"
#define     STR_CamISOpen            "����Ѵ�"
#define     STR_CamISClose            "����ѹر�"

extern    CString  g_strMeasure[1] ;//������ʾ�ı�
extern    CString  g_strCalibLeft[5] ;
extern    CString  g_strCalibRight[5]  ;
extern    CString  g_strCalibDouble[5] ;
extern    CString  g_strCalibFinishLeft   ; 
extern    CString  g_strCalibFinishRight  ; //�������Ի���
extern    CString  g_strCalibFinishDouble ;
//��ǰ�궨��ʾ
extern    CString  g_strCurCalibTishi[10];
//�궨���������ʾ
extern    CString g_strEvaluate[5];
#else

#endif

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


