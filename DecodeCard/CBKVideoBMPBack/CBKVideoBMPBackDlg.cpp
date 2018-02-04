// CBKVideoBMPBackDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBKVideoBMPBack.h"
#include "CBKVideoBMPBackDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

XMcode xmcodethr[10];
extern CCBKVideoBMPBackApp theApp;
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
// CCBKVideoBMPBackDlg dialog

CCBKVideoBMPBackDlg::CCBKVideoBMPBackDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBKVideoBMPBackDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCBKVideoBMPBackDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBKVideoBMPBackDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCBKVideoBMPBackDlg)
	DDX_Control(pDX, IDC_LIST_CARD, m_ListCard);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCBKVideoBMPBackDlg, CDialog)
	//{{AFX_MSG_MAP(CCBKVideoBMPBackDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNReList, OnBTNReList)
	ON_BN_CLICKED(IDC_BTN_RECARD, OnBtnRecard)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MY_MSG,OnQHMsg)
	ON_BN_CLICKED(IDC_BTN_YPLS, OnBtnYpls)
	ON_WM_DESTROY()
	ON_MESSAGE(UM_SOCKZ,OnSock)//TCP
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKVideoBMPBackDlg message handlers
LRESULT CCBKVideoBMPBackDlg::OnQHMsg(WPARAM wParam, LPARAM lParam)
{
	//////////////////////////////////////////////////////////////////////////
	CString sendstr;
	sendstr.Format("%d",lParam);
	mysockz.SendString("127.0.0.1",EXMVideo,sendstr,5287);
	CString log;
	log.Format("QHThreadFunc EXMVideo Stop ����:%s,code=[%d]",sendstr,xmcodethr[lParam].errori);
	WriteLog(log);
	
	CString dvrIP,dvrChannel,errcode,filename;
	int ifwww=(int)wParam;
	if (ifwww ==0)
	{
		filename.Format(".\\ZZIPChannel.dat");
	}
	else
	{
		filename.Format(".\\IPChannel.dat");
	}
	errcode.Format("%d",xmcodethr[lParam].errori);
	if(GetPrivateProfileString(errcode,"Channel","",dvrChannel.GetBuffer(MAX_PATH),MAX_PATH,filename))
	{
		GetPrivateProfileString(errcode,"IP","",dvrIP.GetBuffer(MAX_PATH),MAX_PATH,filename);
		if((dvrIP.IsEmpty() || dvrChannel.IsEmpty()))
		{
			sendstr.Format("%s,%s,%d",dvrChannel,dvrIP,lParam);
			mysockz.SendString("127.0.0.1",SXMVideo,sendstr,5287);
			log.Format("QHThreadFunc DisXMChannel :Code=[%s],Send=[%s]",errcode,sendstr);
			WriteLog(log);
		}					
	}
	else
	{
		WriteLog(filename+" error");
	}
	return 0;
}
BOOL CCBKVideoBMPBackDlg::OnInitDialog()
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
	m_pr_errorint=1;
	uXMStop=GetPrivateProfileInt("CONFIG","XMSTOP",0,".\\search.cfg");
	if (!mysockz.InitSock(m_hWnd,5288))
	{
		WriteLog("TCP ��ʧ��,�˿�5288");
		this->SendMessage(WM_CLOSE);
		return FALSE;
	}	
	if (!OnInitSQL())
	{
		//CBKVideoDeCode
		HWND hWnd=::FindWindow(NULL,_T("CBKVideoDeCode"));//����ѹ��������ر�
		if (NULL !=hWnd)
		{
			::SendMessage(hWnd,WM_CLOSE,0,0);
		}
		//////////////////////////////////////////////////////////////////////////
		this->SendMessage(WM_CLOSE);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	m_ListCard.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListCard.InsertColumn(0,_T("���"),LVCFMT_LEFT,40,-1);
	m_ListCard.InsertColumn(1,_T("������"),LVCFMT_LEFT,60,-1);
	m_ListCard.InsertColumn(2,_T("׼��֤�����"),LVCFMT_LEFT,90,-1);
	m_ListCard.InsertColumn(3,_T("����"),LVCFMT_LEFT,60,-1);
	m_ListCard.InsertColumn(4,_T("�Ա�"),LVCFMT_LEFT,40,-1);
	m_ListCard.InsertColumn(5,_T("��ˮ��"),LVCFMT_LEFT,100,-1);
	m_ListCard.InsertColumn(6,_T("���֤�����"),LVCFMT_LEFT,120,-1);//MSG3
	m_ListCard.InsertColumn(7,_T("��У"),LVCFMT_LEFT,130,-1);
	m_ListCard.InsertColumn(8,_T("����Ա"),LVCFMT_LEFT,60,-1);
	m_ListCard.InsertColumn(9,_T("����"),LVCFMT_LEFT,20,-1);
	m_ListCard.InsertColumn(10,_T("��������"),LVCFMT_LEFT,120,-1);

	m_ListCard.InsertColumn(11,_T("���"),LVCFMT_LEFT,40,-1);
	m_ListCard.InsertColumn(12,_T("ʱ��"),LVCFMT_LEFT,60,-1);
	m_ListCard.InsertColumn(13,_T("��ǰ״̬"),LVCFMT_LEFT,120,-1);
	m_ListCard.InsertColumn(14,_T("�۷�"),LVCFMT_LEFT,40,-1);
	m_ListCard.InsertColumn(15,_T("�ɼ�"),LVCFMT_LEFT,30,-1);
	m_ListCard.InsertColumn(16,_T("N"),LVCFMT_LEFT,30,-1);
//	m_ListCard.InsertColumn(17,_T("N"),LVCFMT_LEFT,30,-1);
	//////////////////////////////////////////////////////////////////////////
	DvrWriteIPChannel();
	ReadDisPlayChannel();
	ReadDVRPZ();
	TCHAR *szPath =__targv[1];
	if (!(  _tcscmp(szPath,_T("start"))))
	{
		OnBtnRecard();//
		WriteLog("CBKVideoBMPBack Start OK");
	}
	iItemLine=m_ListCard.GetItemCount();
	//////////////////////////////////////////////////////////////////////////
	CBKDeSkinH = LoadLibrary(TEXT("CBKDeSkin.dll")); 
	if (CBKDeSkinH == NULL) 
	{ 
		AfxMessageBox("CBKDeSkin.dllʧ��!"); 
		this->SendMessage(WM_CLOSE);
		return FALSE; 
	}	
    DrawLogo = (DrawImage) GetProcAddress(CBKDeSkinH,TEXT("DrawImage"));
	if(DrawLogo ==NULL)
	{
		WriteLog("GetProcAddress DrawLogo is NULL!");
		this->SendMessage(WM_CLOSE);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	CTime tempt=CTime::GetCurrentTime();
	for (int i=0;i<16;i++)
	{
		ztxmii[i]=0;
		xmStartTime[i]=tempt;
	}
	pr_count=1;
	OnSelectProc();
	OnprErrorALL();
	ISRunTime=FALSE;
	SetTimer(1,2000,NULL);
	SetTimer(2,1000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCBKVideoBMPBackDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCBKVideoBMPBackDlg::OnPaint() 
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
HCURSOR CCBKVideoBMPBackDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
//////////////////////////////////////////////////////////////////////////
BOOL CCBKVideoBMPBackDlg::OnInitSQL()
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
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\search.cfg");
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
		//�������ݿ�����
		
		CString Sqlstr;
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		
		//res = m_pConn->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=abc.mdb","","",adModeUnknown);
		
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
		ShellExecute(NULL,"open",".\\sqlLink.exe","search.cfg",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}
void CCBKVideoBMPBackDlg::WriteLog(CString sstr)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%H:%M:%S");
	FILE *fp=fopen(theApp.logFileName,"a+");
	fprintf(fp,"[%s]:%s#\n",Data,sstr);
	fclose(fp);
}


//////////////////////////////////////////////////////////////////////////
//�������ļ���ȡ������ʾͨ����Ӧ�Ŀ���
void CCBKVideoBMPBackDlg::ReadDisPlayChannel()
{
	int tempi=GetPrivateProfileInt("CONFIG","displaychannel",0,".\\search.cfg");;//��ʾͨ������ �ж��ٸ�����
	if (tempi==0)
	{
		AfxMessageBox("���뿨����δ��ʼ��!!!");
	}
	int iItem;
	CString temps,strkc,tempdd;
	for (int i=1;i<=tempi;i++)
	{
		temps.Format("%d",i);
		if(GetPrivateProfileString("channel",temps,"",strkc.GetBuffer(MAX_PATH),MAX_PATH,".\\CardChannel.cfg"))
		{
			if (strkc.IsEmpty())
			{
				iItem=m_ListCard.GetItemCount();
				tempdd.Format("%02d",iItem+1);
				m_ListCard.InsertItem(iItem,tempdd);
				m_ListCard.SetItemText(iItem,1,strkc);//������
				m_ListCard.SetItemText(iItem,16,"0");
			}
		}
	}
}

BOOL CCBKVideoBMPBackDlg::OnSelectProc()
{
	try
	{
		if (m_pConn == NULL) return FALSE;
		m_pConn->CursorLocation =adUseClient;	
		_RecordsetPtr m_pRecordset;
		_CommandPtr m_pCommand;
		m_pRecordset.CreateInstance("ADODB.Recordset");		
		m_pCommand.CreateInstance("ADODB.Command");	
		m_pCommand->ActiveConnection = m_pConn;
		m_pCommand->CommandType = adCmdStoredProc;
		m_pCommand->CommandText = _bstr_t(_T("CardTestN"));	
		m_pCommand->Parameters->Refresh(); 		
		m_pRecordset =  m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
		//////////////////////////////////////////////////////////////////////////
		m_pCommand.Release();
		m_pCommand.Detach();
		_variant_t vat;
	 
		CString strKch,strtemp,tempmsg;
		int Listi;
		if(m_pRecordset != NULL)
		{
			while(!m_pRecordset->adoEOF)
			{
				vat =m_pRecordset->GetCollect("������");
				if (vat.vt != VT_NULL)
				{
					strKch = (LPCSTR)_bstr_t(vat);
					Listi=CardNumInList(strKch);
					if (Listi !=-1)
					{
						vat =m_pRecordset->GetCollect("׼��֤�����");
						if (vat.vt != VT_NULL)
						{
							strtemp = (LPCSTR)_bstr_t(vat);
							if ((strcmp(strtemp,m_ListCard.GetItemText(Listi,2))!=0))
							{
								WriteLog("׼��֤�����:"+strtemp);
								m_ListCard.SetItemText(Listi,2,strtemp);
								strKch.Format("kch%d",Listi);
								for (int i=0;i<8;i++)
								{
									tempmsg.Format("MSG%d",i);
									vat =m_pRecordset->GetCollect(tempmsg.GetBuffer(0));
									if (vat.vt != VT_NULL)
									{
										strtemp=(LPCSTR)_bstr_t(vat);
										m_ListCard.SetItemText(Listi,3+i,strtemp);
										::WritePrivateProfileString(strKch,tempmsg,strtemp,".\\allcard.msg");
									}
								}
								::WritePrivateProfileString(strKch,"MSG8","100",".\\allcard.msg");//�ɼ�
								//׼��֤����Ų�һ��
								OnDrawLogo(Listi,1);
								//////////////////////////////////////////////////////////////////////////
							}//strcmp zkzm
						}//zkzm !=VT_NULL
					}
				}
				m_pRecordset->MoveNext();
			}
			m_pRecordset->Close();
			m_pRecordset.Release();
		}		
	}
	catch(_com_error e)
	{
		CString log;
		log.Format("���ô洢���� CardTestN ʧ��!ԭ��:%s",e.ErrorMessage());
		WriteLog(log);
		m_pr_errorint+=1;
		if (m_pr_errorint >10)
		{
			EndDialog(5);
		}
		return FALSE;
	}
	return TRUE;
}

int CCBKVideoBMPBackDlg::CardNumInList(CString kch)
{
	int ri=-1;
	int iItem=m_ListCard.GetItemCount();
	CString temp;
	for (int i=0;i<iItem;i++)
	{
		temp=m_ListCard.GetItemText(i,1);
		if ((strcmp(kch,temp)==0))
		{
			ri =i;
			break;
		}
	}
	return ri;
}
//////////////////////////////////////////////////////////////////////////

void CCBKVideoBMPBackDlg::OnBTNReList() 
{
	// TODO: Add your control notification handler code here
//	OnSelectProc();
//	OnprErrorALL();
	SetOSDKSCC();
}

DWORD WINAPI QHThreadFuncc(LPVOID param)
{
	XMcode * mycode =(XMcode *)param;
	int i=mycode->thi;
	CString errcode;
	int QHTi,erri;
	erri=xmcodethr[i].errori;
	errcode.Format("%d",erri);
	QHTi=xmcodethr[i].thi;
	CString temp,temp1;
	GetPrivateProfileString(errcode,"Time","5000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
	temp1.Format("%s",temp);
	DWORD sleepd=(DWORD) atol(temp1);
	temp.Format("QHThreadFunc sleep=%s,%s",temp1,errcode);
	theApp.WriteLog(temp);
	Sleep(sleepd);
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,0,QHTi);
	if (xmcodethr[i].errori >203509 && xmcodethr[i].errori <203700)
	{
		GetPrivateProfileString(errcode,"Time2","5000",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\ZZIPChannel.dat");
		temp1.Format("%s",temp);
		DWORD sleepd=(DWORD) atol(temp1);
		temp.Format("QHThreadFunc���� sleep=%s,%d",temp1,xmcodethr[i].errori);
		theApp.WriteLog(temp);
		Sleep(sleepd);
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_MY_MSG,1,QHTi);
	}
	delete [] mycode;
	return 0;
}
void CCBKVideoBMPBackDlg::OnListNtoZero()
{
// 	int iItem=m_ListCard.GetItemCount();
// 	CString temp,kf,kf2;
// 	for (int i=0;i<iItem;i++)
// 	{
// 		temp =m_ListCard.GetItemText(i,16);
// 		if ("0" ==temp)//ȡ N��ֵ
// 		{
// 			continue;
// 		}
// 		m_ListCard.SetItemText(i,16,"0");
// 		int swi=_ttoi(temp);
// 		switch (swi)
// 		{
// 		case 1:
// 			{
// // 				SavePho(i);
// // 				DrawLogo(1,i,0);//17C51
// // 				xmStartTime[i]=CTime::GetCurrentTime();
// 				break;
// 			}
// 		case 2:
// 			{
// 				int xmcode=_ttoi(m_ListCard.GetItemText(i,11));
// 				DrawLogo(2,i,xmcode);
// 				CString sendstr;
// 				CString dvrIP,dvrChannel;
// 				CString errcode;
// 				errcode.Format("%d",xmcode);
// 				if(GetPrivateProfileString(errcode,"Channel","",dvrChannel.GetBuffer(MAX_PATH),MAX_PATH,".\\IPChannel.dat"))
// 				{
// 					GetPrivateProfileString(errcode,"IP","",dvrIP.GetBuffer(MAX_PATH),MAX_PATH,".\\IPChannel.dat");
// 					if((dvrIP.IsEmpty() || dvrChannel.IsEmpty()))
// 					{
// 						sendstr.Format("%s,%s,%d",dvrChannel,dvrIP,i);
// 						mysockz.SendString("127.0.0.1",SXMVideo,sendstr,5287);
// 						errcode.Format("DisXMChannel :Code=[%d],Send=%s",xmcode,sendstr);
// 						WriteLog(errcode);
// 						CFileFind FindDBFile;
// 						if (FindDBFile.FindFile("ZZIPChannel.dat"))
// 						{
// 							//WriteLog("ZZIPChannel.dat");
// 							if ( (xmcode>=501 && xmcode <599) || 249==xmcode || 259==xmcode)
// 							{
// 								xmcodethr[i].errori=xmcode;
// 								xmcodethr[i].thi=i;
// 								XMcode * mycode =new XMcode;
// 								mycode->errori=xmcode;
// 								mycode->thi=i;
// 								hThread= CreateThread(NULL,0,QHThreadFuncc,mycode,0,0);
// 								CloseHandle(hThread);
// 								WriteLog("�໭���л�!����--ʪ��·---���");
// 							}
// 						}
// 					}
// 				}
// 				break;
// 			}
// 		case 3:
// 			{
// 				DrawLogo(3,i,ztxmii[i]);//���һ������ ��0 ��ʼ 0 1 2
// 				ztxmii[i]+=1;
// 				break;
// 			}
// 		case 4:
// 			{
// 				DrawLogo(6,i,2);//���ϸ�				
// 				break;
// 			}
// 		case 5:
// 			{
// 				int xmcode=_ttoi(m_ListCard.GetItemText(i,11));
// 				DrawLogo(5,i,xmcode);
// 				if (uXMStop!=0)
// 				{
// 					CString sendstr;
// 					sendstr.Format("%d",i);
// 					mysockz.SendString("127.0.0.1",EXMVideo,sendstr,5287);
// 					WriteLog("EXMVideo Stop="+sendstr);
// 				}
// 				break;
// 			}
// 		case 6:
// 			{
// 				DrawLogo(6,i,1);//�ϸ�
// 				
// 				break;
// 			}
// 		default:
// 			break;
// 		}
// 		if (swi>0 && swi<7)
// 		{
// 			CString sendstr;
// 			sendstr.Format("%d",i);
// 			mysockz.SendString("127.0.0.1",DisLOGO,sendstr,5287);
// 			WriteLog("DisLOGO sendstr="+sendstr);
// 		}
// 		
// 		
// 	}
}
void CCBKVideoBMPBackDlg::SavePho(UINT isave)
{
	CDC *pDC = this->GetDC();
	CDC dcMem; 
	dcMem.CreateCompatibleDC(pDC); 
	this->ReleaseDC(pDC);
	CString temp;
	temp.Format("%d.pho",isave);
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BMPPHOBK); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	dcMem.SelectObject(&bmpBackground);
	CRect rt(0,0,bitmap.bmWidth,bitmap.bmHeight);
	CString tempzkzm;
	tempzkzm.Format("%s",m_ListCard.GetItemText(isave,2));
	ReadPhotoFromDB(&dcMem,tempzkzm,&rt);
	HBITMAP hBmp;
	hBmp=CopyDCToBitmap(dcMem.GetSafeHdc(),rt);   
	SaveBmp2(hBmp,temp);
	DeleteObject(hBmp);
	temp.Format("SavePho [%s],[%d.pho]",tempzkzm,isave);
	WriteLog(temp);
	CBitmap bmpBackground1; 
	bmpBackground1.LoadBitmap(IDB_BMPPHOBK); 
	dcMem.SelectObject(&bmpBackground1);
	ReadMJPhotoFromDB(&dcMem,tempzkzm,&rt);
	hBmp=CopyDCToBitmap(dcMem.GetSafeHdc(),rt);	
	temp.Format("%d.phomj",isave);
	SaveBmp2(hBmp,temp);
	DeleteObject(hBmp);
	temp.Format("SavePho [%s],[%d.phomj]",tempzkzm,isave);
	WriteLog(temp);
}
//////////////////////////////////////////////////////////////////////////

BOOL CCBKVideoBMPBackDlg::ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
	
	CString temp;
	try
	{		
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		strSQL.Format("SELECT ��Ƭ FROM StudentPhoto WHERE ׼��֤����� = '%s'",sCard);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		if(pRecordset->RecordCount != 1)
		{
			return FALSE;
		}		
		char* pbuf = NULL;
		
		long lDataSize = pRecordset->GetFields()->GetItem("��Ƭ")->ActualSize; //���ݿ���ͼ�����ݳ���
		if (lDataSize > 0)
		{
			_variant_t varBLOB;
			varBLOB = pRecordset->GetFields()->GetItem("��Ƭ")->GetChunk(lDataSize);
			m_pBMPBuffer = new char[lDataSize+1];
			
			pRecordset->Close();
			pRecordset.Release();
			
			if (varBLOB.vt == (VT_ARRAY|VT_UI1))
			{
				SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
				memcpy(m_pBMPBuffer, pbuf, lDataSize);//�������ݵ�������m_pBMPBuffer
				
				SafeArrayUnaccessData(varBLOB.parray);
				
				//******************************************************//
				IStream* pStm;
				LONGLONG cb = lDataSize;
				HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
				LPVOID pvData;
				if (hGlobal != NULL)
				{
					pvData = GlobalLock(hGlobal);
					memcpy(pvData, m_pBMPBuffer, cb);
					GlobalUnlock(hGlobal);
					CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
				}
				else
				{
					//AfxMessageBox("��ȡ��Ƭ��Ϣ����");
					return FALSE;
				}
				
				IPicture *pImg = NULL;    
				if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
				{
					OLE_XSIZE_HIMETRIC hmWidth;
					OLE_YSIZE_HIMETRIC hmHeight;
					
					pImg->get_Width(&hmWidth);
					pImg->get_Height(&hmHeight);
					pImg->Render(*pDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
				}
				else
				{
					//AfxMessageBox("��ӡ��Ƭʧ��!");
					return FALSE;
				}
				pStm->Release ();
				pImg->Release();
				GlobalFree(hGlobal); 
				if(m_pBMPBuffer)
				{
					delete m_pBMPBuffer;
					m_pBMPBuffer = NULL;
				}
				
			}
		}
		
		pRecordset->Close();
		pRecordset.Release();
		
	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}	
	return TRUE;
}
BOOL CCBKVideoBMPBackDlg::ReadMJPhotoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
	
	CString temp;
	try
	{		
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		strSQL.Format("SELECT �Ž���Ƭ FROM StudentPhoto WHERE ׼��֤����� = '%s'",sCard);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		if(pRecordset->RecordCount != 1)
		{
			return FALSE;
		}		
		char* pbuf = NULL;
		
		long lDataSize = pRecordset->GetFields()->GetItem("�Ž���Ƭ")->ActualSize; //���ݿ���ͼ�����ݳ���
		if (lDataSize > 0)
		{
			_variant_t varBLOB;
			varBLOB = pRecordset->GetFields()->GetItem("�Ž���Ƭ")->GetChunk(lDataSize);
			m_pBMPBuffer = new char[lDataSize+1];
			
			pRecordset->Close();
			pRecordset.Release();
			
			if (varBLOB.vt == (VT_ARRAY|VT_UI1))
			{
				SafeArrayAccessData(varBLOB.parray, (void **)&pbuf);
				memcpy(m_pBMPBuffer, pbuf, lDataSize);//�������ݵ�������m_pBMPBuffer
				
				SafeArrayUnaccessData(varBLOB.parray);
				
				//******************************************************//
				IStream* pStm;
				LONGLONG cb = lDataSize;
				HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb);
				LPVOID pvData;
				if (hGlobal != NULL)
				{
					pvData = GlobalLock(hGlobal);
					memcpy(pvData, m_pBMPBuffer, cb);
					GlobalUnlock(hGlobal);
					CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);
				}
				else
				{
					//AfxMessageBox("��ȡ��Ƭ��Ϣ����");
					return FALSE;
				}
				
				IPicture *pImg = NULL;    
				if(SUCCEEDED(OleLoadPicture(pStm,lDataSize,TRUE,IID_IPicture,(LPVOID*)&pImg)))
				{
					OLE_XSIZE_HIMETRIC hmWidth;
					OLE_YSIZE_HIMETRIC hmHeight;
					
					pImg->get_Width(&hmWidth);
					pImg->get_Height(&hmHeight);
					pImg->Render(*pDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
				}
				else
				{
					//AfxMessageBox("��ӡ��Ƭʧ��!");
					return FALSE;
				}
				pStm->Release ();
				pImg->Release();
				GlobalFree(hGlobal); 
				if(m_pBMPBuffer)
				{
					delete m_pBMPBuffer;
					m_pBMPBuffer = NULL;
				}
				
			}
		}
		
		pRecordset->Close();
		pRecordset.Release();
		
	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}	
	return TRUE;
}
HBITMAP CCBKVideoBMPBackDlg::CopyDCToBitmap(HDC hScrDC, LPRECT lpRect)   
{   
	HDC  hMemDC;               
	//   ��Ļ���ڴ��豸������   
	HBITMAP hBitmap,hOldBitmap;         
	//   λͼ���   
	int  nX, nY, nX2, nY2;               
	//   ѡ����������   
	int  nWidth, nHeight;               
	//   λͼ��Ⱥ͸߶�   
	
	//   ȷ��ѡ������Ϊ�վ���   
	if   (IsRectEmpty(lpRect))   
		return   NULL;   
	
	//   ���ѡ����������   
	nX   = lpRect->left;   
	nY   = lpRect->top;   
	nX2  = lpRect->right;   
	nY2  = lpRect->bottom;   
	nWidth  = nX2 - nX;   
	nHeight = nY2 - nY;   
	//   Ϊ��Ļ�豸�����������ݵ��ڴ��豸������   
	hMemDC  = CreateCompatibleDC(hScrDC);   
	//   ����һ������Ļ�豸��������ݵ�λͼ,������Ҫ��CreateCompatibleDC���ʹ��   
	hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);   
	//   ����λͼѡ���ڴ��豸��������   
	hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);   
	//   ����Ļ�豸�����������ڴ��豸��������
	//   StretchBlt������Դ�����и���һ��λͼ��Ŀ����Σ���Ҫʱ��ĿǰĿ���豸���õ�ģʽ����ͼ��������ѹ����
	StretchBlt(hMemDC,0,0,nWidth,nHeight,  hScrDC,nX,nY,nWidth,nHeight,SRCCOPY);  // SRCCOPY
	//BitBlt(hMemDC,   0,   0,   nWidth,   nHeight,hScrDC,   nX,   nY,   SRCCOPY);   
	//   �õ���Ļλͼ�ľ��     
	
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);   
	//   ���     
	
	//DeleteDC(hScrDC);   
	DeleteDC(hMemDC);   
	DeleteObject(hOldBitmap);   
	//   ����λͼ���   
	return   hBitmap;   
}   

