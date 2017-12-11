// CTestProQueryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CTestProQuery.h"
#include "CTestProQueryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCTestProQueryDlg dialog

CCTestProQueryDlg::CCTestProQueryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCTestProQueryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCTestProQueryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCTestProQueryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCTestProQueryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCTestProQueryDlg, CDialog)
	//{{AFX_MSG_MAP(CCTestProQueryDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_MESSAGE(UM_SOCKETZ,OnSocketZ)//TCP 257
	ON_MESSAGE(UM_SOCKUDPZ,OnSocketUDPZ)//UDP
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCTestProQueryDlg message handlers

BOOL CCTestProQueryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	bThreadisRun=false;
	CString tempPath;
	TCHAR   exeFullPath[MAX_PATH];
	::GetModuleFileName(NULL,exeFullPath,MAX_PATH);
	tempPath=exeFullPath;
	ModulePath.Format("%s",tempPath.Left(tempPath.ReverseFind('\\')));
	configfile.Format("%s\\config.ini",ModulePath);//��ǰĿ¼��


	dwlogID=logz.Register("���Թ�����Ϣ.log");

	if (!OnInitSQL())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	if (!m_tcps.InitSockS(m_hWnd,6708,UM_SOCKETZ,dwlogID))//���տ�������TCP����
	{
		CDialog::OnCancel();
		return FALSE;
	}
	if (!jmqudp.InitSockU(m_hWnd,6710,UM_SOCKUDPZ))
	{
		CDialog::OnCancel();
		return FALSE;
	}
	jmqudp.SetSendAddr("127.0.0.1",6709);

	uSQLORTCP=GetPrivateProfileInt("CONFIG","SQLORTCP",0,configfile);
	if (uSQLORTCP==0)
	{		
		//AfxBeginThread(QueryDataThread,this);
		logz.WriteLog(dwlogID,"��ʼ�����! ��ѯ���ݿ�汾! SQLORTCP=0 Build 20171010 1415");
	}
	else
	{
		logz.WriteLog(dwlogID,"��ʼ�����! ���ϴ��Ͱ汾!   SQLORTCP=1 Build 20171010 1415");
	}

	PostMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
	jmqudp.SendPack(QueryStart,"",0,0);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCTestProQueryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCTestProQueryDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCTestProQueryDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCTestProQueryDlg::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize��NULL��δ��ʼ��
	{
		TRACE("��ʼ�����Ӷ���ʧ��\n");
		logz.WriteLog(dwlogID,"��ʼ�����Ӷ���ʧ��!CoInitialize NULL");
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
			logz.WriteLog(dwlogID,"�ɹ����ӵ����ݿ�!");
		}
		else
		{
			TRACE("�����ݿ�ʧ��!");
			logz.WriteLog(dwlogID,"�������ݿ�ʧ��!");
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

void CCTestProQueryDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(nType == SIZE_MINIMIZED)
	{ 
		ShowWindow(SW_HIDE); // ����С��ʱ������������ 
	}
}

void CCTestProQueryDlg::OnSocketUDPZ(WPARAM wParam,LPARAM lParam)//UDP
{
	// 	int iEvent = WSAGETSELECTEVENT(lParam); //��Ϣ��� 
	// 	int iErrorCode = WSAGETSELECTERROR(lParam);//������� 
	if ((LOWORD(lParam) & FD_READ)==FD_READ)
	{
		PACKTYPE type;
		int ikch;
		char *recvbuf=NULL;
		if(jmqudp.RecvFromSock(type,&recvbuf,ikch)==TRUE)
		{
			TRACE("[%d,%d,%s]\n",type,ikch,recvbuf);
			logz.WriteLog(dwlogID,"[type=[%d],������=[%d],recvbuf=[%s]]",type,ikch,recvbuf);		
			switch (type)
			{
			case AllCarNum:
				m_CarIntMap[ikch].m_KCH.Format("����%d",ikch);
				break;
			case JMQStart:	
				{		
					jmqudp.SendPack(QueryStart,"",0,0);
					uSQLORTCP=GetPrivateProfileInt("CONFIG","SQLORTCP",0,configfile);
					if (uSQLORTCP==0)
					{		
						if (bThreadisRun==false)
						{
							AfxBeginThread(QueryDataThread,this);
							bThreadisRun=true;
							logz.WriteLog(dwlogID,"������ѯ�߳�!");
						}
						else
						{
							logz.WriteLog(dwlogID,"��ѯ�߳��ѿ���!");
						}
						
					}
				}
				break;
			default:
				break;
			}

		}
		else
		{
			logz.WriteLog(dwlogID,"jmqudp.RecvFromSock FALSE!");
		}
	}
}

