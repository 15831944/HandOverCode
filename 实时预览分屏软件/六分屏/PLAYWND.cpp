// PLAYWND.cpp : implementation file
//

#include "stdafx.h"
#include "CFourPic.h"
#include "PLAYWND.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPLAYWND dialog


CPLAYWND::CPLAYWND(CWnd* pParent /*=NULL*/)
	: CDialog(CPLAYWND::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPLAYWND)
	m_MSG = _T("");
	//}}AFX_DATA_INIT
}


void CPLAYWND::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPLAYWND)
	DDX_Text(pDX, IDC_STATIC_MSG, m_MSG);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPLAYWND, CDialog)
	//{{AFX_MSG_MAP(CPLAYWND)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDM_MREALPLAY, OnMrealplay)
	ON_COMMAND(IDM_MSOUNDSHARE, OnMsoundshare)
	ON_COMMAND(IDM_MSWITCH, OnMswitch)
	ON_COMMAND(IDM_MVOICECOM, OnMvoicecom)
	ON_COMMAND(IDM_EXIT, OnMExit)
	ON_COMMAND(IDM_MSTARTALL, OnMstartall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPLAYWND message handlers


void CPLAYWND::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,CliRcMin.x,CliRcMin.y,SWP_NOMOVE | SWP_NOOWNERZORDER );
}

void CPLAYWND::SetCliRcSize(int x, int y)
{
	CliRcMin.x=x;
	CliRcMin.y=y;
}

BOOL CPLAYWND::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString temp;
	temp.Format("����%d",iKch);	
	GetDlgItem(IDC_WNDMIN)->MoveWindow(0,0,CliRcMin.x,CliRcMin.y-24);
	GetDlgItem(IDC_STATIC_MSG)->MoveWindow(2,CliRcMin.y-24,CliRcMin.x-4,24);
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText(temp);

	for (int i=0;i<3;i++)
	{
		m_lUserID[i]=-1;
		m_lChannel[i]=-1;
		m_lPlayHandle[i]=-1;
		m_byStartDTalkChan[i]=-1;
		m_lVoiceHandle[i]=-1;
	}

	hbr1=CreateSolidBrush(RGB(17,20,26));
	hbr2=CreateSolidBrush(RGB(30,76,100));
	menu.LoadMenu(IDR_MENU1);

	m_bRealPlay=false;
	m_bPlaySound=false;
	m_bSwitch=false;
	m_bVoiceCom=false;
	if(!NET_DVR_SetAudioMode(2))
	{
		WriteLog("�豸��������NET_DVR_SetAudioMode faild ����%d! Errid=%d",iKch,NET_DVR_GetLastError());
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPLAYWND::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

bool CPLAYWND::DvrInitLogin(int pzi)
{
	CString temp,temp1,temp2;
	temp1.Format("����%d",iKch);
	temp2.Format("%d",pzi);
	GetPrivateProfileString(temp1,temp2,"",temp.GetBuffer(MAX_PATH),MAX_PATH,".\\aconfig.ini");
	temp.ReleaseBuffer();
	if (temp.IsEmpty())
	{
		WriteLog("����%d,�ĵ�һ���л�û������",iKch);
		return false;
	}
	int pos=temp.Replace(",",",");
	if (pos !=4)
	{
		WriteLog("��ȡ���ó���%d,[%s]",iKch,temp);
		return false;
	}
	CString sIPaddr,sUser,sPwd,sPort,sChannel;
	pos =temp.Find(",");
	sIPaddr =temp.Left(pos);
	temp1=temp.Mid(pos+1);
	pos =temp1.Find(",");
	sUser=temp1.Left(pos);
	temp=temp1.Mid(pos+1);
	pos =temp.Find(",");
	sPwd =temp.Left(pos);
	temp1=temp.Mid(pos+1);
	pos =temp1.Find(",");
	sPort=temp1.Left(pos);
	sChannel=temp1.Mid(pos+1);
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	memset(&struDeviceInfo, 0, sizeof(NET_DVR_DEVICEINFO_V30));//����豸�����Ľṹ��
	LONG lUserID;
	lUserID = NET_DVR_Login_V30(sIPaddr.GetBuffer(0),atoi(sPort),sUser.GetBuffer(0),sPwd.GetBuffer(0), &struDeviceInfo);
	if (lUserID < 0)
	{
		WriteLog("����%d��¼ʧ��! NET_DVR_Login_V30 failed! Errid=%d,IP=%s,USER=%s,PWD=%s,PORT=%s",\
			iKch,NET_DVR_GetLastError(),sIPaddr,sUser,sPwd,sPort);
		return false;
	}
	m_byStartDTalkChan[pzi]=struDeviceInfo.byStartDTalkChan;
	m_lUserID[pzi]=lUserID;//�����¼ID
	m_lChannel[pzi]=atol(sChannel);
	WriteLog("[����%d]��¼�ɹ�!,IP=%s,USER=%s,PWD=%s,PORT=%s ",iKch,sIPaddr,sUser,sPwd,sPort);
	return true;
}

bool CPLAYWND::DvrRealPlay(int pzi)
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = GetDlgItem(IDC_WNDMIN)->GetSafeHwnd();
	ClientInfo.lChannel     = m_lChannel[pzi];
	ClientInfo.lLinkMode    = 0;//0x80000000;//������ TCP
    ClientInfo.sMultiCastIP = NULL;
	
	LONG lPlayHandle;
	lPlayHandle= NET_DVR_RealPlay_V30(m_lUserID[pzi],&ClientInfo,NULL,NULL,TRUE);
	if(-1 == lPlayHandle)
	{
		WriteLog("ʵʱԤ��ʧ��!Error=%d,����%d",NET_DVR_GetLastError(),pzi);
		return false;
	}
	m_lPlayHandle[pzi]=lPlayHandle;
	return true;
}

