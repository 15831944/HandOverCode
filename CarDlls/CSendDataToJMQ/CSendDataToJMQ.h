// CSendDataToJMQ.h : main header file for the CSENDDATATOJMQ DLL
//

#if !defined(AFX_CSENDDATATOJMQ_H__AAA8D3AE_11D2_4CF5_9F64_21D3196AD31A__INCLUDED_)
#define AFX_CSENDDATATOJMQ_H__AAA8D3AE_11D2_4CF5_9F64_21D3196AD31A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <mmsystem.h>
#pragma comment(lib,"Winmm.lib")

#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#define UM_JMQDATA WM_USER+8604
#define  PACKHAND  sizeof(PACKTYPE)+sizeof(int)

typedef enum PACKTYPE{ 
	SOCKZREEOR,
	GNSSDATA,
	JGPTDATA
};

typedef struct _gnssmsg
{
	double	gnssX;//����
	double	gnssY;//γ��
	float	gnssR;//�����
	float	gnssSD;//�ٶ�
	float	gnssLC;//���

	//�ź�
	int aqd;	//��ȫ��
	int js;		//��ɲ
	int ss;		//��ɲ
	int fs;		//��ɲ
	int lh;		//���
	int lb;		//����
	int zzx;	//��ת��
	int yzx;	//��ת��
	int ygd;	//Զ���
	int jgd;	//�����
	int jsd;	//��ʾ��
	int xh;		//Ϩ��
	int kgm;	//������
	int dw;		//��λ
	
}GNSSMSG;
/////////////////////////////////////////////////////////////////////////////
// CCSendDataToJMQApp
// See CSendDataToJMQ.cpp for the implementation of this class

class CCSendDataToJMQApp : public CWinApp
{
public:	
	int iGNssMap;
	//////////////////////////////////////////////////////////////////////////
	HINSTANCE dllHandleMAPTCP;
	typedef BOOL (* DLLMATSENDC)(DOUBLE,DOUBLE);
	DLLMATSENDC dllMapSendC;
	//////////////////////////////////////////////////////////////////////////
	int ignsssend;
	CStringList m_sendList;
	CString JMQIP;
	DWORD lSendTime;
	double oldX[3],oldY[3];
	float GetGnssFx(double x,double y);
	int GNSSXY;
	int GNSSR;
	int m_iKCH;
	double CalculateRotateAngle(double dBeginx,double dBeginy,double dNextx,double dNexty);
	void SendXMData(UINT type,LPCTSTR msgz);
	void SGnssData(PACKTYPE packtype, char *msg,int msglen);
	BOOL InitSocketz(HWND hwndz);
	SOCKET m_socketz;
	SOCKADDR_IN addrSrv;
	void WriteLog(LPCTSTR pstrFormat, ...);
	CCSendDataToJMQApp();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSendDataToJMQApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCSendDataToJMQApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString logfilename;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSENDDATATOJMQ_H__AAA8D3AE_11D2_4CF5_9F64_21D3196AD31A__INCLUDED_)
