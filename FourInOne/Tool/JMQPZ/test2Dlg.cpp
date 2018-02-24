// test2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "test2.h"
#include "test2Dlg.h"
#include "HCNetSDK.h"
#include <winsock2.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CString NochangeNum;

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
// CTest2Dlg dialog

CTest2Dlg::CTest2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest2Dlg)
	m_user = _T("admin");
	m_pwd = _T("12345");
	m_port = _T("8000");
	m_num = _T("1");
	m_carnum = _T("");
	m_total = _T("");
	m_play1 = 0;
	m_play2 = 0;
	m_play3 = 0;
	m_play4 = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest2Dlg)
	DDX_Control(pDX, IDC_COMBO_THREE, m_boxthree);
	DDX_Control(pDX, IDC_COMBO_TWO, m_boxtwo);
	DDX_Control(pDX, IDC_COMBO_ONE, m_boxone);
	DDX_Control(pDX, IDC_SecEDIT, m_Secedit);
	DDX_Control(pDX, IDC_IPADDRESS1, m_mIP);
	DDX_Control(pDX, IDC_LIST_Second, m_SecList);
	DDX_Control(pDX, IDC_LIST1, m_JLLlist);
	DDX_Text(pDX, IDC_EDIT2, m_user);
	DDX_Text(pDX, IDC_EDIT3, m_pwd);
	DDX_Text(pDX, IDC_EDIT4, m_port);
	DDX_Text(pDX, IDC_EDIT5, m_num);
	DDX_Text(pDX, IDC_EDIT_Carnum, m_carnum);
	DDX_Text(pDX, IDC_STATIC_NUMtotal, m_total);
	DDX_Text(pDX, IDC_EDIT_play1, m_play1);
	DDV_MinMaxInt(pDX, m_play1, 0, 3);
	DDX_Text(pDX, IDC_EDIT_play2, m_play2);
	DDV_MinMaxInt(pDX, m_play2, 0, 3);
	DDX_Text(pDX, IDC_EDIT_play3, m_play3);
	DDV_MinMaxInt(pDX, m_play3, 0, 3);
	DDX_Text(pDX, IDC_EDIT_play4, m_play4);
	DDV_MinMaxInt(pDX, m_play4, 0, 3);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest2Dlg, CDialog)
	//{{AFX_MSG_MAP(CTest2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, ButtonSave)
	ON_BN_CLICKED(IDC_Research, OnResearch)
	ON_BN_CLICKED(IDC_Delete, OnDelete)
	ON_BN_CLICKED(IDC_ConnectBUTTON, OnConnectBUTTON)
	ON_NOTIFY(NM_CLICK, IDC_LIST_Second, OnClickLISTSecond)
	ON_BN_CLICKED(IDC_BUTTON2, OnCarAddButton)
	ON_BN_CLICKED(IDC_CarDeleteBUTTON, OnCarDeleteBUTTON)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, OnItemchangedList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_Second, OnDblclkLISTSecond)
	ON_EN_KILLFOCUS(IDC_SecEDIT, OnKillfocusSecEDIT)
	ON_BN_CLICKED(IDC_BTNPZ, OnBtnpz)
	ON_CBN_SELCHANGE(IDC_COMBO_ONE, OnSelchangeComboOne)
	ON_CBN_SELCHANGE(IDC_COMBO_TWO, OnSelchangeComboTwo)
	ON_CBN_SELCHANGE(IDC_COMBO_THREE, OnSelchangeComboThree)
	ON_EN_CHANGE(IDC_EDIT_play1, OnChangeEDITplay1)
	ON_EN_CHANGE(IDC_EDIT_play2, OnChangeEDITplay2)
	ON_EN_CHANGE(IDC_EDIT_play3, OnChangeEDITplay3)
	ON_EN_CHANGE(IDC_EDIT_play4, OnChangeEDITplay4)
	ON_BN_CLICKED(IDC_SaveplayBUTTON, OnSaveplayBUTTON)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest2Dlg message handlers

