// ADO.cpp: implementation of the ADO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ADO.h"
#include <afxdisp.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern 

ADO::ADO()
{

}

ADO::~ADO()
{

}

void ADO::OnInitADOConn(int SQLorOracle)
{

	::CoInitialize(NULL);
	 CString m_strInEdit;
	try
	{
       m_pConnection.CreateInstance("ADODB.Connection");  //�������Ӷ���ʵ��
	   ////////////////////////////////��Ƭ���ݿ�//////////////////////////////////////////
	  
	   GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,".\\ConfigPhoto.ini");
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
	   /*
	   CString strConnect;
		if (SQLorOracle==1)
		{
			strConnect.Format("driver={SQL Server};Server=%s;DATABASE=%s;UID=%s;PWD=%s",m_IP,m_DataBase,m_Name,m_Pwd);
		}else
		{
			strConnect.Format("Provider=OraOLEDB.Oracle.1;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=%s)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=bkzy)));User ID=sapho;Password=%s",m_IP,m_Pwd);
		}
		*/
	   m_pConnection->Open((_bstr_t)m_strInEdit,"","",adModeUnknown); //�����ݿ�
	}
	catch(_com_error e)
	{
		CString errormessage;
		errormessage.Format("���ݿ���ʾ,����2ndPhoto��Ƭ���ݿ�ʧ��!,������Ϣ:%s,����������Ƭ���ݿ�����",e.ErrorMessage());
		AfxMessageBox(errormessage);
		//AfxMessageBox(m_strInEdit);
		if (SQLorOracle==1)
		{
			ShellExecute(NULL,"open",".\\sqlLink.exe",".\\ConfigPhoto.ini",NULL,SW_SHOW);//����SQL����
		}
		else
		{
			ShellExecute(NULL,"open",".\\oracle.exe",".\\ConfigPhoto.ini",NULL,SW_SHOW);//����Oracle����
		}
	}


}

_RecordsetPtr&  ADO::OpenRecordset(CString sql)
{
	ASSERT(!sql.IsEmpty());										//SQL��䲻��Ϊ��
	try
	{
		m_pRecordset.CreateInstance(__uuidof(Recordset)); //������¼������ʵ��
		m_pRecordset->Open(_bstr_t(sql), m_pConnection.GetInterfacePtr(),
			adOpenDynamic, adLockOptimistic, adCmdText); 	//ִ��SQL�õ���¼��
	}
	catch(_com_error e)											//������ܵ��쳣
	{
		AfxMessageBox(e.Description());
	}
	return m_pRecordset;
}

void ADO::CloseRecordset()
{ 
	if(m_pRecordset->GetState() == adStateOpen)	//�жϵ�ǰ�ļ�¼��״̬
		m_pRecordset->Close();					//�رռ�¼��
}

void ADO::CloseConn()
{
	m_pConnection->Close();									//�ر����ݿ�����
	::CoUninitialize();										//�ͷ�COM����
}

UINT ADO::GetRecordCount(_RecordsetPtr pRecordset)
{
	int nCount = 0;											//���������¼���ı���
	try{
		pRecordset->MoveFirst();								//����¼��ָ���ƶ�����һ����¼
	}
	catch(...)												//��׽���ܳ��ֵĴ���
	{
		return 0;											//��������ʱ����0
	}
	if(pRecordset->adoEOF)									//�жϼ�¼�����Ƿ�û�м�¼
		return 0;											//�޼�¼ʱ����0
	while (!pRecordset->adoEOF)								//����¼��ָ��û��ָ�����ʱ
	{
		pRecordset->MoveNext();								//����¼��ָ���ƶ�����һ����¼
		nCount = nCount + 1;								//��¼�����ı�����1
	}
	pRecordset->MoveFirst();									//����¼��ָ���ƶ�����һ����¼
	return nCount;											//���ؼ�¼��
}



CString ADO::GetStrFromRecordset(char * str)
{
	 		  _variant_t var;
			  CString strPosInfo;
		      var= m_pRecordset->GetCollect(str);          //����Ϊ��
			  if(var.vt != VT_NULL)   //ΪNULL
               strPosInfo = (char*)(_bstr_t)var;
	 	      else
 			  strPosInfo = "";

			  return strPosInfo;
}

int ADO::GetIntFromRecordset(char *str)
{
	_variant_t var;
 	int  iPosInfo=0;
	var= m_pRecordset->GetCollect(str);          //����Ϊ��
	if(var.vt != VT_NULL)   //ΪNULL
		iPosInfo =atoi((char*)(_bstr_t)var);         //
	else 
		iPosInfo =0;

  
	
	return iPosInfo;
}

CString ADO::GetDateTime(char * str,char * sDateFormat)
{
        if(sDateFormat==NULL)
		{
			sDateFormat="%Y-%m-%d";
		}
	    CString strTimeGet;  
        _variant_t varTimeGet;  
        COleDateTime oleTimeGet;    
      
        varTimeGet=m_pRecordset->GetCollect(str);  
        if (varTimeGet.vt!=VT_NULL)  
        {  

            oleTimeGet=(COleDateTime)varTimeGet;  
            strTimeGet=oleTimeGet.Format(sDateFormat);
           // AfxMessageBox(strTimeGet);  
        }  
		else
        {
			strTimeGet="";
		}
	
		
		return strTimeGet;

}
