// CatchPhotoHK.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CatchPhotoHK.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//HANDLE CCatchPhotoHKApp::hMutexM=CreateMutex(NULL,FALSE,NULL);//�����������
//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCatchPhotoHKApp

BEGIN_MESSAGE_MAP(CCatchPhotoHKApp, CWinApp)
	//{{AFX_MSG_MAP(CCatchPhotoHKApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCatchPhotoHKApp construction

CCatchPhotoHKApp::CCatchPhotoHKApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCatchPhotoHKApp object

CCatchPhotoHKApp theApp;

//char *p =(LPSTR)(LPCSTR)str1
//CString cstr;
extern "C" __declspec( dllexport ) char * DLLEnCode(LPTSTR InPutstr)
{
	CString cstr;
	cstr=theApp.Decode(InPutstr);
	return (LPSTR)(LPCTSTR)cstr;
}
// extern "C" __declspec( dllexport ) string DLLEnCode(LPTSTR InPutstr)
// {
// 	std::string tempstr;
// 	CString cstr;
// 	cstr=theApp.Decode(InPutstr);
// 	tempstr= cstr.GetBuffer(0);
// 
// 	return tempstr;
// }

// extern "C" __declspec( dllexport ) void DLLEnCode(LPTSTR InPutstr,LPTSTR OutPutStr)
// {
// 	CString cstr;
// 	cstr=theApp.Decode(InPutstr);
// 	strcpy(OutPutStr,cstr.GetBuffer(cstr.GetLength()));
// 	cstr.ReleaseBuffer();
// }

extern "C" __declspec( dllexport ) void UpdCatchPhoto(LPTSTR Strkch, LPTSTR Strdrcs, LPTSTR Strkscs, LPTSTR Strzkzmbh,LPTSTR strEcode)
{
	theApp.CatchPhotoEx(Strkch,Strdrcs,Strkscs,Strzkzmbh,strEcode);
}
extern "C" __declspec( dllexport ) char * InitCatchPhoto(LPTSTR IPAddress,LPTSTR SQLPWD,int isqlororacle)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	theApp.LastError=_T("");
	if (isqlororacle==0)
	{
		theApp.uSqlOrOracle=1;
	}
	else
	{
		theApp.uSqlOrOracle=0;
	}
	HRESULT hr;		//��¼�������
	CString strSQL,logstr;
	_ConnectionPtr m_Connection;
	theApp.WriteLog("InitCatchPhoto ���ó�ʼ����ʼ %d",isqlororacle);
	try
	{
		hr = m_Connection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			if (theApp.uSqlOrOracle==1)
			{
				strSQL.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",IPAddress,"2ndDrivingTestSystem","sa",SQLPWD);
			}
			else
			{
				strSQL.Format("Provider=OraOLEDB.Oracle.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));User ID=%s;Password=%s",\
				IPAddress,"1521","bkzy","sa",SQLPWD);
			}
			m_Connection->Open((_bstr_t)strSQL,"","",adModeUnknown); 
		}
	}
	catch (_com_error e)
	{
		logstr.Format("�������ݿ�ʧ��!IP=%s",IPAddress);
		theApp.WriteLog(logstr);
		theApp.LastError.Format("%s#",logstr);
		return (LPSTR)(LPCTSTR)theApp.LastError;
	}
	theApp.WriteLog("���ݿ����ӳɹ�! 20170916 2036 uSqlOrOracle=%d",theApp.uSqlOrOracle);

	theApp.m_SQLIPAddress.Format("%s",IPAddress);
	theApp.m_SQLPassword.Format("%s",SQLPWD);
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		if (theApp.uSqlOrOracle==1)
		{
			strSQL.Format("select ���+'_'+Nid as ������,�豸IP,�û���,����,�˿ں�,ͨ����,Nid from TBKVideo where (��� like '����%%') and (Nid=1 OR Nid=5) order by �豸IP asc");//
		}
		else
		{
			strSQL.Format("select ���||'_'||Nid as ������,�豸IP,�û���,����,�˿ں�,ͨ����,Nid from TBKVideo where (��� like '����%%') and (Nid=1 OR Nid=5) order by �豸IP asc");//
		}
		
		TRACE(strSQL);
		_RecordsetPtr pSet =m_Connection->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
		_variant_t vat;
		int tempNid;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				HKLogin tempLogin;
				MYKCH tempkch;
				vat =pSet->GetCollect("������");
				if (vat.vt != VT_NULL)
				{
					tempkch.m_KCH.Format("%s",(LPCSTR)_bstr_t(vat));
				}
				vat =pSet->GetCollect("�豸IP");
				if (vat.vt != VT_NULL)
				{
					tempLogin.m_IPaddrL.Format("%s",(LPCSTR)_bstr_t(vat));
					tempkch.m_IPAddr.Format("%s",(LPCSTR)_bstr_t(vat));
				}
				vat =pSet->GetCollect("�û���");
				if (vat.vt != VT_NULL)
				{
					tempLogin.m_User.Format("%s",(LPCSTR)_bstr_t(vat));	
				}
				vat =pSet->GetCollect("����");
				if (vat.vt != VT_NULL)
				{
					tempLogin.m_Password.Format("%s",(LPCSTR)_bstr_t(vat));	
				}
				vat =pSet->GetCollect("�˿ں�");
				if (vat.vt != VT_NULL)
				{
					tempLogin.m_Port=atoi((LPCSTR)_bstr_t(vat));	
				}
				vat =pSet->GetCollect("ͨ����");
				if (vat.vt != VT_NULL)
				{
					tempkch.m_Channel=atoi((LPCSTR)_bstr_t(vat));	
				}
				vat =pSet->GetCollect("Nid");
				if (vat.vt != VT_NULL)
				{
					tempNid=atoi((LPCSTR)_bstr_t(vat));	
				}
				else
				{
					tempNid=6;
				}
				theApp.AddHKLogin(tempLogin,tempNid);
				theApp.AddKch(tempkch);
				pSet->MoveNext();
			}//while
		}//if		
		else
		{
			logstr.Format("û�ж�ȡ�����ݿ���е�����");
			theApp.WriteLog(logstr);
		}
	}
	catch (_com_error &comError)
	{
		logstr.Format("������ʾ,��ѯӲ��¼���������Ϣʧ��!,%s",comError.Description());
		AfxMessageBox(logstr.GetBuffer(0));
		theApp.WriteLog(logstr);
		theApp.LastError.Format("%s#",logstr);
	}
	
	return (LPSTR)(LPCTSTR)theApp.LastError;
}

