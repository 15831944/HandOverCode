// StuEndExam.cpp : implementation file
//

#include "stdafx.h"
#include "cfourpic.h"
#include "StuEndExam.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStuEndExam dialog
extern CCFourPicApp theApp;

CStuEndExam::CStuEndExam(CWnd* pParent /*=NULL*/)
	: CDialog(CStuEndExam::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStuEndExam)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CStuEndExam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStuEndExam)
	DDX_Control(pDX, IDC_COMBOITEM, m_CBItem);
	DDX_Control(pDX, IDC_COMBOERR, m_CBErrInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStuEndExam, CDialog)
	//{{AFX_MSG_MAP(CStuEndExam)
	ON_CBN_SELCHANGE(IDC_COMBOITEM, OnSelchangeComboitem)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADDERR, OnBtnAdderr)
	ON_BN_CLICKED(IDC_BTN_STUEND, OnBtnStuend)
	ON_BN_CLICKED(IDC_BTNONCANCEL, OnBtnoncancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStuEndExam message handlers


void CStuEndExam::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

BOOL CStuEndExam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnInitSQL();
	m_CBItem.InsertString(0,"ͨ������");
	m_CBItem.InsertString(1,"�ϳ�׼��");
	m_CBItem.InsertString(2,"��");
	m_CBItem.InsertString(3,"�ᳵ");
	m_CBItem.InsertString(4,"��ת��");
	m_CBItem.InsertString(5,"��ת��");
	m_CBItem.InsertString(6,"ֱ��·��");
	m_CBItem.InsertString(7,"�������");
	m_CBItem.InsertString(8,"����");
    m_CBItem.InsertString(9,"ͨ�����к��");
	m_CBItem.InsertString(10,"��ͷ");
	m_CBItem.InsertString(11,"ͨ����������վ");
	m_CBItem.InsertString(12,"ͨ��ѧУ����");
	m_CBItem.InsertString(13,"ֱ����ʻ");
    m_CBItem.InsertString(14,"����ͣ��");	
	
    m_CBItem.SetCurSel(0);

	m_CBErrInfo.InsertString(0,"�ڵ����رճ�������Ƶ����豸(�˹�����)");
	m_CBErrInfo.InsertString(1,"��������Աָ���ʻ(�˹�����)");
	m_CBErrInfo.InsertString(2,"��ʻ����˫��ͬʱ�뿪ת����(�˹�����)");
	m_CBErrInfo.InsertString(3,"���ֿ���ת����ʱ����ƽ�ȿ�����ʻ����(�˹�����)");
	m_CBErrInfo.InsertString(4,"��������ʻ�е�ͷ����������2�ιҵ�����(�˹�����)");
	m_CBErrInfo.InsertString(5,"�����뿪��ʻ���򳬹�2s(�˹�����)");
	m_CBErrInfo.InsertString(6,"ͨ����ˮ·����������в����ٵȲ�������ʻ(�˹�����)");
	m_CBErrInfo.InsertString(7,"ͨ�����к����ͣ�������������ó���������(�˹�����)");
	m_CBErrInfo.InsertString(8,"��ʻ�������κβ�λ�������(�˹�����)");
	m_CBErrInfo.InsertString(9,"�Կ��ܳ���Σ�յ�����δ��ȡ���ٵȰ�ȫ��ʩ(�˹�����)");
	m_CBErrInfo.InsertString(10,"��۲졢�жϻ��߲�����������Σ�����(�˹�����)");
	m_CBErrInfo.InsertString(11,"����δ����ԤԼ����ʱ��μӿ���(�˹�����)");
	m_CBErrInfo.InsertString(12,"��ʻ���Ʋ���ȷ(�˹�����)");
	m_CBErrInfo.InsertString(13,"����ת�����ַ�������(�˹�����)");
	m_CBErrInfo.InsertString(14,"����ʱ��������ײ��(�˹�����)");
	m_CBErrInfo.InsertString(15,"�����ʱ�������ʹ����������������Ƴ���(�˹�����)");
	m_CBErrInfo.InsertString(16,"�ƶ���ƽ˳(�˹�����)");

	m_CBErrInfo.SetCurSel(0);
	
	
	if(EndType==2)
	{
        GetDlgItem(IDC_BTN_ADDERR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTNONCANCEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STUEND)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_BTN_ADDERR)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTNONCANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STUEND)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStuEndExam::OnSelchangeComboitem() 
{
	// TODO: Add your control notification handler code here
	//��int nIndex = m_combo.GetCurSel();
	
	
	int nIndex=m_CBItem.GetCurSel();
    
	m_CBErrInfo.ResetContent();
	switch(nIndex)
	{
	case 0: //ͨ������
		m_CBErrInfo.InsertString(0,"�ڵ����رճ�������Ƶ����豸(�˹�����)");
		m_CBErrInfo.InsertString(1,"��������Աָ���ʻ(�˹�����)");
		m_CBErrInfo.InsertString(2,"��ʻ����˫��ͬʱ�뿪ת����(�˹�����)");
		m_CBErrInfo.InsertString(3,"���ֿ���ת����ʱ����ƽ�ȿ�����ʻ����(�˹�����)");
		m_CBErrInfo.InsertString(4,"��������ʻ�е�ͷ����������2�ιҵ�����(�˹�����)");
		m_CBErrInfo.InsertString(5,"�����뿪��ʻ���򳬹�2s(�˹�����)");
		m_CBErrInfo.InsertString(6,"ͨ����ˮ·����������в����ٵȲ�������ʻ(�˹�����)");
		m_CBErrInfo.InsertString(7,"ͨ�����к����ͣ�������������ó���������(�˹�����)");
	    m_CBErrInfo.InsertString(8,"��ʻ�������κβ�λ�������(�˹�����)");
		m_CBErrInfo.InsertString(9,"�Կ��ܳ���Σ�յ�����δ��ȡ���ٵȰ�ȫ��ʩ(�˹�����)");
		m_CBErrInfo.InsertString(10,"��۲졢�жϻ��߲�����������Σ�����(�˹�����)");
		m_CBErrInfo.InsertString(11,"����δ����ԤԼ����ʱ��μӿ���(�˹�����)");
		m_CBErrInfo.InsertString(12,"��ʻ���Ʋ���ȷ(�˹�����)");
		m_CBErrInfo.InsertString(13,"����ת�����ַ�������(�˹�����)");
		m_CBErrInfo.InsertString(14,"����ʱ��������ײ��(�˹�����)");
		m_CBErrInfo.InsertString(15,"�����ʱ�������ʹ����������������Ƴ���(�˹�����)");
		m_CBErrInfo.InsertString(16,"�ƶ���ƽ˳(�˹�����)");	   
		break;

	case 1: //�ϳ�׼��
		m_CBErrInfo.InsertString(0,"�򿪳���ǰ���۲�󷽽�ͨ���(�˹�����)");
		break;
	case 2: //��
		m_CBErrInfo.InsertString(0,"�ƶ���ѹ������(�˹�����)");
		m_CBErrInfo.InsertString(1,"��ǰδ�۲���Ӿ��Ͳ�ͷ�۲�󷽽�ͨ���(�˹�����)");
		m_CBErrInfo.InsertString(2,"��ͨ�������ʱ�𲽲��ܺ���ʹ������(�˹�����)");
		m_CBErrInfo.InsertString(3,"��ʱ������������(�˹�����)");
		m_CBErrInfo.InsertString(4,"����������ǰ������ʻ���Ρ����Ӿ����Ǳ�(�˹�����)");	                            
		
        break;
	case 3: //�ᳵ
		m_CBErrInfo.InsertString(0,"�ᳵ����ʱ������(�˹�����)");
		m_CBErrInfo.InsertString(1,"����ȫ����жϲ����ת����öԷ�����(�˹�����)");

        break;
	case 4: //��ת��
		m_CBErrInfo.InsertString(0,"�����涨���ٻ�ͣ���t��(��ת�˹�����)");
		m_CBErrInfo.InsertString(1,"���ҷ���ת��·��ʱ��δ�۲콻ͨ���(��ת�˹�����)");

        break;
	case 5: //��ת��
		m_CBErrInfo.InsertString(0,"�����涨���ٻ�ͣ���t��(��ת�˹�����)");
		m_CBErrInfo.InsertString(1,"���ҷ���ת��·��ʱ��δ�۲콻ͨ���(��ת�˹�����)");
        break;
	case 6: //ֱ��·��
		m_CBErrInfo.InsertString(0,"�����涨���ٻ�ͣ���t��(ֱ���˹�����)");
		m_CBErrInfo.InsertString(1,"���ҷ���ת��·��ʱ��δ�۲콻ͨ���(ֱ���˹�����)");
		break;
	case 7: //�������
		m_CBErrInfo.InsertString(0,"�������δͨ�����Ӿ��۲�󷽵�·��ͨ���(�˹�����)");
		m_CBErrInfo.InsertString(1,"�������ʱ�жϳ�����ȫ���벻����(�˹�����)");
        break;
	case 8: //����
		m_CBErrInfo.InsertString(0,"����ǰ��ͨ��������Ӿ��۲�󷽺���ཻͨ(�˹�����)");
		m_CBErrInfo.InsertString(1,"����ʱ��ѡ�񲻺���Ӱ����������������ʻ(�˹�����)");
		m_CBErrInfo.InsertString(2,"����ʱ��δ��ͷ�۲챻��Խ������̬(�˹�����)");
	    m_CBErrInfo.InsertString(3,"����ʱδ�뱻��Խ�������ְ�ȫ����(�˹�����)");
	    m_CBErrInfo.InsertString(4,"������ת��ʻ��ԭ������������������ʻ(�˹�����)");
		m_CBErrInfo.InsertString(5,"û�������ߺ�ͬ����ֻ��һ���г������Ҳ೬��(����)");
		m_CBErrInfo.InsertString(6,"���󳵷��������źţ������ٿ�������(�˹�����)");
		
        break;
	case 9: //ͨ�����к��
		m_CBErrInfo.InsertString(0,"���۲����ҷ���ͨ���(���к���˹�����)");
		m_CBErrInfo.InsertString(1,"δͣ����������(���к���˹�����)");
	    break;
	case 10: //��ͷ
		m_CBErrInfo.InsertString(0,"������ȷ�۲콻ͨ���ѡ���ͷʱ��(�˹�����)");
		m_CBErrInfo.InsertString(1,"��ͷ�ص�ѡ�񲻵�(�˹�����)");
		m_CBErrInfo.InsertString(2,"��ͷǰδ������ͷ�ź�(�˹�����)");
		m_CBErrInfo.InsertString(2,"��ͷʱ����������ʻ�ĳ���������ͨ��(�˹�����)");

	    break;
	case 11: //ͨ����������վ
		m_CBErrInfo.InsertString(0,"���۲����ҷ���ͨ���(��������վ�˹�����)");
		m_CBErrInfo.InsertString(1,"δͣ����������(��������վ�˹�����)");
	    break;
	case 12: //ͨ��ѧУ����
		m_CBErrInfo.InsertString(0,"���۲����ҷ���ͨ���(ѧУ�����˹�����)");
		m_CBErrInfo.InsertString(1,"δͣ����������(ѧУ�����˹�����)");
	    break;
	case 13: //ֱ����ʻ
		m_CBErrInfo.InsertString(0,"����ʱͨ���ڡ�����Ӿ��۲�󷽽�ͨ���(�˹�����)");
		m_CBErrInfo.InsertString(1,"δ��ʱ����·���ϰ��﷢���ϰ���δ��ʱ����(�˹�����)");

        break;
	case 14: //����ͣ��
		m_CBErrInfo.InsertString(0,"ͣ��ǰ����ͨ�����Ӿ��۲콻ͨ���(�˹�����)");
		m_CBErrInfo.InsertString(1,"����Ա��������ͣ��ָ��δ���ڹ涨����ͣ��(�˹�����)");
		m_CBErrInfo.InsertString(1,"���ҷ���ת��·��ʱ��δ�۲콻ͨ���(ֱ���˹�����)");
        break;

	}

}

