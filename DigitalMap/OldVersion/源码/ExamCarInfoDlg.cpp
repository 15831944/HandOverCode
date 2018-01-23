// ExamCarInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cbkdigitalmap.h"
#include "ExamCarInfoDlg.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CCBKDigitalMapApp theApp;
CPoint temPoint;  //��ǰ�Ի����ƶ�����λ��
UINT StretchDlg_thd(LPVOID p); //���������Ի�����߳�
/////////////////////////////////////////////////////////////////////////////
// CExamCarInfoDlg dialog


CExamCarInfoDlg::CExamCarInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExamCarInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExamCarInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ShowDlgState = 2;// Ĭ��ֻ��ʾ�ױ�
	MouseIsThisRect = 0;//Ĭ�ϲ����������
	StartDlgPoint.x = 1000;
	StartDlgPoint.y = -637;//��Ϊ����Ի���ĸ߶�Ϊ280 ����ֻ¶�����µ�3���߶�
	EndDlgPoint.x = 1000;
	EndDlgPoint.y = 0;//��Ϊ�����Ļ��պð������Ի�����ʾ����
}


void CExamCarInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExamCarInfoDlg)
	DDX_Control(pDX, IDC_TAB_OPTION, m_ctrlTab);
	DDX_Control(pDX, IDC_STATIC_TIME, m_Time);
	DDX_Control(pDX, IDC_STATIC_DATE, m_Date);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExamCarInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CExamCarInfoDlg)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OPTION, OnSelchangeTabOption)
	ON_BN_CLICKED(IDC_BUTTONMAIN, OnButtonmain)
	ON_BN_CLICKED(IDC_BUTTONCOPILOT, OnButtoncopilot)
	ON_BN_CLICKED(IDC_BUTTONOUTSIDE, OnButtonoutside)
	ON_BN_CLICKED(IDC_BUTTONALL, OnButtonall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExamCarInfoDlg message handlers

BOOL CExamCarInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	::SetWindowPos(this->m_hWnd,HWND_BOTTOM,1000,0,280,640,SWP_NOZORDER);

	// ���߳� �������������������Ϣ
	AfxBeginThread(&StretchDlg_thd, this);
	m_Date.MoveWindow(80,40,160,20);
	m_Time.MoveWindow(80,60,160,20);
	m_ctrlTab.MoveWindow(2,128,276,415);
	m_ctrlTab.InsertItem(0,"����");
	m_ctrlTab.InsertItem(1,"����");
	//��������ҳ��ҳ 
//	DetailsDlg.Create(IDD_DIALOG_DETAILS,GetDlgItem(IDC_TAB_OPTION));  
//	ExamCarDlg.Create(IDD_DIALOG_EXAM,GetDlgItem(IDC_TAB_OPTION));
	ExamCarDlg.Create(IDD_DIALOG_EXAM,&m_ctrlTab);
	DetailsDlg.Create(IDD_DIALOG_DETAILS,&m_ctrlTab);  

	//����ҳ���λ����m_tab�ؼ���Χ�� 
	CRect rect; 
//	GetWindowRect(&rect);
	m_ctrlTab.GetClientRect(&rect); 
	ExamCarDlg.MoveWindow(rect.left + 2,rect.top + 25,rect.Width() -4,rect.Height()-25);
	DetailsDlg.MoveWindow(rect.left + 2,rect.top + 25,rect.Width()-4,rect.Height()-25);
	ExamCarDlg.ShowWindow(SW_SHOW);
	m_ctrlTab.SetCurSel(0);

	GetDlgItem(IDC_BUTTONALL)->MoveWindow(8,600,60,25);
	GetDlgItem(IDC_BUTTONMAIN)->MoveWindow(76,600,60,25);
	GetDlgItem(IDC_BUTTONCOPILOT)->MoveWindow(144,600,60,25);
	GetDlgItem(IDC_BUTTONOUTSIDE)->MoveWindow(212,600,60,25);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExamCarInfoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CExamCarInfoDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CPaintDC dc(this); 
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BITMAP_ONLINECAR); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, 
		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
	
	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}

