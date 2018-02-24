// Buttonz.cpp : implementation file
//

#include "stdafx.h"
#include "CMFC5286D.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonz

CButtonz::CButtonz()
{
	m_bHover =FALSE;
	m_bTracking = FALSE;

	m_pToolTip = NULL;
}

CButtonz::~CButtonz()
{
	if(m_pToolTip)
	{	
		delete m_pToolTip;
	}
}


BEGIN_MESSAGE_MAP(CButtonz, CButton)
	//{{AFX_MSG_MAP(CButtonz)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonz message handlers

void CButtonz::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}
//////////////////////////////////////////////////////////////////////////
void CButtonz::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE | TME_HOVER;
		tme.dwHoverTime =50;
		m_bTracking =_TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CButtonz::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking =FALSE;
	m_bHover =FALSE;
	Invalidate(TRUE);
	return 0;
}
LRESULT CButtonz::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover =TRUE;
	Invalidate(TRUE);
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CButtonz::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	//��ȡ������DRAWITEMSTRUCT�ṹ�����ڻ��ư�ťʱ����ʹ�õ��豸������
	CDC* mydc=CDC::FromHandle(lpDrawItemStruct->hDC);
	//�������ݵ��豸������
	CDC* pMenDC=new CDC;
	pMenDC->CreateCompatibleDC(mydc);
	
	//����ɶ���
	CBitmap * pOldBitmap;
	pOldBitmap =pMenDC ->SelectObject(&mybitmap);
	
//	CPoint point(0,0);
	//�жϰ�ť�Ƿ���ѡ��״̬������������ѡ��״̬��λͼ��Ҳ����ѡ�еڶ���
	if (lpDrawItemStruct->itemState & ODS_SELECTED) //ODS_SELECTED
	{
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCCOPY);
		
	}
	else
	{	//�ж�����Ƿ��뿪�����ڰ�ť���棬�Ա������Ӧ��λͼ
		if (m_bHover)
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,0,SRCCOPY);
			
		}else
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,0,SRCCOPY);
		}
	}
	pMenDC ->SelectObject(pOldBitmap);
	delete pMenDC;
}
//////////////////////////////////////////////////////////////////////////
BOOL CButtonz::LoadBitmap(UINT bitmapid)
{
	//����ͼƬ
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid),\
		IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP bitmapbits;
	//��ȡλͼ��Ϣ������Bitmapbits�ṹ��
	mybitmap.GetBitmap(&bitmapbits);
	
	//ȡλͼ��Ӧ�ĸ߶Ⱥ� 1/3���
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/3;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

BOOL CButtonz::LoadBitmap(LPCTSTR lpszName)
{
	//����ͼƬ
	mybitmap.m_hObject=(::LoadImage(::AfxGetInstanceHandle(),lpszName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE));
	if (mybitmap.m_hObject==NULL)
	{
		//TRACE("����ͼƬʧ��\n");
		CString temp;
		temp.Format("����[%s]ͼƬʧ��",lpszName);
		AfxMessageBox(temp);
		return FALSE;
	}
	BITMAP bitmapbits;
	//��ȡλͼ��Ϣ������Bitmapbits�ṹ��
	mybitmap.GetBitmap(&bitmapbits);
	
	//ȡλͼ��Ӧ�ĸ߶Ⱥ� 1/3���
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/3;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

BOOL CButtonz::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_pToolTip != NULL)
	{
		if ((::IsWindow(m_pToolTip->m_hWnd)) && pMsg->message ==WM_MOUSEMOVE )
		{
			m_pToolTip->RelayEvent(pMsg);		
		}
	}
	return CButton::PreTranslateMessage(pMsg);
}

void CButtonz::SetToolTipText(CString spText, BOOL bActivate)
{
	if (spText.IsEmpty()) return;
	
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl;
		m_pToolTip->Create(this);
		//m_pToolTip->Activate(TRUE);
	}
	m_tooltext = spText;
// 	if (m_pToolTip->GetToolCount() == 0)
// 	{
// 		CRect rectBtn; 
// 		GetClientRect(rectBtn);
// 		m_pToolTip->AddTool(this, m_tooltext, rectBtn, 1);
// 	}
// 	m_pToolTip->UpdateTipText(m_tooltext, this, 1);
	m_pToolTip->AddTool(this, m_tooltext);
	m_pToolTip->SetDelayTime(1000);
	m_pToolTip->Activate(bActivate);
}

void CButtonz::SetToolTipText(UINT nId, BOOL bActivate)
{
	m_tooltext.LoadString(nId);
	// If string resource is not empty
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}
}
