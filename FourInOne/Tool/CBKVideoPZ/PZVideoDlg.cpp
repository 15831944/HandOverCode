// PZVideoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBKVideoPZ.h"
#include "PZVideoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPZVideoDlg dialog


CPZVideoDlg::CPZVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPZVideoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPZVideoDlg)
	m_Path = _T("");
	m_Edit_BZ = _T("");
	m_EditPort = _T("8000");
	m_EditPwd = _T("12345");
	m_EditTdh = _T("");
	m_EditUser = _T("admin");
	//}}AFX_DATA_INIT
}


void CPZVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPZVideoDlg)
	DDX_Control(pDX, IDC_IPADDRESS2, m_MediaIPAddr);
	DDX_Control(pDX, IDC_COMBO_ML, m_ComboBoxML);
	DDX_Control(pDX, IDC_COMBO_SXNUM, m_ComboSXBH);
	DDX_Control(pDX, IDC_COMBO2, m_ComSkin);
	DDX_Control(pDX, IDC_BTN_EXIT, m_BTNClose);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddrCtrl);
	DDX_Control(pDX, IDC_COMBO_BH, m_ComboBoxBH);
	DDX_Control(pDX, IDC_LISTBKVideo, m_ListConVideo);
	DDX_Text(pDX, IDC_EPATH, m_Path);
	DDX_Text(pDX, IDC_EDIT_BZ, m_Edit_BZ);
	DDX_Text(pDX, IDC_EDIT_PROT, m_EditPort);
	DDX_Text(pDX, IDC_EDIT_PWD, m_EditPwd);
	DDX_Text(pDX, IDC_EDIT_TDH, m_EditTdh);
	DDX_Text(pDX, IDC_EDIT_USER, m_EditUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPZVideoDlg, CDialog)
	//{{AFX_MSG_MAP(CPZVideoDlg)
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTNCHANGE, OnBtnchange)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBO_BH, OnSelchangeComboBh)
	ON_BN_CLICKED(IDC_BTNSAVE, OnBtnsave)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTBKVideo, OnItemchangedLISTBKVideo)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangeComboSkin)
	ON_BN_CLICKED(IDC_BTNSKINSET, OnBtnskinset)
	ON_BN_CLICKED(IDC_BTN_LMTIP, OnBtnLmtip)
	ON_BN_CLICKED(IDC_BTN_TYIPPWD, OnBtnTyippwd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPZVideoDlg message handlers


BOOL CPZVideoDlg::OnInitSQLEX()
{
	//////////////////////////////////////////////////////////////////////////
//	AfxOleInit();
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
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\config.ini");
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
	try
	{
		//�������ݿ�����
		CString Sqlstr;
		Sqlstr =m_strInEdit;
		res = m_pConn->Open((_bstr_t)Sqlstr,"","",0);
		if(res==S_OK)//0
		{
			TRACE("�����ݿ�ɹ�!");
		}
		else
		{
			TRACE("�����ݿ�ʧ��!");
			return FALSE;
		}
	}
	catch(_com_error e)		//��׽�쳣
	{
		CString errormessage;
		errormessage.Format("���ݿ���ʾ,�������ݿ�ʧ��!,������Ϣ:%s,�����������ݿ�����",e.ErrorMessage());
		zbmpmessagebox(errormessage.GetBuffer(0));
		ShellExecute(NULL,"open",".\\sqlLink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}

BOOL CPZVideoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (!OnInitSQLEX())
	{
		CDialog::OnCancel();
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	m_ListConVideo.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_ListConVideo.InsertColumn(0,_T("���"),LVCFMT_LEFT,60,-1);
	m_ListConVideo.InsertColumn(1,_T("�豸IP"),LVCFMT_LEFT,100,-1);
	m_ListConVideo.InsertColumn(2,_T("�û���"),LVCFMT_LEFT,60,-1);
	m_ListConVideo.InsertColumn(3,_T("����"),LVCFMT_LEFT,-10,-1);
	m_ListConVideo.InsertColumn(4,_T("ͨ��"),LVCFMT_LEFT,40,-1);
	m_ListConVideo.InsertColumn(5,_T("�˿�"),LVCFMT_LEFT,40,-1);	
	m_ListConVideo.InsertColumn(6,_T("N"),LVCFMT_LEFT,30,-1);
	m_ListConVideo.InsertColumn(7,_T("����"),LVCFMT_LEFT,40,-1);
	m_ListConVideo.InsertColumn(8,_T("��ý��"),LVCFMT_LEFT,90,-1);
	m_ListConVideo.InsertColumn(9,_T("��ע"),LVCFMT_LEFT,140,-1);

	m_ComboSXBH.InsertString(0,"1");
	m_ComboSXBH.InsertString(1,"2");
	m_ComboSXBH.InsertString(2,"3");
	m_ComboSXBH.InsertString(3,"4");
	m_ComboSXBH.InsertString(4,"5");
	m_ComboSXBH.InsertString(5,"6");
	m_ComboSXBH.InsertString(6,"7");
	m_ComboSXBH.InsertString(7,"8");
	m_ComboSXBH.SetCurSel(0);

	m_ComboBoxML.InsertString(0,"������"); 
	m_ComboBoxML.InsertString(1,"������");
	m_ComboBoxML.SetCurSel(1);

	UpdateList();
	UpdateCombox();
	m_ComboBoxBH.SetCurSel(0);//

	GetDlgItem(IDC_BTN_EXIT)->MoveWindow(663,0,47,22);
	m_BTNClose.LoadBitmap(IDB_BMPCLOSE);

	
	hbr1=CreateSolidBrush(RGB(255,255,255)); 
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPZVideoDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x, point.y));
	CDialog::OnLButtonDown(nFlags, point);
}

void CPZVideoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	SetWindowPos(NULL,0,0,720,450,SWP_NOMOVE | SWP_NOOWNERZORDER );
	CRgn m_rgn;
	CRect rc;	
	GetClientRect(&rc);
	rc-=rc.TopLeft();
	m_rgn.CreateRoundRectRgn(rc.left,rc.top,rc.right,rc.bottom,5,5);
	SetWindowRgn(m_rgn,TRUE);
}

void CPZVideoDlg::OnBtnchange() 
{
	// TODO: Add your control notification handler code here
	CString ReturnPach;								//�ַ�������
	TCHAR szPath[_MAX_PATH];                       	//����·������
	BROWSEINFO bi;                                  	// BROWSEINFO�ṹ����
	bi.hwndOwner    = NULL;                          	//HWND���
	bi.pidlRoot       = NULL;                          	//Ĭ��ֵΪNULL
	bi.lpszTitle       = _T("�ļ�����Ի���");          	//�Ի������
	bi.pszDisplayName = szPath;                         //ѡ���ļ���·��
	bi.ulFlags        = BIF_RETURNONLYFSDIRS;       //���
	bi.lpfn           = NULL;                          	//Ĭ��ֵΪNULL
	bi.lParam         = NULL;                          	//�ص���Ϣ
	LPITEMIDLIST pItemIDList = SHBrowseForFolder(&bi);  	//��ʾ�ļ�����Ի���
	if(pItemIDList)
	{
		if(SHGetPathFromIDList(pItemIDList,szPath)) 		//�ж��Ƿ����ļ���·��
			ReturnPach = szPath;					//����ļ���·��
	}
	else
	{
		ReturnPach = "C:\\"; 							//�ļ���·��Ϊ��
	}
	if ( ReturnPach.Right(1) != "\\" )
	{
		ReturnPach+="\\";
	}
	m_Path=ReturnPach;				//��ʾ�ļ���·��
	UpdateData(FALSE);
//////////////////////////////////////////////////////////////////////////
	ULARGE_INTEGER totalsize;
	ULARGE_INTEGER freesize;
	ULARGE_INTEGER availablesize;
	CString str,temp;
	str = ReturnPach.Left(3);
	::GetDiskFreeSpaceEx(str,&totalsize,&freesize,&availablesize);
	temp.Format("%ldM",totalsize.QuadPart/1024/1024);
	GetDlgItem(IDC_STATIC_SYKJ)->SetWindowText("ʣ��ռ䣺"+temp);
	if(totalsize.QuadPart/1024/1024 < 300)
	{
		MessageBox("ʣ��ռ䲻��300M�����������");
	}
	//////////////////////////////////////////////////////////////////////////
	m_Path.Replace("\\","\\\\");
	WritePrivateProfileString("UIskin","avipath",m_Path,".\\search.cfg");
}

BOOL CPZVideoDlg::UpdateList()
{	
	int k=0;
	CString tempk;
	m_ListConVideo.DeleteAllItems();//����б�
	try
	{
			VARIANT cnt;
			cnt.vt = VT_INT;
			CString temp;
			temp.Format("SELECT * FROM TBKVideo ORDER BY ���");
			_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
			_variant_t vat;
			CString strbh,strsbip,strport,struser,strpwd,strtdh,strbz,strnid,strml,strmedia;
			if(pSet != NULL)
			{
				while(!pSet->adoEOF)
				{
					vat =pSet->GetCollect("���");
					if (vat.vt != VT_NULL)
					{
						strbh = (LPCSTR)_bstr_t(vat);
					}else{strbh=_T("");}
					vat =pSet->GetCollect("�豸IP");
					if (vat.vt != VT_NULL)
					{
						strsbip = (LPCSTR)_bstr_t(vat);
					}else{strsbip=_T("");}
					vat =pSet->GetCollect("�˿ں�");
					if (vat.vt != VT_NULL)
					{
						strport = (LPCSTR)_bstr_t(vat);
					}else{strport=_T("");}
					vat =pSet->GetCollect("�û���");
					if (vat.vt != VT_NULL)
					{
						struser = (LPCSTR)_bstr_t(vat);
					}else{struser=_T("");}
					vat =pSet->GetCollect("����");
					if (vat.vt != VT_NULL)
					{
						strpwd = (LPCSTR)_bstr_t(vat);
					}else{strpwd=_T("");}
					vat =pSet->GetCollect("ͨ����");
					if (vat.vt != VT_NULL)
					{
						strtdh = (LPCSTR)_bstr_t(vat);
					}else{strtdh=_T("");}
					vat =pSet->GetCollect("��ע");
					if (vat.vt != VT_NULL)
					{
						strbz = (LPCSTR)_bstr_t(vat);
					}else{strbz=_T("");}
					vat =pSet->GetCollect("Nid");
					if (vat.vt != VT_NULL)
					{
						strnid = (LPCSTR)_bstr_t(vat);
					}else{strnid=_T("");}
					vat =pSet->GetCollect("MediaIP");
					if (vat.vt != VT_NULL)
					{
						strmedia = (LPCSTR)_bstr_t(vat);
					}else{strmedia=_T("");}
					vat =pSet->GetCollect("TransMode");
					if (vat.vt != VT_NULL)
					{
						strml = (LPCSTR)_bstr_t(vat);
					}else{strml=_T("");}
					//tempk.Format("%d",k+1);
					m_ListConVideo.InsertItem(k,strbh);
					m_ListConVideo.SetItemText(k,1,strsbip);
					m_ListConVideo.SetItemText(k,2,struser);
					m_ListConVideo.SetItemText(k,3,strpwd);
					m_ListConVideo.SetItemText(k,4,strtdh);
					m_ListConVideo.SetItemText(k,5,strport);
					m_ListConVideo.SetItemText(k,6,strnid);
					m_ListConVideo.SetItemText(k,7,strml);
					m_ListConVideo.SetItemText(k,8,strmedia);
					m_ListConVideo.SetItemText(k,9,strbz);
					k++;
					pSet->MoveNext();
				}//while
			}//if
			else
			{				
				TRACE("��ѯ����¼Ϊ��\n");
				return FALSE;
			}
	}
	catch (_com_error &comError)
	{
		//TRACE("�����ݿ��쳣��%s!\n",comError.ErrorMessage());
 		tempk.Format("������ʾ,����ͨ����Ӧ�б�ʧ��!,����[%s]",comError.Description());
		zbmpmessagebox(tempk.GetBuffer(0));
	}
	return TRUE;
}

void CPZVideoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}