//�����Ի�����߳�
UINT StretchDlg_thd(LPVOID p)
{
	Sleep(5000);
	
	CExamCarInfoDlg *pExamCarInfoDlg = (CExamCarInfoDlg *)p;
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	temPoint = pExamCarInfoDlg->StartDlgPoint;

	while (pCCBKDigitalMapDlg->iContrl) //�����߳�һֱ������
	{
		switch(pExamCarInfoDlg->ShowDlgState)
		{
		case 0:  //���Ի������������
			{
				if (temPoint.y < pExamCarInfoDlg->EndDlgPoint.y) //˵����û��ȫ���쳤����
				{
					temPoint.y = temPoint.y + 80;
					pExamCarInfoDlg->MoveWindow(700,temPoint.y,280,640);
					pCCBKDigitalMapDlg->iStateDlgShow = 1;
					
				//	pCCBKDigitalMapDlg->Invalidate(FALSE);

/*
					if(1)//˵��������Ƶ���ڲ���
					{
						CRect pRect(0,0,pCCBKDigitalMapDlg->CurrentScrWidth-320,pCCBKDigitalMapDlg->CurrentScrHeight-240);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(320,0,1000,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1000,temPoint.y,1280,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1280,0,pCCBKDigitalMapDlg->CurrentScrWidth,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
					}
					else //˵��û����Ƶ������
					{
						CRect pRect(0,0,1000,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1000,temPoint.y,1280,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1280,0,pCCBKDigitalMapDlg->CurrentScrWidth,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
					}	
*/

				}
				else
				{
					//˵���Ѿ�ȫ���쳤������
					pExamCarInfoDlg->ShowDlgState = 2;
					pCCBKDigitalMapDlg->iStateDlgShow = 0;
				}
				Sleep(10);
			}
			break;
		case 1:// ���Ի��������ջ�ȥ
			{
				if (temPoint.y > pExamCarInfoDlg->StartDlgPoint.y)//˵����û��ȫ���ջ�ȥ
				{
					temPoint.y = temPoint.y - 320;
					pExamCarInfoDlg->MoveWindow(700,temPoint.y,280,640);
					pCCBKDigitalMapDlg->iStateDlgShow = 1;
				//	pCCBKDigitalMapDlg->Invalidate(FALSE);
/*
					if(1)//˵��������Ƶ���ڲ���
					{
						CRect pRect(0,0,pCCBKDigitalMapDlg->CurrentScrWidth-320,pCCBKDigitalMapDlg->CurrentScrHeight-240);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(320,0,1000,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1000,temPoint.y,1280,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1280,0,pCCBKDigitalMapDlg->CurrentScrWidth,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
					}
					else //˵��û����Ƶ������
					{
						CRect pRect(0,0,1000,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1000,temPoint.y,1280,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
						pRect.SetRect(1280,0,pCCBKDigitalMapDlg->CurrentScrWidth,pCCBKDigitalMapDlg->CurrentScrHeight);
						pCCBKDigitalMapDlg->InvalidateRect(&pRect,FALSE);
					}
					*/

					/*
					if (temPoint.y <= pExamCarInfoDlg->StartDlgPoint.y)//˵����û��ȫ���ջ�ȥ
					{
						Sleep(10);
					}*/
				}
				else
				{
					//˵���Ѿ�ȫ���쳤������
					pExamCarInfoDlg->ShowDlgState = 2;
					pCCBKDigitalMapDlg->iStateDlgShow = 0;
				}
			//	Sleep(5);
			}
			break;
		case 2: //���ֶԻ��򲻱�
			{
				//����������������ں�ʱ��
				CTime t = CTime::GetCurrentTime();
				CString pDate,pTime;
				pDate.Format("����:%04d-%02d-%02d",t.GetYear(),t.GetMonth(),t.GetDay());
				pTime.Format("ʱ��:%02d:%02d:%02d",t.GetHour(),t.GetMinute(),t.GetSecond());
				
				pExamCarInfoDlg->m_Date.SetWindowText(pDate);
				pExamCarInfoDlg->m_Time.SetWindowText(pTime);
				//CRect pCrect(120,30,220,120);
				CRect pCrect(80,60,240,80);
				pExamCarInfoDlg->InvalidateRect(&pCrect);
			}
			Sleep(500);
			break;
		}
	}
	pCCBKDigitalMapDlg->WriteLog("�Ի��� StretchDlg_thd");
	return 0;
}

