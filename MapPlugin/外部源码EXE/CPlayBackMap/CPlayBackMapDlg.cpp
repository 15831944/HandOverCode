// CPlayBackMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CPlayBackMap.h"
#include "CPlayBackMapDlg.h"

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
// CCPlayBackMapDlg dialog

CCPlayBackMapDlg::CCPlayBackMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPlayBackMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPlayBackMapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCPlayBackMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPlayBackMapDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCPlayBackMapDlg, CDialog)
	//{{AFX_MSG_MAP(CCPlayBackMapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPlayBackMapDlg message handlers

BOOL CCPlayBackMapDlg::OnInitDialog()
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

	DWORD dwPidIfExist;	
	if (TRUE == bDemonsPorisRun(&dwPidIfExist,"CPlayBackMap.exe"))//����
	{		
		HANDLE hProcessForWait;
		hProcessForWait = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwPidIfExist);
		TerminateProcess(hProcessForWait, 0);			
	}
	if ( !(_tcscmp(__targv[1],_T("1346708777"))))
	{
		CDialog::OnCancel();
		return FALSE;
	}
	if (!OnInitSQL())
	{
		CDialog::OnCancel();
		return FALSE;
	}	
	CString tempPath;
	tempPath.Format("D:\\Temp");
	DWORD dwAttr=GetFileAttributes(tempPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(tempPath,NULL);//
	}
	COleDateTime curTime =COleDateTime::GetCurrentTime();
	strPath.Format("%s\\%s",tempPath,curTime.Format("%Y-%m-%d"));
	dwAttr=GetFileAttributes(strPath);
	if(dwAttr==0xFFFFFFFF) 
	{
		CreateDirectory(strPath,NULL);//
	}
	//////////////////////////////////////////////////////////////////////////
	if (DownMapLog()==TRUE)
	{
		//���̷߳���UDP
		AfxBeginThread(SendThread,(LPVOID)this);
	}
	else
	{
		CDialog::OnCancel();
	}

	SetTimer(1,30,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BOOL CCPlayBackMapDlg::bDemonsPorisRun(DWORD* pdwPID,CString strName)
{
	PROCESSENTRY32 processEntry32 = {0};
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		//AfxMessageBox("CreateToolhelp32Snapshot() Failed!!!");
		return FALSE;
	}
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hProcessSnap,&processEntry32);
	CString strModuleNames;
	while (bRet)
	{
		//char szName[20] = "CHMQ.exe";//������Ϊ && ����ID��Ϊ������ID
		if (!StrCmpNI(processEntry32.szExeFile,(LPCTSTR)strName,strName.GetLength()) && //��Ⱦͷ���0
			GetCurrentProcessId() != processEntry32.th32ProcessID ) //��������ͬ���Ľ���
		{
			CloseHandle(hProcessSnap); 
			*pdwPID = processEntry32.th32ProcessID;
			return TRUE;	
		}	
		
		bRet = Process32Next(hProcessSnap,&processEntry32);
	}
	return FALSE;
	
}

void CCPlayBackMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCPlayBackMapDlg::OnPaint() 
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
HCURSOR CCPlayBackMapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCPlayBackMapDlg::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize��NULL��δ��ʼ��
	{
		TRACE("��ʼ�����Ӷ���ʧ��\n");
		AfxMessageBox("��ʼ�����Ӷ���ʧ��!CoInitialize NULL");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,"D:\\bkzyMapOcx\\config.ini");
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
		//Sqlstr +=";OLEDB.Net=True;PLSQLRSet=1;Persist Security Info=True;";//
		// 		AfxMessageBox(Sqlstr);
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("�����ݿ�ɹ�!");
		}
		else
		{
			AfxMessageBox("�����ݿ�ʧ��!");
			return FALSE;
		}
	}
	catch(_com_error e)		//��׽�쳣
	{
		CString errormessage;
		errormessage.Format("���ݿ���ʾ,�������ݿ�ʧ��!,������Ϣ:%s,�����������ݿ�����",e.ErrorMessage());
		AfxMessageBox(errormessage);
		return FALSE;
	}
	return TRUE;
}

