// BKPlayWnd.cpp: implementation of the CBKPlayWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "play.h"
#include "BKPlayWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBKPlayWnd::CBKPlayWnd()
{
	ISPlaywndd	=FALSE;
	m_bIsRecording = FALSE;
	dwlogid=logz.Register("CBKPlayWnd.log");
}

CBKPlayWnd::~CBKPlayWnd()
{
	DvrLoginout();
}

BOOL CBKPlayWnd::DvrInitLogin(HWND hWnd, char *sDVRIP, char *sUserName, char *sPassword, WORD wDVRPort)
{
	NET_DVR_Init();// ��ʼ��
// 	//��������ʱ��������ʱ��
// 	NET_DVR_SetConnectTime(2000, 1);
 	NET_DVR_SetReconnect(10000, true);
	
	m_lUserID = NET_DVR_Login_V30(sDVRIP,wDVRPort,sUserName,sPassword, &struDeviceInfo);
	m_zhwnd =hWnd;
	if (  m_lUserID == -1 )
	{
		GetLastErrorz();
		NET_DVR_Cleanup();
		return FALSE;
	}
// 	//�����쳣��Ϣ�ص�����
// 	NET_DVR_SetExceptionCallBack_V30(0, NULL,g_ExceptionCallBack, NULL);
	logz.WriteLog(dwlogid,"��¼�ɹ�!%s,%d",sDVRIP,wDVRPort);
	return TRUE;
}
BOOL CBKPlayWnd::DvrLoginout()
{
	//�ر�Ԥ��
	if (ISPlaywndd)
	{
		if(m_bIsRecording)  //����¼����ֹͣ
		{
			DvrStopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle);
		NET_DVR_Logout(m_lUserID);
		NET_DVR_Cleanup();
		ISPlaywndd	=FALSE;
	}
	return TRUE;
}
BOOL CBKPlayWnd::DvrRealPlay(LONG lChannel)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = m_zhwnd;
	ClientInfo.lChannel     = lChannel;
	ClientInfo.lLinkMode    = 0;//0x80000000;//������ TCP
    ClientInfo.sMultiCastIP = NULL;

	m_lPlayHandle= NET_DVR_RealPlay_V30(m_lUserID,&ClientInfo,NULL,NULL,TRUE);
	if(-1 == m_lPlayHandle)
	{
		GetLastErrorz();
		logz.WriteLog(dwlogid,"NET_DVR_RealPlay_V30 faild!NET_DVR_GetLastError=[%d],lChannel=%d",NET_DVR_GetLastError(),lChannel);
		return FALSE;
	}
	return TRUE;
}

