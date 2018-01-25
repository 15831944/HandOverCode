// JMQManager.cpp: implementation of the CJMQManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CJMQ.h"
#include "JMQManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJMQManager::CJMQManager()
{
	AppLogID=logz.Register("������������.log");
	dwSLogID=logz.Register("HMQSuccess.log");
	dwFLogID=logz.Register("HMQError.log");
	CString tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	ModulePath.Format("%s",tempPath.Left(tempPath.ReverseFind('\\')));
	configfile.Format("%s\\config.ini",ModulePath);//��ǰĿ¼��
	NET_DVR_Init();//����SDK��ʼ��
	
	uMergeVideo = 0;
	m_bBigCar = false;
	m_bEncrypt = false;

	UINT uBigCar = GetPrivateProfileInt("CONFIG","BigCar",0,configfile);	//�󳵿�Ŀ���ϸ����Ϊ90
	if (1 == uBigCar)
	{
		m_bBigCar = true;
	}

	UINT uEncrypt = GetPrivateProfileInt("CONFIG","ENCRYPT",0,configfile);
	if (1 == uEncrypt)
	{
		m_bEncrypt = true;
	}
}

CJMQManager::~CJMQManager()
{
	NET_DVR_Cleanup();//�ͷ�
	if (dllYuvToH264TH !=NULL)
	{
		FreeLibrary(dllYuvToH264TH);
	}
}
//////////////////////////////////////////////////////////////////////////

void CJMQManager::TermailDEInit()
{	
	LoadTFDll();//��̬����DLL
	DWORD dwVersion = NET_DVR_GetSDKBuildVersion();
	int jmqnum=GetPrivateProfileInt("JMQ","NUM",0,configfile);//��ȡ����������
	int EVEN=GetPrivateProfileInt("JMQ","EVEN",0,configfile);//��ȡ�Ƿ���к���
	uKSKM=GetPrivateProfileInt("CONFIG","KSKM",2,configfile);//���Կ�Ŀ
	uWNDTWO=GetPrivateProfileInt("CONFIG","WND2",0,configfile);//�����״̬ 0
	logz.WriteLog(AppLogID,"��ȡ��%d̨������!EVEN=[%d],��Ŀ%d,����2=%d,HCNetSDK V%d.%d.%d.%d",jmqnum,EVEN,uKSKM,uWNDTWO,\
		(0xff000000 & dwVersion)>>24,(0x00ff0000 & dwVersion)>>16,(0x0000ff00 & dwVersion)>>8, (0x000000ff & dwVersion));
	CString lpAppName,lpKeyName;
	UINT ukch=0;
	m_DisPlayShowi[4]=GetPrivateProfileInt("CONFIG","VIDEOWND",1,configfile);
	for (int i=0;i<4;i++)
	{
		lpKeyName.Format("DISPLAY%d",i+1);
		m_DisPlayShowi[i]=GetPrivateProfileInt("CONFIG",lpKeyName,i,configfile);
		ukch+=m_DisPlayShowi[i];
	}
	if (ukch!=6)
	{
		for (int i=0;i<4;i++)
		{
			lpKeyName.Format("DISPLAY%d",i+1);
			lpAppName.Format("%d",i);
			::WritePrivateProfileString("CONFIG",lpKeyName,lpAppName,configfile);
			m_DisPlayShowi[i]=i;
		}
	}
	logz.WriteLog(AppLogID,"�Ӵ�������[%d,%d,%d,%d],��Ƶ����[%d]",m_DisPlayShowi[0],m_DisPlayShowi[1],m_DisPlayShowi[2],\
		m_DisPlayShowi[3],m_DisPlayShowi[4]);
	for (i=1;i<=jmqnum;i++)
	{
		if (OnDS63Init(i)==true)//�豸��ʼ���ɹ�
		{
			lpAppName.Format("JMQ%d",i);
			for (int j=0;j<m_Ability.struDviInfo.byChanNums;j++)//DVI����ѭ��
			{
				if (EVEN==1 && j%2==1)
				{
					continue;
				}
				lpKeyName.Format("BNC%d",j+1);//��1��ʼ
				ukch=GetPrivateProfileInt(lpAppName,lpKeyName,0,configfile);
				if (0 == ukch)//û������
				{
					logz.WriteLog(AppLogID,"������[JMQ%d]��BNCͨ��[%s]���ڿ���,��������!",i,lpKeyName);
				}
				else
				{
					OnCheckChan(ukch,j);
				}
			}
		}
		else
		{
			logz.WriteLog(AppLogID,"������[JMQ%d]δ��ʼ���ɹ�!",i);
		}
	}
}