CString CCatchPhotoHKApp::Decode(CString str)
{
	CString outStr=_T("");
	if (str.IsEmpty())
	{
		return outStr;
	}
	outStr=str;
	outStr.MakeReverse();
	int iLen =outStr.GetLength();	
	for(int i=0;i<iLen;i++)
	{		
		if (i%3==0)
		{
			outStr.SetAt(i,outStr[i]+2);
		}
		else
		{
			outStr.SetAt(i,outStr[i]+1);
		}
	}
	return outStr;
}
BOOL CCatchPhotoHKApp::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CString tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log",tempPath.Left(tempPath.ReverseFind('\\')));//��ǰĿ¼�µ�LOG�ļ�·��
	DWORD dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();	
	tempPath.Format("%s\\%s",strPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//���������ļ���
	}
	strPath.Format("%s\\",tempPath);
	logFile.Format("%s\\ץ����־CatchPhotoHK.log",tempPath);
	
	NET_DVR_Init();//��ʼ��
	DWORD dwVersion = NET_DVR_GetSDKBuildVersion();

	WriteLog("��ȷ����DLL InitInstance V2.3 Build 20171024 1341 139���� HCNetSDK V%d.%d.%d.%d",(0xff000000 & dwVersion)>>24,(0x00ff0000 & dwVersion)>>16,(0x0000ff00 & dwVersion)>>8, (0x000000ff & dwVersion));
	::CoInitialize(NULL);
	return CWinApp::InitInstance();
}

int CCatchPhotoHKApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();
	CString tempPath,strPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log\\*",tempPath.Left(tempPath.ReverseFind('\\')));//��ǰĿ¼�µ�LOG�ļ�·��    
	
	CString sDayFileName[SAVEDAYS];
	COleDateTime tempDT;	
	COleDateTime TodayTime;
	COleDateTimeSpan  ts;
	TodayTime = COleDateTime::GetCurrentTime();	
	//��¼��Ҫ�������ļ�������
	for (int i=0; i<SAVEDAYS; i++)
	{
		ts.SetDateTimeSpan(i,0,0,0);
		tempDT = TodayTime - ts; 
		sDayFileName[i].Format("%s",tempDT.Format("%Y-%m-%d"));
	}
	//�����ļ����ڵ��ļ����ļ��У������ڱ�����Χ�ڵģ�һ��ɾ��
	CFileFind filefind;	
	BOOL bFound = filefind.FindFile(strPath);
	int bDelFlag = 1;
	while(bFound)     
	{     
		bFound = filefind.FindNextFile();     
		if(filefind.IsDots())     
		{
			continue;
		}		
		//ȥ���ļ�(��)ֻ��������     
		SetFileAttributes(filefind.GetFilePath(),FILE_ATTRIBUTE_NORMAL);		
		bDelFlag = 1;
		for (i=0; i<SAVEDAYS; i++)
		{
			if (filefind.GetFileName() == sDayFileName[i])
			{
				bDelFlag = 0;
			}
		}		
		if (bDelFlag == 1)
		{
			if(filefind.IsDirectory())     
			{     
				//�ݹ�ɾ�����ļ���     
				DeleteDirectory(filefind.GetFilePath());     
				RemoveDirectory(filefind.GetFilePath());     
			}     
			else     
			{     
				//ɾ���ļ�     
				DeleteFile(filefind.GetFilePath());     
			}     
		}
	}     
	filefind.Close(); 
	NET_DVR_Cleanup();
	return CWinApp::ExitInstance();
}
void CCatchPhotoHKApp::DeleteDirectory(CString strDir)
{
	if(!strDir.IsEmpty())
	{		
		CFileFind ff;     
		BOOL bFound = ff.FindFile(strDir+"\\*");     
		while(bFound)     
		{     
			bFound = ff.FindNextFile();     
			if(ff.IsDots())     
			{
				continue;
			}    
			//ȥ���ļ�(��)ֻ��������     
			SetFileAttributes(ff.GetFilePath(),   FILE_ATTRIBUTE_NORMAL);     
			if(ff.IsDirectory())     
			{     
				//�ݹ�ɾ�����ļ���     
				DeleteDirectory(ff.GetFilePath());     
				RemoveDirectory(ff.GetFilePath());     
			}     
			else     
			{     
				//ɾ���ļ�     
				DeleteFile(ff.GetFilePath());     
			}     
		}     
		ff.Close();   	
		//Ȼ��ɾ�����ļ���     
		RemoveDirectory(strDir);    
	}
}


void CCatchPhotoHKApp::WriteLog(LPCTSTR pstrFormat, ... )
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	TRACE(logstr+"\n");
	
	SYSTEMTIME st;
	GetLocalTime(&st);
	CString strTime;
	strTime.Format("[%02d:%02d:%02d.%03d]",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	
	FILE *fp=fopen(logFile,"a+");
	fprintf(fp,"%s:%s#\n",strTime,logstr);
	fclose(fp);	
}

