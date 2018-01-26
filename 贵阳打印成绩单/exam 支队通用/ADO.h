// ADO.h: interface for the ADO class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADO_H__E0F8432E_F797_4979_8FB0_EAC60CA6C7F9__INCLUDED_)
#define AFX_ADO_H__E0F8432E_F797_4979_8FB0_EAC60CA6C7F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ADO  
{
public:

    _ConnectionPtr m_pConnection;					//���Ӷ���ָ��
	_RecordsetPtr  m_pRecordset;					//��¼������ָ��

public:
    CString GetDateTime(char * str,char * sDateFormat=NULL);
	int GetIntFromRecordset(char * str);
	CString GetStrFromRecordset(char * str );
	ADO();
	virtual ~ADO();
	void OnInitADOConn(CString m_IP,CString m_DataBase,CString m_Name,CString m_Pwd,int SQLorOracle);							//�������ݿ�
	_RecordsetPtr&  OpenRecordset(CString sql);		//�򿪼�¼��
	void CloseRecordset();							//�رռ�¼��
	void CloseConn();								//�ر����ݿ�����
	UINT GetRecordCount(_RecordsetPtr pRecordset);	//��ü�¼��	
};

#endif // !defined(AFX_ADO_H__E0F8432E_F797_4979_8FB0_EAC60CA6C7F9__INCLUDED_)