bool CJMQManager::OnDS63Init(int id)
{
	CString temp,temp1;
	temp1.Format("%d",id);
	GetPrivateProfileString("JMQ",temp1,"",temp.GetBuffer(MAX_PATH),MAX_PATH,configfile);
	temp.ReleaseBuffer();
	int pos=temp.Replace(",",",");
	if (pos !=3)
	{
		logz.WriteLog(dwFLogID,"OnDS63Init:��ȡ���ó���%d,[%s]",id,temp);
		return false;
	}
	CString sIPaddr,sUser,sPwd,sPort;
	pos =temp.Find(",");
	sIPaddr =temp.Left(pos);
	temp1=temp.Mid(pos+1);
	pos =temp1.Find(",");
	sUser=temp1.Left(pos);
	temp=temp1.Mid(pos+1);
	pos =temp.Find(",");
	sPwd =temp.Left(pos);
	sPort=temp.Mid(pos+1);
	//////////////////////////////////////////////////////////////////////////
	m_lUserID=-1;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//����豸�����Ľṹ��
	m_lUserID = NET_DVR_Login_V30(sIPaddr.GetBuffer(0),atoi(sPort),sUser.GetBuffer(0),sPwd.GetBuffer(0), &struDeviceInfo);
	if (m_lUserID < 0)
	{
		logz.WriteLog(dwFLogID,"��¼ʧ��! NET_DVR_Login_V30 failed! errid=%d,IP=%s,USER=%s,PWD=%s,PORT=%s",\
		NET_DVR_GetLastError(),sIPaddr,sUser,sPwd,sPort);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	char cTemp[SERIALNO_LEN] = {0};
	ZeroMemory(cTemp, SERIALNO_LEN);
	memcpy(cTemp, struDeviceInfo.sSerialNumber, SERIALNO_LEN);
	memset(&m_Ability, 0, sizeof(NET_DVR_MATRIX_ABILITY_V41));//��ȡ�豸������
    if (!NET_DVR_GetDeviceAbility(m_lUserID, MATRIXDECODER_ABILITY_V41, NULL, 0, (char*)&m_Ability, sizeof(NET_DVR_MATRIX_ABILITY_V41)))
    {
		logz.WriteLog(dwFLogID,"��ȡ�豸������ʧ��! JMQ NET_DVR_GetDeviceAbility failed! errid=%d,TID=%d",NET_DVR_GetLastError(),id);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
	}	
	logz.WriteLog(AppLogID,"������=[%d],IP=%s,[%s],DecN=%d,BncN=%d",id,sIPaddr,cTemp,m_Ability.byDecChanNums,m_Ability.struBncInfo.byChanNums);
	//////////////////////////////////////////////////////////////////////////
	//��ȡ���� �����豸����  �������ܽ���ܶ�����
	NET_DVR_AUTO_REBOOT_CFG m_struRebootTime;
	memset(&m_struRebootTime, 0, sizeof(m_struRebootTime));
	DWORD dwReturned = 0;
	if (!NET_DVR_GetDVRConfig(m_lUserID, NET_DVR_GET_AUTO_REBOOT_CFG, 0, &m_struRebootTime, sizeof(m_struRebootTime), &dwReturned))
	{
		logz.WriteLog(dwFLogID,"��ȡ�Զ���������ʧ��!JMQ NET_DVR_GET_AUTO_REBOOT_CFG failed!errid=%d,TID=%d",NET_DVR_GetLastError(),id);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
	}
	else
	{
		if (m_struRebootTime.struRebootTime.byDate!=7)
		{
			m_struRebootTime.dwSize = sizeof(NET_DVR_AUTO_REBOOT_CFG);
			m_struRebootTime.struRebootTime.byDate = 7;
			m_struRebootTime.struRebootTime.byHour = 5;
			m_struRebootTime.struRebootTime.byMinute = 9;
			if (!NET_DVR_SetDVRConfig(m_lUserID, NET_DVR_SET_AUTO_REBOOT_CFG, 0, &m_struRebootTime, sizeof(m_struRebootTime)))
			{
				logz.WriteLog(dwFLogID,"�����Զ���������ʧ��!JMQ NET_DVR_GET_AUTO_REBOOT_CFG failed!errid=%d,TID=%d",NET_DVR_GetLastError(),id);
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	return true;
}

bool CJMQManager::OnCheckChan(UINT kch,int bncn)//������ BNC���
{
	DWORD dechanisopen;
	BYTE byDecChan[4];//����ͨ��
	DWORD dwDecChan;
	//����ͨ�����
	//////////////////////////////////////////////////////////////////////////
	for (int i=0; i<4;i++)
	{
		byDecChan[i]=m_Ability.byStartChan+bncn*4 + i;
		dwDecChan=byDecChan[i];
		if (NET_DVR_MatrixGetDecChanEnable(m_lUserID,dwDecChan/*byDecChan[i]*/,&dechanisopen))
		{
			if (0==dechanisopen)//ָ��DWORD��ָ�룬ȡ����ֵ0��ʾ�ر� 
			{
				if (!NET_DVR_MatrixSetDecChanEnable(m_lUserID,byDecChan[i],1))//��ͨ��
				{
					logz.WriteLog(dwFLogID,"NET_DVR_MatrixSetDecChanEnable failed!,errid=%d,��ͨ��%dʧ��!",NET_DVR_GetLastError(),byDecChan[i]);
					::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
					return false;
				}
			}
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	//��ʾͨ�����
	DWORD dwDispChan;
	NET_DVR_MATRIX_VOUTCFG m_DispChanCfg;
	memset(&m_DispChanCfg, 0, sizeof(NET_DVR_MATRIX_VOUTCFG));	
	dwDispChan=m_Ability.struDviInfo.byStartChan+bncn;//��ʾͨ��
	//��������ȡ��ʾͨ������
	if (!NET_DVR_MatrixGetDisplayCfg_V41(m_lUserID, dwDispChan, &m_DispChanCfg))
	{
		logz.WriteLog(dwFLogID,"��ȡ��ʾͨ������ʧ��! NET_DVR_MatrixGetDisplayCfg_V41 failed! errid=%d,channel=%d",NET_DVR_GetLastError(),dwDispChan);
		::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
		return false;
    }
	if ((4!=m_DispChanCfg.dwWindowMode) || \
		(m_DispChanCfg.byJoinDecChan[0] != byDecChan[m_DisPlayShowi[0]]) || \
		(m_DispChanCfg.byJoinDecChan[1] != byDecChan[m_DisPlayShowi[1]]) || \
		(m_DispChanCfg.byJoinDecChan[2] != byDecChan[m_DisPlayShowi[2]]) || \
		(m_DispChanCfg.byJoinDecChan[3] != byDecChan[m_DisPlayShowi[3]]) )//��ʾͨ�������ķָ����
	{
		m_DispChanCfg.byJoinDecChan[0]=byDecChan[m_DisPlayShowi[0]];
		m_DispChanCfg.byJoinDecChan[1]=byDecChan[m_DisPlayShowi[1]];
		m_DispChanCfg.byJoinDecChan[2]=byDecChan[m_DisPlayShowi[2]];		
		m_DispChanCfg.byJoinDecChan[3]=byDecChan[m_DisPlayShowi[3]];
		m_DispChanCfg.byAudio=1;//������Ƶ
		m_DispChanCfg.byAudioWindowIdx= m_DisPlayShowi[4];//��Ƶ�Ӵ��� 1
		m_DispChanCfg.byVedioFormat =1;//��Ƶ��ʽ��1- NTSC��2- PAL
		m_DispChanCfg.dwResolution= 67207228;
		m_DispChanCfg.dwWindowMode= 4;
		m_DispChanCfg.byScale= 0;//��ʵ
		//������������ʾͨ������ 
		if (!NET_DVR_MatrixSetDisplayCfg_V41(m_lUserID,dwDispChan,&m_DispChanCfg)) 
        {
			if (NET_DVR_GetLastError()==29)
			{
				logz.WriteLog(dwFLogID,"����29:�豸����ʧ��,�뽫��������ȫ�ָ���.�豸����=>�ָ�Ĭ�ϲ���=>��ɻָ�");
			}
			logz.WriteLog(dwFLogID,"NET_DVR_MatrixSetDisplayCfg_V41 failed! errid=%d,DisPlayChann=%d",NET_DVR_GetLastError(),dwDispChan);
			::PostMessage(AfxGetMainWnd()->m_hWnd,UM_HKLASTERROR,NET_DVR_GetLastError(),0);
			return false;		
		}
	}
	//////////////////////////////////////////////////////////////////////////
	m_JMQCar[kch].InitCar(kch,m_lUserID,byDecChan,dwSLogID,dwFLogID,ModulePath);//������ʼ��
	if (kch>0 && kch<100)
	{
		dllTFInit(kch,UM_JGPTDATA,AfxGetMainWnd()->m_hWnd);
	}
	return true;
}

BOOL CJMQManager::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize��NULL��δ��ʼ��
	{
		TRACE("��ʼ�����Ӷ���ʧ��\n");
		logz.WriteLog(AppLogID,"��ʼ�����Ӷ���ʧ��!CoInitialize NULL");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,configfile);
	m_strInEdit.ReleaseBuffer();
	if (!m_strInEdit.IsEmpty())
	{
		int iLen =m_strInEdit.GetLength();
		
		for(int i=0;i<iLen;i++)
		{
			m_strInEdit.SetAt(i,m_strInEdit[i]-1);
		}
		m_strInEdit.MakeReverse();
	}
	//////////////////////////////////////////////////////////////////////////
	try
	{
		CString Sqlstr;
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("�����ݿ�ɹ�!");
			logz.WriteLog(AppLogID,"�ɹ����ӵ����ݿ�!");
		}
		else
		{
			TRACE("�����ݿ�ʧ��!");
			logz.WriteLog(AppLogID,"�������ݿ�ʧ��!");
			return FALSE;
		}
	}
	catch(_com_error e)		//��׽�쳣
	{
		CString errormessage;
		errormessage.Format("���ݿ���ʾ,�������ݿ�ʧ��!,������Ϣ:%s,�����������ݿ�����",e.ErrorMessage());
		AfxMessageBox(errormessage);
		errormessage.Format("%s\\sqlLink.exe",ModulePath);
		ShellExecute(NULL,"open",errormessage,"config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

BOOL CJMQManager::SetTerminalPZ()
{
	CString sqltemp,strNid;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;
		int sqlororacle=GetPrivateProfileInt("CONFIG","SQLORACLE",1,configfile);//SQL ���� Oracle
		if(sqlororacle==0)
		{
			sqltemp.Format("SELECT ���,�豸IP,�û���,����,�˿ں�,ͨ����,TransMode,MediaIP,���||'_'||Nid as N FROM TBKVideo order by ���");//
			logz.WriteLog(AppLogID,"Oracle �汾");
		}
		else
		{
			sqltemp.Format("SELECT ���,�豸IP,�û���,����,�˿ں�,ͨ����,TransMode,MediaIP,���+'_'+Nid as N FROM TBKVideo order by ���");//
			logz.WriteLog(AppLogID,"SQL �汾");
		}

		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("N");
				if (vat.vt != VT_NULL)
				{
					strNid=(LPCSTR)_bstr_t(vat);
					vat =pSet->GetCollect("���");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].TermainlBH,(LPCSTR)_bstr_t(vat),10);
					}
					vat =pSet->GetCollect("�豸IP");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].TerminalIP,(LPCSTR)_bstr_t(vat),16);
					}
					vat =pSet->GetCollect("�û���");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].RasUser,(LPCSTR)_bstr_t(vat),16);
					}
					vat =pSet->GetCollect("����");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].RasPassword,(LPCSTR)_bstr_t(vat),16);
					}
					vat =pSet->GetCollect("�˿ں�");
					if (vat.vt != VT_NULL)
					{
						m_VideoPZ[strNid].TerminalPort=atoi((LPCSTR)_bstr_t(vat));
					}
					vat =pSet->GetCollect("ͨ����");
					if (vat.vt != VT_NULL)
					{
						m_VideoPZ[strNid].dwChannel=atoi((LPCSTR)_bstr_t(vat));
					}
					vat =pSet->GetCollect("TransMode");
					if (vat.vt != VT_NULL)
					{
						m_VideoPZ[strNid].imllx=atoi((LPCSTR)_bstr_t(vat));
					}
					vat =pSet->GetCollect("MediaIP");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_VideoPZ[strNid].mediaIP,(LPCSTR)_bstr_t(vat),16);
					}
				}
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		logz.WriteLog(AppLogID,"��ѯӲ��¼���ͨ��������Ϣʧ��!FUNC SetTerminalPZ,%s",comError.Description());
		return FALSE;
	}
	logz.WriteLog(AppLogID,"��ȡ���ݿ�����FUNC SetTerminalPZ ��%d��",m_VideoPZ.size());
	return TRUE;
}