BOOL CCBKVideoBMPBackDlg::SaveBmp(HBITMAP hBitmap, CString FileName)   
{   
	if (FileName.IsEmpty())
	{
		FileName=_T("BKVideoBMPBack.bmp");
	}
	HDC   hDC;   
	//��ǰ�ֱ�����ÿ������ռ�ֽ���   
	int   iBits;   
	//λͼ��ÿ������ռ�ֽ���   
	WORD  wBitCount;   
	//�����ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С��д���ļ��ֽ���
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;     
	//λͼ���Խṹ     
	BITMAP   Bitmap;       
	//λͼ�ļ�ͷ�ṹ   
	BITMAPFILEHEADER   bmfHdr;       
	//λͼ��Ϣͷ�ṹ     
	BITMAPINFOHEADER   bi;       
	//ָ��λͼ��Ϣͷ�ṹ       
	LPBITMAPINFOHEADER lpbi;       
	//�����ļ��������ڴ�������ɫ����     
	HANDLE  fh, hDib, hPal, hOldPal=NULL;     
    
	//����λͼ�ļ�ÿ��������ռ�ֽ���     
	hDC   = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);   
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);     
	DeleteDC(hDC);     
	if (iBits <= 1)wBitCount = 1;     
	else if (iBits <= 4) wBitCount = 4;     
	else if (iBits <= 8) wBitCount = 8;     
	else wBitCount = 24;     
    
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);   
	bi.biSize = sizeof(BITMAPINFOHEADER);   
	bi.biWidth = Bitmap.bmWidth;   
	bi.biHeight = Bitmap.bmHeight;   
	bi.biPlanes = 1;   
	bi.biBitCount = wBitCount;   
	bi.biCompression = BI_RGB;   
	bi.biSizeImage = 0;   
	bi.biXPelsPerMeter = 0;   
	bi.biYPelsPerMeter = 0;   
	bi.biClrImportant = 0;   
	bi.biClrUsed = 0;   
    
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;   
//    dwBmBitsSize =(Bitmap.bmWidth * Bitmap.bmHeight * wBitCount) /8;//zsz
	//   Ϊλͼ���ݷ����ڴ�     
	hDib  = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));     
	lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);     
	*lpbi = bi;     
    
	//   �����ɫ��       
	hPal  = GetStockObject(DEFAULT_PALETTE);     
	if   (hPal)     
	{     
		hDC = ::GetDC(NULL);     
		//hDC   =   m_pDc->GetSafeHdc();   
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);     
		RealizePalette(hDC);     
	}   
	//   ��ȡ�õ�ɫ�����µ�����ֵ     
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)     
		+dwPaletteSize,(BITMAPINFO *)lpbi, DIB_RGB_COLORS);     
    
	//   �ָ���ɫ��       
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}    
    
	//   ����λͼ�ļ�       
	fh = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE;
    
	//   ����λͼ�ļ�ͷ     
	bmfHdr.bfType = 0x4D42;   //   ;BM;
	dwDIBSize   = 608312;
	bmfHdr.bfSize   = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//   д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//   д��λͼ�ļ���������
	dwDIBSize=dwDIBSize-14;
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//   ���
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
    
	return TRUE;
}
BOOL CCBKVideoBMPBackDlg::SaveBmp2(HBITMAP hBitmap, CString FileName)   
{   
	if (FileName.IsEmpty())
	{
		FileName=_T("BKVideoBMPBack.pho");
	}
	HDC   hDC;   
	//��ǰ�ֱ�����ÿ������ռ�ֽ���   
	int   iBits;   
	//λͼ��ÿ������ռ�ֽ���   
	WORD  wBitCount;   
	//�����ɫ���С��λͼ�������ֽڴ�С��λͼ�ļ���С��д���ļ��ֽ���
	DWORD dwPaletteSize=0, dwBmBitsSize=0, dwDIBSize=0, dwWritten=0;     
	//λͼ���Խṹ     
	BITMAP   Bitmap;       
	//λͼ�ļ�ͷ�ṹ   
	BITMAPFILEHEADER   bmfHdr;       
	//λͼ��Ϣͷ�ṹ     
	BITMAPINFOHEADER   bi;       
	//ָ��λͼ��Ϣͷ�ṹ       
	LPBITMAPINFOHEADER lpbi;       
	//�����ļ��������ڴ�������ɫ����     
	HANDLE  fh, hDib, hPal, hOldPal=NULL;     
    
	//����λͼ�ļ�ÿ��������ռ�ֽ���     
	hDC   = CreateDC(_T("DISPLAY"), NULL, NULL, NULL);   
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);     
	DeleteDC(hDC);     
	if (iBits <= 1)wBitCount = 1;     
	else if (iBits <= 4) wBitCount = 4;     
	else if (iBits <= 8) wBitCount = 8;     
	else wBitCount = 24;     
    
	GetObject(hBitmap, sizeof(Bitmap), (LPSTR)&Bitmap);   
	bi.biSize = sizeof(BITMAPINFOHEADER);   
	bi.biWidth = Bitmap.bmWidth;   
	bi.biHeight = Bitmap.bmHeight;   
	bi.biPlanes = 1;   
	bi.biBitCount = wBitCount;   
	bi.biCompression = BI_RGB;   
	bi.biSizeImage = 0;   
	bi.biXPelsPerMeter = 0;   
	bi.biYPelsPerMeter = 0;   
	bi.biClrImportant = 0;   
	bi.biClrUsed = 0;   
    
	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;   
	//    dwBmBitsSize =(Bitmap.bmWidth * Bitmap.bmHeight * wBitCount) /8;//zsz
	//   Ϊλͼ���ݷ����ڴ�     
	hDib  = GlobalAlloc(GHND,dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));     
	lpbi  = (LPBITMAPINFOHEADER)GlobalLock(hDib);     
	*lpbi = bi;     
    
	//   �����ɫ��       
	hPal  = GetStockObject(DEFAULT_PALETTE);     
	if   (hPal)     
	{     
		hDC = ::GetDC(NULL);     
		//hDC   =   m_pDc->GetSafeHdc();   
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);     
		RealizePalette(hDC);     
	}   
	//   ��ȡ�õ�ɫ�����µ�����ֵ     
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)lpbi + sizeof(BITMAPINFOHEADER)     
		+dwPaletteSize,(BITMAPINFO *)lpbi, DIB_RGB_COLORS);     
    
	//   �ָ���ɫ��       
	if (hOldPal)
	{
		::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE); 
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}    
    
	//   ����λͼ�ļ�       
	fh = CreateFile(FileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
								FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
    
	if (fh == INVALID_HANDLE_VALUE) 
		return FALSE;
    
	//   ����λͼ�ļ�ͷ     
	bmfHdr.bfType = 0x4D42;   //   ;BM;
	dwDIBSize   = 52032;
	bmfHdr.bfSize   = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits   = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
	//   д��λͼ�ļ�ͷ
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
	//   д��λͼ�ļ���������
	dwDIBSize=dwDIBSize-14;
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
	//   ���
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);
    
	return TRUE;
}