BOOL CPLAYWND::PreTranslateMessage(MSG* pMsg) 
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
				//return TRUE;
				::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
				//	GetParent()->SendMessage(WM_CLOSE);
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

void CPLAYWND::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	pSubMenu = menu.GetSubMenu(0);
	if (m_bRealPlay)
	{
		pSubMenu->ModifyMenu(0, MF_BYPOSITION, IDM_MREALPLAY, "�ر�Ԥ��");
		pSubMenu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
		pSubMenu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);
		pSubMenu->EnableMenuItem(3, MF_BYPOSITION | MF_ENABLED);
		if (m_bVoiceCom)
		{
			pSubMenu->ModifyMenu(2, MF_BYPOSITION, IDM_MVOICECOM, "ֹͣ�Խ�");
		}
		else
		{
			pSubMenu->ModifyMenu(2, MF_BYPOSITION, IDM_MVOICECOM, "�����Խ�");
		}
		if (m_bPlaySound)
		{
			pSubMenu->ModifyMenu(3, MF_BYPOSITION, IDM_MSOUNDSHARE, "�ر�����");
		}
		else
		{
			pSubMenu->ModifyMenu(3, MF_BYPOSITION, IDM_MSOUNDSHARE, "������");
		}
	}
	else
	{
		pSubMenu->ModifyMenu(0, MF_BYPOSITION, IDM_MREALPLAY, "����Ԥ��");
		pSubMenu->EnableMenuItem(1, MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
		pSubMenu->EnableMenuItem(2, MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
 		pSubMenu->EnableMenuItem(3, MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
	}
	CPoint oPoint;//����һ������ȷ�����λ�õ�λ��	
	GetCursorPos( &oPoint);//��ȡ��ǰ����λ�ã��Ա�ʹ�ò˵����Ը�����
	pSubMenu->TrackPopupMenu (TPM_LEFTALIGN, oPoint.x, oPoint.y, this); //��ָ��λ����ʾ�����˵�

	CDialog::OnRButtonDown(nFlags, point);
}

void CPLAYWND::OnMExit() 
{
	// TODO: Add your command handler code here
	if (MessageBox("�Ƿ��˳�����!","��ʾ",MB_OKCANCEL) == IDOK)
	{
		//return TRUE;
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
		//	GetParent()->SendMessage(WM_CLOSE);
	}
}

HBRUSH CPLAYWND::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( pWnd->GetDlgCtrlID()==IDC_WNDMIN )
	{
// 		pDC->SetBkMode(TRANSPARENT);  
// 		pDC->SetTextColor(RGB(20,20,20));
		 
		return   hbr1; 
 	}
	if (pWnd->GetDlgCtrlID()==IDC_STATIC_MSG )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		return hbr2;
	}
	if (nCtlColor==CTLCOLOR_DLG)
	{
		return hbr2;
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPLAYWND::OnMrealplay() 
{
	// TODO: Add your command handler code here
	if (m_bRealPlay)
	{
		//���������,�ȹر�����
		if (m_bPlaySound)
		{
			OnMsoundshare();
		}
		if (m_bVoiceCom)//����򿪶Խ�,�ȹرնԽ�
		{
			OnMvoicecom();
		}
		m_bRealPlay=false;
		if (m_lPlayHandle[1]!=-1)
		{
			NET_DVR_StopRealPlay(m_lPlayHandle[1]);
			m_lPlayHandle[1]=-1;
		}
		if (m_lPlayHandle[2]!=-1)
		{
			NET_DVR_StopRealPlay(m_lPlayHandle[2]);
			m_lPlayHandle[2]=-1;
		}
		Invalidate();

	}
	else
	{
		m_bRealPlay=true;
		if(m_lUserID[0]<0 )
		{
			if (DvrInitLogin(0)==false)
			{
				m_MSG.Format("����%d ��¼ʧ��������!",iKch);
			}
			else
			{
				DvrRealPlayNoShow();
			}
		}
		if(m_lUserID[1]<0 )
		{
			if (DvrInitLogin(1)==false)
			{
				m_MSG.Format("����%d��¼ʧ��!",iKch);
			}
		}
		if(m_lUserID[2]<0)
		{
			if(DvrInitLogin(2)==false)
			{
				m_MSG.Format("����%d��¼ʧ��!",iKch);
			}
		}
		if (m_lPlayHandle[1]==-1)
		{
			if(DvrRealPlay(1))
			{
				m_bSwitch=true;
			}
		}
	}
}

void CPLAYWND::OnMsoundshare() 
{
	// TODO: Add your command handler code here
	if (m_bPlaySound)
	{
		if (m_bSwitch)
		{
			if(!NET_DVR_CloseSoundShare(m_lPlayHandle[1]))
			{
				WriteLog("�ر�����ʧ�ܿ���%d����1! Errid=%d",iKch,NET_DVR_GetLastError());
			}
			else
			{
				m_bPlaySound=false;
			}
		}
		else
		{
			if(!NET_DVR_CloseSoundShare(m_lPlayHandle[2]))
			{
				WriteLog("�ر�����ʧ�ܿ���%d����2! Errid=%d",iKch,NET_DVR_GetLastError());
			}
			else
			{
				m_bPlaySound=false;
			}
		}		
	}
	else
	{		
		if (m_bSwitch)
		{
			if(!NET_DVR_OpenSoundShare(m_lPlayHandle[1]))
			{
				WriteLog("������ʧ�ܿ���%d����1! Errid=%d",iKch,NET_DVR_GetLastError());
			}
			else
			{
				m_bPlaySound=true;
			}
		}
		else
		{
			if(!NET_DVR_OpenSoundShare(m_lPlayHandle[2]))
			{
				WriteLog("������ʧ�ܿ���%d����2! Errid=%d",iKch,NET_DVR_GetLastError());
			}
			else
			{
				m_bPlaySound=true;
			}
		}
	}
}

void CPLAYWND::OnMswitch() 
{
	// TODO: Add your command handler code here
	//���������,�ȹر�����
	if (m_bPlaySound)
	{
		OnMsoundshare();
	}
	if (m_bVoiceCom)//����򿪶Խ�,�ȹرնԽ�
	{
		OnMvoicecom();
	}
	if (m_bSwitch)//true
	{		
		if(!NET_DVR_StopRealPlay(m_lPlayHandle[1]))
		{
			WriteLog("ֹͣԤ������1ʧ��!%d,������%d",NET_DVR_GetLastError(),iKch);
		}
		else
		{
			m_lPlayHandle[1]=-1;
			if (DvrRealPlay(2)==true)
			{
				m_bSwitch=false;
			}			
		}
	}
	else
	{		
		if(!NET_DVR_StopRealPlay(m_lPlayHandle[2]))
		{
			WriteLog("ֹͣԤ������2ʧ��!%d,������%d",NET_DVR_GetLastError(),iKch);
		}
		else
		{
			m_lPlayHandle[2]=-1;
			if (DvrRealPlay(1)==true)
			{
				m_bSwitch=true;
			}
		}
	}
}

void CPLAYWND::OnMvoicecom() 
{
	// TODO: Add your command handler code here
	int itemp=0;
	if (m_bSwitch)
	{
		itemp=1;
	}
	else
	{
		itemp=2;
	}
	itemp=0;//�Խ���0
	if (m_bVoiceCom)
	{
		if (!NET_DVR_StopVoiceCom(m_lVoiceHandle[itemp]))
		{
			WriteLog("ֹͣ�Խ�ʧ��!����%d����%d,Errid=%d",iKch,itemp,NET_DVR_GetLastError());
		}
		m_bVoiceCom=false;		
	}
	else
	{	
		m_lVoiceHandle[itemp]= NET_DVR_StartVoiceCom_V30(m_lUserID[itemp],m_byStartDTalkChan[itemp]+m_lChannel[itemp],1,NULL,NULL);
		if (-1 ==m_lVoiceHandle[itemp])
		{
			WriteLog("�򿪶Խ�ʧ��!����%d����%d,Errid=%d",iKch,itemp,NET_DVR_GetLastError());
		}
		else
		{
			m_bVoiceCom=true;
		}			
	}
}

void CPLAYWND::OnMstartall() 
{
	// TODO: Add your command handler code here
	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_STARTALLVIDEO_MSG,0,0);
}

void CPLAYWND::StartRealPlay()
{
	if (!m_bRealPlay)
	{
		OnMrealplay();
	}
}

bool CPLAYWND::DvrRealPlayNoShow()
{
	NET_DVR_CLIENTINFO ClientInfo;
	ClientInfo.hPlayWnd     = NULL;
	ClientInfo.lChannel     = m_lChannel[0];
	ClientInfo.lLinkMode    = 0;//0x80000000;//������ TCP
    ClientInfo.sMultiCastIP = NULL;
	
	LONG lPlayHandle;
	lPlayHandle= NET_DVR_RealPlay_V30(m_lUserID[0],&ClientInfo,NULL,NULL,TRUE);
	if(-1 == lPlayHandle)
	{
		WriteLog("DvrRealPlayNoShowʧ��!Error=%d,����0",NET_DVR_GetLastError());
		return false;
	}
	m_lPlayHandle[0]=lPlayHandle;
	return true;
}