BOOL CJMQManager::SetErrorData()
{
	CString sqltemp,strNid;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;

		if (m_bEncrypt)
		{
			sqltemp.Format("Set ARITHABORT ON;select ������,dbo.charDecode(�۷�����) as �۷�����,dbo.intDecode(�۳�����) as �۳����� from ErrorData");
		}
		else
		{
			sqltemp.Format("select ������,�۷�����,�۳����� from ErrorData");
		}

		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("������");
				if (vat.vt != VT_NULL)
				{
					strNid=(LPCSTR)_bstr_t(vat);
					vat =pSet->GetCollect("�۷�����");
					if (vat.vt != VT_NULL)
					{
						strncpy(m_Errordata[strNid].errorlx,(LPCSTR)_bstr_t(vat),30);
					}
					vat =pSet->GetCollect("�۳�����");
					if (vat.vt != VT_NULL)
					{
						m_Errordata[strNid].ikcfs=atoi((LPCSTR)_bstr_t(vat));
					}
				}
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		logz.WriteLog(AppLogID,"��ѯ��ErrorData��Ϣʧ��!FUNC SetErrorData,%s",comError.Description());
		return FALSE;
	}
	logz.WriteLog(AppLogID,"��ȡ���ݿ�����FUNC SetErrorData ��%d��",m_Errordata.size());
	return TRUE;
}