void CPZVideoDlg::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	CPZVideoDlg::OnCancel();
}

BOOL CPZVideoDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CPaintDC dc(this); 
	CRect rect; 
	GetClientRect(&rect); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 
	CBitmap bmpBackground; 
	bmpBackground.LoadBitmap(IDB_BITMAP_BACK); 
	CBitmap *pbmpOld=dcMem.SelectObject(&bmpBackground); 
	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,720,450,SRCCOPY); 
	return TRUE;
//	return CDialog::OnEraseBkgnd(pDC);
}

BOOL CPZVideoDlg::UpdateCombox()
{
	int k=0;
	CString tempk;
	//	m_ComboBoxBH.ResetContent();
	try
	{
		VARIANT cnt;
		cnt.vt = VT_INT;
		CString temp;
		temp.Format("select ������,�۷����� from ErrorData where �۳�����='0'");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		_variant_t vat;
		CString errorcode,kfls;
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("������");
				if (vat.vt != VT_NULL)
				{
					errorcode = (LPCSTR)_bstr_t(vat);
				}
				vat =pSet->GetCollect("�۷�����");
				if (vat.vt != VT_NULL)
				{
					kfls = (LPCSTR)_bstr_t(vat);
				}
				//tempk.Format("%d",k+1);
				//m_ListConVideo.InsertItem(k,strbh);
				m_ComboBoxBH.InsertString(k,errorcode);
				ComboboxArray.Add(kfls);
				k++;
				pSet->MoveNext();
			}//while
		}//if
		else
		{
			
			TRACE("��ѯ����¼Ϊ��\n");
			return FALSE;
		}
		pSet->Close();
		temp.Format("select ��Ŀ from SysCfg where ��Ŀ like '����%%'");
		pSet =m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				vat =pSet->GetCollect("��Ŀ");
				if (vat.vt != VT_NULL)
				{
					errorcode = (LPCSTR)_bstr_t(vat);
				}
				m_ComboBoxBH.InsertString(k,errorcode);
				ComboboxArray.Add(errorcode);
				k++;
				pSet->MoveNext();
			}//while
		}//if		
	}
	catch (_com_error &comError)
	{
		//TRACE("�����ݿ��쳣��%s!\n",comError.ErrorMessage());
		tempk.Format("������ʾ,����Combox�б�ʧ��!,%s",comError.Description());
		zbmpmessagebox(tempk.GetBuffer(0));
	}
	return TRUE;
}


