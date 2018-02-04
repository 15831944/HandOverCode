// BKJieMaz.h: interface for the CBKJieMaz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BKJIEMAZ_H__A6D43A05_B185_465B_BF25_93BA1752F9B5__INCLUDED_)
#define AFX_BKJIEMAZ_H__A6D43A05_B185_465B_BF25_93BA1752F9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//////////////////////////////////////////////////////////////////////////
#include "DataType.h"
#include "HikVisionSdk.h"
#pragma comment(lib,"DS40xxSDK.lib")

#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")
//////////////////////////////////////////////////////////////////////////
#define MAX_DISCHANNEL 16
#define MAX_CHANNELS 100
#define MAX_OSD_BYTES 88
//#define CP_GB2312		936

typedef struct STRU_DECODE_CHAN 
{
	BOOL		isCard;//�Ƿ��ǳ�����Ƶ
	BOOL		bDecPlay;//�Ƿ��ڽ��벥��
	HANDLE		hChannelHandle;//����ͨ�����
	int m_Num;
	BOOL	bRealPlay;//
	LONG	lPlayHandle;//
	STRU_DECODE_CHAN()
	{
		m_Num=0;
		bDecPlay=FALSE;
		bRealPlay=FALSE;
	}
	DISPLAY_PARA dsPara;//��Ƶ��ʾ���� 2014-06-12
}DECODE_CHAN;

typedef struct tagUSER_CHANNEL_INFO
{
	LONG lDvrUserID;						//DVR�û�ID����NET_DVR_Login�ķ���ֵ
	LONG lRealPlay;							//���ž������NET_DVR_RealPlay�ķ���ֵ
	LONG lChannel; 							//ͨ���ţ�1-32Ϊģ��ͨ����33-64ΪIPͨ��
	LONG lLinkMode;							//���λ(31)Ϊ0��ʾ��������Ϊ1��ʾ��������0��30λ��ʾ�������ӷ�ʽ��0-TCP,1-UDP,2-MCAST,3-RTP,4-����Ƶ�ֿ�

}USER_CHANNEL_INFO, *PUSER_CHANNEL_INFO;

typedef struct tagUSER_SERVER_INFO
{
	LONG lDvrUserID;						//DVR�û�ID����NET_DVR_Login�ķ���ֵ
	CHAR sDvrIP[16];						//DVR��IP��ַ
	CHAR sDvrUserName[16];					//DVR�û���
	CHAR sDvrPassword[16];					//DVR������
	DWORD dwDvrPort;						//DVR�˿ں�
	BYTE byChanNum;				    //ģ��ͨ������
    BYTE byStartChan;			        //��ʼͨ����,����DVS-1,DVR - 1
}USER_SERVER_INFO;

class CBKJieMaz  
{
public:
	CBKJieMaz();
	virtual ~CBKJieMaz();
public:
	UINT OsdSD[MAX_DISCHANNEL][91];//�ٶ�
	UINT OsdSC[MAX_DISCHANNEL][91];//ʱ��
	BOOL DvrSetDisPOSD(UINT disChannel,CString mStrs,UINT line);//��ʾͨ�������ַ�
	BOOL TransMultiByteToWideByte(UINT *pFormat, UINT *pFormatOut);
	//////////////////////////////////////////////////////////////////////////
	void DvrPlayStopXM(UINT Channel);
	void F_DVR_Logoutz();
	BOOL DvrPlayChannel(LONG lChannel,CString dIP,UINT jiechannel,BOOL cz=FALSE);
	DWORD F_Login(LPTSTR lpIP, DWORD dwPort, LPTSTR lpUserName, LPTSTR lpUserPassword);
	void SetDispChanLogo(UINT nChannel,BOOL m_bDisLogoEnable=TRUE);
	BOOL SetDisRGN();
	void ChannelClosez();
	BOOL InitDsp(HWND hParent);
	UINT		g_nTotalDecChannels/* = 0*/;			 //����ͨ����
	UINT		g_nTotalDspCnt/* = 0*/;
	UINT		g_nTotalDispChannels/* = 0*/;			 //��ʾͨ����
	UINT		g_nDevCount/* = 0*/; //��¼�豸����
};

#endif // !defined(AFX_BKJIEMAZ_H__A6D43A05_B185_465B_BF25_93BA1752F9B5__INCLUDED_)
