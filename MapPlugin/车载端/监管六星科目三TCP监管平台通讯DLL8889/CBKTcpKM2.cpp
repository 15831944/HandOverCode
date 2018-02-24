// CBKTcpKM2.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "CBKTcpKM2.h"
#include "afxmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DLLEXPORTZ extern "C" __declspec( dllexport )
#define SAVEDAYS 15 //LOG��������
CEvent g_SENDLIST_EVT;
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
// CCBKTcpKM2App
//#define  DLLEXPORTZ
BEGIN_MESSAGE_MAP(CCBKTcpKM2App, CWinApp)
	//{{AFX_MSG_MAP(CCBKTcpKM2App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKTcpKM2App construction

CCBKTcpKM2App::CCBKTcpKM2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKTcpKM2App object

CCBKTcpKM2App theApp;

/////////////////////////////////////////////////////////////////////////////
//HANDLE HsendThread;
//int conniT=0;
DWORD WINAPI SendThreadProc(LPVOID param)
{
	theApp.WriteLogOne("�����̴߳���!SendThreadProc Create!");
	while (TRUE)
	{
		if (theApp.ConnOK==FALSE)
		{
			break;
		}
  		if (theApp.SendData()==FALSE)
		{
			Sleep(1000);
		}
		else
		{
			Sleep(1000);
			//theApp.WriteLogThree("SendThreadProc ");
		}
	}
	theApp.WriteLogOne("�����̹߳ر�!SendThreadProc Close!");
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CCBKTcpKM2App::WriteLogOne(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	CString Data;
	SYSTEMTIME st;
	GetLocalTime(&st);
	Data.Format("%02d:%02d:%02d.%03d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	FILE *fp=fopen(logFileOne,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}

void CCBKTcpKM2App::WriteLogTwo(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	CString Data;
	SYSTEMTIME st;
	GetLocalTime(&st);
	Data.Format("%02d:%02d:%02d.%03d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	FILE *fp=fopen(logFileTwo,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}

void CCBKTcpKM2App::WriteLogThree(LPCTSTR pstrFormat, ...)
{
	CString logstr;
	va_list avlist;
	va_start(avlist, pstrFormat);
	logstr.FormatV(pstrFormat, avlist);
	va_end(avlist);
	
	CString Data;
	SYSTEMTIME st;
	GetLocalTime(&st);
	Data.Format("%02d:%02d:%02d.%03d",st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	FILE *fp=fopen(logFileThree,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,logstr);
	fclose(fp);
}

//////////////////////////////////////////////////////////////////////////

BOOL CCBKTcpKM2App::InitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	//////////////////////////////////////////////////////////////////////////
	CString strPath,tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	strPath.Format("%s\\log",tempPath.Left(tempPath.ReverseFind('\\')));
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
		CreateDirectory(tempPath,NULL);//
	}
	logFileOne.Format("%s\\CBKTcpKM2One��־�ļ�.log",tempPath);
	logFileTwo.Format("%s\\CBKTcpKM2Two��־�ļ�.log",tempPath);
	logFileThree.Format("%s\\CBKTcpKM2Three��־�ļ�.log",tempPath);
	//////////////////////////////////////////////////////////////////////////
	WriteLogOne("InitInstance START CBKTcpKM2 DLL! 20170530 1119 km3");
	//////////////////////////////////////////////////////////////////////////
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	
	wVersionRequested = MAKEWORD( 2, 2 );
	
	err = WSAStartup( wVersionRequested, &wsaData );
	if ( err != 0 ) {
		WriteLogOne("WSAStartup failed!");
		return FALSE;
	}
	
	
	if ( LOBYTE( wsaData.wVersion ) != 2 ||
        HIBYTE( wsaData.wVersion ) != 2 ) {
		
		WSACleanup();
		WriteLogOne("WSAStartup Version failed!");
		return FALSE; 
	}
	if(m_socketz != NULL)
    {
		closesocket(m_socketz);
		m_socketz = NULL;
	}
	CString JGPTIP;
	GetPrivateProfileString("CONFIG","Xmip","",JGPTIP.GetBuffer(MAX_PATH),MAX_PATH,".\\config.dat");
	JGPTIP.ReleaseBuffer();
	addrSrv.sin_addr.S_un.S_addr=inet_addr(JGPTIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8889);
	WriteLogOne(JGPTIP+"1:9889 20170521");
	//////////////////////////////////////////////////////////////////////////
	sendi=1;
	ConnOK=FALSE;
	UINT tempcardnum;
	tempcardnum=GetPrivateProfileInt("ICODE","Icode",99,".\\config.dat");
	CardNumi=tempcardnum+1;
	r11=FALSE;
	r55=FALSE;
	fzb=TRUE;
	//////////////////////////////////////////////////////////////////////////
	CString linestr,getstr;
	if (GetPrivateProfileString("DATA","DATA","",getstr.GetBuffer(MAX_PATH),MAX_PATH,".\\CSTRNOTSEND.dll"))
	{
		linestr.Format("%s",curTime.Format("%Y-%m-%d"));
		getstr.ReleaseBuffer();
		if (getstr !=linestr)
		{
			CFileFind FindLogFile;
			if (FindLogFile.FindFile("CSTRNOTSEND.dll"))
			{
				CFile::Remove("CSTRNOTSEND.dll");
			}
			FindLogFile.Close();
		}
	}
	CString msendbuf;
	msendbuf.Format("&TIME9-%d-%d*TIME9*****#",CardNumi,rand()%100);
	m_sendList.AddTail(msendbuf);
	int forcount=GetPrivateProfileInt("DATA","COUNTL",0,".\\CSTRNOTSEND.dll");
	for (int li=0;li<forcount;li++)
	{
		linestr.Format("%d",li);
		if(GetPrivateProfileString("LISTSTR",linestr,"",getstr.GetBuffer(MAX_PATH),MAX_PATH,".\\CSTRNOTSEND.dll"))
		{
			getstr.ReleaseBuffer();
			WriteLogOne("δ�ϴ�����:"+getstr);
			m_sendList.AddTail(getstr);
		}
	}
	CFileFind FindLogFile;
	if (FindLogFile.FindFile("CSTRNOTSEND.dll"))
	{
		CFile::Remove("CSTRNOTSEND.dll");
	}
	//////////////////////////////////////////////////////////////////////////
	g_SENDLIST_EVT.SetEvent();


	for(int i=0;i<20;i++)
	{
		mytime[i].T52ST=1000;
		mytime[i].T52SPAN=1000;
		mytime[i].T53ST=1000;
		mytime[i].T53SPAN=1000;
		mytime[i].T55ST=1000;
		mytime[i].T55SPAN=1000;
		mytime[i].T52OK=1000;
		mytime[i].T55OK=1000;
	}
	TC53=timeGetTime();
	sendtime=timeGetTime();
	XMi=0;
	SXMi=0;
	connSumi=0;
	iSendCount=0;

	return CWinApp::InitInstance();
}

int CCBKTcpKM2App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_socketz)
		closesocket(m_socketz);
	WSAAsyncSelect(m_socketz, m_hWndz, 0, 0);	//�ͷ��׽����봰�ھ���Ĺ���
	WSACleanup();
	//////////////////////////////////////////////////////////////////////////
	CFileFind FindLogFile;
	if (FindLogFile.FindFile("CSTRNOTSEND.dll"))
	{
		CFile::Remove("CSTRNOTSEND.dll");
	}
	//////////////////////////////////////////////////////////////////////////
	CString linestr,writestr;
	int countk=m_sendList.GetCount();
	if (countk>0)
	{
		COleDateTime curTime =COleDateTime::GetCurrentTime();
		linestr.Format("%s",curTime.Format("%Y-%m-%d"));
		::WritePrivateProfileString("DATA","DATA",linestr,".\\CSTRNOTSEND.dll");
		linestr.Format("%d",countk);
		::WritePrivateProfileString("DATA","COUNTL",linestr,".\\CSTRNOTSEND.dll");
	}
	for (int li=0;li<countk;li++)
	{
		linestr.Format("%d",li);
		writestr.Format("%s",m_sendList.GetHead());
		::WritePrivateProfileString("LISTSTR",linestr,writestr,".\\CSTRNOTSEND.dll");
		m_sendList.RemoveHead();
	}
	//////////////////////////////////////////////////////////////////////////
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
	//////////////////////////////////////////////////////////////////////////
	WriteLogOne("ExitInstance CBKTcpKM2 DLL!");
	return CWinApp::ExitInstance();
}
//////////////////////////////////////////////////////////////////////////
void CCBKTcpKM2App::DeleteDirectory(CString strDir)
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
//////////////////////////////////////////////////////////////////////////

int conniii=0;
UINT CCBKTcpKM2App::ConnentThread(LPVOID Pparam)
{
	if(conniii>1)
	{
		return 1;
	}
	conniii+=1;
	CCBKTcpKM2App *dlg=(CCBKTcpKM2App *)Pparam;
	while (TRUE)
	{
		if (dlg->ConnOK==TRUE)
		{
			break;
		}
		connect(dlg->m_socketz,(SOCKADDR*)&dlg->addrSrv,sizeof(SOCKADDR));
		dlg->WriteLogOne("Connect .....Wait!");		
		Sleep(3000);
	}
	dlg->WriteLogOne("ConnentThread .....break!");
	conniii-=1;
	return 0;
}

//////////////////////////////////////////////////////////////////////////
BOOL CCBKTcpKM2App::InitSocketz(HWND hwndz)
{
	m_hWndz =hwndz;
	m_socketz = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//�����͵��׽��� SOCK_STREAM
	//ASSERT(m_socketz != NULL);
	if(m_socketz == INVALID_SOCKET)
	{
		AfxMessageBox("�����׽���ʧ��!");
		WriteLogOne("socket failed !");
		closesocket(m_socketz);
		WSACleanup();
		return FALSE;
	}
	//�������¼����¼������������
	if(WSAAsyncSelect(m_socketz, m_hWndz, UM_SOCKETZ, FD_CONNECT|FD_READ|FD_CLOSE) ==SOCKET_ERROR)
	{
		WriteLogOne("WSAAsyncSelect failed !");
		return FALSE;
	}
	ConnOK=FALSE;
	AfxBeginThread(CCBKTcpKM2App::ConnentThread,this);	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

void CCBKTcpKM2App::OnConnect(SOCKET CurSock, int error)
{
	if(0 ==error)
	{
		if(CurSock == m_socketz)
		{
			ConnOK=TRUE;
			WriteLogOne("�ɹ����ӵ�������!");
			::CreateThread(NULL,0,SendThreadProc,NULL,0,0);
		}
	}
	else
	{
		WriteLogOne("���ӵ�������ʧ��!");
	}
}

void CCBKTcpKM2App::OnClose(SOCKET CurSock)
{
//��������Ӧ�Ŀͻ��˵�ͨ�ţ��ͷ���Ӧ��Դ
	WriteLogOne("Socket �Ͽ�,׼������...");	
	if(m_socketz)
		closesocket(m_socketz);
	WSAAsyncSelect(m_socketz, m_hWndz, 0, 0);
	m_socketz = socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);//�����͵��׽��� SOCK_STREAM
	if(m_socketz == INVALID_SOCKET)
	{
		WriteLogOne("OnClose socket failed !");
		closesocket(m_socketz);
		WSACleanup();
		return ;
	}
	if(WSAAsyncSelect(m_socketz, m_hWndz, UM_SOCKETZ, FD_CONNECT|FD_READ/*|FD_WRITE*/|FD_CLOSE) ==SOCKET_ERROR)
	{
		WriteLogOne("WSAAsyncSelect failed !");
	}
	ConnOK=FALSE;
	CString JGPTIP;
	GetPrivateProfileString("CONFIG","Xmip","",JGPTIP.GetBuffer(MAX_PATH),MAX_PATH,".\\config.dat");
	JGPTIP.ReleaseBuffer();
	addrSrv.sin_addr.S_un.S_addr=inet_addr(JGPTIP);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(8889);
	AfxBeginThread(CCBKTcpKM2App::ConnentThread,this);

}
void CCBKTcpKM2App::OnReceive(SOCKET CurSock)
{
//�������绺�����е����ݰ�
	WSABUF wsabuf;
	wsabuf.buf=new char[200];
	wsabuf.len=200;
	memset(wsabuf.buf,0,200);
	DWORD dwRead;
	DWORD dwFlag=0;
	SOCKADDR_IN addrFrom;
	int len=sizeof(SOCKADDR);
	CString strTemp;
	if(SOCKET_ERROR==WSARecvFrom(CurSock,&wsabuf,1,&dwRead,&dwFlag,
		(SOCKADDR*)&addrFrom,&len,NULL,NULL))
	{
		WriteLogOne("��������ʧ��!");
		return ;
	}
	CString temp;
	temp.Format("%s",wsabuf.buf);	
//	theApp.WriteLogOne("OnReceive(%d,%s)",CurSock,temp);//201705312048 ����־
	if (1 ==temp.Replace("*","*"))
	{
		CString tmp1,tmp2;
		tmp1.Format("%s",temp.Left(temp.Find("*")));
		if (tmp1.Find("17Z02")!= -1)
		{
			theApp.WriteLogThree("20170531 2104 17Z02�յ�[%s]",temp);//201705312048 ����־
			str17Z02.Format("%s",temp.Mid(temp.Find("*")+1));
			return ;
		}
		BOOL bIsListEmpty;

		WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
		bIsListEmpty = m_sendList.IsEmpty();
		g_SENDLIST_EVT.SetEvent();

		if(!bIsListEmpty)
		{
			WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
			tmp2=m_sendList.GetHead();
			g_SENDLIST_EVT.SetEvent();

			if (tmp2.Find(tmp1) != -1)
			{
				WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
				m_sendList.RemoveHead();//ɾ����һ��
				g_SENDLIST_EVT.SetEvent();
				iSendCount=0;
				if (tmp1.Find("17C52")!= -1)
				{
					mytime[SXMi].T52OK =timeGetTime();
				}
				if (tmp1.Find("17C55")!= -1)
				{
					mytime[SXMi].T55OK =timeGetTime();
					SXMi+=1;
				}					
				int errori=GetLastErrorStr(temp);
			}
			else
			{
				CString logstr;
				logstr.Format("����ֵ���б�ֵ��һ��:[%s!=%s]",tmp1,tmp2);
				WriteLogOne(logstr);
			}
		}
		
	}
	else
	{
		WriteLogOne("����ֵ*�Ŵ���һ��");
	}
	delete[] wsabuf.buf;//ɾ��
	connSumi=0;
}
//////////////////////////////////////////////////////////////////////////

BOOL CCBKTcpKM2App::SendData()
{
	
	BOOL bIsListEmpty;
	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	bIsListEmpty = m_sendList.IsEmpty();
	g_SENDLIST_EVT.SetEvent();

	if (bIsListEmpty)
	{
		//WriteLog("û��Ҫ���͵�����!,size=0");
		return FALSE;
	}
	if (timeGetTime()-sendtime<900)
	{
		return TRUE;
	}
	sendtime=timeGetTime();

	CString msendbuf;

	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	msendbuf.Format("%s",m_sendList.GetHead());
	g_SENDLIST_EVT.SetEvent();


	if (msendbuf.Left(6)=="&17C52")
	{
		int tempii;
		if(0==SXMi)
		{
			tempii=0;
		}
		else
		{
			tempii=SXMi-1;
		}
		if(timeGetTime()-mytime[tempii].T55OK < mytime[SXMi].T52SPAN)
		{
			return TRUE;
		}
	}
	if (msendbuf.Left(6)=="&17C53")
	{
		if(timeGetTime()-mytime[SXMi].T52OK < mytime[SXMi].T53SPAN)
		{
			return TRUE;
		}
		if(timeGetTime()-TC53<1000)
		{
			return TRUE;
		}
		TC53=timeGetTime();
	}
	if (msendbuf.Left(6)=="&17C55")
	{
		if(timeGetTime()-mytime[SXMi].T52OK < mytime[SXMi].T55SPAN+1000)
		{
			return TRUE;
		}
	}
	if (msendbuf.Left(6)=="&17C56")
	{
		if(timeGetTime()-TC53<3000)
		{
			return TRUE;
		}
		TC53=timeGetTime();
	}
	int len;
	len=msendbuf.GetLength();	
	WSABUF wsabuf;
	DWORD dwSend;
	wsabuf.buf=msendbuf.GetBuffer(len);	
	wsabuf.len=len+1;
	iSendCount+=1;
	if (iSendCount >=10)
	{
		iSendCount=0;
		OnClose(0);
		WriteLogOne("��������10��û�н��յ�����");
	}
	if(SOCKET_ERROR==WSASendTo(m_socketz,&wsabuf,1,&dwSend,0,
		(SOCKADDR*)&addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		WriteLogOne("���͵�����ʧ��!WSASendTo:"+msendbuf);
		connSumi+=1;
		if(connSumi>10)
		{
			iSendCount=0;
			connSumi=0;
			OnClose(0);
			WriteLogOne("OnClose(0)");
		}
		return TRUE;
	}

	WriteLogTwo("����"+msendbuf);
//	theApp.TSend[theApp.xmii] =timeGetTime();
	return TRUE;
}

int CCBKTcpKM2App::GetLastErrorStr(CString str)
{
	CString templ,tempr,msgstr;
	templ.Format("%s",str.Left(5));
	tempr.Format("%s",str.Mid(str.Find("*")+1));
	GetPrivateProfileString(templ,tempr,"δ֪����ֵ",msgstr.GetBuffer(MAX_PATH),MAX_PATH,".\\CBKTcpKM2.dat");
	msgstr.ReleaseBuffer();

	WriteLogTwo(str+msgstr);
	fzb=TRUE;
	if (templ=="17Z02")
	{
		str17Z02.Format("%s",tempr);
		return 1;
	}
	if(!strcmp(tempr,"1"))
	{	
		if (templ=="17C51")
		{
			r11=TRUE;
		}
		if (templ=="17C55")
		{
			r55=TRUE;
		}
		return 1;
	}
	if (!strcmp(tempr,"-90"))
	{	
		if (templ=="17C51")
		{
			r11=TRUE;
		}
		if (templ=="17C55")
		{
			r55=TRUE;
		}
		return -90;
	}
	if (!strcmp(tempr,"-2"))
	{
		if (templ=="17C52")
		{
			D45 = TRUE;
			return 1;
		}
	}
	if (!strcmp(tempr,"-91"))
	{
		if (templ=="17C52")
		{
			B91 = TRUE;
			return 1;
		}
	}
	//20160804
	if (templ=="17C56")
	{
		if (strcmp(tempr,"-1"))
		{
			b17C56fz=TRUE;
		}
	}	
	if ("TIME9"==templ)
	{
		SetSysTime(tempr);
		return 1;	
	}
	fzb=FALSE;
	if (!strcmp(tempr,"$E"))//$E
	{
		AfxMessageBox(msgstr);
		return -1000;
	}
	WriteLogTwo("ERRORMSG:"+msgstr);
	AfxMessageBox(msgstr);
	return atoi(tempr);
}

void CCBKTcpKM2App::SaveTime(int timei)
{
	CString strlog;
	switch(timei)
	{
	case 1:
		XMi=0;
		SXMi=0;
		mytime[0].T55ST=timeGetTime();
		mytime[0].T55OK=timeGetTime();
		break;
	case 2:
		{
		mytime[XMi].T52ST=timeGetTime();
		int tempi;
		if(XMi ==0)
		{
			tempi=0;
		}
		else
		{
			tempi=XMi-1;
		}
		mytime[XMi].T52SPAN =(mytime[XMi].T52ST - mytime[tempi].T55ST)/2;

		strlog.Format("T52SPAN= %ld,XMi=%d,tempi=%d",mytime[XMi].T52SPAN,XMi,tempi);
		WriteLogThree(strlog);
		break;
		}
	case 3:
		{
		mytime[XMi].T53ST=timeGetTime();
		mytime[XMi].T53SPAN =mytime[XMi].T53ST - mytime[XMi].T52ST;

		strlog.Format("T53SPAN= %ld,XMi=%d",mytime[XMi].T53SPAN,XMi);
		WriteLogThree(strlog);
		break;
		}
	case 4:
		break;
	case 5:
		{
		mytime[XMi].T55ST=timeGetTime();
		mytime[XMi].T55SPAN =mytime[XMi].T55ST - mytime[XMi].T52ST;
		strlog.Format("T55SPAN= %ld,XMi=%d",mytime[XMi].T55SPAN,XMi);
		WriteLogThree(strlog);

		XMi+=1;
		break;
		}
	case 6:
		break;
	default:
		break;
	}
}

BOOL CCBKTcpKM2App::SetSysTime(CString strTime)
{
	COleDateTime tm; // 
	SYSTEMTIME st;
	tm.ParseDateTime(strTime); //ת��ΪCOleDtaTimeʱ��
	tm.GetAsSystemTime(st); // ��ȡSYSTEMTIMEʱ��

	if(0==SetLocalTime(&st))//����ϵͳʱ��
	{		
		WriteLogOne("���ñ���ʱ��Ϊ:%s ʧ��!",strTime);
		return FALSE;
	}	
	WriteLogOne("���ñ���ʱ��Ϊ:%s �ɹ�!",strTime);
	return TRUE;
}

BOOL CCBKTcpKM2App::DataIsEmpty()
{
	BOOL bIsListEmpty;
	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	bIsListEmpty = m_sendList.IsEmpty();
	g_SENDLIST_EVT.SetEvent();
	if (b17C56fz==TRUE)
	{
		return TRUE;
	}
	if (bIsListEmpty)
	{
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
DLLEXPORTZ BOOL DInitSocket(HWND hwndd)
{
	if (!theApp.InitSocketz(hwndd))
	{
		theApp.WriteLogOne("InitSocketz faild!");
		return FALSE;
	}
	return TRUE;
}
DLLEXPORTZ LRESULT OnSockTGPS(WPARAM wParam,LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam); //��Ϣ��� 
	int iErrorCode = WSAGETSELECTERROR(lParam);//������� 
	SOCKET CurSock = (SOCKET)wParam; //Ŀ��socket 
	switch(iEvent) 
	{ 
	case FD_CONNECT: //�ͻ������������¼� 
		theApp.OnConnect(CurSock,iErrorCode);
		return 0;
	case  FD_READ:
		theApp.OnReceive(CurSock);
		return 0;
	case FD_CLOSE:
		theApp.OnClose(CurSock);
		return 0;
// 	case FD_WRITE:
// 		theApp.OnSend(CurSock);
// 		break;
	default: 
		return 0;
	}
	return 0;
}

DLLEXPORTZ BOOL SendString(UINT packi,LPTSTR strs)
{
	CString strlog;
	if (123==packi)
	{
		if (theApp.ConnOK==TRUE)//���ӳɹ�
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	if (91==packi)//�����ӵ�
	{
		return theApp.B91;
	}
	if (11==packi)
	{
//		theApp.SendData();
		return theApp.r11;
	}
	if (45==packi)
	{
//		theApp.SendData();
		return theApp.D45;
	}
	if (66==packi)
	{
		if (theApp.DataIsEmpty()==FALSE)
		{
			theApp.WriteLogThree("66 date TRUE");
			return TRUE;
		}
		else
		{
			theApp.WriteLogThree("66 date FALSE");
			return FALSE;
		}
	}
	if (55==packi)
	{
		return FALSE;
	}
	if (999==packi)
	{
		return theApp.fzb;
	}
	if (22==packi)
	{
		return theApp.fzb;
	}
	if (1==packi)
	{
		theApp.b17C56fz=FALSE;
		theApp.SaveTime(1);
		theApp.r11=FALSE;
	}
	if (2==packi)
	{
		theApp.b17C56fz=FALSE;
		theApp.B91=FALSE;
		theApp.D45 = FALSE;
		theApp.SaveTime(2);//����17C52��ʱ��
		theApp.r55=FALSE;
	}
	if(3==packi)
	{
		theApp.SaveTime(3);
	}
	if (5==packi)
	{
		theApp.SaveTime(5);//
		theApp.r55=FALSE;
	}

	CString msendbuf;
	theApp.sendi+=1;
	if (201==packi )
	{
		msendbuf.Format("&17Z01-%d-%d%s#",theApp.CardNumi,theApp.sendi,strs);
		WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
		theApp.Send17Z02(msendbuf);
		g_SENDLIST_EVT.SetEvent();
		return TRUE;
	}
	else if (202==packi)
	{
		msendbuf.Format("&17Z02-%d-%d%s#",theApp.CardNumi,theApp.sendi,strs);
		WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
		theApp.Send17Z02(msendbuf);
		g_SENDLIST_EVT.SetEvent();
		return TRUE;
	}
	else if (208==packi)
	{
		msendbuf.Format("&17Z08-%d-%d%s#",theApp.CardNumi,theApp.sendi,strs);
	}
	else if (209==packi)
	{
		msendbuf.Format("&17Z09-%d-%d%s#",theApp.CardNumi,theApp.sendi,strs);
	}
	else
	{
		msendbuf.Format("&17C5%d-%d-%d%s#",packi,theApp.CardNumi,theApp.sendi,strs);
	}
	
	theApp.WriteLogThree(msendbuf);

	WaitForSingleObject(g_SENDLIST_EVT.m_hObject,20);
	theApp.m_sendList.AddTail(msendbuf);
	g_SENDLIST_EVT.SetEvent();

	return TRUE;
}

DLLEXPORTZ char* Get17Z02()
{
	try
	{
		theApp.WriteLogThree("Get17Z02(%s)",theApp.str17Z02);
		return theApp.str17Z02.GetBuffer(0);
	}
	catch (...)
	{
		theApp.WriteLogThree("Get17Z02 Exception");
		return "";
	}
}

void CCBKTcpKM2App::Send17Z02(CString str)
{
	CString msendbuf;
	msendbuf.Format("%s",str);
	int len;
	len=msendbuf.GetLength();	
	WSABUF wsabuf;
	DWORD dwSend;
	wsabuf.buf=msendbuf.GetBuffer(len);	
	wsabuf.len=len+1;
	
	if(SOCKET_ERROR==WSASendTo(m_socketz,&wsabuf,1,&dwSend,0,
		(SOCKADDR*)&addrSrv,sizeof(SOCKADDR),NULL,NULL))
	{
		WriteLogOne("���͵�����ʧ��!%s",msendbuf);		
	}
	else
	{
		WriteLogThree("����:%s",msendbuf);
	}
}