void CBKPlayWnd::Drawztext(CString zstr)
{
	HDC hdc=GetDC(m_zhwnd);
	CRect rc;
	GetClientRect(m_zhwnd,&rc);
	DrawText(hdc,zstr,strlen(zstr),&rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

void CBKPlayWnd::GetLastErrorz()
{
	switch (NET_DVR_GetLastError())
	{
	case NET_DVR_PASSWORD_ERROR://1
		Drawztext("�û����������,��鿴�����ļ���!");
		break;
	case NET_DVR_NOINIT://3
		Drawztext("SDKδ��ʼ��!");
		break;
	case NET_DVR_OVER_MAXLINK://5
		Drawztext("���ӵ��豸���û������������!");
		break;
	case NET_DVR_NETWORK_FAIL_CONNECT://7
		Drawztext("�����豸ʧ��.�豸�����߻�����ԭ����������ӳ�ʱ!");
		break;
	case NET_DVR_NETWORK_SEND_ERROR://8
		Drawztext("���豸����ʧ��!");
		break;
	case NET_DVR_MAX_USERNUM://52
		Drawztext("��¼�豸���û����ﵽ���!");
		break;
	case NET_DVR_BINDSOCKET_ERROR://72
		Drawztext("���׽���ʧ��!");
		break;
	case NET_DVR_SOCKETCLOSE_ERROR://73
		Drawztext("socket�����ж�,�˴���ͨ�������������жϻ�Ŀ�ĵز��ɴ�!");
		break;
	case NET_DVR_IPCHAN_NOTALIVE://83
		Drawztext("Ԥ��ʱ���IPͨ��������!");
		break;
	case NET_DVR_RTSP_SDK_ERROR://84
		Drawztext("���ر�׼Э��ͨѶ��StreamTransClientʧ��!");
		break;
	case NET_DVR_CONVERT_SDK_ERROR://85
		Drawztext("����ת��װ��ʧ��!");
		break;
	default:
		Drawztext("ʵʱԤ�����ų���!");
		break;
	}
}

BOOL CBKPlayWnd::DvrStartRecord(CString str)
{
	if(m_lPlayHandle == -1)
	{
		return FALSE;
	}
	if(m_bIsRecording)
	{
		DvrStopRecord();
	}
	char RecName[256] = {0};
	sprintf(RecName,"%s.mp4",str);
	if(!NET_DVR_SaveRealData(m_lPlayHandle,RecName))
	{
		return FALSE;
	}
	m_bIsRecording = TRUE;
	return TRUE;
}

BOOL CBKPlayWnd::DvrStopRecord()
{
	if(!NET_DVR_StopSaveRealData(m_lPlayHandle))
	{
		return FALSE;
	}
	m_bIsRecording = FALSE;
	return TRUE;
}
//����Ƶ1����
BOOL CBKPlayWnd::OpenVoice()
{
	if (!m_VoiceState)
	{
		if ( !NET_DVR_OpenSound( m_lPlayHandle ) )
		{
			m_VoiceState = FALSE;
			CString str;
			str.Format("������ʧ��!!![ M_Handle=%ld,Errid=%d ]",m_lPlayHandle,NET_DVR_GetLastError());
			AfxMessageBox(str);		
		}
		else
		{
			m_VoiceState = TRUE;			
			return TRUE;
		}
	}
	else
	{
		if( !NET_DVR_CloseSound() )
		{			
			CString str;
			str.Format("�ر�����ʧ��!!![ M_Handle=%ld,Errid=%d ]",m_lPlayHandle,NET_DVR_GetLastError());
			if (NET_DVR_GetLastError() == 51)
			{
				m_VoiceState = FALSE;
			}
			else
			{
				AfxMessageBox(str);
			}
		}
		else
		{
			m_VoiceState = FALSE;
			return FALSE;
		}
	}
	return FALSE;
}

//�ر�����
BOOL CBKPlayWnd::CloseVoice()
{
	m_VoiceState = FALSE;
	return FALSE;
}

//�����Խ�
BOOL CBKPlayWnd::VoiceIntercom()
{
	if (m_bVoiceCom)
	{
		if ( !NET_DVR_StopVoiceCom(m_lVocieComHandle) )
		{			
			CString str;
			str.Format("ֹͣ�Խ�ʧ��!!![ UserID=%ld,StartDTalkChan=%ld,m_lChannel=%ld,Voice_Handle=%ld,Errid=%d ]",m_lUserID,m_byStartDTalkChan,m_lChannel,m_lVocieComHandle,NET_DVR_GetLastError());
			AfxMessageBox(str);
		}
		m_bVoiceCom=false;	
		return FALSE;
	}
	else
	{	
		m_lVocieComHandle = NET_DVR_StartVoiceCom_V30(m_lUserID,m_byStartDTalkChan+m_lChannel,1,NULL,NULL);
		if ( m_lVocieComHandle == -1 )
		{
			CString str;
			str.Format("�򿪶Խ�ʧ��!!![ UserID=%ld,StartDTalkChan=%ld,m_lChannel=%ld,Voice_Handle=%ld,Errid=%d ]",m_lUserID,m_byStartDTalkChan,m_lChannel,m_lVocieComHandle,NET_DVR_GetLastError());
			AfxMessageBox(str);			
			return FALSE;
		}
		else
		{		
			m_bVoiceCom=true;
			return TRUE;
		}			
	}
	return FALSE;
}

//�ر�������־λ
BOOL CBKPlayWnd::CloseVoiceCom()
{
	m_bVoiceCom = FALSE;
	return FALSE;
}