void CStuEndExam::OnBtnAdderr() 
{
	// TODO: Add your control notification handler code here
	CString sItem="",sErrInfo="",sInfo="";
	m_CBItem.GetWindowText(sItem);
    m_CBErrInfo.GetWindowText(sErrInfo);
    

	if(sItem.IsEmpty() || sErrInfo.IsEmpty())
	{
		MessageBox("��ѡ��۷���Ŀ�Ϳ۷���");
		return;
	}

	if (IDOK == MessageBox("���ÿ���������ֹ!\n�ÿ������ο��Բ��ϸ�\n�Ƿ�ȷ��?", 
		"��ȷ��...", MB_OKCANCEL|MB_ICONQUESTION))
	{
		ManuExamEndStr(sEndExamCard,sErrInfo);
		//��־��¼
		
	}
	else
	{
		return;
	}

	
   MessageBox("�˹����������ɣ�");

   SendMessage(WM_CLOSE);


}

void CStuEndExam::ManuExamEndStr(CString sCard,CString sErr)
{
	_RecordsetPtr pRecordset;
	CString strSQL,sStuCard;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vSir1,vSir2,vKSRQ,vRd;
	int iZT,iDrcs,iKscs;
	if (sCard == "׼��֤��:")
	{
		AfxMessageBox("׼��֤����Ų���Ϊ��");
		return;
	}
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM StudentInfo WHERE ׼��֤�����='%s'",sCard);
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		

			vZT  = pRecordset->GetCollect("״̬");
			vKscs  = pRecordset->GetCollect("���Դ���");
			vDrcs  = pRecordset->GetCollect("���մ���");
			vStuCard = pRecordset->GetCollect("׼��֤�����");
			vSir1 = pRecordset->GetCollect("����Ա1");
			vSir2 = pRecordset->GetCollect("����Ա2");
		    vKSRQ = pRecordset->GetCollect("Ԥ������");
			vRd =   pRecordset->GetCollect("����·��");
		
		iZT = atoi(VariantToStr(vZT));
        iKscs = atoi(VariantToStr(vKscs));
		iDrcs = atoi(VariantToStr(vDrcs));
		sStuCard=VariantToStr(vStuCard);
		if (iZT != 3)
		{
			AfxMessageBox("�ÿ�������ɿ��ԣ��޷�Զ����ֹ!\n��ͨ���˹����жԱ��ο���׷������!");
		}
		else
		{  
			if(sErr=="ͨ������-�������׻�Υ�����")// 
			{
				ManuExamAllEndAddErr(sStuCard,iKscs,iDrcs,sErr);
				pRecordset->PutCollect("��������",_variant_t("2"));
			}
			else
			{ 
				ManuExamEndAddErr(sStuCard,iKscs,iDrcs,sErr);
				pRecordset->PutCollect("��������",_variant_t("1"));
			}
			pRecordset->Update();
		}
		
		pRecordset->Close();


		
	
		ModifyRecord(sEndExamCard,"�˹�����","",m_stSIR);
		
		
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr.Format("��ȡ%sѧԱ��Ϣʧ��!",sCard);
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}	
	

	
}