BOOL CTest2Dlg::OnInitDialog()
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

	//��ʼ����������ݣ���ʾ��������
	//////////////////////////////////////////////////////////////////////////

	/////�����ļ���һ���б�
	m_JLLlist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_JLLlist.InsertColumn(0,_T("������"),LVCFMT_LEFT,55,-1);
	m_JLLlist.InsertColumn(1,_T("IP"),LVCFMT_LEFT,130,-1);
	m_JLLlist.InsertColumn(2,_T("�û�"),LVCFMT_LEFT,100,-1);
	m_JLLlist.InsertColumn(3,_T("����"),LVCFMT_LEFT,100,-1);
	m_JLLlist.InsertColumn(4,_T("�˿�"),LVCFMT_LEFT,100,-1);

	////��ʾ�������б�
	m_SecList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
//	m_SecList.InsertColumn(0,_T("���"),LVCFMT_LEFT,50,-1);
//	m_SecList.InsertColumn(1,_T("������"),LVCFMT_LEFT,120,-1);
	m_SecList.InsertColumn(0,_T("ͨ����"),LVCFMT_LEFT,150,-1);
	m_SecList.InsertColumn(1,_T("������"),LVCFMT_LEFT,150,-1);
	
	//����һ���༭���������б�ؼ��У����б�ؼ���ɿɱ༭�Ŀؼ�
	m_Secedit.ShowWindow(SW_HIDE);

	m_boxone.ResetContent();
	m_boxone.AddString(_T("���н���eg:1,3,5,7"));
	m_boxone.AddString(_T("�����н���,ȫ������"));
	int z = GetPrivateProfileInt("JMQ","EVEN",0,".//config.ini");
	m_boxone.SetCurSel(z);

	m_boxtwo.ResetContent();
	m_boxtwo.AddString(_T("2"));
	m_boxtwo.AddString(_T("3"));

	int i = GetPrivateProfileInt("CONFIG","KSKM",0,".//config.ini");
	int m = 2,n;
	n = i-m;
	m_boxtwo.SetCurSel(n);

	m_boxthree.ResetContent();
	m_boxthree.AddString(_T("1"));
	m_boxthree.AddString(_T("2"));
	m_boxthree.AddString(_T("3"));
	m_boxthree.AddString(_T("4"));
	int q = GetPrivateProfileInt("CONFIG","VIDEOWND",0,".//config.ini");
	int l;
	l = q-1;
	m_boxthree.SetCurSel(l);

	int play1 = GetPrivateProfileInt("CONFIG","DISPLAY1",0,".//config.ini");
	int play2 = GetPrivateProfileInt("CONFIG","DISPLAY2",0,".//config.ini");
	int play3 = GetPrivateProfileInt("CONFIG","DISPLAY3",0,".//config.ini");
	int play4 = GetPrivateProfileInt("CONFIG","DISPLAY4",0,".//config.ini");

	CString temp1="",temp2="",temp3="",temp4="";
	temp1.Format("%d",play1);
	temp2.Format("%d",play2);
	temp3.Format("%d",play3);
	temp4.Format("%d",play4);
	GetDlgItem(IDC_EDIT_play1)->SetWindowText(temp1);
	GetDlgItem(IDC_EDIT_play2)->SetWindowText(temp2);
	GetDlgItem(IDC_EDIT_play3)->SetWindowText(temp3);
	GetDlgItem(IDC_EDIT_play4)->SetWindowText(temp4);

	Oninittext();

	//////////////////////////////////////////////////////////////////////////
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
/////////����б�ؼ���������ʾ
BOOL CTest2Dlg::Oninittext()
{
	UpdateData(TRUE);
	UINT inum;
	int m_num=0,pos=0,i=1;
	CString temp,temp1,temp2,temp4,sIPaddr,sUser,sPwd,sPort;
	
	m_JLLlist.DeleteAllItems();

	inum = GetPrivateProfileInt("JMQ","NUM",0,".//config.ini");	

	for (i;i <= (int)inum;i++)
	{
		temp4.Format("%d",i);
		GetPrivateProfileString("JMQ",temp4,"",temp.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
		temp.ReleaseBuffer();

		temp1.Format("%d",m_num+1);
		m_JLLlist.InsertItem(m_num,temp1);
		/////�������ļ����е����ݷֿ����	

		pos = temp.Find(",");
		sIPaddr = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,1,sIPaddr);
			
		temp2 = temp.Mid(pos+1);
		pos = temp2.Find(",");
		sUser = temp2.Left(pos);
		m_JLLlist.SetItemText(m_num,2,sUser);
			
		temp = temp2.Mid(pos+1);
		pos = temp.Find(",");
		sPwd = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,3,sPwd);
			
		sPort = temp.Mid(pos+1);
		m_JLLlist.SetItemText(m_num,4,sPort);
			
		m_num++;	
	}
	m_total.Format("%d",inum);
	
	SetDlgItemText(IDC_STATIC_NUMtotal,m_total);
	return true;
}
void CTest2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTest2Dlg::OnPaint() 
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
HCURSOR CTest2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////����б�ؼ������ݵı��������
void CTest2Dlg::ButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp,temp1,temp9,strIP,temp6,inum,temp5;

	if (m_num.IsEmpty() || m_port.IsEmpty() || m_pwd.IsEmpty() ||  m_user.IsEmpty() )
	{
		AfxMessageBox("������ʾ:������������Ϣ�������޸Ļ����");
		return ;
	}

	BYTE nField0,nField1,nField2,nField3;
	m_mIP.GetAddress(nField0,nField1,nField2,nField3);
	temp6.Format("%u.%u.%u.%u",nField0,nField1,nField2,nField3);