void CCBKVideoBMPBackDlg::OnBtnRecard() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BTN_RECARD)->EnableWindow(FALSE);

	
	CString kch,dvrIP,dvrChannel,temps;
	int iItem=m_ListCard.GetItemCount();
	for (int i=0;i<iItem;i++)
	{
		// ���ݿ����� ��ȡ���ݿ�����
		kch=m_ListCard.GetItemText(i,1);

		DvrIPandChannel(kch,dvrIP,dvrChannel);
		if (dvrIP.IsEmpty() || dvrChannel.IsEmpty())
		{
			WriteLog("DvrIPandChannel ����ֵΪ��!,IP����ͨ���Ų���ȷ,�����ú�����!");
			continue;
		}
		//ͨ����,IP,��ʾͨ����0��ʼ,���һλ 0 Ϊ���� 1 Ϊ��Ŀ
		CString sendstr;
		sendstr.Format("%s,%s,%d",dvrChannel,dvrIP,i);
		mysockz.SendString("127.0.0.1",SCardVideo,sendstr,5287);
		Sleep(300);
		temps.Format("DisXMChannel [%s]������Ϣ:%s",kch,sendstr);
		WriteLog(temps);
	}
}
void CCBKVideoBMPBackDlg::ReadDVRPZ()
{
	int DVRi=0;
	CString sqltemp;
	CString dIP,dPort,dPwd,dUser;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		sqltemp.Format("select DISTINCT(�豸IP),�û���,����,�˿ں� from TBKVideo order by �豸IP asc");//��ѯ�����ڿ��Ŀ���
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("�豸IP");
				if (vat.vt != VT_NULL)
				{
					dIP = (LPCSTR)_bstr_t(vat);	
				}
				vat =pSet->GetCollect("�û���");
				if (vat.vt != VT_NULL)
				{
					dUser = (LPCSTR)_bstr_t(vat);	
				}
				vat =pSet->GetCollect("����");
				if (vat.vt != VT_NULL)
				{
					dPwd = (LPCSTR)_bstr_t(vat);	
				}
				vat =pSet->GetCollect("�˿ں�");
				if (vat.vt != VT_NULL)
				{
					dPort = (LPCSTR)_bstr_t(vat);
				}
				//jiemak.F_Login(dIP.GetBuffer(0),wPort,dUser.GetBuffer(0),dPwd.GetBuffer(0));
				CString tempDvr;
				tempDvr.Format("DVR%d",DVRi);
				::WritePrivateProfileString(tempDvr,"xmsbip",dIP,".\\search.cfg");//�豸IP
				::WritePrivateProfileString(tempDvr,"xmsbport",dPort,".\\search.cfg");//�豸�˿�
				::WritePrivateProfileString(tempDvr,"xmsbuser",dUser,".\\search.cfg");//�豸�û�
				::WritePrivateProfileString(tempDvr,"xmsbpwd",dPwd,".\\search.cfg");//�豸����
				mysockz.SendString("127.0.0.1",DVRLOGIN,tempDvr,5287);
				WriteLog("DVRLOGIN �豸׼����¼:"+dIP+tempDvr);
				DVRi+=1;
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		sqltemp.Format("������ʾ,��ѯӲ��¼���������Ϣʧ��!,%s",comError.Description());
		AfxMessageBox(sqltemp.GetBuffer(0));
		WriteLog(sqltemp);
	}
}

