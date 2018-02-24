// CFourPicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CFourPic.h"
#include "CFourPicDlg.h"
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WSPACE 1
#define HSPACE 1
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
// CCFourPicDlg dialog

CCFourPicDlg::CCFourPicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCFourPicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCFourPicDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCFourPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCFourPicDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCFourPicDlg, CDialog)
	//{{AFX_MSG_MAP(CCFourPicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_STARTALLVIDEO_MSG,OnStartAllVideo)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCFourPicDlg message handlers
void CCFourPicDlg::OnStartAllVideo(WPARAM wParam, LPARAM lParam)
{
	for (int i=0;i<6;i++)
	{
		myPlaywnd[i].StartRealPlay();
	}
	//return 0;
}
BOOL CCFourPicDlg::OnInitDialog()
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


	hbr1=CreateSolidBrush(RGB(60,60,60));
	ShowWindow(SW_SHOWMAXIMIZED);// ȫ��
	NET_DVR_Init();// ��ʼ��

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
	logFileName.Format("%s\\��־.log",tempPath);
	//////////////////////////////////////////////////////////////////////////
	if (!OnInitSQL())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	GetMyHostIP();
	//////////////////////////////////////////////////////////////////////////
	GetClientRect(&CliRc);
	CliRcMin.x=CliRc.Width()/3-2*WSPACE;
	CliRcMin.y=(CliRc.Height()-35)/2-2*HSPACE;
	GetDlgItem(IDC_STATIC_WND1)->MoveWindow(WSPACE,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND2)->MoveWindow(WSPACE*3+CliRcMin.x,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND3)->MoveWindow(WSPACE*5+CliRcMin.x*2,HSPACE,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND4)->MoveWindow(WSPACE,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND5)->MoveWindow(WSPACE*3+CliRcMin.x,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);
	GetDlgItem(IDC_STATIC_WND6)->MoveWindow(WSPACE*5+CliRcMin.x*2,HSPACE*3+CliRcMin.y,CliRcMin.x,CliRcMin.y);

	GetDlgItem(IDC_STATIC_WND7)->MoveWindow(0,CliRc.Height()-35,CliRc.Width(),35);
	CString temp;
	CWnd * outBroad;
	int i=0;
	for (i=0;i<6;i++)
	{
		myPlaywnd[i].SetCliRcSize(CliRcMin.x,CliRcMin.y);
		temp.Format("����%d",i+1);
		myPlaywnd[i].iKch=GetPrivateProfileInt("����",temp,0,".\\aconfig.ini");
		outBroad=GetDlgItem(IDC_STATIC_WND1+i);
		myPlaywnd[i].Create(IDD_DLGPLAY,outBroad);
		myPlaywnd[i].ShowWindow(SW_SHOW);
		myPlaywnd[i].CenterWindow();
	}
	OnCountHGL();
	SetTimer(1,10000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCFourPicDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCFourPicDlg::OnPaint() 
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
HCURSOR CCFourPicDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCFourPicDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	NET_DVR_Cleanup();//�ͷ�
	CDialog::OnClose();
}

BOOL CCFourPicDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//MessageBox(_T("�س�������"));
			//			return TRUE;
		case VK_ESCAPE:
			//MessageBox(_T("ESC������"));
			if (MessageBox("�Ƿ��˳�����!","��ʾ",MB_OKCANCEL) == IDOK)
			{
				this->SendMessage(WM_CLOSE);
			}
			else
			{
				return TRUE;
			}
			break;
			//return TRUE;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCFourPicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor==CTLCOLOR_DLG )
	{
		return hbr1;
	}
	if (pWnd->GetDlgCtrlID()==IDC_STATIC_WND7 )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return hbr1;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}



BOOL CCFourPicDlg::GetMyHostIP()  
{  
	WSADATA wsadata;
	if(0 != WSAStartup(MAKEWORD(2, 2), &wsadata))   //��ʼ��
	{
		AfxMessageBox("��ʼ�����绷��ʧ��!");
		return FALSE;
	}
	char szHostName[MAX_PATH + 1];
	gethostname(szHostName, MAX_PATH);  //�õ��������
	hostent *p = gethostbyname(szHostName); //�Ӽ�������õ�������Ϣ
	if(p == NULL)
	{
		AfxMessageBox("�õ�����������Ϣʧ��!");
		return FALSE;
	}
	//	m_strName = szHostName;       //����������
	int nCount = 0;                 //����������ַ
	char *pIP = inet_ntoa(*(in_addr *)p->h_addr_list[0]);//��32λIPת��Ϊ�ַ���IP
	CString strIP;
	strIP = pIP;             //��������IP��ַ
    //UpdateData(FALSE);          //��ʾ��������
	OnSelectSQL(strIP);
	WSACleanup();               //�ͷ�Winsock API
	return TRUE;
}  