BOOL CJMQManager::OnFindCar(int ikch)
{
	MAP_INT_JMQCar::iterator iter;	
	iter = m_JMQCar.find(ikch);	
	if(iter == m_JMQCar.end())
	{
		logz.WriteLog(dwFLogID,"OnFindCar not find car [%d]",ikch);
		return FALSE;
	}
	return TRUE;
}

void CJMQManager::RunMap()
{
	MAP_INT_JMQCar::iterator iter;
	CString temp;
	STerminalPZ temppz;
	LONG lph;
	uMergeVideo=GetPrivateProfileInt("CONFIG","MergeVideo",0,configfile);//�Ƿ�ϳ���Ƶ
	
	for (iter=m_JMQCar.begin();iter!=m_JMQCar.end();iter++)
	{
		Sleep(10);
		temp.Format("����%d_1",iter->first);	
		if(GetTerminalPZ(temppz,temp)==TRUE)
		{
			iter->second.StartDynamic(temppz,0);//������Ƶ��̬
		}	
		
		if (0 == uMergeVideo)
		{
			temp.Format("����%d_2",iter->first);	//������Ƶ	
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				iter->second.StartDynamic(temppz,1);
			}
		}
		else
		{
			temp.Format("����%d_10",iter->first);		//���Ӻ����Ƶ
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				iter->second.StartDynamic(temppz,1);
			}	
		}


// 		temp.Format("����%d_3",iter->first);	
// 		if(GetTerminalPZ(temppz,temp)==TRUE)
// 		{
// 			iter->second.StartDynamic(temppz,2);//������Ƶ��̬
// 		}	
		if(iter->second.StartPassive(2,lph)==true)
		{
			dTFPassH(lph,iter->first,3);
		}
		if(iter->second.StartPassive(3,lph)==true)
		{
			dTFPassH(lph,iter->first,4);
		}
	}
}

BOOL CJMQManager::GetTerminalPZ(STerminalPZ &tpz,CString str)
{
	MAP_STR_VIDEOPZ::iterator iter;	
	iter = m_VideoPZ.find(str);	
	if(iter == m_VideoPZ.end())
	{
		logz.WriteLog(AppLogID,"����:[%s]����δ����,��������! GetTerminalPZ faild !",str);
		return FALSE;
	}
	tpz=iter->second;
	return TRUE;
}

void CJMQManager::OnJ17C51(int ikch, CString str)
{
	int ikscs,idrcs;
	GetCS(str,ikscs,idrcs);
//	m_JMQCar[ikch].On17C51(str,ikscs,idrcs);
	dTF17C51(ikch,str,ikscs,idrcs);
	//////////////////////////////////////////////////////////////////////////
	if (uWNDTWO > 1)
	{		
		CString temp;
		STerminalPZ temppz;
		temp.Format("10001_1");//201
		if (0 == uMergeVideo)	//���ǲ��úϳ���Ƶ��ģʽ, ��Ƶ�ϳɳ����£��ĺ�һ�������л�
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//�ڶ��������Ŀ
			}
		}

	}
}

