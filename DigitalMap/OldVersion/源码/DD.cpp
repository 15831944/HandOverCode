// DD.cpp : implementation file
//

#include "stdafx.h"
#include "CBKDigitalMap.h"
#include "DD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CCBKDigitalMapApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CDD dialog


CDD::CDD(CWnd* pParent /*=NULL*/)
	: CDialog(CDD::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDD)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDD::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDD)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDD, CDialog)
	//{{AFX_MSG_MAP(CDD)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()  //���ؼ��ı�����Ϊ͸��
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDD message handlers

BOOL CDD::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default

	return CDialog::OnEraseBkgnd(pDC);
}

void CDD::OnButton1() 
{
	// TODO: Add your control notification handler code here
	
}

void CDD::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	CPaintDC dc(this); 
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BITMAPSUB); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	int nRight=bitmap.bmWidth;
	int nBtonn=bitmap.bmHeight;
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0, 
		bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 

////////��ֹGDIй¶
	bmpBackground.DeleteObject();  //��ֹGDIй¶
	pbmpOld->DeleteObject();

//	return TRUE;
	// Do not call CDialog::OnPaint() for painting messages
	GetDlgItem(IDC_STATIC_CARNUMBER1)->SetWindowText(CSCAR);
	GetDlgItem(IDC_STATIC_NAME1)->SetWindowText(szExamineeName);
	GetDlgItem(IDC_STATIC_ZKZH1)->SetWindowText(strzkzm);
	GetDlgItem(IDC_STATIC_EXAMMODEL1)->SetWindowText(strCarModel);
	OnShowPHO();  //��ʾ����Ƭ
	this->ReleaseDC(&dcMem);
}

void CDD::SetupRegion(CDC *pDC, CBitmap &cBitmap, COLORREF TransColor)
{
    CDC memDC;
    //�����봫��DC���ݵ���ʱDC
    memDC.CreateCompatibleDC(pDC);
    
    CBitmap *pOldMemBmp=NULL;
    //��λͼѡ����ʱDC
    pOldMemBmp=memDC.SelectObject(&cBitmap);
    
    CRgn wndRgn;
    //�����ܵĴ������򣬳�ʼregionΪ0
    wndRgn.CreateRectRgn(0,0,0,0);
    
    
    BITMAP bit;   
    cBitmap.GetBitmap (&bit);//ȡ��λͼ����������Ҫ�õ�λͼ�ĳ��Ϳ�     
    
    int y;
    for(y=0;y<bit.bmHeight  ;y++)//y=0;y<=bit.bmHeight  ;y++)
    {
        CRgn rgnTemp; //������ʱregion
        
        int iX = 0;
        do
        {
            //����͸��ɫ�ҵ���һ����͸��ɫ�ĵ�.
            while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) == TransColor)
                iX++;
            
            //��ס�����ʼ��
            int iLeftX = iX;
            
            //Ѱ���¸�͸��ɫ�ĵ�
            while (iX <= bit.bmWidth  && memDC.GetPixel(iX, y) != TransColor)
                ++iX;
            
            //����һ������������ص���Ϊ1���ص���ʱ��region��
            rgnTemp.CreateRectRgn(iLeftX, y, iX, y+1);
            
            //�ϲ�����"region".
            wndRgn.CombineRgn(&wndRgn, &rgnTemp, RGN_OR);
            
            //ɾ����ʱ"region",�����´δ���ʱ�ͳ���
            rgnTemp.DeleteObject();
        }while(iX <bit.bmWidth );
        iX = 0;
    }
    
    if(pOldMemBmp)
        memDC.SelectObject(pOldMemBmp);
    
    CWnd * pWnd = pDC->GetWindow();
    pWnd->SetWindowRgn(wndRgn,TRUE);   // ��������һ�����ڵ�����.ֻ�б���������������ڵĵط��Żᱻ�ػ�,���������������ڵ���������ϵͳ��������ʾ.  
    pWnd->SetForegroundWindow();    
	this->ReleaseDC(&memDC);
	pOldMemBmp->DeleteObject();
	
}


BOOL CDD::OnInitDialog() 
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_STATIC_CARNUMBER)->MoveWindow(10,10,80,25); //������
	GetDlgItem(IDC_STATIC_CARNUMBER1)->MoveWindow(100,10,80,25); //������
	GetDlgItem(IDC_STATIC_NAME)->MoveWindow(10,45,80,25); //��������
	GetDlgItem(IDC_STATIC_NAME1)->MoveWindow(100,45,80,25); //��������
	GetDlgItem(IDC_STATIC_ZKZH)->MoveWindow(10,80,80,25); //׼��֤��
	GetDlgItem(IDC_STATIC_ZKZH1)->MoveWindow(100,80,80,25); //׼��֤��
	GetDlgItem(IDC_STATIC_EXAMMODEL)->MoveWindow(10,115,80,25); //���Գ���
	GetDlgItem(IDC_STATIC_EXAMMODEL1)->MoveWindow(100,115,80,25); //���Գ���

	GetDlgItem(IDC_STATIC_PHO)->MoveWindow(220,10,90,110);  //�ƶ���Ƭ�ľ�̬�༭��λ��

//	GetDlgItem(IDC_STATIC_CARNUMBER)->SetWindowText("�����ţ�");
//	GetDlgItem(IDC_STATIC_NAME)->SetWindowText("����������");
//	GetDlgItem(IDC_STATIC_ZKZH)->SetWindowText("׼��֤�ţ�");
//	GetDlgItem(IDC_STATIC_EXAMMODEL)->SetWindowText("�������ͣ�");

	
	// TODO: Add extra initialization here
//	m_bmp.LoadBitmap(IDB_BITMAPSUB);
	CClientDC dc(this);  // ��CClientDC������CDC���ڹ���ʱ������Windows����GetDC��������ʱ������ReleaseDC������ζ�ź�CClientDC������ص��豸�������Ǵ��ڵĿͻ�����

//    SetupRegion(&dc,m_bmp,RGB(255,0,255));


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CDD::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)  //���ؼ��ı�����Ϊ͸��
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if   (nCtlColor   ==   CTLCOLOR_STATIC)        //���ÿؼ�Ϊ͸��
	{     
		pDC->SetBkMode(TRANSPARENT);     
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}


BOOL CDD::ReadExamInfoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	CTime pTime = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format("%d-%d-%d",pTime.GetYear(),pTime.GetMonth(),pTime.GetDay());

 //  LPVOID m_pBMPBuffer;
	CString temp;
	try
	{
	//	BOOL sInit=theApp.OnInitSQL();
	//	if (sInit)
	//	{
			CString strSQL;	
			VARIANT cnt;
			cnt.vt = VT_INT;
			strSQL.Format("SELECT * FROM StudentInfo WHERE ������ = '%s' and ״̬ = '3' and Ԥ������ = '%s'",sCard,szTime);
// 			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
// 				adOpenDynamic, adLockOptimistic, adCmdText);
			_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
			_variant_t vat;
			if(pSet != NULL && (!pSet->adoEOF))
			{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
				while(!pSet->adoEOF)
				{
					vat =pSet->GetCollect("����");
					if (vat.vt != VT_NULL)
					{
						szExamineeName = (LPCSTR)_bstr_t(vat);
					}
					vat =pSet->GetCollect("׼��֤�����");
					if (vat.vt != VT_NULL)
					{
						strzkzm = (LPCSTR)_bstr_t(vat);
					}
					vat =pSet->GetCollect("���Գ���");
					if (vat.vt != VT_NULL)
					{
						strCarModel = (LPCSTR)_bstr_t(vat);
					}
					
				//	pSet->MoveNext();
					pSet->Close();
	  				//�ͷ�
	  				pSet.Release();
					
					break;
				}
		//	}
		//	Sleep(50);


			}
			else
			{
				szExamineeName = _T("");
				strzkzm = _T("");
				strCarModel = _T("");
			}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
 		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}

BOOL CDD::ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
   
	CString temp;
	try
	{
	//	BOOL sInit=theApp.OnInitSQL();
	//	if (sInit)
	//	{
			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;
			CString strSQL;	
	//		strSQL.Format("SELECT * FROM StudentPhoto WHERE ׼��֤����� = '%s' and ״̬ = '3'",sCard);
			strSQL.Format("SELECT * FROM StudentPhoto WHERE ׼��֤����� = '%s'",sCard);
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
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
				pbuf = NULL;
			}
			else
			{
				pRecordset->Close();
				pRecordset.Release();
			}
	//	}

	}
	catch (_com_error e) 
	{
		if(m_pBMPBuffer)
		{
			delete m_pBMPBuffer;
			m_pBMPBuffer = NULL;
		}
		return FALSE;
	}
	
	return TRUE;

}

void CDD::OnShowPHO()
{
	CDC *pDC = this->GetDC();
	CDC dcMem; 
	dcMem.CreateCompatibleDC(pDC); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BMPPHOBK); 
	BITMAP bitmap; 
	bmpBackground.GetBitmap(&bitmap);
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground);
	this->ReleaseDC(pDC);
	CRect rt(0,0,bitmap.bmWidth,bitmap.bmHeight);
	ReadPhotoFromDB(&dcMem,strzkzm,&rt);
	//////////////////////////////////////////////////////////////////////////
	HDC hdc;
	hdc=GetDlgItem(IDC_STATIC_PHO)->GetDC()->m_hDC;
	CDC *pDC1= CDC::FromHandle(hdc);
	pDC1->StretchBlt(0,0,bitmap.bmWidth,bitmap.bmHeight,&dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY);

//��ֹGDIй¶
	bmpBackground.DeleteObject();  //��ֹGDIй¶
	pbmpOld->DeleteObject();
	this->ReleaseDC(pDC1);

}