void CStuEndExam::ManuExamStuEnd(CString sCard,int inum)//�ϸ�������в��ϸ�
{
	if (sCard == "׼��֤��:")
	{
		AfxMessageBox("׼��֤����Ų���Ϊ��");
		return;
	}
	
	_RecordsetPtr pRecordset;
	CString strSQL,sStuCard;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard;
	int iZT,iDrcs,iKscs;
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM StudentInfo WHERE ׼��֤�����='%s'",sCard);
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);

			vZT  = pRecordset->GetCollect("״̬");
			vKscs  = pRecordset->GetCollect("���Դ���");
			vDrcs  = pRecordset->GetCollect("���մ���");
			vStuCard = pRecordset->GetCollect("׼��֤�����");
			vStuSir1 = pRecordset->GetCollect("����Ա1");
			vStuSir2 = pRecordset->GetCollect("����Ա2");
			vStuKSRQ = pRecordset->GetCollect("Ԥ������");
			vStuRd =   pRecordset->GetCollect("����·��");

		iZT = atoi(VariantToStr(vZT));
        iKscs = atoi(VariantToStr(vKscs));
		iDrcs = atoi(VariantToStr(vDrcs));
		sStuCard=VariantToStr(vStuCard);
		if (iZT != 1)
		{
			AfxMessageBox("�ÿ�����Ϊ�ϸ�!\n�޷�����!");
		}
		else
		{  
			
			CString anum;
			anum.Format("%d",inum);
			ManuExamAllEndAddErr(sStuCard,iKscs,iDrcs,anum);
			ManuExamAllEndAddExam(sStuCard,iKscs,iDrcs);
			pRecordset->PutCollect("���մ���", _variant_t("2"));
			pRecordset->Update();
			
			ModifyRecord(sEndExamCard,"�˹�����(�ϸ��)","",m_stSIR);
		}
		
		pRecordset->Close();
		
		
		
		
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr.Format("��ȡ%sѧԱ��Ϣʧ��!",sCard);
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}	
	
	
}