BOOL CCPlayBackMapDlg::DownMapLog()
{

	fileName.Format("%s\\%s_%s_%s.dat",strPath,__targv[1],__targv[2],__targv[3]);
	CFileFind mffind;
	if (mffind.FindFile(fileName)) //��ͼ�ļ��Ѵ���
	{
		CFile::Remove(fileName);
	}
	mffind.Close();
	try
	{	
		_RecordsetPtr pRecordsetz;
		pRecordsetz.CreateInstance("ADODB.Recordset");
		pRecordsetz->CursorLocation = adUseClient;
		_variant_t vat;
		CString strSQL;
		strSQL.Format("SELECT * FROM MAPLOG WHERE ׼��֤�����='%s' AND ���Դ��� ='%s' AND ���մ���='%s'",__targv[1],__targv[2],__targv[3]);
		pRecordsetz->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);		
		if(pRecordsetz != NULL && (!pRecordsetz->adoEOF))
		{
			char* pbuf = NULL;
			long lDataSize = pRecordsetz->GetFields()->GetItem("��ͼ����")->ActualSize; //���ݿ������ݳ���
			if (lDataSize > 0)
			{
				_variant_t	varBLOB;
				LPVOID m_pBuffer;
				varBLOB = pRecordsetz->GetFields()->GetItem("��ͼ����")->GetChunk(lDataSize);
				m_pBuffer = new char[lDataSize+1];
				if (varBLOB.vt == (VT_ARRAY|VT_UI1))
				{
					SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
					memcpy(m_pBuffer, pbuf, lDataSize);//�������ݵ�������m_pBMPBuffer
					SafeArrayUnaccessData(varBLOB.parray);
					//SafeArrayDestroy(varBLOB.parray);
					CFile f;
					if(f.Open(fileName,CFile::modeWrite|CFile::modeCreate))
					{
						f.WriteHuge(m_pBuffer,lDataSize);
						f.Close();
					}
					else
					{
						//CString logstr;
						//logstr.Format("File Create faild! [%s]",m_filename);
					}
				}
				delete [] m_pBuffer;
				m_pBuffer=NULL;
			}
		}
		else
		{
			AfxMessageBox("û�пɻط�����!");
			return FALSE;
		}
		pRecordsetz->Close();
		pRecordsetz.Release();
	}
	catch (_com_error &e) 
	{
		CString temp;
		temp.Format("���ػط�����ʧ��! failed!ErrorMessage:%s",e.ErrorMessage());
		AfxMessageBox(temp);
		return FALSE;
	}
	return TRUE;
}

UINT CCPlayBackMapDlg::SendThread(LPVOID pParam)
{
	CCPlayBackMapDlg *pInfo=(CCPlayBackMapDlg*)pParam;
	CString strPathName;
	strPathName.Format("%s",pInfo->fileName);
	CStdioFile file;	
	if (!file.Open(strPathName, CFile::modeRead))
	{
		::AfxMessageBox(_T("�ļ���ʧ��"));
		return 0;
	}
	CString strText;

	UDPz sendUdp;
	sendUdp.InitSockUC();
	sendUdp.SetSendAddr("127.0.0.1",9705);
	while (file.ReadString(strText))
	{
		TRACE("%s\n",strText);
		if(strText.Replace(",",",")==32)
		{
			CStringArray te;
			pInfo->DoHaoCString(strText,te,',');
			if(te.GetSize()==33)
			{
//				sendUdp.SendPack(strText.GetBuffer(0),strText.GetLength());
				TRACE("%s,%s,%s\n",te.GetAt(3),te.GetAt(2),te.GetAt(3));			
				struRBody temprBody;
				CString tempIndex;
				tempIndex.Format("%s",te.GetAt(3));//����			
				strncpy(temprBody.sKchp.cKchp,tempIndex,10);//����
				temprBody.sGnss.gnssX=atof(te.GetAt(17));
				temprBody.sGnss.gnssY=atof(te.GetAt(18));
				temprBody.sGnss.gnssR=atof(te.GetAt(23));
				temprBody.sGnss.gnssSD=atof(te.GetAt(11));//�ٶ�
				temprBody.sGnss.gnssLC=atof(te.GetAt(12));//���
				strncpy(temprBody.sStu.bStuSFZ,te.GetAt(5),20);
				temprBody.sKchp.index=0;
				
				sendUdp.SendPack((char *)&temprBody,sizeof(struRBody));
			}
		}
		else
		{
			AfxMessageBox("���ݸ�ʽ����ȷ����ʧ��!");
		}
		Sleep(1000);
	}


	pInfo->OnCancel();
	return 0;
}

void CCPlayBackMapDlg::DoHaoCString(CString source,  CStringArray& dest, char division)
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

void CCPlayBackMapDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	ShowWindow(SW_HIDE);
	CDialog::OnTimer(nIDEvent);
}