void CPZVideoDlg::OnSelchangeComboBh() 
{
	// TODO: Add your control notification handler code here
	m_Edit_BZ.Format("%s",ComboboxArray.GetAt(m_ComboBoxBH.GetCurSel()));
	UpdateData(FALSE);
}

void CPZVideoDlg::OnBtnsave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	AddTBKVideo();
	UpdateList();
}

void CPZVideoDlg::AddTBKVideo()
{

	//////////////////////////////////////////////////////////////////////////
	_RecordsetPtr pRecordset;
	_variant_t vtemp;
	CString temp,temp1,m_editIP,m_mediaip;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		int Count=m_ComboBoxBH.GetCurSel();
		int isxbh =m_ComboSXBH.GetCurSel()+1;
		int imllx =m_ComboBoxML.GetCurSel();

		int n=m_ComboBoxBH.GetLBTextLen(Count);
		m_ComboBoxBH.GetLBText(Count,temp.GetBuffer(n));
		//////////////////////////////////////////////////////////////////////////
		BYTE nField0,nField1,nField2,nField3;
		m_IPAddrCtrl.GetAddress(nField0,nField1,nField2,nField3);
		m_editIP.Format("%u.%u.%u.%u",nField0,nField1,nField2,nField3);
		m_MediaIPAddr.GetAddress(nField0,nField1,nField2,nField3);
		m_mediaip.Format("%u.%u.%u.%u",nField0,nField1,nField2,nField3);
		if (m_mediaip=="0.0.0.0")
		{
			m_mediaip=_T("");
		}
		strSQL.Format("select * from TBKVideo where ��� = '%s' and Nid=%d ", temp,isxbh);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		if(pRecordset->RecordCount == 1)
		{
			temp1.Format("�޸���ʾ,���ݿ����Ѵ��ڱ��[%s],�Ƿ�Ҫ������Ϣ!",temp);
			if (zbmpmessagebox(temp1.GetBuffer(0)))
			{
				pRecordset->PutCollect("���",(_variant_t)temp);
				pRecordset->PutCollect("�豸IP",(_variant_t)m_editIP);				
				pRecordset->PutCollect("�û���",(_variant_t)m_EditUser);
				pRecordset->PutCollect("����",(_variant_t)m_EditPwd);
				pRecordset->PutCollect("ͨ����",(_variant_t)m_EditTdh);
				pRecordset->PutCollect("�˿ں�",(_variant_t)m_EditPort);
				pRecordset->PutCollect("��ע",(_variant_t)m_Edit_BZ);
				pRecordset->PutCollect("Nid",_variant_t((long)isxbh));
				pRecordset->PutCollect("TransMode",_variant_t((long)imllx));
				pRecordset->PutCollect("MediaIP",(_variant_t)m_mediaip);
				pRecordset->Update();
			}
		}
		else if(pRecordset->RecordCount == 0)
		{
			temp1.Format("������ʾ,�Ƿ��������[%s]��Ϣ!",temp);
			if (zbmpmessagebox(temp1.GetBuffer(0)))
			{
				pRecordset->AddNew();
				pRecordset->PutCollect("���",(_variant_t)temp);
				pRecordset->PutCollect("�豸IP",(_variant_t)m_editIP);				
				pRecordset->PutCollect("�û���",(_variant_t)m_EditUser);
				pRecordset->PutCollect("����",(_variant_t)m_EditPwd);
				pRecordset->PutCollect("ͨ����",(_variant_t)m_EditTdh);
				pRecordset->PutCollect("�˿ں�",(_variant_t)m_EditPort);
				pRecordset->PutCollect("��ע",(_variant_t)m_Edit_BZ);
				pRecordset->PutCollect("Nid",_variant_t((long)isxbh));
				pRecordset->PutCollect("TransMode",_variant_t((long)imllx));
				pRecordset->PutCollect("MediaIP",(_variant_t)m_mediaip);
				pRecordset->Update();
			}
		}
		else
		{
			zbmpmessagebox("��ʾ,�˱�������ݿ��д��ڲ�ֹ1����¼!");
		}
		
		pRecordset->Close();
		pRecordset.Release();		
	}
	catch (_com_error e) 
	{
		temp1.Format("������ʾ,���ͨ����Ϣʧ��!!,%s",e.Description());
		zbmpmessagebox(temp1.GetBuffer(0));
	}	
}