//////////////////////////////////////////////////////////////////////////
//���ݱ�Ŵ����ݿ�ȡIP��ͨ����
void CCBKVideoBMPBackDlg::DvrIPandChannel(CString bh, CString &dIP, CString &lChannel)
{
	dIP=_T("");
	lChannel=_T("");
	CString sqltemp;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		sqltemp.Format("select �豸IP,ͨ���� from TBKVideo where ���='%s' ",bh);//
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("�豸IP");
				if (vat.vt != VT_NULL)
				{
					dIP = (LPCSTR)_bstr_t(vat);	
				}
				vat =pSet->GetCollect("ͨ����");
				if (vat.vt != VT_NULL)
				{
					lChannel = (LPCSTR)_bstr_t(vat);	
				}
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		sqltemp.Format("������ʾ,��ѯӲ��¼���ͨ��������Ϣʧ��!,%s",comError.Description());
		//AfxMessageBox(sqltemp.GetBuffer(0));
		WriteLog(sqltemp);
	}
}

void CCBKVideoBMPBackDlg::DvrWriteIPChannel()
{
	CString sqltemp,dIP,lChannel,bhstr;
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		sqltemp.Format("select ���,�豸IP,ͨ���� from TBKVideo");//
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("���");
				if (vat.vt != VT_NULL)
				{
					bhstr = (LPCSTR)_bstr_t(vat);	
				}
				vat =pSet->GetCollect("�豸IP");
				if (vat.vt != VT_NULL)
				{
					dIP = (LPCSTR)_bstr_t(vat);	
				}
				vat =pSet->GetCollect("ͨ����");
				if (vat.vt != VT_NULL)
				{
					lChannel = (LPCSTR)_bstr_t(vat);	
				}
				::WritePrivateProfileString(bhstr,"IP",dIP,".\\IPChannel.dat");
				::WritePrivateProfileString(bhstr,"Channel",lChannel,".\\IPChannel.dat");
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		sqltemp.Format("������ʾ,��ѯӲ��¼���ͨ��������Ϣʧ��!DvrWriteIPChannel,%s",comError.Description());
		WriteLog(sqltemp);
	}
}

void CCBKVideoBMPBackDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch (nIDEvent)
	{
	case 1:
		{
			//KillTimer(1);
			if (ISRunTime==FALSE)
			{
				ISRunTime=TRUE;				
				OnSelectProc();
				OnprErrorN();
				ISRunTime=FALSE;
			}
			break;
		}		
	case 2:
		{
			SetOSDKSCC();
			break;
		}
	case 3:
		break;
	default :
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CCBKVideoBMPBackDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	mysockz.CloseSocket();
	CDialog::OnClose();
}

BOOL CCBKVideoBMPBackDlg::OnprError()
{
	try
	{
		int listi;
		CString sZkzm,errjlbh;
		for (listi=0;listi<iItemLine;listi++)
		{
			sZkzm.Format("%s",m_ListCard.GetItemText(listi,2));
			if (sZkzm =="")//׼��֤�����Ϊ��
			{
				continue;
			}
		errjlbh.Format("%s",m_ListCard.GetItemText(listi,16));
		if (m_pConn == NULL) return FALSE;
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
		if (1==retVal )
		{
			if(m_pRecordset != NULL && (!m_pRecordset->adoEOF))
			{
				pr_count=m_pRecordset->RecordCount;
				int zti=0,cj=0,si=0,ei=0;
				vat =m_pRecordset->GetCollect("״̬");
				zti = atoi((LPCSTR)_bstr_t(vat));
				vat =m_pRecordset->GetCollect("�ɼ�");
				cj= atoi((LPCSTR)_bstr_t(vat));
				vat =m_pRecordset->GetCollect("S");
				si= vat.lVal;
				vat =m_pRecordset->GetCollect("E");
				ei= vat.lVal;				
				if (zti ==1)
				{
					errjlbh.Format("״̬=%d,�ɼ�=%d,S=%d,E=%d",zti,cj,si,ei);
					WriteLog(errjlbh);
					if ( (si==127 && ei==127) || (si==126 && ei==126))
					{
						EmptyList(listi);
						OnDrawLogo(listi,6);
						WriteLog("OnDrawLogo 6");
					}					
				}
				if (zti ==2)
				{
					errjlbh.Format("״̬=%d,�ɼ�=%d,S=%d,E=%d",zti,cj,si,ei);
					WriteLog(errjlbh);
					if ( cj<80)
					{
						EmptyList(listi);
						OnDrawLogo(listi,4);
						WriteLog("OnDrawLogo 4");
					}
				}
				m_pRecordset->Close();
				m_pRecordset.Release();
			}
		}
		if (3==retVal )
		{//
			if(m_pRecordset != NULL && (!m_pRecordset->adoEOF))
			{
				CString strtemp,tempfs,tempsj,tempkflx,tempcj;
				pr_count=m_pRecordset->RecordCount;
				
				while(!m_pRecordset->adoEOF)
				{	
//					strtemp.Format("pr_count=%d",pr_count);
// 					WriteLog(strtemp);
					vat =m_pRecordset->GetCollect("��¼���");
					if (vat.vt != VT_NULL)
					{
						strtemp = (LPCSTR)_bstr_t(vat);						
						m_ListCard.SetItemText(listi,16,strtemp);//����
					}
					vat =m_pRecordset->GetCollect("������");
					if (vat.vt != VT_NULL)
					{
						strtemp = (LPCSTR)_bstr_t(vat);
						if ((strcmp(strtemp,m_ListCard.GetItemText(listi,11))!=0))
						{
							m_ListCard.SetItemText(listi,11,strtemp);
							int xmcode=_ttoi(strtemp);
							
							vat =m_pRecordset->GetCollect("����ʱ��");
							if (vat.vt != VT_NULL)
							{
								tempsj=(LPCSTR)_bstr_t(vat);
								m_ListCard.SetItemText(listi,12,tempsj);
							}
							vat =m_pRecordset->GetCollect("�۷�����");
							if (vat.vt != VT_NULL)
							{
								tempkflx=(LPCSTR)_bstr_t(vat);
								m_ListCard.SetItemText(listi,13,tempkflx);
								CString strKch;
								strKch.Format("kch%d",listi);
								::WritePrivateProfileString(strKch,"MSG9",tempkflx,".\\allcard.msg");
							}
							vat =m_pRecordset->GetCollect("�ɼ�");
							if (vat.vt != VT_NULL)
							{
								tempcj=(LPCSTR)_bstr_t(vat);
								m_ListCard.SetItemText(listi,15,tempcj);								
							}
							vat =m_pRecordset->GetCollect("�۳�����");
							if (vat.vt != VT_NULL)
							{
								tempfs=(LPCSTR)_bstr_t(vat);
								m_ListCard.SetItemText(listi,14,tempfs);
								if (strcmp(tempfs,"0")!=0)
								{
									CString strKch;
									strKch.Format("kch%d",listi);
									::WritePrivateProfileString(strKch,"MSG13",tempsj,".\\allcard.msg");
									::WritePrivateProfileString(strKch,"MSG14",tempkflx,".\\allcard.msg");
									::WritePrivateProfileString(strKch,"MSG15",tempfs,".\\allcard.msg");
									::WritePrivateProfileString(strKch,"MSG8",tempcj,".\\allcard.msg");//�ɼ�
									OnDrawLogo(listi,3);
								}						
							}
							if ((xmcode >201509 && xmcode <201700) || (xmcode >204509 && xmcode <204700) || \
								(xmcode >203509 && xmcode <203700) || (xmcode >206509 && xmcode <206700) || \
						(xmcode >207509 && xmcode <207700) || xmcode==214510 || xmcode==215510 || xmcode==216510)
							{
								OnDrawLogo(listi,2);
							}
							if (  xmcode ==201990 || xmcode ==204990 || xmcode ==203990 || 
						xmcode ==206990 || xmcode ==207990 || xmcode ==214990 || xmcode ==216990 || xmcode ==215990)
							{	
								OnDrawLogo(listi,5);
							}
						}//if				
					}
					pr_count-=1;
					m_pRecordset->MoveNext();
				} //while
				m_pRecordset->Close();
				m_pRecordset.Release();
			}//if m_pRecordset 
		}
	}//for
	}
	catch(_com_error e)
	{
		CString log;
		log.Format("���ô洢����pr_ErrorRecords  ʧ��! ԭ��:%s",e.ErrorMessage());
		WriteLog(log);
		return FALSE;
	}
	return TRUE;
}