void CStuEndExam::ManuExamEndAddErr(CString sStuCard,int iKscs,int iDric,CString sErr) //���Ӵ�����
{
	
	_RecordsetPtr ipRecordset;
	CString istrSQL;
	ipRecordset.CreateInstance("ADODB.Recordset");
	ipRecordset->CursorLocation = adUseClient;
	istrSQL.Format("SELECT * FROM ErrorData WHERE �۷�����='%s'",sErr);
	ipRecordset->Open((_variant_t)_bstr_t(istrSQL), _variant_t((IDispatch*)m_pConnex, true), adOpenDynamic, adLockOptimistic, adCmdText);
	
	_variant_t num;
	int inum;
	CString iinum;
	num=ipRecordset->GetCollect("������");
	inum = atoi(VariantToStr(num));
	iinum.Format("%d",inum);
	
	
	
	
	CString strSQL;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vDateTime,vErr;
	
	CString nDateTime;
	CString nDate;
	COleDateTime OleTime = COleDateTime::GetCurrentTime();
	nDateTime.Format("%s", OleTime.Format("%Y-%m-%d %H:%M:%S"));
	
	
	_RecordsetPtr pRecordset;
	
	
	
	ChangeToVariant(sStuCard, &vStuCard, adBSTR);
	ChangeToVariant(iKscs, &vKscs, adInteger);
	ChangeToVariant(iDric, &vDrcs, adInteger);
	ChangeToVariant(nDateTime, &vDateTime, adBSTR); //�����ʱ��
   	ChangeToVariant(sErr, &vErr, adBSTR); //�����ʱ��
	
	
	
	
    
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL.Format("SELECT * FROM ErrorRecords WHERE (׼��֤����� = '%s') AND (���Դ��� = %d) AND (���մ��� = %d)",sStuCard,iKscs,iDric);
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		pRecordset->AddNew();
		
		pRecordset->PutCollect("׼��֤�����",vStuCard);
		pRecordset->PutCollect("������",_variant_t(iinum));
		pRecordset->PutCollect("����ʱ��", vDateTime);
		pRecordset->PutCollect("���Դ���", vKscs);
		pRecordset->PutCollect("���մ���", vDrcs);
		pRecordset->PutCollect("��ע", vErr);
		
		
		pRecordset->Update();
		
		pRecordset->Close();
		
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr.Format("��ȡ%sѧԱ��Ϣʧ��!",sStuCard);
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}			
}

