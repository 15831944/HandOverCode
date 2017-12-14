// JMQCarz.cpp: implementation of the CJMQCarz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JMQCarz.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJMQCarz::CJMQCarz()
{
	m_iKCH	=-1;
	m_lUserID	=-1;

}

CJMQCarz::~CJMQCarz()
{
// 	if (m_TPic!=NULL)
// 	{
// 		delete m_TPic;
// 	}
// 	if (m_FPic!=NULL)
// 	{
// 		delete m_FPic;
// 	}
}

void CJMQCarz::InitCar(int iKch, LONG lUserID, BYTE (&deChan)[4],DWORD sid,DWORD fid,CString mPath)
{
	m_iKCH=iKch;
	m_lUserID=lUserID;
	m_deChannl[0]=deChan[0];
	m_deChannl[1]=deChan[1];
	m_deChannl[2]=deChan[2];
	m_deChannl[3]=deChan[3];
	//////////////////////////////////////////////////////////////////////////
	slogid=sid;
	flogid=fid;
	logz.WriteLog(slogid,"��ʼ������%d,����ͨ��=%d,%d,%d,%d",iKch,m_deChannl[0],m_deChannl[1],m_deChannl[2],m_deChannl[3]);
	//////////////////////////////////////////////////////////////////////////
}

bool CJMQCarz::StartDynamic(STerminalPZ tpz, int iwnd)
{
	NET_DVR_PU_STREAM_CFG_V41 dt;
	ZeroMemory(&dt, sizeof(dt));
	dt.dwSize = sizeof(NET_DVR_PU_STREAM_CFG_V41);
	dt.byStreamMode = 1;//ͨ��IP������ȡ��
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byChanType =0;//ͨ�����ͣ�0-��ͨͨ��,1-��ͨ��,2-��ID,3-��������Դ��4-������������ͨ��
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byChannel =0;//�ò�����Ч��ͨ���ż�dwChannel 
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byTransProtocol = 0;//����Э������0-TCP��1-UDP	
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byFactoryType=0;/*ǰ���豸��������,ͨ���ӿڻ�ȡ*/
	
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byTransMode =tpz.imllx;//��������ģʽ 0�������� 1��������    
	sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.byAddress, "%s", tpz.TerminalIP);//�豸����
	sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.sUserName, "%s", tpz.RasUser);//��½�ʺ�
	sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.sPassword, "%s", tpz.RasPassword);//����	
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.wDVRPort = tpz.TerminalPort;//�˿ں�
	dt.uDecStreamMode.struDecStreamDev.struDevChanInfo.dwChannel = tpz.dwChannel;//ͨ����
	
	if (strlen(tpz.mediaIP)!=0)//��ý��IP��Ϊ��
	{
		dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.byValid=1;//������ý��
		dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.wDevPort=554;//�˿�
		dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.byTransmitType=0;//TCP
		sprintf((char *)dt.uDecStreamMode.struDecStreamDev.struStreamMediaSvrCfg.byAddress, "%s", tpz.mediaIP);//��ý��IP
	}
	if (!NET_DVR_MatrixStartDynamic_V41(m_lUserID,m_deChannl[iwnd], &dt))//����ͨ�� 0-15
	{		
		logz.WriteLog(flogid,"��̬����ʧ��! NET_DVR_MatrixStartDynamic_V41 Eid=%d,ecode:[%s],����%d",NET_DVR_GetLastError(),tpz.TermainlBH,m_iKCH);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}
	logz.WriteLog(slogid,"������̬����!����%d,ecode=%s,�Ӵ���=%d,mediaIP=%s,����%d(0������,1������),%s_%d",\
		m_iKCH,tpz.TermainlBH,iwnd,tpz.mediaIP,tpz.imllx,tpz.TerminalIP,tpz.dwChannel);
	return true;
}

bool CJMQCarz::StartPassive(int iwnd,LONG &lph)
{
	lph=-1;
	NET_DVR_MATRIX_PASSIVEMODE m_PassiveMode;//��������
	m_PassiveMode.wTransProtol=0;//wTransProtol 	����Э�飺0-TCP��1-UDP��2-MCAST 
	m_PassiveMode.wPassivePort = 8000;//TCP����UDP�˿ڣ�TCPʱ�˿�Ĭ����8000����ͬ�Ľ���ͨ��UDP�˿ں�������Ϊ��ͬ��ֵ
	m_PassiveMode.byStreamType =2;//��������: 1-ʵʱ��, 2-�ļ���
	LONG lPassiveHandle=NET_DVR_MatrixStartPassiveDecode(m_lUserID,m_deChannl[iwnd],&m_PassiveMode);
	if (lPassiveHandle < 0)
	{	
		logz.WriteLog(flogid,"������������ʧ��!NET_DVR_MatrixStartPassiveDecode! errid=%d,����ͨ��=%d,�Ӵ���=%d,����%d",NET_DVR_GetLastError(),m_deChannl[iwnd],iwnd,m_iKCH);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}
	lph=lPassiveHandle;
	logz.WriteLog(slogid,"������������!����%d,ͨ����:%d,�Ӵ���:%d,lPassiveHandle:%ld",m_iKCH,m_deChannl[iwnd],iwnd,lPassiveHandle);	
	return true;
}
//NET_DVR_MatrixStopDynamic ֹͣ��̬����
void CJMQCarz::StopDynamic(int iwnd)
{
	if(NET_DVR_MatrixStopDynamic(m_lUserID, m_deChannl[iwnd]))
	{
		logz.WriteLog(slogid,"ֹͣ��̬����ɹ�!����%d,�Ӵ���:%d",m_iKCH,m_deChannl[iwnd]);
	}
	else
	{
		logz.WriteLog(flogid,"ֹͣ��̬����ʧ��!����%d,�Ӵ���:%d,errorid=%d",m_iKCH,m_deChannl[iwnd],NET_DVR_GetLastError());
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
	}
}

bool CJMQCarz::StopPassive(int iwnd)
{
	return true;
}

void CJMQCarz::On17C51(CString zkzm, int idrcs, int ikscs)
{

}

void CJMQCarz::On17C52(CString zkzm, int ic, CString msgz)
{

}

void CJMQCarz::On17C53(CString timestr, CString msgz, int ikcfs)
{

}

void CJMQCarz::On17C55(int ic, CString msgz)
{

}

void CJMQCarz::On17C56(int itype, int ikscj)
{

}

void CJMQCarz::OnGnssData(LPVOID msgz)
{

}