//	temp6.Format("%s",strIP);

	temp9.Format("�Ƿ�Ҫ�޸Ļ���ӣ� \n ������:%s\n IP:%s\n �û���:%s\n ����:%s\n �˿�:%s\n\n�����������䣬�����޸ģ�",m_num,temp6,m_user,m_pwd,m_port);
	if (MessageBox(temp9,"��ʾ",MB_OKCANCEL) == IDOK)
	{	//�޸�
		temp1.Format("%s",m_num);
		temp.Format("%s,%s,%s,%s",temp6,m_user,m_pwd,m_port);
		::WritePrivateProfileString("JMQ",temp1,temp,".//config.ini");		
		GetPrivateProfileString("JMQ","NUM","", inum.GetBuffer(MAX_PATH), MAX_PATH,".//config.ini");
		if ( _ttoi(m_num)>_ttoi(inum))
		{	//�����ֶ������ʱ�����ӵĻ���һ��Ĭ��ֵ
			///////////////////////////////////////////////////
			for (int i = _ttoi(inum)+1 ; i <= _ttoi(m_num) ; i++)
			{
				temp5.Format("%d",i);
				::WritePrivateProfileString("JMQ",temp5,temp,".//config.ini");	
			}		
			WritePrivateProfileString("JMQ","NUM",m_num,".//config.ini");
		}
	}
	Oninittext();
}
///�Զ���ѯ���������ļ����е����ݵ��б�ؼ�����
void CTest2Dlg::OnResearch() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str,temp9,temp,temp1,temp2,temp4,sIPaddr,sUser,sPwd,sPort;
	UINT inum;
	int m_num=0,pos=0,i=1;

	temp9.Format("������������");
	AfxMessageBox(temp9);

	m_JLLlist.DeleteAllItems();

	inum = GetPrivateProfileInt("JMQ","NUM",0,".//config.ini");
		
	for (i;i <= (int)inum;i++)
	{
		temp4.Format("%d",i);
		GetPrivateProfileString("JMQ",temp4,"0",temp.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
		temp.ReleaseBuffer();
			
		temp1.Format("%d",m_num+1);
		m_JLLlist.InsertItem(m_num,temp1);
		/////�������ļ��е����ݷֿ���ʾ			

		pos = temp.Find(",");
		sIPaddr = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,1,sIPaddr);
			
		temp2 = temp.Mid(pos+1);
		pos = temp2.Find(",");
		sUser = temp2.Left(pos);
		m_JLLlist.SetItemText(m_num,2,sUser);
			
		temp = temp2.Mid(pos+1);
		pos = temp.Find(",");
		sPwd = temp.Left(pos);
		m_JLLlist.SetItemText(m_num,3,sPwd);
			
		sPort = temp.Mid(pos+1);
		m_JLLlist.SetItemText(m_num,4,sPort);
		
		m_num++;
	}		
}
//////����б����ݵ�ɾ��
void CTest2Dlg::OnDelete() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (m_num.IsEmpty() || m_port.IsEmpty() || m_pwd.IsEmpty() ||  m_user.IsEmpty())
	{
		AfxMessageBox("������ʾ:��д��������Ѿ�Ϊ��");
		return ;
	}
	CString str,temp,strIP,temp6,temp2;
	int a=0,b=0,i=0,inum=0;
	unsigned char *pIP;
	DWORD dwIP;

	m_mIP.GetAddress(dwIP);
	pIP = (unsigned char*)&dwIP;
	strIP.Format("%u.%u.%u.%u",*(pIP+3),*(pIP+2),*(pIP+1),*pIP);
	temp6.Format("%s",strIP);

	temp.Format("�Ƿ�Ҫɾ��:\n ������:%s\n IP:%s\n �û���:%s\n ����:%s\n �˿�:%s\n",m_num,temp6,m_user,m_pwd,m_port);
	str.Format("");
	if (MessageBox(temp,"��ʾ",MB_OKCANCEL) == IDOK)
	{ 
		//ʵ����ɾ����ʱ��ɾ��һ�У����Զ�����
	    //////////////////////////////////////////////////////////////////
		::WritePrivateProfileString("JMQ",m_num,str,".//config.ini");
		inum = GetPrivateProfileInt("JMQ","NUM",0,".//config.ini");			
		b = _ttoi(m_num)+1;	
		for(i = b;i <= inum;i++)
		{
			temp.Format("%d", i);
			GetPrivateProfileString("JMQ",temp,"",temp.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
			temp2.Format("%d", b-1);
			WritePrivateProfileString("JMQ",temp2,temp,".//config.ini");
			b++;
		}
		temp.Format("%d", inum-1);
	    ::WritePrivateProfileString("JMQ","NUM",temp,".//config.ini");
		temp.Format("%d", inum);
		WritePrivateProfileString("JMQ",temp,NULL,".//config.ini");	
		///////////////////////////////////////////////////////////////
		m_num = _T("");
		m_port = _T("");
		m_pwd = _T("");
	//	m_mIP=_T("");
		m_user = _T("");
	}
	////ɾ��֮����տؼ�����
	GetDlgItem(IDC_EDIT2)->SetWindowText("");
	GetDlgItem(IDC_EDIT3)->SetWindowText("");
	GetDlgItem(IDC_EDIT5)->SetWindowText("");
	GetDlgItem(IDC_EDIT4)->SetWindowText("");
	GetDlgItem(IDC_IPADDRESS1)->SetWindowText("");

	Oninittext();
	UpdateData(FALSE);
}
///////�����ݿ���������ȡͨ����
void CTest2Dlg::OnConnectBUTTON() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	NET_DVR_Init();
	//ע���豸	
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	//IP�ؼ�ת�����CString����
	unsigned char *pIP;
	CString strIP,temp5,temp,temp9;
	DWORD dwIP;

	m_mIP.GetAddress(dwIP);
	pIP = (unsigned char*)&dwIP;
	strIP.Format("%u.%u.%u.%u",*(pIP+3),*(pIP+2),*(pIP+1),*pIP);
	temp5.Format("%s",strIP);

	lUserID = NET_DVR_Login_V30(temp5.GetBuffer(0), atoi(m_port), m_user.GetBuffer(0),m_pwd.GetBuffer(0), &struDeviceInfo);
	if (lUserID < 0)
	{
		AfxMessageBox("����ʧ��");
		m_SecList.DeleteAllItems();
		NET_DVR_Cleanup();
		return;
	}
	else
	{
		AfxMessageBox("��¼�ɹ�");
		temp.Format("JMQ%s",m_num);
		//ͨ��SDK����豸��������ȡÿ��IP��ͨ����
		NET_DVR_MATRIX_ABILITY_V41 struDecoderCapability={0};
		NET_DVR_GetDeviceAbility(lUserID,MATRIXDECODER_ABILITY_V41,NULL,0,(char *)&struDecoderCapability,sizeof(struDecoderCapability));
		int bcnum = struDecoderCapability.struBncInfo.byChanNums;
		temp9.Format("%d",bcnum);
		//д�뵽�ļ�����ͨ������
		WritePrivateProfileString(temp,"N",temp9,".//config.ini");
		//ˢ���б���ʾ����
		OinitextSecond(m_num);
		return;
	}
}
///�ұ��б�ؼ�ˢ��������ʾ
void CTest2Dlg::OinitextSecond(CString m_num)
{
	UpdateData(FALSE);
	CString temp,temp1,temp2,temp3,temp4,temp5,temp9;
	int m=0,inum,z,k=1;
	m_SecList.DeleteAllItems();

	temp.Format("JMQ%s",m_num);
	//��ȡÿ����������ͨ������
	inum = GetPrivateProfileInt(temp,"N",0,".//config.ini");

	z = _ttoi(m_total);
	  //ͨ��ѭ��������д��
		for (k = 1 ; k <= inum ; k++)
		{	
			temp2.Format("%d",k);
			temp3.Format("%d",m);
			temp4.Format("%s",m_num);
//			m_SecList.InsertItem(m,temp3);		
//			m_SecList.SetItemText(m,1,temp4);
			m_SecList.InsertItem(m,temp2);
			//�����ļ������ŵĻ�ȡ
			temp5.Format("BNC%s",temp2);
			GetPrivateProfileString(temp,temp5,"",temp1.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
			m_SecList.SetItemText(m,1,temp1);
		    m++;
		}		
}
///�ѿ�������Ϣ��ʾ���༭�ؼ�����
void CTest2Dlg::OnClickLISTSecond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_carnum = m_SecList.GetItemText(pNMListView->iItem,1);
	UpdateData(FALSE);
	///��ȡ��ͨ������д�뿼���ŵ������ļ�
	m_passway = m_SecList.GetItemText(pNMListView->iItem,0);
	*pResult = 0;
}
/////���ӻ����޸������ļ��еĿ�����
void CTest2Dlg::OnCarAddButton() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp9,temp1,temp2,temp3,temp4,temp5,temp6,str;
	int inum,number,compare,k=1;
	temp1.Format("JMQ%s",m_num);
	temp2.Format("BNC%s",m_passway);
	temp3.Format("%s",m_carnum);

	if (m_carnum.IsEmpty())
	{
		AfxMessageBox("������ʾ:�����Ų��ܿ�");
		return ;
	}
	//��ȡÿ����������ͨ������
	inum = GetPrivateProfileInt(temp1,"N",0,".//config.ini");
	number = inum;
	if (number == 0)
	{
		AfxMessageBox("��������ʧ�ܣ��޷���ӿ�����");
		m_carnum=_T("");
		return;
	}

	compare=_ttoi(m_passway);
	if ( compare > number)
	{
		AfxMessageBox("��ѡ���ͨ����,����ͨ���ŵĸ�����������ѡ��ͨ����");
		return;
	}
	if (temp2 == "BNC")
	{
		AfxMessageBox("��ѡ��ͨ����");
		return;
	}
	//�ж����ļ��п������Ƿ��Ѿ����ڹ���
	for (k ;k <= inum ; k++)
	{
		temp5.Format("BNC%d",k);
		GetPrivateProfileString(temp1,temp5,"",temp6.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
		temp6.ReleaseBuffer();
		if (temp3 == temp6)
		{
			str.Format("%s�������Ѿ�����,�������뿼����",temp3);
			AfxMessageBox(str);
			GetDlgItem(IDC_EDIT_Carnum)->SetWindowText("");
			return;
		}
	}
	temp9.Format("�Ƿ�Ҫ�޸Ļ����:\nͨ����Ϊ:%s ������:%s\n",m_passway,m_carnum);
	if (MessageBox(temp9,"��ʾ",MB_OKCANCEL) == IDOK)
	{	//�޸Ļ�������
		WritePrivateProfileString(temp1,temp2,temp3,".//config.ini");
	}

	OinitextSecond(m_num);

}
//////ɾ��������
void CTest2Dlg::OnCarDeleteBUTTON() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp1,temp2,temp,temp5;
	int num,compare,number;

	temp5.Format("JMQ%s",m_num);
	//��ȡÿ����������ͨ������
	if (m_carnum.IsEmpty())
	{
		AfxMessageBox("������ʾ:�����Ų��ܿ�");
		return ;
	}
	num = GetPrivateProfileInt(temp5,"N",0,".//config.ini");
	if (num == 0)
	{
		AfxMessageBox("��������ʧ�ܣ��޷���ӿ�����");
		m_carnum = _T("");
		return;
	}
	number = num;
	compare = _ttoi(m_passway);
	if ( compare > number)
	{
		AfxMessageBox("��ѡ���ͨ����,����ͨ���ŵĸ�����������ѡ��ͨ����");
		return;
	}
	temp2.Format("BNC%s",m_passway);
	if (temp2 == "BNC")
	{
		AfxMessageBox("��ѡ��ͨ����");
		return;
	}
	temp.Format("�Ƿ�Ҫɾ��:\n ͨ����Ϊ:%s ������:%s\n",m_passway,m_carnum);	
	if (MessageBox(temp,"��ʾ",MB_OKCANCEL) == IDOK)
	{
		temp1.Format("JMQ%s",m_num);

		//ɾ�����ݵ������ļ���
		WritePrivateProfileString(temp1,temp2,NULL,".//config.ini");			
	}
	m_carnum = _T("");
	//ˢ�½���
	OinitextSecond(m_num);
}
//��Ӧ�¼�ˢ�½��浱������б�ؼ����Զ�ˢ������һ���б�
void CTest2Dlg::OnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CString temp,temp1;

	m_mIP.SetWindowText(m_JLLlist.GetItemText(pNMListView->iItem,1));

	m_user = m_JLLlist.GetItemText(pNMListView->iItem,2);
	m_pwd = m_JLLlist.GetItemText(pNMListView->iItem,3);
	m_port = m_JLLlist.GetItemText(pNMListView->iItem,4);
	m_num = m_JLLlist.GetItemText(pNMListView->iItem,0);	
	
	temp1.Format("JMQ%s",m_num);
	int inum = GetPrivateProfileInt(temp1,"N",0,".//config.ini");
	if (inum == 0)
	{
		m_SecList.DeleteAllItems();
	}
	else
	{
		OinitextSecond(m_num);
	}			
	m_carnum = _T("");
	UpdateData(FALSE);
	*pResult = 0;
}