void CStuEndExam::ManuExamAllEndAddErr(CString sStuCard,int iKscs,int iDric,CString sErr) //���Ӵ�����
{
	
	CString strSQL;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vDateTime,vErr;
	
	CString nDateTime;
	CString nDate;
	COleDateTime OleTime = COleDateTime::GetCurrentTime();
	nDateTime.Format("%s", OleTime.Format("%Y-%m-%d %H:%M:%S"));
	
	
	_RecordsetPtr pRecordset;
	
	
	
	ChangeToVariant(sStuCard, &vStuCard, adBSTR);
	ChangeToVariant(iKscs, &vKscs, adInteger);
//	ChangeToVariant(iDric, &vDrcs, adInteger);
	ChangeToVariant(nDateTime, &vDateTime, adBSTR); //�����ʱ��
   	ChangeToVariant(sErr, &vErr, adBSTR); //�����ʱ��
	
    
	for(int i=iDric;i<=2;i++)
	{
		if(i>0 && i<3)
		{
			try
			{

		        //////////////////////////////////////////////////////////////////////////
				pRecordset.CreateInstance("ADODB.Recordset");
				pRecordset->CursorLocation = adUseClient;
				strSQL.Format("SELECT * FROM ErrorRecords WHERE (׼��֤����� = '%s') AND (���Դ��� = %d) AND (���մ��� = %d)",sStuCard,iKscs,i);
				
				pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
					adOpenDynamic, adLockOptimistic, adCmdText);
				

				pRecordset->AddNew();
				
// 				int iinum;
// 				iinum =atoi(sErr);
				ChangeToVariant(i, &vDrcs, adInteger);
				pRecordset->PutCollect("׼��֤�����",vStuCard);
				pRecordset->PutCollect("������",_variant_t(sErr));
				pRecordset->PutCollect("����ʱ��", vDateTime);
				pRecordset->PutCollect("���Դ���", vKscs);
				pRecordset->PutCollect("���մ���", vDrcs);
				pRecordset->PutCollect("��ע", vErr);
				
				
				pRecordset->Update();
				
				pRecordset->Close();
                //////////////////////////////////////////////////////////////////////////


				////////////////////////////////////////////////////////////////////////////





				////////////////////////////////////////////////////////////////////////////


				
			}
			catch (_com_error e)
			{
				CString strErr;
				strErr.Format("��ȡ%sѧԱ��Ϣʧ��!",sStuCard);
				AfxMessageBox(strErr);
				AfxMessageBox(e.Description());
				return ;
			}
		}
	}



	
}


