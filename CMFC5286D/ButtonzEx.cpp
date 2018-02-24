// ButtonzEx.cpp : implementation file
//

#include "stdafx.h"
#include "CMFC5286D.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonzEx

CButtonzEx::CButtonzEx()
{
	m_bHover =FALSE;
	m_bTracking = FALSE;
	m_bmpn = 3;
	for (int i=0; i<5 ;i++)
	{
		m_BdrawX[i] = i % m_bmpn;
	}
	m_pToolTip = NULL;
}

CButtonzEx::~CButtonzEx()
{
	if(m_pToolTip)
	{	
		delete m_pToolTip;
	}
}


BEGIN_MESSAGE_MAP(CButtonzEx, CButton)
	//{{AFX_MSG_MAP(CButtonzEx)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonzEx message handlers
void CButtonzEx::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

BOOL CButtonzEx::PreTranslateMessage(MSG* pMsg) 
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

void CButtonzEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE | TME_HOVER;
		tme.dwHoverTime =10;
		m_bTracking =_TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CButtonzEx::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking =FALSE;
	m_bHover =FALSE;
	Invalidate(TRUE);
	return 0;
}
LRESULT CButtonzEx::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover =TRUE;
	Invalidate(TRUE);
	return 0;
}

void CButtonzEx::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
	UINT nState = lpDrawItemStruct->itemState;
	
	int nImgIndex = 0;//bmpNormal
	if(m_bHover)
	{
		if(nState & ODS_SELECTED)
		{
			nImgIndex = 3;//bmpDown
		}
		else
		{
			nImgIndex = 1;//bmpHover
		}
	}
	else if(nState & ODS_FOCUS)
	{
		nImgIndex = 2;//bmpFocus
	}
	else if(nState & ODS_DISABLED)
	{
		nImgIndex = 4;//bmpDisable
	}
	mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*m_BdrawX[nImgIndex],0,SRCCOPY);
	pMenDC ->SelectObject(pOldBitmap);
	delete pMenDC;
}
//////////////////////////////////////////////////////////////////////////
void CButtonzEx::SetToolTipText(CString spText, BOOL bActivate)
{
	if (spText.IsEmpty()) return;
	
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl;
		m_pToolTip->Create(this);
	}
	m_tooltext = spText;

	m_pToolTip->AddTool(this, m_tooltext);
	m_pToolTip->SetDelayTime(1000);
	m_pToolTip->Activate(bActivate);
}

void CButtonzEx::SetToolTipText(UINT nId, BOOL bActivate)
{
	m_tooltext.LoadString(nId);
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}
}
//////////////////////////////////////////////////////////////////////////
BOOL CButtonzEx::LoadBitmap(UINT bitmapid,UINT bmpn)
{
	//����ͼƬ
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid),\
		IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP bitmapbits;
	//��ȡλͼ��Ϣ������Bitmapbits�ṹ��
	mybitmap.GetBitmap(&bitmapbits);
	m_bmpn = bmpn;
	//ȡλͼ��Ӧ�ĸ߶Ⱥ� 1/m_bmpn���
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/m_bmpn;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

BOOL CButtonzEx::LoadBitmap(LPCTSTR lpszName,UINT bmpn)
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
	m_bmpn = bmpn;
	//ȡλͼ��Ӧ�ĸ߶Ⱥ� 1/m_bmpn���
	m_ButtonSize.cy =bitmapbits.bmHeight;
	m_ButtonSize.cx =bitmapbits.bmWidth/m_bmpn;
	
	SetWindowPos(NULL,0,0,m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE | SWP_NOOWNERZORDER );
	return TRUE;
}

void CButtonzEx::SetBtnDrawNum(UINT bmpNormal, UINT bmpHover, UINT bmpFocus, UINT bmpDown, UINT bmpDisable)
{
	m_BdrawX[0] = bmpNormal % m_bmpn;
	m_BdrawX[1] = bmpHover % m_bmpn;
	m_BdrawX[2] = bmpFocus % m_bmpn;
	m_BdrawX[3] = bmpDown % m_bmpn;
	m_BdrawX[4] = bmpDisable % m_bmpn;
}
