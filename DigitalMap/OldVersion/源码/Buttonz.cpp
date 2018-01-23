// Buttonz.cpp : implementation file
//

#include "stdafx.h"
//#include "	\ add additional includes here"
#include "Buttonz.h"
#include "CBKDigitalMap.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKDigitalMapApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CButtonz

CButtonz::CButtonz()
{
	m_bHover =FALSE;
	m_bTracking = FALSE;

	m_pToolTip = NULL;
	ExamCarState = 0;
	isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
	bTrack = FALSE;//Ĭ��û�и���
	CurCar_Xpoint = 0; //��ǰ�Ŀ������꾭γ��
	CurCar_Ypoint = 0; //��ǰ�Ŀ������꾭γ��
	CurCar_ZT=1;
	CurCar_EID=1000;

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
	ON_MESSAGE(WM_MOUSEMOVE, OnMouseMove)
	ON_WM_ERASEBKGND()
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

		CRect    rectBtn;  //��ť�ķ�Χ
		CPoint    ppoint;
		
		GetWindowRect( rectBtn );
		Cur_xPoint = rectBtn.left;
		Cur_yPoint = rectBtn.top;
	   GetCursorPos( &ppoint );

		TRACKMOUSEEVENT tme;
		tme.cbSize =sizeof(tme);
		tme.hwndTrack =m_hWnd;
		tme.dwFlags =TME_LEAVE | TME_HOVER;
		tme.dwHoverTime =1;
		m_bTracking =_TrackMouseEvent(&tme);


	}
	
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CButtonz::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking =FALSE;
	m_bHover =FALSE;
	Sleep(50);
	Invalidate(FALSE);
	return 0;
}

/**********************************************************
bErase����ָ�����ڴ�����������ʱ���Ƿ�Ҫ�������������ڵı�����
���bEraseΪTRUE���򵱵���BeginPaint��ʱ�򣬽�����������
���bEraseΪFALSE���򱳾����ֲ��䡣������ڸ���������κβ���bEraseΪTRUE��
����������ı������ᱻ�������������Ǹ����Ĳ��֡� 
**********************************************************/
LRESULT CButtonz::OnMouseHover(WPARAM wparam, LPARAM lparam)
{
	m_bHover =TRUE;
	Sleep(50);
	Invalidate(FALSE);
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CButtonz::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	// TODO: Add your code to draw the specified item
	//��ȡ������DRAWITEMSTRUCT�ṹ�����ڻ��ư�ťʱ����ʹ�õ��豸������
	CDC* mydc=CDC::FromHandle(lpDrawItemStruct->hDC);
	//�������ݵ��豸������
	CDC* pMenDC=new CDC;
	pMenDC->CreateCompatibleDC(mydc);
	CDC m_dcBuf;
	m_dcBuf.CreateCompatibleDC(mydc);
	//����ɶ���
	CBitmap * pOldBitmap;
	pOldBitmap =pMenDC ->SelectObject(&mybitmap);


	//	CPoint point(0,0);
	//�жϰ�ť�Ƿ���ѡ��״̬������������ѡ��״̬��λͼ��Ҳ����ѡ�еڶ���
	if (lpDrawItemStruct->itemState & ODS_SELECTED) //ODS_SELECTED
	{
		//mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCCOPY);
		// ��Դ�豸�����Ŀ���λͼ�������ǰ�豸������
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,m_ButtonSize.cy,SRCAND);
		mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCPAINT);
		
	}
	else
	{	
		//�ж�����Ƿ��뿪�����ڰ�ť���棬�Ա������Ӧ��λͼ
		if (m_bHover)
		{
			//mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,0,SRCCOPY);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,m_ButtonSize.cy,SRCAND);
			mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx*2,0,SRCPAINT);
			::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSGZ,examCarNumber,ThreadNumber);
			
		}else
		{
			//mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,0,SRCCOPY);
			if (bTrack)
			{
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,m_ButtonSize.cy,SRCAND);
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,m_ButtonSize.cx,0,SRCPAINT);
			}else
			{
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,m_ButtonSize.cy,SRCAND);
				mydc->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMenDC,0,0,SRCPAINT);
			}

			::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSGZLeft,ThreadNumber,0);
		}
	}
//	pMenDC->SetTextColor(RGB(0,255,0));
	CRect RC;
	GetClientRect(&RC);
	CString CarNumber;
	CarNumber.Format(" %d ",examCarNumber); 

	CFont pFont;
	pFont.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"����");                 // lpszFacename

//	pFont.CreatePointFont(100, "Arial", NULL);  
	CFont* def_font = pMenDC->SelectObject(&pFont);
	pMenDC->DrawText(CarNumber,RC,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
	pMenDC->SelectObject(def_font);
	pFont.DeleteObject();
	pMenDC ->SelectObject(pOldBitmap);
	

	delete pMenDC;
	this->ReleaseDC(mydc);
	this->ReleaseDC(&m_dcBuf);
	if (pCCBKDigitalMapDlg->iCountFlushExamCar > 0)
	{
		pCCBKDigitalMapDlg->iCountFlushExamCar--;//����ͳ�ƿ���ˢ�µĴ��� ��Ϊ0��ʱ��ſ���ˢ�µ�ͼ
	}
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
	m_ButtonSize.cy =bitmapbits.bmHeight/2;
	m_ButtonSize.cx =bitmapbits.bmWidth/3;

// 	CRgn rgn;
//     rgn. CreateEllipticRgn(0,0,m_ButtonSize.cx,m_ButtonSize.cy);
//     SetWindowRgn(rgn,TRUE);

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


BOOL CButtonz::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

/**********************************8
����˵����д������Ϣ���������ļ�
************************************/
void CButtonz::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