void CCBKVideoBMPBackDlg::EmptyList(UINT linei)
{
	ztxmii[linei]=0;
	for (int i=2;i<15;i++)
	{
		m_ListCard.SetItemText(linei,i,"");
	}
}
BOOL CCBKVideoBMPBackDlg::OnprErrorALL()
{
	try
	{
		int listi;
		CString sZkzm;
		for (listi=0;listi<iItemLine;listi++)
		{
			sZkzm.Format("%s",m_ListCard.GetItemText(listi,2));
			if (sZkzm =="")//׼��֤�����Ϊ��
			{
				continue;
			}
			if (m_pConn == NULL) return FALSE;
			m_pConn->CursorLocation =adUseClient;	
			_RecordsetPtr m_pRecordset;
			_CommandPtr m_pCommand;
			m_pRecordset.CreateInstance("ADODB.Recordset");		
			m_pCommand.CreateInstance("ADODB.Command");	
			m_pCommand->ActiveConnection = m_pConn;
			m_pCommand->CommandType = adCmdStoredProc;
			m_pCommand->CommandText = _bstr_t(_T("pr_ErrorRecordALL"));	
			m_pCommand->Parameters->Refresh(); 	
			m_pCommand->Parameters->Item[_variant_t(_bstr_t("@ZKZM_Number"))]->Value =_variant_t(sZkzm);
			m_pRecordset =  m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
			//////////////////////////////////////////////////////////////////////////
			_variant_t vat;
			VARIANT intCol;
			int errxmcode;
			int eline=0;
			CString strtemp,tempfs,tempsj,tempkflx,tempcj;
			m_pCommand.Release();
			m_pCommand.Detach();
			if(m_pRecordset != NULL && (!m_pRecordset->adoEOF))
			{	
				while(!m_pRecordset->adoEOF)
				{
					vat =m_pRecordset->GetCollect("��¼���");
					if (vat.vt != VT_NULL)
					{
						strtemp = (LPCSTR)_bstr_t(vat);						
						m_ListCard.SetItemText(listi,16,strtemp);//����
					}
					intCol =m_pRecordset->GetCollect("������");
					errxmcode=intCol.intVal;
					vat =m_pRecordset->GetCollect("�۷�����");
					if (vat.vt != VT_NULL)
					{
						tempkflx=(LPCSTR)_bstr_t(vat);
						CString strKch;
						strKch.Format("kch%d",listi);
						::WritePrivateProfileString(strKch,"MSG9",tempkflx,".\\allcard.msg");
					}
					
					if ((errxmcode >201509 && errxmcode <201700) || (errxmcode >204509 && errxmcode <204700) || \
						(errxmcode >203509 && errxmcode <203700) || (errxmcode >206509 && errxmcode <206700) || \
						(errxmcode >207509 && errxmcode <207700) || errxmcode==214510 || errxmcode==215510 || errxmcode==216510)
					{
						DrawLogo(2,listi,errxmcode);
					}
					else if ( errxmcode ==201990 || errxmcode ==204990 || errxmcode ==203990 || 
						errxmcode ==206990 || errxmcode ==207990 || errxmcode ==214990 || errxmcode ==216990 || errxmcode ==215990)
					{
						DrawLogo(5,listi,errxmcode);
					}
					else
					{
						vat =m_pRecordset->GetCollect("����ʱ��");
						if (vat.vt != VT_NULL)
						{
							tempsj=(LPCSTR)_bstr_t(vat);							
						}else{tempsj=_T("");}
						vat =m_pRecordset->GetCollect("�۳�����");
						if (vat.vt != VT_NULL)
						{
							tempfs=(LPCSTR)_bstr_t(vat);							
						}else{tempfs=_T("");}
						CString strKch;
						strKch.Format("kch%d",listi);
						::WritePrivateProfileString(strKch,"MSG13",tempsj,".\\allcard.msg");
						::WritePrivateProfileString(strKch,"MSG14",tempkflx,".\\allcard.msg");
						::WritePrivateProfileString(strKch,"MSG15",tempfs,".\\allcard.msg");
						DrawLogo(3,listi,eline);
						eline+=1;
					}
					m_pRecordset->MoveNext();
				}
				m_pRecordset->Close();
				m_pRecordset.Release();
//				ztxmii[listi]=eline;
			}
		}//for
	}
	catch(_com_error e)
	{
		CString log;
		log.Format("���ô洢���� pr_ErrorRecordALL  ʧ��!ԭ��:%s",e.ErrorMessage());
		WriteLog(log);
		return FALSE;
	}
	return TRUE;
}