HBRUSH CExamCarInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	if   (nCtlColor   ==   CTLCOLOR_STATIC )        //���ÿؼ�Ϊ͸��
	{     
		pDC->SetBkMode(TRANSPARENT);     
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	return hbr;
}

void CExamCarInfoDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BITMAP_ONLINECAR); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, 
		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
	

	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

//DEL BOOL CExamCarInfoDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
//DEL {
//DEL 	// TODO: Add your specialized code here and/or call the base class
//DEL 	
//DEL 	return CDialog::OnNotify(wParam, lParam, pResult);
//DEL }

void CExamCarInfoDlg::OnSelchangeTabOption(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here  
	
	int CurSel; 
	CurSel=m_ctrlTab.GetCurSel(); 
	switch(CurSel) 
	{ 
	case 0: 
		ExamCarDlg.ShowWindow(TRUE); 
		DetailsDlg.ShowWindow(FALSE); 
		break; 
	case 1: 
		ExamCarDlg.ShowWindow(FALSE); 
		DetailsDlg.ShowWindow(TRUE); 
		break; 
	default: ; 
	} 

	*pResult = 0;
}

void CExamCarInfoDlg::OnButtonmain() 
{
	// TODO: Add your control notification handler code here
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);

	CString pContent;
	BOOL bRET = FALSE;
	GetDlgItem(IDC_BUTTONMAIN)->GetWindowText(pContent);
	if(pContent == "������")
	{
		if(pCCBKDigitalMapDlg->iISCreate == 0)
		{
			bRET = pCCBKDigitalMapDlg->OnButtonopen();
			if(bRET == FALSE)
				return;
		}
		else
		{
			// ����һ����Ϣ ��ʾ����Ի���
			/////////����һ����Ϣ�ò�����Ƶ�������////////
			HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWnd();
			if(hWnd != NULL)
			{    
		//		return;
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				pWnd->SendMessage(WM_TEXTMESSAGE, 0, 2);  //��������ڹر��ӽ��̳���  ����� 0 ����ر� 1��������  2 ������ʾ  3 ֻ�ǹر���Ƶ
				/////////////////
			}

		}
		GetDlgItem(IDC_BUTTONMAIN)->SetWindowText("������");	
	}
	else if(pContent == "������")
	{
		//����һ����Ϣ����������Ի���
		/////////����һ����Ϣ�ò�����Ƶ�������////////
		HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWnd();
		if(hWnd != NULL)
		{    
		//		return;
			CWnd *pWnd = CWnd::FromHandle(hWnd);
			pWnd->SendMessage(WM_TEXTMESSAGE, 0, 1);  //��������ڹر��ӽ��̳���
			/////////////////
			GetDlgItem(IDC_BUTTONMAIN)->SetWindowText("������");	
		}

	}

}