void CStuEndExam::ManuExamAllEndAddExam(CString sStuCard,int iKscs,int iDric) //����Exam
{

	CString strSQL;
	
	_variant_t vZT,vKscs,vDrcs,vStuCard,vDateTime,vErr;
	
	CString nDateTime;
	CString nDate;
	COleDateTime OleTime = COleDateTime::GetCurrentTime();
	nDateTime.Format("%s", OleTime.Format("%Y-%m-%d %H:%M:%S"));
	
	
	_RecordsetPtr pRecordset;
	
	
	
	ChangeToVariant(sStuCard, &vStuCard, adBSTR);
	ChangeToVariant(iKscs, &vKscs, adInteger);
	//	ChangeToVariant(iDric, &vDrcs, adInteger);
	ChangeToVariant(nDateTime, &vDateTime, adBSTR); //�����ʱ��
 
	
    CString sDRCS;

	for(int i=iDric;i<=2;i++)
	{
		if(i>0 && i<3)
		{
			sDRCS.Format("%d",i);
			ChangeToVariant(i, &vDrcs, adInteger);

			try
			{
				pRecordset.CreateInstance("ADODB.Recordset");
	        	pRecordset->CursorLocation = adUseClient;
		
				strSQL.Format("SELECT * FROM ExamRecord WHERE ׼��֤����� = '%s' AND ���Դ��� = %d AND ���մ���=%d",sStuCard,iKscs,i);
			
				
				pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
					adOpenDynamic, adLockOptimistic, adCmdText);

				while (!pRecordset->adoEOF)
				{
					pRecordset->Delete(adAffectCurrent);
					pRecordset->MoveNext();
				}
				pRecordset->UpdateBatch(adAffectAll);

				//��ӵ�SQL
				pRecordset->AddNew();
				pRecordset->PutCollect("׼��֤�����", vStuCard);
				pRecordset->PutCollect("���մ���", vDrcs);
				pRecordset->PutCollect("���Դ���", vKscs);
				pRecordset->PutCollect("����Ա1", vStuSir1);
				pRecordset->PutCollect("����Ա2", vStuSir2);
				pRecordset->PutCollect("����·��", vStuRd);
				pRecordset->PutCollect("��������", vStuKSRQ);

				
				pRecordset->Update();
				pRecordset->Close();
				


				
			}
			catch (_com_error e)
			{
				AfxMessageBox("���Խ����¼����ɾ��!");
				AfxMessageBox(e.Description());
				AfxMessageBox(e.ErrorMessage());
			}
		}
	}



}



