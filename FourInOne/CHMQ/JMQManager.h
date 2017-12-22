// JMQManager.h: interface for the CJMQManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JMQMANAGER_H__552C3098_4290_4455_975D_A65E29E08B9E__INCLUDED_)
#define AFX_JMQMANAGER_H__552C3098_4290_4455_975D_A65E29E08B9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "JMQCarz.h"
#include <map>
#include <list>
using namespace std;

typedef map<int,CJMQCarz> MAP_INT_JMQCar;
typedef map<CString,STerminalPZ> MAP_STR_VIDEOPZ;
typedef map<CString,ERRORDATA> MAP_STR_ERRORDATA;
//typedef list<JGPTLISTSTRU> LIST_JGPT;

typedef struct _XMCODE
{
	int xmCode;
	int ikch;
}XMcode;
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CJMQManager  
{
public:	
	int iQhKCH[100];
	void GetKCHArray(CStringArray& kcharr);//��ȡ���г�ʼ����ɵĿ�����
	void OnJGPTData(CString strmsg);
	void OnJ17C51(int ikch, CString zkzmbh);	
	void OnJ17C52(int ikch, CString zkzmbh,CString smsg);
	void OnJ17C53(int ikch, CString smsg,CString timestr);
	void OnJ17C55(int ikch, CString zkzmbh,CString smsg);
	void OnJ17C56(int ikch, CString zkzmbh,CString smsg);

	void OnQH17C52(int ikch,int ibh);
	//////////////////////////////////////////////////////////////////////////
	//���ݿ�
	_ConnectionPtr m_pConn;//���ݿ�
	BOOL OnInitSQL();//��ʼ�����ݿ�����
	BOOL SetTerminalPZ();
	BOOL SetErrorData();
	BOOL GetTerminalPZ(STerminalPZ &tpz,CString str);
	//////////////////////////////////////////////////////////////////////////
	void TermailDEInit();//��������ʼ��	
	void RunMap();//��ʼ��
	//////////////////////////////////////////////////////////////////////////
	BOOL OnFindCar(int ikch);//�����Ƿ��ʼ�����
public:
	BOOL LoadTFDll();
	void ReRunData(int ikch,int itype,CString zkzm,CString msg);
	void GetCS(CString str,int &ikscs,int &idrcs);
	void DelMSGList(int ikch,int itype);
	void AddMSGList(int ikch,int itype,CString zkzmbh,CString msgstr,int iruntime=5);
	HANDLE ListhEvent;
	void RunListMsgThread();
	static UINT ListMSGThread(LPVOID pParam);
//	LIST_JGPT m_ListJgpt;
	void OnGnssData(char *recvbuf,int ikch);
	CString GetErrorKFLX(CString str);
	CJMQManager();
	virtual ~CJMQManager();
private:	
	void CutOutString(CString source,  CStringArray& dest, char division);//�ַ�����ȡ
	int GetErrorKCFS(CString str);//��ȡ�۳�����
	bool OnCheckChan(UINT kch,int bncn);//ͨ����⼰��ʼ��
	bool OnDS63Init(int id);//�豸��ʼ��
private:
	UINT m_DisPlayShowi[5];
	MAP_INT_JMQCar m_JMQCar;//�����б�
	MAP_STR_ERRORDATA m_Errordata;//�۷�����
	MAP_STR_VIDEOPZ m_VideoPZ;//����ͷ����
	//////////////////////////////////////////////////////////////////////////
	NET_DVR_MATRIX_ABILITY_V41 m_Ability;//�豸������
	LONG m_lUserID;//��¼ID
	//////////////////////////////////////////////////////////////////////////
	CString configfile;//�����ļ�
	CString ModulePath;//��ǰĿ¼
	//��־
	DWORD AppLogID,dwSLogID,dwFLogID;//��־ID
	CWriteLog logz;//��־��
	UINT uKSKM;//���Կ�Ŀ��
	UINT uWNDTWO;//�����
	
	UINT uMergeVideo;
private:
	//////////////////////////////////////////////////////////////////////////
	typedef void (* TFInit)(int /*ikch*/,int /*wMSG*/,HWND /*hwndz*/);
	typedef void (* TFPassH)(LONG /*lPassHandle*/,int /*ikch*/,int /*itf*/);
	typedef void (* TF17C51)(int /*ikch*/,CString /*zkzm*/,int /*ikscs*/,int /*idrcs*/);
	typedef void (* TF17C52)(int /*ikch*/,CString /*zkzm*/, int /*ic*/, CString /*msgz*/);
	typedef void (* TF17C53)(int /*ikch*/,CString /*timestr*/, CString /*msgz*/, int /*ikcfs*/);
	typedef void (* TF17C54)(int /*ikch*/,LPVOID /*msgz*/);
	typedef void (* TF17C55)(int /*ikch*/,int /*ic*/, CString /*msgz*/);
	typedef void (* TF17C56)(int /*ikch*/,int /*itype*/, int /*ikscj*/);
	
	TFInit dllTFInit;
	TFPassH dTFPassH;
	TF17C51 dTF17C51;
	TF17C52 dTF17C52;
	TF17C53 dTF17C53;
	TF17C54 dTF17C54;
	TF17C55 dTF17C55;
	TF17C56 dTF17C56;
	HINSTANCE dllYuvToH264TH;
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_JMQMANAGER_H__552C3098_4290_4455_975D_A65E29E08B9E__INCLUDED_)