HBRUSH CPZVideoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if ( nCtlColor ==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);  
		pDC->SetTextColor(RGB(82,82,82));
		return   hbr1; 
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CPZVideoDlg::OnBtnDelete() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	CString videobh;
	int Count=m_ComboBoxBH.GetCurSel();
	int n=m_ComboBoxBH.GetLBTextLen(Count);
	m_ComboBoxBH.GetLBText(Count,videobh.GetBuffer(n));	
	int isxbh =m_ComboSXBH.GetCurSel()+1;
	temp.Format("ɾ����ʾ,�Ƿ�Ҫɾ�����[%s],����ͷ���[%d]",videobh,isxbh);
	if (zbmpmessagebox(temp.GetBuffer(0)))
	{
		try
		{
			_RecordsetPtr pRecordset;
			CString strSQL;	
			_variant_t vTemp;

			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;				
			if (isxbh==0)
			{
				strSQL.Format("select * from TBKVideo where ��� = '%s' and Nid is null ", videobh);
			}
			else
			{
				strSQL.Format("select * from TBKVideo where ��� = '%s' and Nid=%d ", videobh,isxbh);
			}
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
				adOpenDynamic, adLockOptimistic, adCmdText);
			
			if(pRecordset->RecordCount == 0)
			{
				zbmpmessagebox("ɾ����ʾ,���ݿ����޸ü�¼!");
				pRecordset->Close();
				pRecordset.Release();
				return;
			}
			else
			{
				pRecordset->Delete(adAffectCurrent);		
				pRecordset->UpdateBatch(adAffectAll);
			}		
			
			pRecordset->Close();
			pRecordset.Release();
			temp.Format("��ʾ,ɾ�����[%s]�ɹ�!,����ͷ���[%d]",videobh,isxbh);
			zbmpmessagebox(temp.GetBuffer(0));
			UpdateList();
		}
		catch (_com_error e) 
		{
			
			temp.Format("������ʾ,ɾ�����ʧ��!,%s",e.Description());
			zbmpmessagebox(temp.GetBuffer(0));
		}
	}//if
}