void CJMQManager::OnJGPTData(CString strmsg)
{
	CString strTemp;
	strTemp.Format("%s",strmsg);
	if(strTemp.Replace('*',',')==9)
	{
		int icmd;
		int ikch;
		CStringArray te;
		CutOutString(strTemp,te,',');
		if(te.GetSize()==10)
 		{
			ikch=atoi(te.GetAt(1));
			if (OnFindCar(ikch))
		 	{
				icmd=atoi(te.GetAt(2));
				switch (icmd)
				{
				case 1:
					OnJ17C51(ikch,te.GetAt(6));
					break;
				case 2:
					OnJ17C52(ikch,te.GetAt(6),te.GetAt(5));
					break;
				case 3:
					OnJ17C53(ikch,te.GetAt(5),te.GetAt(7));
					break;
				case 5:
					OnJ17C55(ikch,te.GetAt(6),te.GetAt(5));
					break;
				case 6:
					OnJ17C56(ikch,te.GetAt(6),te.GetAt(5));
					break;
				default:
					break;
				}
				logz.WriteLog(AppLogID,"����%d:17C5%d,׼��֤�����:[%s][%s]",ikch,icmd,te.GetAt(6),te.GetAt(5));
			}else
			{
				logz.WriteLog(AppLogID,"OnJGPTData OnFindCar Error:%s",strTemp);
			}
		}
	}
	else
	{
		logz.WriteLog(AppLogID,"OnJGPTData Error:%s",strTemp);
	}
}

void CJMQManager::OnJ17C56(int ikch,CString zkzmbh,CString smsg)
{
	int kshgcj=80;
	int ikscj=atoi(smsg);
	if(uKSKM==3)//��Ŀ��
	{
		kshgcj=90;
	}
	else
	{
		if (m_bBigCar)
		{
			kshgcj = 90;
		}
	}
	if(ikscj>=kshgcj)//���Ժϸ�
	{
		//m_JMQCar[ikch].On17C56(1,ikscj);
		dTF17C56(ikch,1,ikscj);
	}
	else//���Բ��ϸ�
	{
		//m_JMQCar[ikch].On17C56(2,ikscj);
		dTF17C56(ikch,2,ikscj);
	}

	if (uWNDTWO > 1)
	{		
		CString temp;
		STerminalPZ temppz;
		temp.Format("10086_1");//201	
		if (0 == uMergeVideo)	//���ǲ��úϳ���Ƶ��ģʽ, ��Ƶ�ϳɳ����£��ĺ�һ�������л�
		{
			if (GetTerminalPZ(temppz,temp)==TRUE)	
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//�ڶ��������Ŀ
			}

		}
	}
}

//////////////////////////////////////////////////////////////////////////
DWORD WINAPI QHThreadFuncc(LPVOID param)
{
	XMcode * mycode =(XMcode *)param;
	CString errcode;
	int ikch=mycode->ikch;
	errcode.Format("Q%d",mycode->xmCode);
	CString temp,temp1;
	GetPrivateProfileString(errcode,"Time","2000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
	temp1.Format("%s",temp);
	DWORD sleepd=(DWORD) atol(temp1);	
	Sleep(sleepd);
	
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,1,ikch);
// 	if (mycode->xmCode >500 && mycode->xmCode <510)
// 	{
// 		GetPrivateProfileString(errcode,"Time2","2000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
// 		temp1.Format("%s",temp);
// 		DWORD sleepd=(DWORD) atol(temp1);
// 		Sleep(sleepd);
// 		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,1,ikch);
// 	}
// 	if (mycode->xmCode ==249)
// 	{
// 		GetPrivateProfileString(errcode,"Time2","2000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
// 		temp1.Format("%s",temp);
// 		DWORD sleepd=(DWORD) atol(temp1);
// 		Sleep(sleepd);
// 		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,3,ikch);
// 	}
	delete [] mycode;
	return 0;
}
//////////////////////////////////////////////////////////////////////////
void CJMQManager::OnQH17C52(int ikch,int ibh)
{	
	CString temp;
	STerminalPZ temppz;
	temp.Format("%d_%d",iQhKCH[ikch],ibh);	
	if (0 == uMergeVideo)	//���ǲ��úϳ���Ƶ��ģʽ, ��Ƶ�ϳɳ����£��ĺ�һ�������л�
	{
		if(GetTerminalPZ(temppz,temp)==TRUE)
		{
			m_JMQCar[ikch].StartDynamic(temppz,1);//
		}
		
	}
	logz.WriteLog(AppLogID,"�����л� OnQH17C52 ����%d,%s",ikch,temp);
}
//////////////////////////////////////////////////////////////////////////
void CJMQManager::OnJ17C52(int ikch, CString zkzmbh,CString smsg)
{
	if (uWNDTWO!=1)
	{
		CString temp;
		STerminalPZ temppz;
		temp.Format("%s_1",smsg);//201	
		if (0 == uMergeVideo)	//���ǲ��úϳ���Ƶ��ģʽ, ��Ƶ�ϳɳ����£��ĺ�һ�������л�
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//�ڶ��������Ŀ
			}
			
		}
		//////////////////////////////////////////////////////////////////////////

		int xmcode=atoi(smsg);
