// ZSZButton.cpp : implementation file
//

#include "stdafx.h"
//#include "ff.h"
#include "ZSZButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CZSZButton

CZSZButton::CZSZButton()
{
	m_bHover =FALSE;
	m_bTracking = FALSE;
}

CZSZButton::~CZSZButton()
{
}


BEGIN_MESSAGE_MAP(CZSZButton, CButton)
	//{{AFX_MSG_MAP(CZSZButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZSZButton message handlers

void CZSZButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE | TME_HOVER;
		tme.dwHoverTime =1;
		m_bTracking =_TrackMouseEvent(&tme);
	}
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CZSZButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking =FALSE;
	m_bHover =FALSE;
	Invalidate(TRUE);
	return 0;
}
LRESULT CZSZButton::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover =TRUE;
	Invalidate(TRUE);
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void CZSZButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CButton::PreSubclassWindow();
	ModifyStyle(0, BS_OWNERDRAW);
}

void CZSZButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
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
	
	CPoint point(0,0);
	//�жϰ�ť�Ƿ���ѡ��״̬������������ѡ��״̬��λͼ��Ҳ����ѡ�еڶ���
	if (lpDrawItemStruct->itemState & ODS_SELECTED) //ODS_SELECTED
	{
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCCOPY);
		
	}
	else
	{	//�ж�����Ƿ��뿪�����ڰ�ť���棬�Ա������Ӧ��λͼ
		if (m_bHover)
		{
			//pMenDC->TextOut(10,10,"��ã�");
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,0,SRCCOPY);
			
		}else
		{
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,0,SRCCOPY);
		}
	}
	//clean up
	pMenDC ->SelectObject(pOldBitmap);
	delete pMenDC;
	
}

BOOL CZSZButton::LoadBitmap(UINT bitmapid)
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