void CExamCarInfoDlg::OnButtoncopilot() 
{
	// TODO: Add your control notification handler code here
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);

	CString pContent;
	BOOL bRET = FALSE;
	GetDlgItem(IDC_BUTTONCOPILOT)->GetWindowText(pContent);
	if(pContent == "������")
	{
		if(pCCBKDigitalMapDlg->iISCreateT == 0)
		{
			bRET = pCCBKDigitalMapDlg->OnButtonopenT();
			if(bRET == FALSE)
				return;
		}
		else
		{
			// ����һ����Ϣ ��ʾ����Ի���
			/////////����һ����Ϣ�ò�����Ƶ�������////////
			HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndT();
			if(hWnd != NULL)
			{    
		//		return;
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				pWnd->SendMessage(WM_TEXTMESSAGET, 0, 2);  //��������ڹر��ӽ��̳���  ����� 0 ����ر� 1��������  2 ������ʾ  3 ֻ�ǹر���Ƶ
				/////////////////
			}

		}
		GetDlgItem(IDC_BUTTONCOPILOT)->SetWindowText("�ظ���");	
	}
	else if(pContent == "�ظ���")
	{
		//����һ����Ϣ����������Ի���
		/////////����һ����Ϣ�ò�����Ƶ�������////////
		HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndT();
		if(hWnd != NULL)
		{    
		//		return;
			CWnd *pWnd = CWnd::FromHandle(hWnd);
			pWnd->SendMessage(WM_TEXTMESSAGET, 0, 1);  //��������ڹر��ӽ��̳���
			/////////////////
			GetDlgItem(IDC_BUTTONCOPILOT)->SetWindowText("������");	
		}

	}

	
}

void CExamCarInfoDlg::OnButtonoutside() 
{
	// TODO: Add your control notification handler code here
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);

	CString pContent;
	BOOL bRET = FALSE;
	GetDlgItem(IDC_BUTTONOUTSIDE)->GetWindowText(pContent);
	if(pContent == "������")
	{
		if(pCCBKDigitalMapDlg->iISCreateS == 0)
		{
			bRET = pCCBKDigitalMapDlg->OnButtonopenS();
			if(bRET == FALSE)
				return;
		}
		else
		{
			// ����һ����Ϣ ��ʾ����Ի���
			/////////����һ����Ϣ�ò�����Ƶ�������////////
			HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndS();
			if(hWnd != NULL)
			{    
		//		return;
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				pWnd->SendMessage(WM_TEXTMESSAGES, 0, 2);  //��������ڹر��ӽ��̳���  ����� 0 ����ر� 1��������  2 ������ʾ  3 ֻ�ǹر���Ƶ
				/////////////////
			}

		}
		GetDlgItem(IDC_BUTTONOUTSIDE)->SetWindowText("�س���");	
	}
	else if(pContent == "�س���")
	{
		//����һ����Ϣ����������Ի���
		/////////����һ����Ϣ�ò�����Ƶ�������////////
		HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndS();
		if(hWnd != NULL)
		{    
		//		return;
			CWnd *pWnd = CWnd::FromHandle(hWnd);
			pWnd->SendMessage(WM_TEXTMESSAGET, 0, 1);  //��������ڹر��ӽ��̳���
			/////////////////
			GetDlgItem(IDC_BUTTONOUTSIDE)->SetWindowText("������");	
		}

	}
}