/////˫���ı��б�ؼ�Ϊ�༭����������
void CTest2Dlg::OnDblclkLISTSecond(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
    NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;  
    CRect rc;  
    m_Row = pNMListView->iItem;//���ѡ�е���  
    m_Col = pNMListView->iSubItem;//���ѡ����  	   	   
    if (pNMListView->iSubItem != 0) //���ѡ���������;  
    {  
	    m_SecList.GetSubItemRect(m_Row,m_Col,LVIR_LABEL,rc);//��������RECT��
  	    if (m_Row == -1)
		{
	 	    return;
		}		   
	    m_Secedit.SetParent(&m_SecList);//ת������Ϊ�б���е�����  
	    m_Secedit.MoveWindow(rc);//�ƶ�Edit��RECT���ڵ�λ��;  
	    m_Secedit.SetWindowText(m_SecList.GetItemText(m_Row,m_Col));//���������е�ֵ����Edit�ؼ��У�  
	    m_Secedit.ShowWindow(SW_SHOW);//��ʾEdit�ؼ�  
	    m_Secedit.SetFocus();//����Edit����  
	    m_Secedit.ShowCaret();//��ʾ���  
	    m_Secedit.SetSel(-1);//������ƶ������ 
	}
	///����һ��ȫ�ֱ����ǵ����ݱ��治���� 
	NochangeNum=m_num;
    *pResult = 0;
}
///���б�ؼ��б༭��д�������ļ�
void CTest2Dlg::SetListItemText(void)  
{  		
	UpdateData(TRUE);
	CString tem,temp1,temp2,temp6,temp5;
	int inum,k = 1;
	m_Secedit.GetWindowText(tem);    //�õ��û�������µ�����  	
	if (m_Col == 1)
	{			
		temp1.Format("JMQ%s",m_num);
		temp2.Format("BNC%s",m_passway);			
		//��ȡÿ����������ͨ������
		inum = GetPrivateProfileInt(temp1,"N",0,".//config.ini");
		if (inum == 0)
		{
		//	AfxMessageBox("��������ʧ�ܣ��޷���ӿ�����");
			m_Secedit.SetWindowText(_T(""));
			m_Secedit.ShowWindow(SW_HIDE);
			return;
		}

		//ʹ��һ��ȫ�ֱ�������֤��д�����ݵ�ʱ�򲻻ᷢ��д�����
		//////////////////////////////////////////////////////////////////////////
		if (NochangeNum != m_num)
		{
			m_Secedit.SetWindowText(_T(""));
			m_Secedit.ShowWindow(SW_HIDE);
			return;
		}
		//////////////////////////////////////////////////////////////////////////

		//�ж����ļ��п������Ƿ��Ѿ����ڹ���
		for (k ; k <= inum ; k++)
		{
			temp5.Format("BNC%d",k);
			GetPrivateProfileString(temp1,temp5,"",temp6.GetBuffer(MAX_PATH),MAX_PATH,".//config.ini");
			temp6.ReleaseBuffer();
			if (tem == temp6)
			{
				m_Secedit.SetWindowText(_T(""));
				m_Secedit.ShowWindow(SW_HIDE);
				return;
			}
		}

		WritePrivateProfileString(temp1,temp2,tem,".//config.ini");									
		m_SecList.SetItemText(m_Row,m_Col,tem);   //���ñ༭��������� 
		GetDlgItem(IDC_EDIT_Carnum)->SetWindowText(tem);//���õ��༭����ͬ������
		m_Secedit.ShowWindow(SW_HIDE);				
	}
	
} 
///���ʧȥ�����¼����� 
void CTest2Dlg::OnKillfocusSecEDIT() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDIT_Carnum)->SetWindowText("");
	//д�뵽�����ļ�����
	SetListItemText();

}
////ͨ����Ӧ���ð�ť
void CTest2Dlg::OnBtnpz() 
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
	FreeLibrary(dllHandlet);
}
////��Ӧ�¼�ͨ���������EVENд�������ļ�����
void CTest2Dlg::OnSelchangeComboOne() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxone.GetCurSel();
	CString strCBText,temp;	
	m_boxone.GetLBText(nIndex,strCBText);
	temp.Format("�Ƿ�Ҫ����EVENΪ:%s",strCBText);	
	if (MessageBox(temp,"��ʾ",MB_OKCANCEL) == IDOK)
	{
		if (_T("�����н���,ȫ������")==strCBText)
		{
			strCBText=_T("0");
		}
		else
		{
			strCBText=_T("1");
		}
		WritePrivateProfileString("JMQ","EVEN",strCBText,".//config.ini");
	}

}
////��Ӧ�¼�ͨ���������KSKMд�������ļ�����
void CTest2Dlg::OnSelchangeComboTwo() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxtwo.GetCurSel();
	CString strCBText,temp;	
	m_boxtwo.GetLBText(nIndex,strCBText);
	temp.Format("�Ƿ�Ҫ����KSKMΪ:%s",strCBText);	
	if (MessageBox(temp,"��ʾ",MB_OKCANCEL) == IDOK)
	{
		WritePrivateProfileString("CONFIG","KSKM",strCBText,".//config.ini");
	}
}
////��Ӧ�¼�ͨ���������VIDEOWNDд�������ļ�����
void CTest2Dlg::OnSelchangeComboThree() 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_boxthree.GetCurSel();
	CString strCBText,temp;	
	m_boxthree.GetLBText(nIndex,strCBText);
	temp.Format("�Ƿ�Ҫ����VIDEOWNDΪ:%s",strCBText);	
	if (MessageBox(temp,"��ʾ",MB_OKCANCEL) == IDOK)
	{
		WritePrivateProfileString("CONFIG","VIDEOWND",strCBText,".//config.ini");
	}
}
//��Ӧ�¼�������ֻ�ܱ�������0��3֮��
void CTest2Dlg::OnChangeEDITplay1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num1;
	GetDlgItem(IDC_EDIT_play1)->GetWindowText(tmp); //ÿ�����붼��ȡ�����ж� ������ʾ����

	num1 = tmp.GetLength();
	if(num1 == 1  || num1 == 0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("д�������ֻ����  0,1,2,3");
			GetDlgItem(IDC_EDIT_play1)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY1������ֻ����0,1,2,3,���Ҳ���������ͬ,����������!");
		GetDlgItem(IDC_EDIT_play1)->SetWindowText("");
	}
}
//��Ӧ�¼�������ֻ�ܱ�������0��3֮��
void CTest2Dlg::OnChangeEDITplay2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num2;
	GetDlgItem(IDC_EDIT_play2)->GetWindowText(tmp); //ÿ�����붼��ȡ�����ж� ������ʾ����
	num2 = tmp.GetLength();
	if (num2 == 1  || num2 == 0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("д�������ֻ����  0,1,2,3");
			GetDlgItem(IDC_EDIT_play2)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY2������ֻ����0,1,2,3,���Ҳ���������ͬ,����������!");
		GetDlgItem(IDC_EDIT_play2)->SetWindowText("");
	}

}
//��Ӧ�¼�������ֻ�ܱ�������0��3֮��
void CTest2Dlg::OnChangeEDITplay3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num3;
	GetDlgItem(IDC_EDIT_play3)->GetWindowText(tmp); //ÿ�����붼��ȡ�����ж� ������ʾ����
	num3 = tmp.GetLength();
	if (num3 == 1 || num3 ==0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("д�������ֻ����  0,1,2,3");
			GetDlgItem(IDC_EDIT_play3)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY3������ֻ����0,1,2,3,���Ҳ���������ͬ,����������!");
		GetDlgItem(IDC_EDIT_play3)->SetWindowText("");
	}

}
//��Ӧ�¼�������ֻ�ܱ�������0��3֮��
void CTest2Dlg::OnChangeEDITplay4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString tmp = "";
	int num4;
	GetDlgItem(IDC_EDIT_play4)->GetWindowText(tmp); //ÿ�����붼��ȡ�����ж� ������ʾ����
	num4 = tmp.GetLength();
	if ( num4 == 1 || num4 == 0)
	{
		if (atoi(tmp) > 3)
		{
			AfxMessageBox("д�������ֻ����  0,1,2,3");
			GetDlgItem(IDC_EDIT_play4)->SetWindowText("");	
		}
	}
	else
	{
		AfxMessageBox("DISPLAY4������ֻ����0,1,2,3,���Ҳ���������ͬ,����������!");
		GetDlgItem(IDC_EDIT_play4)->SetWindowText("");
	}

}
////�����ļ���DISPLAY�ı���
void CTest2Dlg::OnSaveplayBUTTON() 
{
	// TODO: Add your control notification handler code here
	CString temp1 = "",temp2 = "",temp3 = "",temp4 = "",temp;
	GetDlgItem(IDC_EDIT_play1)->GetWindowText(temp1);
	GetDlgItem(IDC_EDIT_play2)->GetWindowText(temp2);
	GetDlgItem(IDC_EDIT_play3)->GetWindowText(temp3);
	GetDlgItem(IDC_EDIT_play4)->GetWindowText(temp4);
	if (temp1.IsEmpty() || temp2.IsEmpty() ||temp3.IsEmpty() ||temp4.IsEmpty() )
	{
		AfxMessageBox("DISPLAY���ݲ���Ϊ��");
		return;
	}
	//�ж�ֵ�Ƿ����ظ��������Ƚ�LOW,һ�����Ļ���ƥ��
	//////////////////////////////////////////////////////////////////////////
	if (temp1 == temp2)
	{
		AfxMessageBox("DISPLAY����ֵ������������ͬ");
		return;
	}
	if (temp1 == temp3)
	{
		AfxMessageBox("DISPLAY����ֵ������������ͬ");
		return;
	}
	if (temp1 == temp4)
	{
		AfxMessageBox("DISPLAY����ֵ������������ͬ");
		return;
	}
	if (temp2 == temp3)
	{
		AfxMessageBox("DISPLAY����ֵ������������ͬ");
		return;
	}
	if (temp2 == temp4)
	{
		AfxMessageBox("DISPLAY����ֵ������������ͬ");
		return;
	}
	if (temp3 == temp4)
	{
		AfxMessageBox("DISPLAY����ֵ������������ͬ");
		return;
	}
	//////////////////////////////////////////////////////////////////////////
	///д�������ļ�
	temp.Format("�Ƿ�Ҫ����\n DISPLAY1Ϊ:%s\n DISPLAY2Ϊ:%s\n DISPLAY3Ϊ:%s\n DISPLAY4Ϊ:%s",temp1,temp2,temp3,temp4);	
	if (MessageBox(temp,"��ʾ",MB_OKCANCEL) == IDOK)
	{
		WritePrivateProfileString("CONFIG","DISPLAY1",temp1,".//config.ini");
		WritePrivateProfileString("CONFIG","DISPLAY2",temp2,".//config.ini");
		WritePrivateProfileString("CONFIG","DISPLAY3",temp3,".//config.ini");
		WritePrivateProfileString("CONFIG","DISPLAY4",temp4,".//config.ini");
	}
	//////////////////////////////////////////////////////////////////////////
}