LRESULT CCTestProQueryDlg::OnSocketZ(WPARAM wParam,LPARAM lParam)
{
	int iEvent = WSAGETSELECTEVENT(lParam); //��Ϣ��� 
	int iErrorCode = WSAGETSELECTERROR(lParam);//������� 
	SOCKET CurSock = (SOCKET)wParam; //Ŀ��socket 
	CString recvTemp;
	switch(iEvent) 
	{ 
	case FD_ACCEPT:
		m_tcps.OnFDAccept();
		return 0;		
	case FD_READ:{
		recvTemp=m_tcps.RecvFromSock(CurSock);
		//jmqmanager.OnJGPTData(recvTemp);//����TCP���� ��Ŀ����
		OnCarToJGPTData(recvTemp);
		m_tcps.SendToClient(CurSock,recvTemp);
		return 0;}
	case FD_CLOSE:{
		closesocket(CurSock);
		logz.WriteLog(dwlogID,"TCP �ͻ����˳�! Port:%d",CurSock);
		return 0;}
	default: 
		return 0;
	}
	return 0;
}

BOOL CCTestProQueryDlg::OnCardTestJMQ()
{
	try
	{
		//if (m_pConn == NULL) return FALSE;
		m_pConn->CursorLocation =adUseClient;	
		_RecordsetPtr m_pRecordset;
		_CommandPtr m_pCommand;
		m_pRecordset.CreateInstance("ADODB.Recordset");		
		m_pCommand.CreateInstance("ADODB.Command");	
		m_pCommand->ActiveConnection = m_pConn;
		m_pCommand->CommandType = adCmdStoredProc;
		m_pCommand->CommandText = _bstr_t(_T("CardTestJMQ"));	
		m_pCommand->Parameters->Refresh(); 		
		m_pRecordset =m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
		//////////////////////////////////////////////////////////////////////////
		m_pCommand.Release();
		m_pCommand.Detach();
		_variant_t vat;
		int iKchtemp;
		CString strtemp,strKch,tempmsg,strzkzm;
		if(m_pRecordset != NULL)
		{
			while(!m_pRecordset->adoEOF)
			{
				vat =m_pRecordset->GetCollect("������");
				if (vat.vt != VT_NULL)
				{
					strtemp=(LPCSTR)_bstr_t(vat);
					strtemp.Replace("����","");
					strKch.Format("%s",strtemp);
					vat =m_pRecordset->GetCollect("׼��֤�����");
					if (vat.vt != VT_NULL)
					{
						strzkzm=(LPCSTR)_bstr_t(vat);
						iKchtemp=atoi(strKch);
						if(m_CarIntMap[iKchtemp].m_Zkzmbh!=strzkzm)
						{							
							m_CarIntMap[iKchtemp].m_Zkzmbh=strzkzm;//����׼��֤�����
							SQLDATE tempData;
							tempData.iKch=iKchtemp;
							strncpy(tempData.m_Zkzmbh,strzkzm,20);
							jmqudp.SendPack(M17C51,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,iKchtemp);
							logz.WriteLog(dwlogID,"M17C51��ȡ���µĿ���,׼��֤�����:%s",strzkzm);
						}
					}					
				}//������
				m_pRecordset->MoveNext();
			}//while
			m_pRecordset->Close();
			m_pRecordset.Release();
		}		
	}
	catch(_com_error e)
	{
		logz.WriteLog(dwlogID,"���ô洢���� CardTestJMQ ʧ��!ԭ��:%s",e.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}

BOOL CCTestProQueryDlg::OnprErrorN()
{
	try
	{
		if (m_pConn == NULL)
		{
			logz.WriteLog(dwlogID,"fun OnprErrorN m_pConn == NULL");
			PostMessage(WM_CLOSE,0,0);
			return FALSE;
		}
		CString sZkzm,errjlbh;
		MAP_INT_STUDENTDATA::iterator iter;
		for (iter=m_CarIntMap.begin();iter!=m_CarIntMap.end();iter++)
		{
			sZkzm.Format("%s",iter->second.m_Zkzmbh);
			if (sZkzm.IsEmpty())//׼��֤�����Ϊ��
			{
				//TRACE("׼��֤�����Ϊ��,%s \n",iter->second.m_KCH);
				continue;
			}
			errjlbh.Format("%d",iter->second.errorid);
			m_pConn->CursorLocation =adUseClient;	
			_RecordsetPtr m_pRecordset;
			_CommandPtr m_pCommand;
			m_pRecordset.CreateInstance("ADODB.Recordset");		
			m_pCommand.CreateInstance("ADODB.Command");	
			m_pCommand->ActiveConnection = m_pConn;
			m_pCommand->CommandType = adCmdStoredProc;
			m_pCommand->CommandText = _bstr_t(_T("pr_ErrorRecords"));	
			m_pCommand->Parameters->Refresh(); 	
			m_pCommand->Parameters->Item[_variant_t(_bstr_t("@ZKZM_Number"))]->Value =_variant_t(sZkzm);
			m_pCommand->Parameters->Item[_variant_t(_bstr_t("@Ejlbh"))]->Value =_variant_t(errjlbh);
			m_pRecordset =  m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
			//////////////////////////////////////////////////////////////////////////
			_variant_t vat;	
			int   retVal = -1;				
			//VretVal = m_pCommand->Parameters->GetItem(short(3))->Value;//����Ҳ��
			vat = m_pCommand->Parameters->GetItem(_variant_t("@intErrorCode"))->GetValue();		
			retVal = vat.lVal;
			m_pCommand.Release();
			m_pCommand.Detach();
			if(m_pRecordset != NULL && (!m_pRecordset->adoEOF))
			{
				TRACE("intErrorCode,%d \n",retVal);
				if ( retVal>0 && retVal<7)
				{
					SQLDATE tempData;
					tempData.iKch=iter->first;
					strncpy(tempData.m_Zkzmbh,sZkzm,20);
					switch(retVal)
					{
					case 1:{
						vat =m_pRecordset->GetCollect("��¼���");
						iter->second.errorid=atoi((LPCSTR)_bstr_t(vat));
						   }
					case 2:
					case 5:
						{						
						vat =m_pRecordset->GetCollect("��¼���");
						iter->second.errorid=atoi((LPCSTR)_bstr_t(vat));
						vat =m_pRecordset->GetCollect("������");
						iter->second.m_xmcode=(LPCSTR)_bstr_t(vat);
						strncpy(tempData.m_Msg,(LPCSTR)_bstr_t(vat),20);
						if (retVal==2)
						{
							jmqudp.SendPack(M17C52,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,iter->first);
							logz.WriteLog(dwlogID,"M17C52:������:%s,׼��֤:%s,XMCODE:%s",iter->second.m_KCH,iter->second.m_Zkzmbh,iter->second.m_xmcode);
						}
						else
						{
							jmqudp.SendPack(M17C55,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,iter->first);
							logz.WriteLog(dwlogID,"M17C55:������:%s,׼��֤:%s,XMCODE:%s",iter->second.m_KCH,iter->second.m_Zkzmbh,iter->second.m_xmcode);
						}						
						break;
						}
					case 3:
						{
							vat =m_pRecordset->GetCollect("��¼���");
							iter->second.errorid=atoi((LPCSTR)_bstr_t(vat));
							CString tempss;
							vat =m_pRecordset->GetCollect("������");
							tempss.Format("%s@%s",iter->second.m_xmcode,(LPCSTR)_bstr_t(vat));
							strncpy(tempData.m_Zkzmbh,tempss,20);
							vat =m_pRecordset->GetCollect("����ʱ��");
							strncpy(tempData.m_Msg,(LPCSTR)_bstr_t(vat),20);
							jmqudp.SendPack(M17C53,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,iter->first);
							logz.WriteLog(dwlogID,"M17C53:������:%s,׼��֤:%s,XMCODE:%s",iter->second.m_KCH,iter->second.m_Zkzmbh,iter->second.m_xmcode);
							break;
						}
					case 6:
						{
							iter->second.m_Zkzmbh=_T("");
							iter->second.m_xmcode=_T("");
							iter->second.errorid=0;
							vat =m_pRecordset->GetCollect("�ɼ�");
							strncpy(tempData.m_Msg,(LPCSTR)_bstr_t(vat),20);
							jmqudp.SendPack(M17C56,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,iter->first);
							logz.WriteLog(dwlogID,"M17C56:������:%s,׼��֤:%s,XMCODE:%s",iter->second.m_KCH,iter->second.m_Zkzmbh,iter->second.m_xmcode);
						}						
						break;
					default:
						break;
					}
				}
				m_pRecordset->Close();
				m_pRecordset.Release();
			}
		}
	}
	catch(_com_error e)
	{
		logz.WriteLog(dwlogID,"���ô洢���� pr_ErrorRecordsN  ʧ��! ԭ��:%s",e.ErrorMessage());
		return FALSE;
	}
	return TRUE;
}

UINT CCTestProQueryDlg::QueryDataThread(LPVOID pParam)
{
	Sleep(2000);
	CCTestProQueryDlg *pInfo=(CCTestProQueryDlg *)pParam;
	while(1)
	{
		if (WaitForSingleObject(pInfo->hExitThread, 0) == WAIT_OBJECT_0)
		{
			break;
		}	
		pInfo->OnCardTestJMQ();
		pInfo->OnprErrorN();
		Sleep(2000);

		if (WaitForSingleObject(pInfo->hExitThread, 0) == WAIT_OBJECT_0)
		{
			break;
		}
		if (pInfo->isprErrorN==false)
		{
			pInfo->OnprErrorN();
			Sleep(3000);
		}
		else
		{
			pInfo->OnprErrorN();
			Sleep(2000);
		}
	}
	return 0;
}

void CCTestProQueryDlg::OnCarToJGPTData(CString strmsg)
{
	CString strTemp;
	strTemp.Format("%s",strmsg);
	int ixhnum =strTemp.Replace('*',',');
	if(ixhnum==8 || ixhnum==9 )
	{
		int icmd;
		int ikch;
		CStringArray te;
		CutOutString(strTemp,te,',');
		TRACE("%d \n",te.GetSize());
//		if(te.GetSize()==9)
//		{
			ikch=atoi(te.GetAt(1));
			if (OnFindCar(ikch))
			{
				icmd=atoi(te.GetAt(2));
				CString zkzmbh;
				zkzmbh.Format("%s",te.GetAt(6));
				SQLDATE tempData;
				tempData.iKch=ikch;
				strncpy(tempData.m_Zkzmbh,zkzmbh,20);
				switch (icmd)
				{
				case 1:
					{
						jmqudp.SendPack(M17C51,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,ikch);
						logz.WriteLog(dwlogID,"M17C51�յ��µĿ���,׼��֤�����:%s",zkzmbh);
					}
					break;
				case 2:
					{
						strncpy(tempData.m_Msg,te.GetAt(5),20);
						jmqudp.SendPack(M17C52,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,ikch);
						logz.WriteLog(dwlogID,"M17C52:������:%d,׼��֤:%s,XMCODE:%s",ikch,zkzmbh,te.GetAt(5));
					}
					break;
				case 3:
					{
						strncpy(tempData.m_Zkzmbh,te.GetAt(5),20);
						strncpy(tempData.m_Msg,te.GetAt(7),20);
						jmqudp.SendPack(M17C53,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,ikch);
						logz.WriteLog(dwlogID,"M17C53:������:%d,׼��֤:%s,XMCODE:%s,%s",ikch,te.GetAt(6),te.GetAt(5),te.GetAt(7));
	
					}
					break;
				case 5:
					{
						strncpy(tempData.m_Msg,te.GetAt(5),20);
						jmqudp.SendPack(M17C55,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,ikch);
						logz.WriteLog(dwlogID,"M17C55:������:%d,׼��֤:%s,XMCODE:%s",ikch,zkzmbh,te.GetAt(5));
					}
					break;
				case 6:
					{
						strncpy(tempData.m_Msg,te.GetAt(5),20);
						jmqudp.SendPack(M17C56,(char *)&tempData,sizeof(SQLDATE),"127.0.0.1",6709,ikch);
						logz.WriteLog(dwlogID,"M17C56:������:%d,׼��֤:%s,XMCODE:%s",ikch,zkzmbh,te.GetAt(5));		
					}
					break;
				default:
					break;
				}
			}else
			{
				logz.WriteLog(dwlogID,"OnJGPTData OnFindCar Error:%s",strTemp);
				jmqudp.SendPack(QueryStart,"",0,0);
			}
		//}
	}
	else
	{
		logz.WriteLog(dwlogID,"OnCarToJGPTData *�Ÿ�������ȷ.Error:%s",strTemp);
	}
	
}

BOOL CCTestProQueryDlg::OnFindCar(int ikch)
{
	MAP_INT_STUDENTDATA::iterator iter;	
	iter = m_CarIntMap.find(ikch);	
	if(iter == m_CarIntMap.end())
	{
		logz.WriteLog(dwlogID,"OnFindCar not find car [%d]",ikch);
		return FALSE;
	}
	return TRUE;
}

void CCTestProQueryDlg::CutOutString(CString source,  CStringArray& dest, char division)
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

void CCTestProQueryDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
//	jmqudp.SendPack(QueryStart,"",0,0);
	CDialog::OnTimer(nIDEvent);
}