void CCBKVideoBMPBackDlg::SetOSDKSCC()
{
	CTimeSpan ctempSpan;
	CString sendstr,sZkzm;
	int listi;
	for (listi=0;listi<iItemLine;listi++)
	{
		sZkzm.Format("%s",m_ListCard.GetItemText(listi,2));
		if (sZkzm =="")
		{
			continue;
		}
		ctempSpan = CTime::GetCurrentTime() - xmStartTime[listi];	
		sendstr.Format("%02d:%02d:%02d,%d",ctempSpan.GetHours(),ctempSpan.GetMinutes(),ctempSpan.GetSeconds(),listi);
		mysockz.SendString("127.0.0.1",OSDSC,sendstr,5287);
	}
}

void CCBKVideoBMPBackDlg::OnBtnYpls() 
{
	// TODO: Add your control notification handler code here
	typedef BOOL (* DLGPZVideo)();
	DLGPZVideo pzvideodlg;
	HINSTANCE dllHandlet;
	
	dllHandlet = LoadLibrary(TEXT("CBKVideoPZ.dll")); 
	if (dllHandlet == NULL) 
	{ 
		AfxMessageBox("����CBKVideoPZ.dllʧ��!");
		return ; 
	}  
	pzvideodlg = (DLGPZVideo) GetProcAddress(dllHandlet,TEXT("DLGPZVideo"));	
	pzvideodlg();
}

void CCBKVideoBMPBackDlg::OnDrawLogo(int Listi, int logoi)
{
	int i;
	i=Listi;
	switch (logoi)
	{
	case 1:
		{
			CString strKch;
			strKch.Format("kch%d",i);
			::WritePrivateProfileString(strKch,"MSG9","��ʼ����",".\\allcard.msg");
			pr_count=1;
			SavePho(i);
			DrawLogo(1,i,0);//17C51
			xmStartTime[i]=CTime::GetCurrentTime();
			WriteLog("��ʼ����"+strKch);
			break;
		}
	case 2:
		{
			int xmcode=_ttoi(m_ListCard.GetItemText(i,11));
			DrawLogo(2,i,xmcode);
			CString sendstr;
			CString dvrIP,dvrChannel;
			CString errcode;
			errcode.Format("%d",xmcode);
			if(GetPrivateProfileString(errcode,"Channel","",dvrChannel.GetBuffer(MAX_PATH),MAX_PATH,".\\IPChannel.dat"))
			{
				GetPrivateProfileString(errcode,"IP","",dvrIP.GetBuffer(MAX_PATH),MAX_PATH,".\\IPChannel.dat");
				if((dvrIP.IsEmpty() || dvrChannel.IsEmpty()))
				{
					sendstr.Format("%s,%s,%d",dvrChannel,dvrIP,i);
					mysockz.SendString("127.0.0.1",SXMVideo,sendstr,5287);
					errcode.Format("DisXMChannel :Code=[%d],Send=%s",xmcode,sendstr);
					WriteLog(errcode);
					CFileFind FindDBFile;
					if (FindDBFile.FindFile("ZZIPChannel.dat"))
					{
						//WriteLog("ZZIPChannel.dat");
						if ( (xmcode>=203510 && xmcode <203600) || 214510==xmcode || 215510==xmcode|| 216510==xmcode)
						{
							xmcodethr[i].errori=xmcode;
							xmcodethr[i].thi=i;
							XMcode * mycode =new XMcode;
							mycode->errori=xmcode;
							mycode->thi=i;
							hThread= CreateThread(NULL,0,QHThreadFuncc,mycode,0,0);
							CloseHandle(hThread);
							WriteLog("�໭���л�!����--ʪ��·---���");
						}
						else{
							CString tes;
							tes.Format("�����л���Ŀ[%d]",xmcode);
							WriteLog(tes);
						}
					}
					else
					{
						WriteLog("not find ZZIPChannel.dat ���л�!");
					}
				}
			}
			break;
		}
	case 3:
		{
			DrawLogo(3,i,ztxmii[i]);//���һ������ ��0 ��ʼ 0 1 2
			//ztxmii[i]+=1; �Ĵ����ݿ�ͳ�� 
			break;
		}
	case 4:
		{
			pr_count=1;
			DrawLogo(6,i,2);//���ϸ�	
			if (uXMStop!=0)
			{
				CString sendstr;
				sendstr.Format("%d",i);
				mysockz.SendString("127.0.0.1",EXMVideo,sendstr,5287);
				WriteLog("EXMVideo Stop="+sendstr);
			}
			break;
		}
	case 5:
		{
			int xmcode=_ttoi(m_ListCard.GetItemText(i,11));
			DrawLogo(5,i,xmcode);
			if (uXMStop!=0)
			{
				CString sendstr;
				sendstr.Format("%d",i);
				mysockz.SendString("127.0.0.1",EXMVideo,sendstr,5287);
				WriteLog("EXMVideo Stop="+sendstr);
			}
			break;
		}
	case 6:
		{
			pr_count=1;
			DrawLogo(6,i,1);//�ϸ�
			if (uXMStop!=0)
			{
				CString sendstr;
				sendstr.Format("%d",i);
				mysockz.SendString("127.0.0.1",EXMVideo,sendstr,5287);
				WriteLog("EXMVideo Stop="+sendstr);
			}
			break;
		}
	default:
		break;
	}
	if ((logoi>0 && logoi<7) && pr_count==1)
	{
		CString sendstr;
		sendstr.Format("%d",i);
		mysockz.SendString("127.0.0.1",DisLOGO,sendstr,5287);
		sendstr.Format("DisLOGO sendstr=%d,logoi=%d",i,logoi);
		WriteLog(sendstr);
	}
}