// 		if ( (xmcode>=501 && xmcode <599) || 249==xmcode || 259==xmcode)
// 		{
// 			CFileFind FindDBFile;
// 			if (FindDBFile.FindFile("ZZIPChannel.dat"))
// 			{
// 				iQhKCH[ikch]=xmcode;
// 				XMcode * mycode =new XMcode;
// 				mycode->xmCode=xmcode;
// 				mycode->ikch=ikch;
// 				HANDLE hThread;
// 				hThread= CreateThread(NULL,0,QHThreadFuncc,mycode,0,0);
// 				CloseHandle(hThread);
// 				logz.WriteLog(AppLogID,"�໭���л�!����--ʪ��·---���:%s",smsg);
// 			}
// 		}
		if (15010==xmcode|| 15020==xmcode || 15030==xmcode)
		{
			logz.WriteLog(AppLogID,"CFileFind ǰ��...����:%d",xmcode);
			CFileFind FindDBFile;
			if (FindDBFile.FindFile("ZZIPChannel.dat"))
			{
				if ( 15010==xmcode|| 15020==xmcode || 15030==xmcode)
				{
				iQhKCH[ikch]=xmcode+1;
				XMcode * mycode =new XMcode;
				mycode->xmCode=xmcode;
				mycode->ikch=ikch;
				HANDLE hThread;
				hThread= CreateThread(NULL,0,QHThreadFuncc,mycode,0,0);
				CloseHandle(hThread);
				logz.WriteLog(AppLogID,"����:%s",smsg);
				}
			}
			return ;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	int icode=atoi(smsg);
	int ite;
	if (icode>300 && icode<400)
	{
		ite=201510;
	}
	else if (icode>400 && icode<500)
	{
		ite=204510;
	}
	else if (icode>500 && icode<600)
	{
		ite=203510;
	}
	else if (icode>600 && icode<700)
	{
		ite=206510;
	}
	else if (icode>700 && icode<800)
	{
		ite=207510;
	}
	else
	{
		ite=icode;
	}
	//dTF17C52(ikch,zkzmbh,atoi(smsg),GetErrorKFLX(smsg));
	dTF17C52(ikch,zkzmbh,ite,GetErrorKFLX(smsg));
}

void CJMQManager::OnJ17C55(int ikch, CString zkzmbh,CString smsg)
{
	CString temp;
	if(uKSKM==3)//��Ŀ��
	{		
		STerminalPZ temppz;
		temp.Format("����%d_2",ikch);	
		if (0 == uMergeVideo)	//���ǲ��úϳ���Ƶ��ģʽ, ��Ƶ�ϳɳ����£��ĺ�һ�������л�
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//������Ƶ��̬�ڶ����泵��
			}
			
		}		
		if (atoi(smsg)<700)
		{
			temp.Format("%d",atoi(smsg)+700);
		}
		else
		{
			temp.Format("%d",atoi(smsg));
		}
		//218 -->918
	}
	else
	{
		//201510==>201990
		//m_JMQCar[ikch].StopDynamic(1);
		//temp.Format("%d",atoi(smsg)/1000*1000+990);//302-->399

		temp.Format("%d",atoi(smsg)/1000*1000+990);//
	}

	int icode=atoi(smsg);
	int ite;
	if (icode>300 && icode<400)
	{
		ite=201510;
	}
	else if (icode>400 && icode<500)
	{
		ite=204510;
	}
	else if (icode>500 && icode<600)
	{
		ite=203510;
	}
	else if (icode>600 && icode<700)
	{
		ite=206510;
	}
	else if (icode>700 && icode<800)
	{
		ite=207510;
	}
	else
	{
		ite=icode;
	}

//	dTF17C55(ikch,atoi(smsg),GetErrorKFLX(temp));
	dTF17C55(ikch,ite,GetErrorKFLX(temp));
	if (uWNDTWO > 1)
	{		
		STerminalPZ temppz;
		temp.Format("10086_1");//201	
		if (0 == uMergeVideo)	//���ǲ��úϳ���Ƶ��ģʽ, ��Ƶ�ϳɳ����£��ĺ�һ�������л�
		{
			if(GetTerminalPZ(temppz,temp)==TRUE)
			{
				m_JMQCar[ikch].StartDynamic(temppz,1);//�ڶ��������Ŀ
			}
			
		}
	}
}

void CJMQManager::CutOutString(CString source,  CStringArray& dest, char division)
{
	dest.RemoveAll();
	int nCount=0;
	for(int i=0;i< source.GetLength();i++)
	{
		if(source.GetAt(i)== division)
		{
			nCount=i;
			dest.Add(source.Left(i)); //ȥ���ұ�
			int nIndex=dest.GetSize()-1;
			for(int j=0;j <nIndex;j++)
			{
				dest[nIndex] = dest[nIndex].Right(dest[nIndex].GetLength()-dest[j].GetLength()-1); //ȥ�����
			}
		}
	}
	//�������һ���ָ���division�ұߵ�ʣ�ಿ�֡�
	dest.Add(source);
	int nIndex2=dest.GetSize()-1;
	for(int j=0;j <nIndex2;j++)
	{
		dest[nIndex2] = dest[nIndex2].Right(dest[nIndex2].GetLength()-dest[j].GetLength()-1); //ȥ�����
	}
}

CString CJMQManager::GetErrorKFLX(CString str)
{
	CString temp=_T("");
	MAP_STR_ERRORDATA::iterator iter;	
	iter = m_Errordata.find(str);	
	if(iter == m_Errordata.end())
	{
		logz.WriteLog(AppLogID,"����:[%s]����δ����,��������! GetErrorData faild !",str);
		temp.Format("%s...",str);
		return temp;
	}
	temp.Format("%s",iter->second.errorlx);
	return temp;
}