BOOL CCatchPhotoHKApp::CatchPhoto(CString strkch, CString strdrcs, CString strkscs, CString strzkzmbh,CString strECode)
{
// 	try
// 	{	
// 	CString temp;
// 	temp.Format("��ʼץ��:%s,%s,%s,%s,%s",strkch,  strdrcs,  strkscs,  strzkzmbh, strECode);
// 	WriteLog(temp);
// 		if (strkch.IsEmpty())
// 		{
// 			WriteLog("������Ϊ��!");
// 			return FALSE;
// 		}
// 		int i,j;
// 		for (i=0;i<m_myKch.size();i++)
// 		{
// 			if (strcmp(m_myKch[i].m_KCH,strkch)==0)
// 			{
// 				for (j=0;j<m_HKLogin.size();j++)
// 				{
// 					if (strcmp(m_HKLogin[j].m_IPaddrL,m_myKch[i].m_IPAddr)==0)
// 					{
// 						if (m_HKLogin[j].lUserID ==-1)
// 						{
// 							HKLogin temp;
// 							temp =m_HKLogin[j];
// 							F_Login(temp);
// 						}
// 						CString temp;
// 						temp.Format("%s_%s_%s_%s_%s",strkch,strzkzmbh,strkscs,strdrcs,strECode);
// 						WriteLog("׼��ץ��"+temp);
// 						if (TRUE==CaptureJPEGPic(m_HKLogin[j].lUserID,m_myKch[i].m_Channel,temp))
// 						{
// 							//д���ݿ�
// 							if (TransferPhoto(strkch,strzkzmbh,strkscs,strdrcs,atoi(strECode))==FALSE)
// 							{
// 								Sleep(10);
// 								TransferPhoto(strkch,strzkzmbh,strkscs,strdrcs,atoi(strECode));
// 							}
// 						}
// 						else
// 						{
// 							if(FALSE==NET_DVR_Logout_V30(m_HKLogin[j].lUserID))
// 							{
// 								
// 								temp.Format("�˳�ʧ��!%s,%s",m_HKLogin[j].m_IPaddrL,OnGetLastErrorData(NET_DVR_GetLastError()));
// 								WriteLog(temp);
// 							}
// 							m_HKLogin[j].lUserID=-1;//ץ��ʧ������ 
// 							HKLogin temp;
// 							temp =m_HKLogin[j];
// 							F_Login(temp);//���µ�¼�豸
// 						}
// 						break;
// 					}
// 				}
// 				break;
// 			}
// 		}
// 	}
// 	catch (CMemoryException* e)
// 	{
// 		return FALSE;
// 	}
// 	catch (CFileException* e)
// 	{
// 		return FALSE;
// 	}
// 	catch (CException* e)
// 	{
// 		return FALSE;
// 	}
	return TRUE;
}
BOOL CCatchPhotoHKApp::F_Login(HKLogin login)
{
	CString tIP,tUser,tPWD;
	int port=8000;
	MAP_STR_HKLOGIN::iterator iter;	
	iter = m_HKLogin.find(login.m_IPaddrL);	
	if(iter == m_HKLogin.end())
	{
		return FALSE;
	}
	if (iter->second.lUserID!=-1)
	{
		return TRUE;
	}
	tIP.Format("%s",iter->second.m_IPaddrL);
	tUser.Format("%s",iter->second.m_User);
	tPWD.Format("%s",iter->second.m_Password);
	port=iter->second.m_Port;
	if (tIP.IsEmpty())
	{
		return FALSE;
	}
	LONG lUserID;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//����豸�����Ľṹ��
	
	lUserID = NET_DVR_Login_V30(tIP.GetBuffer(0),port,tUser.GetBuffer(0),tPWD.GetBuffer(0), &struDeviceInfo);
	tIP.ReleaseBuffer();
	tUser.ReleaseBuffer();
	tPWD.ReleaseBuffer();
	if (lUserID < 0)
	{
		CString logstr;
		logstr.Format("��¼ʧ��!IP=%s,ErrID=%s",tIP,OnGetLastErrorData(NET_DVR_GetLastError()));
		WriteLog(logstr);
		logstr.Format("%s#��¼ʧ��!IP=%s,User=%s,ErrID=%d",LastError,tIP,tUser,NET_DVR_GetLastError());
		LastError=logstr;
		return FALSE;
	}
	iter->second.lUserID=lUserID;
	CString logstr;
	logstr.Format("��¼�ɹ�!IP=%s,User=%s,PWD=%s,Port=%d,%ld",tIP,tUser,tPWD,port,iter->second.lUserID);
	WriteLog(logstr);
	return TRUE;
}