BOOL CCBKVideoBMPBackDlg::OnprErrorN()
{
	try
	{
		int listi;
		CString sZkzm,errjlbh;
		for (listi=0;listi<iItemLine;listi++)
		{
			sZkzm.Format("%s",m_ListCard.GetItemText(listi,2));
			if (sZkzm =="")//׼��֤�����Ϊ��
			{
				continue;
			}
			errjlbh.Format("%s",m_ListCard.GetItemText(listi,16));
			if (m_pConn == NULL) return FALSE;
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
				if (6==retVal )
				{			
					pr_count=m_pRecordset->RecordCount;
					int zti=0,cj=0,si=0,ei=0;
					vat =m_pRecordset->GetCollect("״̬");
					zti = atoi((LPCSTR)_bstr_t(vat));
					vat =m_pRecordset->GetCollect("�ɼ�");
					cj= atoi((LPCSTR)_bstr_t(vat));
					vat =m_pRecordset->GetCollect("S");
					si= vat.lVal;
					vat =m_pRecordset->GetCollect("E");
					ei= vat.lVal;				
					if (zti ==1)
					{
						errjlbh.Format("״̬=%d,�ɼ�=%d,S=%d,E=%d",zti,cj,si,ei);
						WriteLog(errjlbh);
						if ( (si==127 && ei==127) || (si==126 && ei==126))
						{
							EmptyList(listi);
							OnDrawLogo(listi,6);
							WriteLog("OnDrawLogo 6");
						}					
					}
					if (zti ==2)
					{
						errjlbh.Format("״̬=%d,�ɼ�=%d,S=%d,E=%d",zti,cj,si,ei);
						WriteLog(errjlbh);
						if ( cj<80)
						{
							EmptyList(listi);
							OnDrawLogo(listi,4);
							WriteLog("OnDrawLogo 4");
						}
					}
				}//6
				if (2==retVal || 5==retVal || 4==retVal)
				{
					CString strtemp,tempkflx;
					vat =m_pRecordset->GetCollect("��¼���");
					if (vat.vt != VT_NULL)
					{
						strtemp = (LPCSTR)_bstr_t(vat);						
						m_ListCard.SetItemText(listi,16,strtemp);//����
					}
					vat =m_pRecordset->GetCollect("������");
					if (vat.vt != VT_NULL)
					{
						strtemp = (LPCSTR)_bstr_t(vat);
						m_ListCard.SetItemText(listi,11,strtemp);
					}			
					vat =m_pRecordset->GetCollect("�۷�����");
					if (vat.vt != VT_NULL)
					{
						tempkflx=(LPCSTR)_bstr_t(vat);
						m_ListCard.SetItemText(listi,13,tempkflx);
						CString strKch;
						strKch.Format("kch%d",listi);
						::WritePrivateProfileString(strKch,"MSG9",tempkflx,".\\allcard.msg");
					}
					if (2==retVal)
					{
						OnDrawLogo(listi,2);
					}
					else if (5==retVal)
					{
						OnDrawLogo(listi,5);
					}	
					else
					{

					}
				}// 2 5
				if (3==retVal)
				{
					CString strtemp,tempkflx,tempfs,tempsj,tempcj;
					vat =m_pRecordset->GetCollect("��¼���");
					if (vat.vt != VT_NULL)
					{
						strtemp = (LPCSTR)_bstr_t(vat);						
						m_ListCard.SetItemText(listi,16,strtemp);//����
					}		
					vat =m_pRecordset->GetCollect("�۷�����");
					if (vat.vt != VT_NULL)
					{
						tempkflx=(LPCSTR)_bstr_t(vat);
						m_ListCard.SetItemText(listi,13,tempkflx);
						CString strKch;
						strKch.Format("kch%d",listi);
						::WritePrivateProfileString(strKch,"MSG9",tempkflx,".\\allcard.msg");
					}
					vat =m_pRecordset->GetCollect("����ʱ��");
					if (vat.vt != VT_NULL)
					{
						tempsj=(LPCSTR)_bstr_t(vat);
						m_ListCard.SetItemText(listi,12,tempsj);
					}
					vat =m_pRecordset->GetCollect("�ɼ�");
					if (vat.vt != VT_NULL)
					{
						tempcj=(LPCSTR)_bstr_t(vat);
						m_ListCard.SetItemText(listi,15,tempcj);								
					}
					vat =m_pRecordset->GetCollect("ELine");
					int Eline;
					Eline= vat.lVal;
					vat =m_pRecordset->GetCollect("�۳�����");
					if (vat.vt != VT_NULL)
					{
						tempfs=(LPCSTR)_bstr_t(vat);
						m_ListCard.SetItemText(listi,14,tempfs);
						if (strcmp(tempfs,"0")!=0)
						{
							CString strKch;
							strKch.Format("kch%d",listi);
							::WritePrivateProfileString(strKch,"MSG13",tempsj,".\\allcard.msg");
							::WritePrivateProfileString(strKch,"MSG14",tempkflx,".\\allcard.msg");
							::WritePrivateProfileString(strKch,"MSG15",tempfs,".\\allcard.msg");
							::WritePrivateProfileString(strKch,"MSG8",tempcj,".\\allcard.msg");//�ɼ�
							ztxmii[listi]=Eline;
							OnDrawLogo(listi,3);
						}
					}
				}//3
				m_pRecordset->Close();
				m_pRecordset.Release();
			}	//m_pRecordset != NULL	
		}//for
	}
	catch(_com_error e)
	{
		CString log;
		log.Format("���ô洢����pr_ErrorRecordsN  ʧ��! ԭ��:%s",e.ErrorMessage());
		WriteLog(log);
		m_pr_errorint+=1;
		if (m_pr_errorint >10)
		{
			EndDialog(5);
		}
		return FALSE;
	}
	return TRUE;
}

void CCBKVideoBMPBackDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(1);
	KillTimer(2);
}

void CCBKVideoBMPBackDlg::OnSock(WPARAM wParam,LPARAM lParam)
{
	switch(LOWORD(lParam))
	{
	case FD_READ:
		OnReadSock();
		break;
	}
}
void CCBKVideoBMPBackDlg::OnReadSock()
{
	CString strTemp;
	strTemp=mysockz.RecvFromSock();
	if (strTemp.IsEmpty() || strTemp.GetLength()<=2)
	{
		return ;
	}
	UINT uCmd=atoi(strTemp.Left(2));
	strTemp =strTemp.Mid(2);
	switch(uCmd)
	{
	case 30:
		{
			WriteLog("OnReadSock" +strTemp);
			int pos=strTemp.Find(",");
			if (pos >=0)
			{
				int errorid=atoi(strTemp.Left(pos));
				int listi =atoi(strTemp.Mid(pos+1));
				SelectKFLX(errorid,listi);
			}
			break;
		}
	case OSDSuDu://�ٶ�
		{
			mysockz.SendString("127.0.0.1",OSDSuDu,strTemp,5287);
			break;
		}
	default:
		break;
	}
}

BOOL CCBKVideoBMPBackDlg::SelectKFLX(int ErrorID,int listi)
{
	try
	{
		CString sqltemp,tempkflx;
		VARIANT cnt;
		cnt.vt = VT_INT;
		sqltemp.Format("select �۷����� from ErrorData where ������='%d'",ErrorID);//
		_RecordsetPtr m_pRecordset =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(m_pRecordset != NULL && (!m_pRecordset->adoEOF))
		{
					
			vat =m_pRecordset->GetCollect("�۷�����");
			if (vat.vt != VT_NULL)
			{
				tempkflx=(LPCSTR)_bstr_t(vat);
				m_ListCard.SetItemText(listi,13,tempkflx);
				CString strKch;
				strKch.Format("kch%d",listi);
				::WritePrivateProfileString(strKch,"MSG9",tempkflx,".\\allcard.msg");
			}
			if (ErrorID==201990 || ErrorID==204990 || ErrorID==203990 || ErrorID==206990 || ErrorID==207990 || ErrorID==214990 || ErrorID==215990 || ErrorID==216990  )
			{
				OnDrawLogo(listi,5);
			}
			else
			{
				OnDrawLogo(listi,2);
			}
		}
	}
	catch(_com_error e)
	{
		CString log;
		log.Format("��ѯ�۷�����ʧ��! ԭ��:%s",e.ErrorMessage());
		WriteLog(log);
		return FALSE;
	}
	return TRUE;
}