int CJMQManager::GetErrorKCFS(CString str)
{
	int ikcfs=0;
	MAP_STR_ERRORDATA::iterator iter;	
	iter = m_Errordata.find(str);	
	if(iter == m_Errordata.end())
	{
		logz.WriteLog(AppLogID,"����:[%s]����δ����,�������±�ErrorData! GetErrorKFFS faild !",str);
		return ikcfs;
	}
	ikcfs=iter->second.ikcfs;
	return ikcfs;
}

void CJMQManager::OnJ17C53(int ikch, CString str,CString timestr)
{
	int pos=str.Find("@");
	int ecodei;
	if (pos <= 0)
	{
		logz.WriteLog(dwFLogID,"OnJ17C53 find @ û���ҵ�%d= %s",ikch,str);
		return ;
	}
	ecodei=atoi(str.Left(pos));
	CString temp;
	if(uKSKM==3)//��Ŀ��
	{		
		switch(ecodei)
		{
		case 201:
			temp.Format("��");
			break;
		case 202:
			temp.Format("��");
			break;
		case 203:
			temp.Format("ֱ��");
			break;
		case 204:
			temp.Format("���");
			break;
		case 205:
			temp.Format("ֱ��");
			break;
		case 206:
			temp.Format("����");
			break;
		case 207:
			temp.Format("ѧУ");
			break;
		case 208:
			temp.Format("��վ");
			break;
		case 209:
			temp.Format("�ᳵ");
			break;
		case 210:
			temp.Format("����");
			break;
		case 211:
			temp.Format("����");
			break;
		case 212:
			temp.Format("��ͷ");
			break;
		case 213:
			temp.Format("ҹ��");
			break;
		case 214:
			temp.Format("��ת");
			break;
		case 215:
			temp.Format("��ת");
			break;
		case 216:
			temp.Format("ֱ��");
			break;	
		case 217:
			temp.Format("�Ӽ�");
			break;
		default:
			temp.Format("�ۺ�");
			break;
		}
	}
	else
	{		
		if (ecodei>201509 && ecodei<201700)
		{
			temp.Format("�������");
		}
		else if (ecodei>204509 && ecodei<204700)
		{
			temp.Format("�෽ͣ��");
		}
		else if (ecodei>203509 && ecodei<203700)
		{
			temp.Format("��������");
		}
		else if (ecodei>206509 && ecodei<206700)
		{
			temp.Format("������ʻ");
		}
		else if (ecodei>207509 && ecodei<207700)
		{
			temp.Format("ֱ��ת��");
		}
		else if (ecodei==259)
		{
			temp.Format("����ʪ��");
		}
		else if (ecodei==249)
		{
			temp.Format("ģ�����");
		}
		else
		{
			temp.Format("�ۺ�����");
		}
	}
	str=str.Mid(pos+1);
	str.Replace("@","");
	//////////////////////////////////////////////////////////////////////////	
	int ikcfs=GetErrorKCFS(str);
	dTF17C53(ikch,temp,GetErrorKFLX(str),ikcfs);
}

void CJMQManager::OnGnssData(char *recvbuf, int ikch)
{
	if (OnFindCar(ikch))
	{
		//m_JMQCar[ikch].OnGnssData(recvbuf);
		dTF17C54(ikch,recvbuf);
	}
}

UINT CJMQManager::ListMSGThread(LPVOID pParam)
{
	CJMQManager *pInfo=(CJMQManager*)pParam;
// 	LIST_JGPT::iterator iter;
// 	JGPTLISTSTRU templs;
// 	bool isEmpty;
// 	Sleep(1000);
// 	while (1)
// 	{
// 		WaitForSingleObject(pInfo->ListhEvent,1500);
// 		isEmpty=pInfo->m_ListJgpt.empty();
// 		SetEvent(pInfo->ListhEvent);
// 		if (isEmpty)
// 		{
// 			TRACE("m_ListJgpt is Empty\n");
// 			Sleep(1500);
// 			continue;
// 		}
// 		WaitForSingleObject(pInfo->ListhEvent,1500);
// 		for (iter=pInfo->m_ListJgpt.begin();iter!=pInfo->m_ListJgpt.end();)
// 		{
// 			if(iter->irunnum==0)
// 			{
// 				iter->irunnum=5;				
// 				iter->ics--;
// 				if (iter->ics<0)
// 				{
// 					TRACE("ɾ��%d,%d \n",iter->ikch,iter->ics);
// 					iter=pInfo->m_ListJgpt.erase(iter);
// 				}
// 				else
// 				{//���´���
// 					TRACE("���´���%d,%d \n",iter->ikch,iter->ics);
// 					pInfo->ReRunData(iter->ikch,iter->itype,iter->zkzmbh,iter->msg);
// 					iter++;
// 				}				
// 			}
// 			else
// 			{
// 				iter->irunnum--;
// 				TRACE("%d,%d,%d,%s \n",iter->ikch,iter->irunnum,iter->itype,iter->msg);
// 				iter++;
// 			}
// 		}
// 		SetEvent(pInfo->ListhEvent);
// 		Sleep(1000);
// 	}	
	return 0;
}

void CJMQManager::RunListMsgThread()
{
// 	ListhEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
// 	SetEvent(ListhEvent);
// 	AfxBeginThread(ListMSGThread,(LPVOID)this);
}