void CCFourPicDlg::OnSelectSQL(CString strIP)
{
	_RecordsetPtr pRecordset;
	CString strSQL,strtemp,strKCH,strtempkch,strJM1,strJM2,strJM0;
	int pos=0;
	_variant_t var;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM liuheoneconfig where �������='%s'",strIP);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		int swd =pRecordset->RecordCount;
		if (swd==0)
		{
			AfxMessageBox("��ȡ����Ϊ��!");		
		}
		else
		{
			var = pRecordset->GetCollect("��������");
			if (var.vt != VT_NULL)
			{	
				KSCYstr.Format("%s",(LPCSTR)_bstr_t(var));
			}		
			while (!pRecordset->adoEOF)
			{
				var = pRecordset->GetCollect("����");
				if (var.vt != VT_NULL)
				{	
					strtemp=(LPCSTR)_bstr_t(var);	
				}else
				{
					strtemp=_T("");
				}
				var = pRecordset->GetCollect("������");
				if (var.vt != VT_NULL)
				{	
					strKCH=(LPCSTR)_bstr_t(var);
					strtempkch.Format("����%s",strKCH);
				}else
				{
					strKCH=_T("");	
				}
				WritePrivateProfileString("����",strtemp,strKCH,".\\aconfig.ini");
				var = pRecordset->GetCollect("����1");
				if (var.vt != VT_NULL)
				{	
					strJM1=(LPCSTR)_bstr_t(var);
					//strtempkch.Format("����%s",strKCH);
				}else
				{
					strJM1=_T("");
					//strtempkch.Format("����%s",strKCH);
				}
				WritePrivateProfileString(strtempkch,"1",strJM1,".\\aconfig.ini");
				var = pRecordset->GetCollect("����2");
				if (var.vt != VT_NULL)
				{	
					strJM2=(LPCSTR)_bstr_t(var);
					//strtempkch.Format("����%s",strKCH);
				}else
				{
					strJM2=_T("");
					//strtempkch.Format("����%s",strKCH);
				}
				
				pos=strJM2.ReverseFind(',');
				if (pos >0)
				{
					strJM0.Format("%s,1",strJM2.Left(pos));
				}
				else
				{
					strJM0=_T("");
				}
				
				WritePrivateProfileString(strtempkch,"2",strJM2,".\\aconfig.ini");
				WritePrivateProfileString(strtempkch,"0",strJM0,".\\aconfig.ini");
				pRecordset->MoveNext();
			}
		}
		pRecordset->Close();
		pRecordset.Release();
	}
	catch(_com_error e)		//��׽�쳣
	{
		CString errormessage;
		errormessage.Format("liuheoneconfig ���ö�ȡ����!",e.ErrorMessage());
		AfxMessageBox(errormessage);
	}
	//CDialog::OnOK();
}

BOOL CCFourPicDlg::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize��NULL��δ��ʼ��
	{
		TRACE("��ʼ�����Ӷ���ʧ��\n");
		return FALSE;
	}
	HRESULT res=-1;
	//////////////////////////////////////////////////////////////////////////
	CString m_strInEdit;
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\aconfig.ini");
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
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		Sqlstr =m_strInEdit;
		TRACE(Sqlstr);
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("�����ݿ�ɹ�!");
			WriteLog("�����ݿ�ɹ�!");
		}
		else
		{
			TRACE("�����ݿ�ʧ��!");
			WriteLog("�����ݿ�ʧ��!");
			return FALSE;
		}
	}
	catch(_com_error e)		//��׽�쳣
	{
		CString errormessage;
		errormessage.Format("���ݿ���ʾ,�������ݿ�ʧ��!,������Ϣ:%s,�����������ݿ�����",e.ErrorMessage());
		AfxMessageBox(errormessage);
		ShellExecute(NULL,"open",".\\sqlLink.exe","aconfig.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

int CCFourPicDlg::OnSelectCount(CString str)
{
	int rcounti=0;
	CString sqltemp;
	try
	{
		sqltemp=str;
		VARIANT cnt;
		cnt.vt = VT_INT;
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			vat =pSet->GetCollect("countN");
			if (vat.vt != VT_NULL)
			{
				rcounti=atoi((LPCSTR)_bstr_t(vat));				
			}
			pSet->Close();
			pSet.Release();
		}
	}
	catch (_com_error &comError)
	{
		sqltemp.Format("OnSelectCount Error ! %s,%s",comError.Description(),comError.ErrorMessage());
		AfxMessageBox(sqltemp);
		return 0;
	}
	return rcounti;
}

void CCFourPicDlg::OnCountHGL()
{
	int iExamSuc = 0; //�ϸ�����    zt=1
	int iExamFla = 0; //���ϸ�����  zt=2
	int iExaming = 0; //�ڿ�����    zt=3
	int iExamAll = 0; //��������    RecordCount
	int iLogNumb = 0; //�ѵ�½����  "��¼״̬"
	
	int iWaitNumb= 0; //δ������    iLogNumb - (iExamSuc+iExamFla+iExaming)
	int iExamEnd = 0; //���Խ�������  iExamFla + iExamSuc
	CString temp;
	CString temp1;
	if (!KSCYstr.IsEmpty())
	{
		temp1.Format(" and ��������='%s' ",KSCYstr);
	}
	temp.Format("SELECT count(*) as countN from studentinfo where to_char(Ԥ������,'yyyy-mm-dd')=to_char(sysdate,'yyyy-mm-dd') %s",temp1);
	iExamSuc =OnSelectCount(temp+" and ״̬='1' ");
	iExamFla =OnSelectCount(temp+" and ״̬='2' ");
	iExaming =OnSelectCount(temp+" and ״̬='3' ");
	iExamAll =OnSelectCount(temp);
	iLogNumb =OnSelectCount(temp+" and ��¼״̬='�ѵ�¼' ");

	iWaitNumb = iLogNumb - (iExamSuc+iExamFla+iExaming); //δ������ 
	iExamEnd  = iExamFla + iExamSuc;   //���Խ�������

	int iExamEndT=iExamEnd;
	if (iExamEndT==0)
	{
		iExamEndT=1;
	}
	temp.Format("��%d������,�ѵ�¼%d����,������%d��,�ѿ�%d��,�ϸ�%d��,�ϸ���%2.2f%%  ,��������[%s]",iExamAll,iLogNumb,iExaming,iExamEnd,iExamSuc,(float)((float)(iExamSuc*100)/iExamEndT),KSCYstr);
	GetDlgItem(IDC_STATIC_WND7)->SetWindowText(temp);
}

void CCFourPicDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	OnCountHGL();
	CDialog::OnTimer(nIDEvent);
}