CString CCatchPhotoHKApp::OnGetLastErrorData(int ierrorid)
{
	CString temp,keyname;
	keyname.Format("%d",ierrorid);
	TRACE("OnHKLastError=[%s]\n",keyname);
	GetPrivateProfileString("LASTERROR",keyname,"",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\HCNetSDKE.DAT");
	temp.ReleaseBuffer();
	keyname.Format("Eid=%d,M=%s",ierrorid,temp);
	return keyname;
}

BOOL CCatchPhotoHKApp::CaptureJPEGPic(LONG lUserID, int Channel,CString Filename)
{
	NET_DVR_JPEGPARA struJpegPara;
	struJpegPara.wPicSize = 0;
	struJpegPara.wPicQuality = 0;	
	char cFilename[256];
	sprintf(cFilename, "%s\\%s.jpg",strPath,Filename);
	BOOL bcapOk=FALSE;
	for (int i=0;i<3;i++)
	{	
		if (!NET_DVR_CaptureJPEGPicture(lUserID,Channel, &struJpegPara, cFilename))
		{		
			WriteLog("ץͼʧ��!NET_DVR_CaptureJPEGPicture failed! %s,filename=%s",OnGetLastErrorData(NET_DVR_GetLastError()),Filename);
			Sleep(1000);
		}
		else
		{
			bcapOk=TRUE;
			break;
		}
	}
	return bcapOk;
}

void CCatchPhotoHKApp::AddKch(const MYKCH &kch)
{
	MAP_STR_MYKCH::iterator iter;	
	iter = m_myKch.find(kch.m_KCH);	
	if(iter == m_myKch.end())
	{
		this->m_myKch.insert(pair<CString,MYKCH>(kch.m_KCH,kch));
	}
}

BOOL CCatchPhotoHKApp::TransferPhoto(CString strkch,CString strzkzmbh,CString strkscs,CString strdrcs,int ixmcode)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//AfxOleInit();
	//::CoInitialize(NULL);
	CString m_JPGname;			
	m_JPGname.Format("%s\\%s_%s_%s_%s_%d.jpg",strPath,strkch,strzkzmbh,strkscs,strdrcs,ixmcode);
	CFileFind FindDBFile;
	if (!FindDBFile.FindFile(m_JPGname))
	{
		WriteLog("û���ҵ���Ƭ"+m_JPGname);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	HRESULT hr;		//��¼�������
	CString strSQL,temp;
	_ConnectionPtr m_Connection;
	try
	{
		hr = m_Connection.CreateInstance("ADODB.Connection");
		if(SUCCEEDED(hr))
		{
			if (uSqlOrOracle==1)
			{
				strSQL.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",m_SQLIPAddress,"2ndPhoto","sa",m_SQLPassword);
			}
			else
			{
				strSQL.Format("Provider=OraOLEDB.Oracle.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=%s))(CONNECT_DATA=(SERVICE_NAME=%s)));User ID=%s;Password=%s",\
 				m_SQLIPAddress,"1521","bkzy","sapho",m_SQLPassword);
			}
			m_Connection->Open((_bstr_t)strSQL,"","",adModeUnknown); 
		}
	}
	catch (_com_error e)
	{
		WriteLog("����2ndPhoto���ݿ�ʧ��!IP=%s",m_SQLIPAddress);
		return FALSE;
	}
	/////////////////////////////////////////////////
	_RecordsetPtr pRdsetArm;
	try
	{
		//////////////////////////////////////////////////////////////////////////
		CString sqlzd;
		if (ixmcode>99999)
		{
			sqlzd.Format("p%d",ixmcode/1000*100);
		}
		else if (ixmcode > 300 && ixmcode <390)
		{
			sqlzd.Format("p20100");//�������
		}
		else if (ixmcode > 400 && ixmcode <490)
		{
			sqlzd.Format("p20400");//�෽ͣ��
		}
		else if (ixmcode > 500 && ixmcode <590)
		{
			sqlzd.Format("p20300");//�µ�����ͣ������
		}
		else if (ixmcode > 600 && ixmcode <690)
		{
			sqlzd.Format("p20600");//������ʻ
		}
		else if (ixmcode > 700 && ixmcode <790)
		{
			sqlzd.Format("p20700");//ֱ��ת��
		}
		else if (ixmcode == 249)
		{
			sqlzd.Format("p21400");//ģ�������ʻ
		}
		else if (ixmcode ==259)
		{
			sqlzd.Format("p21500");//ģ���꣨������ʻ
		}
		/////////////////////����Ŀ///////////////////////////////
		else if (ixmcode > 1010 && ixmcode <1050)
		{
			sqlzd.Format("p20200");//׮��  1
		}
		else if (ixmcode > 1200 && ixmcode <1250)
		{
			sqlzd.Format("p20900");//�����ϰ�  1
		}
		else if (ixmcode > 1100 && ixmcode <1150)
		{
			sqlzd.Format("p20500");//������  1
		}
		else if (ixmcode > 1400 && ixmcode <1450)
		{
			sqlzd.Format("p20800");//�޿���  1
		}
		else if (ixmcode > 1500 && ixmcode <1550)
		{
			sqlzd.Format("p21000");//���· 1
		}
		else if (ixmcode > 2200 && ixmcode <2251)
		{
			sqlzd.Format("p21500");//������  1
		}
		else if (ixmcode > 2300 && ixmcode <2350)
		{
			sqlzd.Format("p21600");//ʪ��·  1
		}
		else if (ixmcode > 1300 && ixmcode <1350)
		{
			sqlzd.Format("p21300");// ɽ����ת 1
		}
		else if (ixmcode > 1800 && ixmcode <1850)
		{
			sqlzd.Format("p21100");//խ·��ͷ   1
		}
		else if (ixmcode > 2100 && ixmcode <2150)
		{
			sqlzd.Format("p21400");//���  1
		}
		else if (ixmcode > 1600 && ixmcode <1650)
		{
			sqlzd.Format("p21200");//���ٹ�· 1
		}
		else if (ixmcode > 1700 && ixmcode <1750)
		{
			sqlzd.Format("p21700");//�������� 1
		}
		else
		{
			sqlzd.Format("p%d",ixmcode);
		}
		//////////////////////////////////////////////////////////////////////////
		pRdsetArm.CreateInstance("ADODB.Recordset");
		pRdsetArm->CursorLocation = adUseClient;
	
		_variant_t vCard;
		strSQL.Format("SELECT ׼��֤�����,���Դ���,���մ���,%s FROM XMPhoto WHERE ׼��֤����� = '%s' and ���մ���='%s' and ���Դ���='%s' ", sqlzd,strzkzmbh,strdrcs,strkscs);
		//AfxMessageBox(strSQL);
		pRdsetArm->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_Connection, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		if(pRdsetArm->RecordCount == 0 )
		{
			pRdsetArm->AddNew();
			pRdsetArm->PutCollect("׼��֤�����",(_variant_t)strzkzmbh);
			pRdsetArm->PutCollect("���Դ���",(_variant_t)strkscs);
			pRdsetArm->PutCollect("���մ���",(_variant_t)strdrcs);
			temp.Format("AddNew ׼��֤�����:%s,%s,%s",strzkzmbh,strkscs,strdrcs);
			WriteLog(temp);
		}
		
		//���������ݿ��в���ͼƬ�ȡ�

		VARIANT varChunk;
		SAFEARRAY* psa;
		SAFEARRAYBOUND rgsabound[1];
		CFile f(m_JPGname.operator LPCTSTR(),CFile::modeRead);
		BYTE bval[1025];
		long uIsRead=0;
		
		while (1) //ÿ1K��Ϊһ����λд�����ݿ⣬��ʵ�ʶ�дС��1024�ֽ�ʱ��ʾ�����ˣ�д�������ѭ��
		{
			uIsRead = f.Read(bval,1024);
			if (uIsRead ==0 ) break;
			rgsabound[0].cElements = uIsRead;
			rgsabound[0].lLbound = 0;
			psa = SafeArrayCreate(VT_UI1,1,rgsabound);
			for (long index=0; index<uIsRead; index++)
			{
				if (FAILED(SafeArrayPutElement(psa,&index,&bval[index])))
				{
					//AfxMessageBox("����");	
					return 0;
				}
			}
			varChunk.vt = VT_ARRAY|VT_UI1;
			varChunk.parray = psa;
			
			pRdsetArm->Fields->GetItem((_variant_t)sqlzd)->AppendChunk(varChunk);
			
			::VariantClear(&varChunk);
			::SafeArrayDestroyData(psa);
			if (uIsRead<1024) break;
		}
		f.Close();
		
		pRdsetArm->Update();
		pRdsetArm->Close();
		pRdsetArm.Release();
	}	
	catch (_com_error &e) 
	{
		temp.Format("д���ݿ�ʧ��!��Ŀ:%d,׼��֤�����:%s,%s,%s ComERROR:[%s]",ixmcode,strzkzmbh,strdrcs,strkscs,e.ErrorMessage());
		WriteLog(temp);
		return FALSE;
	}	
	try
	{
		if (m_Connection->GetState() == adStateOpen)
		{
			m_Connection->Close();
		}
		if (m_Connection)
		{
			m_Connection.Release();
			m_Connection = NULL;
		}
	}
	catch (_com_error &e) 
	{
		temp.Format("m_Connection Close Release ComERROR:[%s]",e.ErrorMessage());
		WriteLog(temp);
		return FALSE;
	}	
	//CoUninitialize();
	return TRUE;
}

void CCatchPhotoHKApp::AddHKLogin(const HKLogin &login,int Nid)
{
	MAP_STR_HKLOGIN::iterator iter;	
	iter = m_HKLogin.find(login.m_IPaddrL);	
	if(iter == m_HKLogin.end())
	{
		this->m_HKLogin.insert(pair<CString,HKLogin>(login.m_IPaddrL,login));
		if (Nid==1)//����NVR �ȵ�¼
		{
//			F_Login(login);
		}	
	}
}

BOOL CCatchPhotoHKApp::CatchPhotoEx(CString strkch, CString strdrcs, CString strkscs, CString strzkzmbh, CString strECode)
{
	try
	{		
		if (strkch.IsEmpty())
		{
			WriteLog("������Ϊ��!");
			return FALSE;
		}
		//��ʼץ��
		CString temp;
		temp.Format("%s_%s_%s_%s_%s",strkch,strzkzmbh,strkscs,strdrcs,strECode);
		WriteLog("CatchPhotoEx��ʼץ��%s",temp);
		if (OnCaptureJPEGPic(strkch,1,temp)==FALSE)
		{
			Sleep(1000);
			if (OnCaptureJPEGPic(strkch,5,temp)==FALSE)
			{
				return FALSE;
			}
		}
		//д���ݿ�
		if (TransferPhoto(strkch,strzkzmbh,strkscs,strdrcs,atoi(strECode))==FALSE)
		{
			WriteLog("д���ݿ�ʧ��!%s_%s_%s_%s_%s",strkch,strzkzmbh,strkscs,strdrcs,strECode);
			return FALSE;
		}
		//	WriteLog("ץ�����%s",temp);
	}
	catch (CMemoryException )
	{
		AfxAbort();
		WriteLog("CatchPhotoEx CMemoryException");
		return FALSE;
	}
	catch (CFileException* e)
	{
		WriteLog("CatchPhotoEx CFileException %s",e->m_strFileName);
		e->ReportError();		
		return FALSE;
	}
	catch (CException )
	{
		WriteLog("CatchPhotoEx CException");
		return FALSE;
	}
	catch (...)
	{
		WriteLog("CatchPhotoEx ...");
		return FALSE;
	}
	return TRUE;
}

BOOL CCatchPhotoHKApp::OnCaptureJPEGPic(CString strKch, UINT index, CString fileName)
{
	CString temptype;
	if (index==1)
	{
		temptype.Format("����ץ��ʧ��!");
	}
	else
	{
		temptype.Format("����ץ��ʧ��!");
	}
	CString tempkch;
	tempkch.Format("%s_%d",strKch,index);
	MAP_STR_MYKCH::iterator KCHiter;	
	KCHiter = m_myKch.find(tempkch);	
	if(KCHiter == m_myKch.end())
	{
		WriteLog("����:%s%s ����δ����!",temptype,tempkch);
		return FALSE;
	}
	LONG templUserID=GetSignalStatic(KCHiter->second);
	if (templUserID==-5)
	{
		WriteLog("�źŶ�ʧ!����Ƶ�ź�!%s%s",temptype,fileName);
		return FALSE;
	}
	if (templUserID<0)
	{
		WriteLog("�豸״̬!%s%s",temptype,fileName);
		return FALSE;
	}
	//��ʼץ��
	if(CaptureJPEGPic(templUserID,KCHiter->second.m_Channel,fileName)==TRUE)
	{
		return TRUE;
	}
	WriteLog("%s%s",temptype,fileName);
	return FALSE;
}

LONG CCatchPhotoHKApp::OnHKLogin(char *sDVRIP, char *sUserName, char *sPassword, WORD wDVRPort)
{
	LONG lUserID;
	lUserID=-1;
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));	
	lUserID = NET_DVR_Login_V30(sDVRIP,wDVRPort,sUserName,sPassword, &struDeviceInfo);
	if (lUserID < 0)
	{
		WriteLog("��¼ʧ��!IP=%s,%s",sDVRIP,OnGetLastErrorData(NET_DVR_GetLastError()));
		return -1;
	}
	WriteLog("��¼OK!IP=%s,wDVRPort=%d",sDVRIP,wDVRPort);
	return lUserID;
}