void CJMQManager::AddMSGList(int ikch,int itype,CString zkzmbh,CString msgstr,int iruntime)
{
// 	JGPTLISTSTRU templ;
// 	templ.ikch=ikch;
// 	templ.itype=itype;
// 	templ.ics=3;//������
// 	templ.irunnum=iruntime;
// 	templ.zkzmbh.Format("%s",zkzmbh);
// 	templ.msg.Format("%s",msgstr);
// 	
// 	WaitForSingleObject(ListhEvent,1500);
// 	m_ListJgpt.push_back(templ);
// 	SetEvent(ListhEvent);
}

void CJMQManager::DelMSGList(int ikch, int itype)
{
// 	if (OnFindCar(ikch))
// 	{
// 		LIST_JGPT::iterator iter;
// 		WaitForSingleObject(ListhEvent,1500);
// 		for (iter=m_ListJgpt.begin();iter!=m_ListJgpt.end();)
// 		{
// 			if ((iter->ikch==ikch) && (iter->itype==itype))
// 			{
// // 				if (itype==1)
// // 				{
// // //					m_JMQCar[ikch].SetZkzmbh(iter->zkzmbh);
// // 				}
// 				m_ListJgpt.erase(iter++);
// 				//logz.WriteLog(AppLogID,"DelMSGList %d,%d",ikch,itype);
// 			}
// 			else
// 			{
// 				++iter;
// 			}
// 		}
// 		SetEvent(ListhEvent);
// 	}
}

void CJMQManager::GetCS(CString str, int &ikscs, int &idrcs)
{
	CString sqltemp;
	try
	{
		ikscs=0;
		idrcs=0;
		VARIANT cnt;
		cnt.vt = VT_INT;

		if (m_bEncrypt)
		{
			sqltemp.Format("Set ARITHABORT ON;SELECT ���Դ���,���մ��� FROM StudentInfo WHERE ׼��֤�����=dbo.charEncode('%s') ",str);
		}
		else
		{
			sqltemp.Format("SELECT ���Դ���,���մ��� FROM StudentInfo WHERE ׼��֤�����='%s' ",str);
		}
		
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			vat =pSet->GetCollect("���Դ���");
			if (vat.vt != VT_NULL)
			{
				ikscs=vat.intVal;
			}
			else
			{
				ikscs=0;
			}
			vat =pSet->GetCollect("���մ���");
			if (vat.vt != VT_NULL)
			{
				idrcs=vat.intVal;
			}
			else
			{
				idrcs=0;
			}	
		}//if		
	}
	catch (_com_error e) 
	{
		ikscs=0;
		idrcs=0;
		logz.WriteLog(AppLogID,"FUN GetCS com_error=%s,׼��֤�����:%s",e.ErrorMessage(),str);
		try
		{
			if (m_pConn->GetState() == adStateOpen)
			{
				m_pConn->Close();
			}
			if (m_pConn)
			{
				m_pConn.Release();
				m_pConn = NULL;
			}
			OnInitSQL();
		}
		catch (_com_error e) 
		{
			logz.WriteLog(AppLogID,"OnprH264Format Close and Release com_error=%s",e.ErrorMessage());
		}
	}
}

void CJMQManager::ReRunData(int ikch, int itype, CString zkzm, CString msg)
{
	switch (itype)
	{
	case 0:		
		break;
	case 1:
		OnJ17C51(ikch,zkzm);
		break;
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		break;
	case 6:
		OnJ17C56(ikch,zkzm,msg);
		break;
	default:
		break;
	}
	logz.WriteLog(AppLogID,"���´�����Ϣ:����%d,״̬:%d,%s",ikch,itype,zkzm);
}

BOOL CJMQManager::LoadTFDll()
{
	dllYuvToH264TH = LoadLibrary(TEXT("CYuvToH264T.dll")); 
	if (dllYuvToH264TH == NULL) 
	{ 
		AfxMessageBox("���� CYuvToH264T.dll ʧ��!"); 
		logz.WriteLog(AppLogID,"���� CYuvToH264T.dll ʧ��!");
		return FALSE; 
	} 
	dllTFInit=(TFInit)GetProcAddress(dllYuvToH264TH,TEXT("TFInit"));
	dTFPassH=(TFPassH)GetProcAddress(dllYuvToH264TH,TEXT("TFPassiveHandle"));
	dTF17C51=(TF17C51)GetProcAddress(dllYuvToH264TH,TEXT("TF17C51"));
	dTF17C52=(TF17C52)GetProcAddress(dllYuvToH264TH,TEXT("TF17C52"));
	dTF17C53=(TF17C53)GetProcAddress(dllYuvToH264TH,TEXT("TF17C53"));
	dTF17C54=(TF17C54)GetProcAddress(dllYuvToH264TH,TEXT("TF17C54"));
	dTF17C55=(TF17C55)GetProcAddress(dllYuvToH264TH,TEXT("TF17C55"));
	dTF17C56=(TF17C56)GetProcAddress(dllYuvToH264TH,TEXT("TF17C56"));
	
	if (dllTFInit ==NULL || dTFPassH ==NULL || dTF17C51 ==NULL|| dTF17C52 ==NULL\
		|| dTF17C53 ==NULL|| dTF17C54 ==NULL|| dTF17C55 ==NULL|| dTF17C56 ==NULL)
	{
		logz.WriteLog(AppLogID,"���� CYuvToH264T.dll ʧ��!");
		return FALSE;
	}
	return TRUE;
}

void CJMQManager::GetKCHArray(CStringArray &kcharr)
{
	MAP_INT_JMQCar::iterator iter;
	CString temp;
	for (iter=m_JMQCar.begin();iter!=m_JMQCar.end();iter++)
	{		
		temp.Format("%d",iter->first);
		kcharr.Add(temp);
	}
}