void CStuEndExam::OnBtnStuend() 
{
	// TODO: Add your control notification handler code here
	CString sErrInfo="";
	m_CBErrInfo.GetWindowText(sErrInfo);
	_RecordsetPtr pRecordset;
	CString strSQL;
	pRecordset.CreateInstance("ADODB.Recordset");
	pRecordset->CursorLocation = adUseClient;
	strSQL.Format("SELECT * FROM ErrorData WHERE �۷�����='%s'",sErrInfo);
	pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
		adOpenDynamic, adLockOptimistic, adCmdText);
	
	_variant_t num;
	int inum;
	
	num=pRecordset->GetCollect("������");
	inum = atoi(VariantToStr(num));
	
	
	if (IDOK == MessageBox("�ÿ������ο�������Ϊ���ϸ�\n�Ƿ�ȷ��?", 
		"��ȷ��...", MB_OKCANCEL|MB_ICONQUESTION))
	{

		ManuExamStuEnd(sEndExamCard,inum);
		//��־��¼
		
		
	}
	else
	{
		return;
	}
	
	
	MessageBox("�˹����������ɣ�");
	
	
	SendMessage(WM_CLOSE);
}

void CStuEndExam::OnBtnoncancel() 
{
	// TODO: Add your control notification handler code here
	SendMessage(WM_CLOSE);
}


//��־��¼
void CStuEndExam::ModifyRecord(CString sCard, CString sModify, CString sBZ, CString sOperator)
{
	_RecordsetPtr pRecordset;
	CString strSQL;
	COleDateTime modifytime;
	modifytime = COleDateTime::GetCurrentTime();
	
	_variant_t vCard, vModify, vtime, vsir1, vsir2, vBZ;
	ChangeToVariant(modifytime, &vtime, adDBDate);
	ChangeToVariant(sCard, &vCard, adBSTR);
	ChangeToVariant(sModify, &vModify, adBSTR);
	ChangeToVariant(sOperator, &vsir1, adBSTR);
	//	ChangeToVariant(m_szTeacher2Name, &vsir2, adBSTR);
	ChangeToVariant(sBZ, &vBZ, adBSTR);
	
	try
	{
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		strSQL = "SELECT top 10 * FROM ModifyRecord";
		
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConnex, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		pRecordset->AddNew();
		pRecordset->PutCollect("��������",vCard);
		pRecordset->PutCollect("��������",vModify);
		pRecordset->PutCollect("ʱ��",vtime);
		pRecordset->PutCollect("������Ա1",vsir1);
		pRecordset->PutCollect("������Ա2",vsir2);
		pRecordset->PutCollect("��ע",vBZ);
		
		pRecordset->Update();
		
		pRecordset->Close();
		pRecordset.Release();
	}
	catch (_com_error e)
	{
		CString strErr;
		strErr="��¼������־ʧ��!";
		AfxMessageBox(strErr);
		AfxMessageBox(e.Description());
		return ;
	}
	
}

BOOL CStuEndExam::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	m_pConnex.CreateInstance("ADODB.Connection");
	if(m_pConnex==NULL)//CoInitialize��NULL��δ��ʼ��
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
	//////////////////////////////////////////////////////////////////////////
	try
	{
		CString Sqlstr;
		//		Sqlstr.Format("Provider=SQLOLEDB;server=%s;UID=%s;PWD=%s;database=%s",SQLSer,SQLUSER,SQLPWD,SQLDB);
		Sqlstr =m_strInEdit;
		TRACE(Sqlstr);
		res = m_pConnex->Open((_bstr_t)Sqlstr,"","",0);
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
		ShellExecute(NULL,"open",".\\sqlLink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	return TRUE;
}