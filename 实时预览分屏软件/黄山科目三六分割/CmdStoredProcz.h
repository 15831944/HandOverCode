// CmdStoredProcz.h: interface for the CCmdStoredProcz class.zsz 
//�洢����
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDSTOREDPROCZ_H__D6CBDD5A_8B98_4B38_B02E_380AB5406218__INCLUDED_)
#define AFX_CMDSTOREDPROCZ_H__D6CBDD5A_8B98_4B38_B02E_380AB5406218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCmdStoredProcz  
{
public:
	BOOL MoveFirst();
	BOOL MovePrevious();
	BOOL MoveNext();
	BOOL MoveLast();
public:
	BOOL GetCollect(LPCSTR strFieldName,CString &strValue);//ȡֵ(�ֶ���,CString)
	BOOL GetCollect(LPCSTR strFieldName,double &Value);//ȡֵ(�ֶ���,double)
	BOOL GetCollect(LPCSTR strFieldName,float &Value);//ȡֵ(�ֶ���,float)
	BOOL GetCollect(LPCSTR strFieldName,long &Value);//ȡֵ(�ֶ���,long)
	BOOL GetCollect(LPCSTR strFieldName,int &Value);//ȡֵ(�ֶ���,int)
	BOOL pRsetISBOF();//ָ���Ƿ����ڼ�¼��ͷ
	BOOL pRsetISEOF();//ָ���Ƿ����ڼ�¼��β
	//////////////////////////////////////////////////////////////////////////
	BOOL RecordSetRelease();//�ͷ�
	BOOL CmdRelease();//�ͷ�
	//////////////////////////////////////////////////////////////////////////
	int GetErrorCode(LPCTSTR strErrorcode);//��ȡ����ֵ
	int GetErrorCode(int index);//��ȡ����ֵ	
	//////////////////////////////////////////////////////////////////////////
	BOOL SetCommandText(LPCTSTR lpstrCommand);//���ô洢��������
	BOOL SetParameters(LPCTSTR lpstr,CString strValue);//���ò���
	BOOL SetParameters(LPCTSTR lpstr, int Valuei);
	long CmdExecute();//ִ��
	BOOL ExecuteCmd();
	//////////////////////////////////////////////////////////////////////////
	CCmdStoredProcz();
	CCmdStoredProcz(_ConnectionPtr pAdoConnection);
	virtual ~CCmdStoredProcz();
private:
	_RecordsetPtr m_pRecordset;
	_CommandPtr m_pCommand;
};

#endif // !defined(AFX_CMDSTOREDPROCZ_H__D6CBDD5A_8B98_4B38_B02E_380AB5406218__INCLUDED_)