void CPZVideoDlg::OnItemchangedLISTBKVideo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here	
	CString temp;
	temp=m_ListConVideo.GetItemText(pNMListView->iItem,0);
	int nIndex = 0;
	nIndex=m_ComboBoxBH.FindString(nIndex,temp);
	m_ComboBoxBH.SetCurSel(nIndex);	//���
 	m_IPAddrCtrl.SetWindowText(m_ListConVideo.GetItemText(pNMListView->iItem,1));//IP
 	m_EditUser=m_ListConVideo.GetItemText(pNMListView->iItem,2);//�û�
 	m_EditPwd=m_ListConVideo.GetItemText(pNMListView->iItem,3);//����
 	m_EditTdh=m_ListConVideo.GetItemText(pNMListView->iItem,4);//ͨ��
	m_EditPort=m_ListConVideo.GetItemText(pNMListView->iItem,5);//�˿�

	temp=m_ListConVideo.GetItemText(pNMListView->iItem,6);
	m_ComboSXBH.SetCurSel(atoi(temp)-1);
	temp=m_ListConVideo.GetItemText(pNMListView->iItem,7);
	m_ComboBoxML.SetCurSel(atoi(temp));
	m_MediaIPAddr.SetWindowText(m_ListConVideo.GetItemText(pNMListView->iItem,8));//��ý��
 	m_Edit_BZ=m_ListConVideo.GetItemText(pNMListView->iItem,9);//��ע



	
	

	UpdateData(FALSE);
	*pResult = 0;
}