void CExamCarInfoDlg::OnButtonall() 
{
	// TODO: Add your control notification handler code here

	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	CString pContent;
	BOOL bRET = FALSE;

	GetDlgItem(IDC_BUTTONALL)->GetWindowText(pContent);
	if (pContent == "��ȫ��")
	{



		GetDlgItem(IDC_BUTTONMAIN)->GetWindowText(pContent);
		if(pContent == "������")
		{
			if(pCCBKDigitalMapDlg->iISCreate == 0)
			{
				bRET = pCCBKDigitalMapDlg->OnButtonopen();
				if(bRET == FALSE)
					return;
			}
			else
			{
				// ����һ����Ϣ ��ʾ����Ի���
				/////////����һ����Ϣ�ò�����Ƶ�������////////
				HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWnd();
				if(hWnd != NULL)
				{    
					//		return;
					CWnd *pWnd = CWnd::FromHandle(hWnd);
					pWnd->SendMessage(WM_TEXTMESSAGE, 0, 2);  //��������ڹر��ӽ��̳���  ����� 0 ����ر� 1��������  2 ������ʾ  3 ֻ�ǹر���Ƶ
					/////////////////
				}
				
			}
			GetDlgItem(IDC_BUTTONMAIN)->SetWindowText("������");
		}


		GetDlgItem(IDC_BUTTONCOPILOT)->GetWindowText(pContent);
		if(pContent == "������")
		{
			if(pCCBKDigitalMapDlg->iISCreateT == 0)
			{
				bRET = pCCBKDigitalMapDlg->OnButtonopenT();
				if(bRET == FALSE)
					return;
			}
			else
			{
				// ����һ����Ϣ ��ʾ����Ի���
				/////////����һ����Ϣ�ò�����Ƶ�������////////
				HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndT();
				if(hWnd != NULL)
				{    
					//		return;
					CWnd *pWnd = CWnd::FromHandle(hWnd);
					pWnd->SendMessage(WM_TEXTMESSAGET, 0, 2);  //��������ڹر��ӽ��̳���  ����� 0 ����ر� 1��������  2 ������ʾ  3 ֻ�ǹر���Ƶ
					/////////////////
				}
				
			}
			GetDlgItem(IDC_BUTTONCOPILOT)->SetWindowText("�ظ���");	
		}

		GetDlgItem(IDC_BUTTONOUTSIDE)->GetWindowText(pContent);
		if(pContent == "������")
		{
			if(pCCBKDigitalMapDlg->iISCreateS == 0)
			{
				bRET = pCCBKDigitalMapDlg->OnButtonopenS();
				if(bRET == FALSE)
					return;
			}
			else
			{
				// ����һ����Ϣ ��ʾ����Ի���
				/////////����һ����Ϣ�ò�����Ƶ�������////////
				HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndS();
				if(hWnd != NULL)
				{    
					//		return;
					CWnd *pWnd = CWnd::FromHandle(hWnd);
					pWnd->SendMessage(WM_TEXTMESSAGES, 0, 2);  //��������ڹر��ӽ��̳���  ����� 0 ����ر� 1��������  2 ������ʾ  3 ֻ�ǹر���Ƶ
					/////////////////
				}
				
			}
			GetDlgItem(IDC_BUTTONOUTSIDE)->SetWindowText("�س���");	
		}
		

		GetDlgItem(IDC_BUTTONALL)->SetWindowText("��ȫ��");
	}
	else if (pContent == "��ȫ��")
	{
		GetDlgItem(IDC_BUTTONMAIN)->GetWindowText(pContent);
		if(pContent == "������")
		{
			//����һ����Ϣ����������Ի���
			/////////����һ����Ϣ�ò�����Ƶ�������////////
			HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWnd();
			if(hWnd != NULL)
			{    
				//		return;
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				pWnd->SendMessage(WM_TEXTMESSAGE, 0, 1);  //��������ڹر��ӽ��̳���
				/////////////////
				GetDlgItem(IDC_BUTTONMAIN)->SetWindowText("������");	
			}
			
		}

		GetDlgItem(IDC_BUTTONCOPILOT)->GetWindowText(pContent);
		if(pContent == "�ظ���")
		{
			//����һ����Ϣ����������Ի���
			/////////����һ����Ϣ�ò�����Ƶ�������////////
			HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndT();
			if(hWnd != NULL)
			{    
				//		return;
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				pWnd->SendMessage(WM_TEXTMESSAGET, 0, 1);  //��������ڹر��ӽ��̳���
				/////////////////
				GetDlgItem(IDC_BUTTONCOPILOT)->SetWindowText("������");	
			}
			
		}


		GetDlgItem(IDC_BUTTONOUTSIDE)->GetWindowText(pContent);
		if(pContent == "�س���")
		{
			//����һ����Ϣ����������Ի���
			/////////����һ����Ϣ�ò�����Ƶ�������////////
			HWND hWnd = pCCBKDigitalMapDlg->GetServerMainWndS();
			if(hWnd != NULL)
			{    
				//		return;
				CWnd *pWnd = CWnd::FromHandle(hWnd);
				pWnd->SendMessage(WM_TEXTMESSAGET, 0, 1);  //��������ڹر��ӽ��̳���
				/////////////////
				GetDlgItem(IDC_BUTTONOUTSIDE)->SetWindowText("������");	
			}
			
		}

		GetDlgItem(IDC_BUTTONALL)->SetWindowText("��ȫ��");	
	}
}
