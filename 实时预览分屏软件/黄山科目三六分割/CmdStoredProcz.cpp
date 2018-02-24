// CmdStoredProcz.cpp: implementation of the CCmdStoredProcz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdStoredProcz.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdStoredProcz::CCmdStoredProcz()
{
	m_pCommand.CreateInstance("ADODB.Command");
	ASSERT(m_pCommand != NULL);
}

CCmdStoredProcz::~CCmdStoredProcz()
{
	if (m_pCommand !=NULL)
	{
		m_pCommand.Release();
	}
}

CCmdStoredProcz::CCmdStoredProcz(_ConnectionPtr pAdoConnection)
{
	ASSERT(pAdoConnection != NULL);
	try
	{
		pAdoConnection->CursorLocation =adUseClient; 		
 		m_pCommand.CreateInstance("ADODB.Command");	
		ASSERT(m_pCommand != NULL);
		m_pCommand->ActiveConnection = pAdoConnection;
		m_pCommand->CommandType = adCmdStoredProc;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning:CCmdStoredProcz���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
}

BOOL CCmdStoredProcz::SetCommandText(LPCTSTR lpstrCommand)
{
	ASSERT(m_pCommand != NULL);
	try
	{
		//m_pCommand->PutCommandText(_bstr_t(lpstrCommand));
		m_pCommand->CommandText = _bstr_t(lpstrCommand);		
		//m_pCommand->Parameters->Refresh(); 	
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetCommandText ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 	
}

BOOL CCmdStoredProcz::SetParameters(LPCTSTR lpstr, CString strValue)
{
	ASSERT(m_pCommand != NULL);
	try
	{
		m_pCommand->Parameters->Item[_variant_t(_bstr_t(lpstr))]->Value =_variant_t(strValue);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetCommandText ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}

BOOL CCmdStoredProcz::SetParameters(LPCTSTR lpstr, int Valuei)
{
	ASSERT(m_pCommand != NULL);
	try
	{
		m_pCommand->Parameters->Item[_variant_t(_bstr_t(lpstr))]->Value =_variant_t((long)Valuei);
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: SetCommandText ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
}
BOOL CCmdStoredProcz::ExecuteCmd()
{
	try
	{
		m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: CCmdStoredProcz ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return TRUE;
}
long CCmdStoredProcz::CmdExecute()
{
	try
	{
		m_pRecordset.CreateInstance("ADODB.Recordset");	
		ASSERT(m_pRecordset !=NULL);
		m_pRecordset =  m_pCommand->Execute(NULL,NULL,adCmdStoredProc);
		return m_pRecordset->GetRecordCount();
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: CCmdStoredProcz ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return -1;
}

BOOL CCmdStoredProcz::GetCollect(LPCSTR strFieldName, CString &strValue)
{
	ASSERT(m_pRecordset != NULL);	
	try
	{
		_variant_t var =m_pRecordset->GetCollect(_variant_t(strFieldName));
		switch (var.vt)
		{
		case VT_BSTR://�ַ���
		case VT_LPSTR://�ַ���
		case VT_LPWSTR://�ַ���
			strValue = (LPCTSTR)(_bstr_t)var;
			break;
		case VT_I1:
		case VT_UI1:
			strValue.Format("%d", var.bVal);
			break;
		case VT_I2://������
			strValue.Format("%d", var.iVal);
			break;
		case VT_UI2://�޷��Ŷ�����
			strValue.Format("%d", var.uiVal);
			break;
		case VT_INT://����
			strValue.Format("%d", var.intVal);
			break;
		case VT_I4: //����
			strValue.Format("%d", var.lVal);
			break;
		case VT_I8: //������
			strValue.Format("%d", var.lVal);
			break;
		case VT_UINT://�޷�������
			strValue.Format("%d", var.uintVal);
			break;
		case VT_UI4: //�޷�������
			strValue.Format("%d", var.ulVal);
			break;
		case VT_UI8: //�޷��ų�����
			strValue.Format("%d", var.ulVal);
			break;
		case VT_VOID:
			strValue.Format("%8x", var.byref);
			break;
		case VT_R4://������
			strValue.Format("%.4f", var.fltVal);
			break;
		case VT_R8://˫������
			strValue.Format("%.8f", var.dblVal);
			break;
		case VT_DECIMAL: //С��
			strValue.Format("%.8f", (double)var);
			break;
		case VT_CY:
			{
				COleCurrency cy = var.cyVal;
				strValue = cy.Format();
			}
			break;
		case VT_BLOB:
		case VT_BLOB_OBJECT:
		case 0x2011:
			strValue = "[BLOB]";
			break;
		case VT_BOOL://������
			
			strValue = var.boolVal ? "TRUE" : "FALSE";
			break;
		case VT_DATE: //������
			{
				DATE dt = var.date;
				COleDateTime da = COleDateTime(dt); 
				strValue = da.Format("%Y-%m-%d %H:%M:%S");
			}
			break;
		case VT_NULL://NULLֵ
			strValue = "";
			break;
		case VT_EMPTY://��
			strValue = "";
			break;
		case VT_UNKNOWN://δ֪����
		default:
			strValue = "UN_KNOW";
			break;
		}
		
		return TRUE;
	}
	catch (_com_error e)
	{
		strValue=_T("");
		return FALSE;
	}
}

int CCmdStoredProcz::GetErrorCode(int index)
{
	ASSERT(m_pCommand != NULL);	
	int   retVal = -1;
	try
	{
		_variant_t vat;	
		vat = m_pCommand->Parameters->GetItem(short(index))->Value;
		retVal = vat.lVal;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetErrorCode int ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return retVal;
}

int CCmdStoredProcz::GetErrorCode(LPCTSTR strErrorcode)
{
	ASSERT(m_pCommand != NULL);	
	int   retVal = -1;
	try
	{
		_variant_t vat;	
		vat = m_pCommand->Parameters->GetItem(_variant_t(strErrorcode))->GetValue();
		retVal = vat.lVal;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetErrorCode int ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
	}
	return retVal;
}

BOOL CCmdStoredProcz::CmdRelease()
{
	ASSERT(m_pCommand != NULL);
	try
	{
		m_pCommand.Release();
		m_pCommand.Detach();
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetErrorCode int ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CCmdStoredProcz::RecordSetRelease()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL && (m_pRecordset->GetState() & adStateOpen))
		{
			m_pRecordset->Close();
		}		
		m_pRecordset.Release();
		return TRUE;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: GetErrorCode int ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CCmdStoredProcz::pRsetISEOF()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		return m_pRecordset->adoEOF;
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: IsEOF ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
}

BOOL CCmdStoredProcz::pRsetISBOF()
{
	ASSERT(m_pRecordset != NULL);
// 	try
// 	{
// 		return m_pRecordset->adoBOF;
// 	}
// 	catch(_com_error e)
// 	{
// 		TRACE(_T("Warning: IsBOF ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
// 		return FALSE;
// 	} 
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////
BOOL CCmdStoredProcz::MoveNext()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveNext());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveNext ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return FALSE;
}
BOOL CCmdStoredProcz::MoveFirst()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveFirst());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveFirst ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}
BOOL CCmdStoredProcz::MoveLast()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MoveLast());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MoveLast ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	} 
	return	FALSE;
}
BOOL CCmdStoredProcz::MovePrevious()
{
	ASSERT(m_pRecordset != NULL);
	try
	{
		if (m_pRecordset != NULL) 
		{
			return SUCCEEDED(m_pRecordset->MovePrevious());
		}
	}
	catch (_com_error e)
	{
		TRACE(_T("Warning: MovePrevious ���������쳣. ������Ϣ: %s; �ļ�: %s; ��: %d\n"), e.ErrorMessage(), __FILE__, __LINE__);
		return FALSE;
	}
	return FALSE;	
}
//////////////////////////////////////////////////////////////////////////

BOOL CCmdStoredProcz::GetCollect(LPCSTR strFieldName, int &Value)
{
	ASSERT(m_pRecordset != NULL);	
	try
	{
		_variant_t var = m_pRecordset->GetCollect(_variant_t(strFieldName));
		switch (var.vt)
		{
		case VT_BOOL:
			Value = var.boolVal;
			break;
		case VT_UI1:
		case VT_I1:
			Value = var.bVal;
			break;
		case VT_UI2:
		case VT_I2:
			Value = var.iVal;
			break;
		case VT_I4:
		case VT_UI4:
			Value = var.lVal;
			break;
		case VT_INT:
			Value = var.intVal;
			break;
		case VT_R4:
			Value = (int)(var.fltVal + 0.5);
			break;
		case VT_R8:
			Value = (int)(var.dblVal + 0.5);
			break;
		case VT_DECIMAL:
			Value = (long)var;
			break;
		case VT_CY:
			Value = (long)var;
			break;
		case VT_BSTR://�ַ���
		case VT_LPSTR://�ַ���
		case VT_LPWSTR://�ַ���
			Value = atoi((LPCTSTR)(_bstr_t)var);
			break;
		case VT_NULL:
		case VT_EMPTY:
			Value = 0;
			break;
		default:
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		Value = 0;
		return FALSE;
	} 
}

BOOL CCmdStoredProcz::GetCollect(LPCSTR strFieldName, long &Value)
{
	ASSERT(m_pRecordset != NULL);	
	try
	{
		_variant_t var = m_pRecordset->GetCollect(_variant_t(strFieldName));
		switch (var.vt)
		{
		case VT_BOOL:
			Value = var.boolVal;
			break;
		case VT_UI1:
		case VT_I1:
			Value = var.bVal;
			break;
		case VT_UI2:
		case VT_I2:
			Value = var.iVal;
			break;
		case VT_I4:
		case VT_UI4:
			Value = var.lVal;
			break;
		case VT_INT:
			Value = var.intVal;
			break;
		case VT_R4:
			Value = (long)(var.fltVal + 0.5);
			break;
		case VT_R8:
			Value = (long)(var.dblVal + 0.5);
			break;
		case VT_DECIMAL:
			Value = (long)var;
			break;
		case VT_CY:
			Value = (long)var;
			break;
		case VT_BSTR://�ַ���
		case VT_LPSTR://�ַ���
		case VT_LPWSTR://�ַ���
			Value = atol((LPCTSTR)(_bstr_t)var);
			break;
		case VT_NULL:
		case VT_EMPTY:
			Value = 0;
			break;
		default:
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		Value = 0;
		return FALSE;
	} 
}

BOOL CCmdStoredProcz::GetCollect(LPCSTR strFieldName, float &Value)
{
	ASSERT(m_pRecordset != NULL);	
	try
	{
		_variant_t result = m_pRecordset->GetCollect(_variant_t(strFieldName));
		switch (result.vt)
		{
		case VT_R4:
			Value = result.fltVal;
			break;
		case VT_UI1:
		case VT_I1:
			Value = result.bVal;
			break;
		case VT_UI2:
		case VT_I2:
			Value = result.iVal;
			break;
		case VT_NULL:
		case VT_EMPTY:
			Value = 0;
			break;
		default:
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		Value = 0;
		return FALSE;
	} 	
}

BOOL CCmdStoredProcz::GetCollect(LPCSTR strFieldName, double &Value)
{
	ASSERT(m_pRecordset != NULL);	
	try
	{
		_variant_t var = m_pRecordset->GetCollect(_variant_t(strFieldName));
		switch (var.vt)
		{
		case VT_R4:
			Value = var.fltVal;
			break;
		case VT_R8:
			Value = var.dblVal;
			break;
		case VT_DECIMAL:
			Value = (double)var;
			break;
		case VT_CY:
			Value = (double)var;
			break;
		case VT_BOOL:
			Value = var.boolVal;
			break;
		case VT_UI1:
		case VT_I1:
			Value = var.bVal;
			break;
		case VT_UI2:
		case VT_I2:
			Value = var.iVal;
			break;
		case VT_UI4:
		case VT_I4:
			Value = var.lVal;
			break;
		case VT_INT:
			Value = var.intVal;
			break;
		case VT_BSTR://�ַ���
		case VT_LPSTR://�ַ���
		case VT_LPWSTR://�ַ���
			Value = atof((LPCTSTR)(_bstr_t)var);
			break;
		case VT_NULL:
		case VT_EMPTY:
			Value = 0;
			break;
		default:
			TRACE(_T("Warning: �޷���ȡ��Ӧ���ֶ�, �������Ͳ�ƥ��; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
			return FALSE;
		}		
		return TRUE;
	}
	catch (_com_error e)
	{
		Value = 0;
		return FALSE;
	} 
}