UINT CCatchPhotoHKApp::GetNVRWorkState(LONG lUserID, int Channel)
{
// 	int itd=Channel-1;
// 	if (pStruWorkStateV40->struChanStatic[itd].bySignalStatic!=0)
// 	{
// 		//WriteLog("�źŶ�ʧ!����Ƶ�ź�!%s",Filename);
// 		return 2;
// 	}
	return 3;
}

LONG CCatchPhotoHKApp::GetSignalStatic(MYKCH myKch)
{
	MAP_STR_HKLOGIN::iterator IPiter;	
	IPiter = m_HKLogin.find(myKch.m_IPAddr);
	if (IPiter==m_HKLogin.end())//δ�ҵ�����
	{
		WriteLog("m_HKLogin find faild ������Ϣ�쳣!%s,%s",myKch.m_IPAddr,myKch.m_KCH);
		return -2;
	}
	if (IPiter->second.lUserID==-1) //���µ�¼�豸
	{
		//��¼�豸
		if (IPiter->second.bisDoLogin==false)
		{
			IPiter->second.bisDoLogin=true;
			IPiter->second.lUserID=OnHKLogin(IPiter->second.m_IPaddrL.GetBuffer(0),IPiter->second.m_User.GetBuffer(0),IPiter->second.m_Password.GetBuffer(0),IPiter->second.m_Port);			
			IPiter->second.bisDoLogin=false;
		}
		else
		{
			Sleep(3000);
		}
		if (IPiter->second.lUserID==-1)
		{
			return -1;
		}
	}
	if (timeGetTime()-IPiter->second.DGetWorkStateTime	> 15000) //���»�ȡ�豸״̬
	{
		IPiter->second.DGetWorkStateTime=timeGetTime();
		//����Ƶ�źŲ�ץ��
		NET_DVR_GETWORKSTATE_COND struWorkStateCond = {0};
		struWorkStateCond.dwSize =  sizeof(NET_DVR_GETWORKSTATE_COND);
		struWorkStateCond.byFindChanByCond = 0;
		struWorkStateCond.byFindHardByCond = 0;
		if (IPiter->second.pStruWorkStateV40==NULL)
		{
			IPiter->second.pStruWorkStateV40=new NET_DVR_WORKSTATE_V40;
			if (IPiter->second.pStruWorkStateV40==NULL)
			{
				WriteLog("�ڴ����ʧ��!%s",myKch.m_IPAddr);
				return -3;
			}
		}
		DWORD dwList = 0;
		if (!NET_DVR_GetDeviceConfig(IPiter->second.lUserID, NET_DVR_GET_WORK_STATUS,1,&struWorkStateCond,sizeof(NET_DVR_GETWORKSTATE_COND),&dwList,\
				IPiter->second.pStruWorkStateV40, sizeof(NET_DVR_WORKSTATE_V40)) ||(dwList != 0))
		{
			WriteLog("��ȡ�豸״̬ʧ��!%s",OnGetLastErrorData(NET_DVR_GetLastError()));
			return -4;
		}
							
	}
	if (IPiter->second.pStruWorkStateV40!=NULL)
	{
		if (IPiter->second.pStruWorkStateV40->struChanStatic[myKch.m_Channel-1].bySignalStatic!=0)
		{
			return -5;
		}
	}
	return IPiter->second.lUserID;

}