void CPZVideoDlg::OnSelchangeComboSkin() 
{
	// TODO: Add your control notification handler code here
	int changecombo=m_ComSkin.GetCurSel();
	changecombo=changecombo%3;
	CRect rect,rc; 
	CDC			m_dcBuf;
	CBitmap		m_bmpTmp;
	CDC *pDC =this->GetDC();
	m_dcBuf.CreateCompatibleDC(pDC);
	m_bmpTmp.CreateCompatibleBitmap(pDC,720,450);
 	m_dcBuf.SelectObject(m_bmpTmp);
	this->ReleaseDC(pDC);

	rect.SetRect(0,0,720,450);

	GetDlgItem(IDC_STATIC_SKIN)->GetClientRect(&rc);
	GetDlgItem(IDC_STATIC_SKIN)->GetDC()->BitBlt(0,0,rc.Width(),rc.Height(),&m_dcBuf,0,0,SRCCOPY);

}

void CPZVideoDlg::OnBtnskinset() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	int changecombo=m_ComSkin.GetCurSel();
	temp.Format("�޸���ʾ,�Ƿ��޸�Ƥ��Ϊ[Ƥ��%d]",changecombo+1);
	if (zbmpmessagebox(temp.GetBuffer(0)))
	{
		changecombo =changecombo %3;
		temp.Format("%d",changecombo);
		WritePrivateProfileString("UIskin","nskin",temp,".\\search.cfg");
	}
}

BOOL CPZVideoDlg::zbmpmessagebox(LPTSTR msg)
{
	CBmpDlg msgdlg;
	msgdlg.dlgMessage=msg;
	int nResponse = msgdlg.DoModal();
	if (nResponse == IDOK)
	{
		return TRUE;
	}
	else if (nResponse == IDCANCEL)
	{
		return FALSE;
	}
	return TRUE;
}

void CPZVideoDlg::OnBtnLmtip() 
{
	// TODO: Add your control notification handler code here
	CString temp;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("update tbkvideo set MediaIP=NULL");
		m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		AfxMessageBox("�޸ĳɹ�!");
		UpdateList();
	}
	catch (_com_error e) 
	{
		temp.Format("�޸�ʧ��!%s",e.Description());
		AfxMessageBox(temp);				
	}
}

void CPZVideoDlg::OnBtnTyippwd() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString temp;
	try
	{		
		VARIANT cnt;
		cnt.vt = VT_INT;
		temp.Format("update tbkvideo set �û���='%s',����='%s' ",m_EditUser,m_EditPwd);
		m_pConn->Execute((_bstr_t)temp,&cnt,adCmdUnknown);
		AfxMessageBox("�޸ĳɹ�!");
		UpdateList();
	}
	catch (_com_error e) 
	{
		temp.Format("�޸�ʧ��!%s",e.Description());
		AfxMessageBox(temp);				
	}
}
