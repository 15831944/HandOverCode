// ThirdPicJMQ.cpp: implementation of the CThirdPicJMQ class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ThirdPicJMQ.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define TEXTCL RGB(255,255,255)
#define TITLECL RGB(255,255,255)
CThirdPicJMQ::CThirdPicJMQ()
{
	m_iMtype=0;
	m_SleepTime=1000;
}

CThirdPicJMQ::~CThirdPicJMQ()
{

}


DWORD CThirdPicJMQ::ThreadMethod()
{
	DWORD times=m_iCarNum*(rand()%13);
	Sleep(times);
	DWORD Rtime,Stime;//�̼߳�ʱ
	hThreadEvent =CreateEvent(NULL,FALSE,FALSE,NULL);
 	SetEvent(hThreadEvent);
	WriteLog("ThirdPic �߳̿���!ThreadRun %d",m_iCarNum);
	Graphics grdc(m_ImageDC.GetSafeHdc());
	threadRun=false;

	CString strtext;
	CFont fontz,fontz2;//
	fontz.CreateFont(20, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "����");
	fontz2.CreateFont(22, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_STROKE_PRECIS, CLIP_STROKE_PRECIS,
		DRAFT_QUALITY, VARIABLE_PITCH | FF_SWISS, "����");
	
	m_ImageDC.SetBkMode(TRANSPARENT);//͸��	
	bool runerror;
	CRect textrc[8];
	textrc[0].SetRect(18,6,350,34);
	for (int i=1;i<8;i++)
	{
		textrc[i].SetRect(90,35*i+5,350,41+35*i);	
	}
	textrc[1].left=58;
	textrc[1].top-=4;
	textrc[2].left=58;
	int iAdozt;
	while (1)
	{
		WaitForSingleObject(hThreadEvent,INFINITE);
		Rtime=timeGetTime();
		//////////////////////////////////////////////////////////////////////////
		if (m_iMtype & MTYPE2)
		{
			iAdozt=CreateAdoConn();
			if (iAdozt !=1)
			{
				WriteLog("�������ݿ�ʧ��!%d",iAdozt);
				Sleep(1000);
				continue;
			}
			if(GetStuMSG()==false)
			{
				Sleep(1000);
				continue;
			}
			if (m_idrcs==0)
			{
				m_idrcs=GetDrcs();
			}
			strtext.Format("%s ����:%d",textstr[2],m_idrcs);
			textstr[2].Format("%s",strtext);			
			grdc.DrawImage(ImgBK,Rect(0,0,352,288));//����
			CRect phorc;	
			phorc.SetRect(242,10,100,126);//pho
			ReadPhotoFromDB(&m_ImageDC,m_zkzmbh,phorc);
			phorc.SetRect(272,140,80,100);//mjpho
			ReadMJPhotoFromDB(&m_ImageDC,m_zkzmbh,phorc);
			RealseAdoConn();
			m_iMtype=m_iMtype & (~ MTYPE2);
		}
		if (m_iMtype & MTYPE1)
		{
			m_iMtype=m_iMtype & (~ MTYPE1);	
			if (m_iMtype & MTYPE3)
			{
				grdc.DrawImage(ImgBK,Rect(0,0,352,288));//����
				m_iMtype=m_iMtype & (~ MTYPE3);	
			}
			m_ImageDC.SetTextColor(TEXTCL);
			m_ImageDC.SelectObject(&fontz);
			for (i=0;i<8;i++)
			{
				if (1==i)
				{
					continue;
				}
				m_ImageDC.DrawText(textstr[i],&textrc[i],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
			} 
			m_ImageDC.SetTextColor(TITLECL);
			m_ImageDC.SelectObject(&fontz2);
			m_ImageDC.DrawText(textstr[1],&textrc[1],DT_LEFT | DT_SINGLELINE | DT_VCENTER );
		}
		if (m_iMtype & MTYPE6)
		{
			//m_iMtype=m_iMtype & (~ MTYPE6);	
			DrawHbHg(&grdc,150,80);
		}
		//////////////////////////////////////////////////////////////////////////
#ifdef SAVE_PNGFILE
		SavePngFile();
		runerror=true;	
#else
		runerror=IsMSendDataT();
#endif
 		//////////////////////////////////////////////////////////////////////////
 		if (runerror==false && threadRun==false)
 		{
 			TRACE("runerror==false \n");
 			Sleep(100);
 			SetEvent(hThreadEvent);
 		}
		//////////////////////////////////////////////////////////////////////////
		if (threadRun==true)
		{
			SetEvent(hThreadEvent);
		}
		else
		{
			if (iRunNum>0)
			{
				iRunNum-=1;
				SetEvent(hThreadEvent);
			}
		}
		Stime =timeGetTime()-Rtime;
		Stime =Stime>m_SleepTime?1:m_SleepTime-Stime;
		Sleep(Stime);
	}
	fontz2.DeleteObject();
	fontz.DeleteObject();
	WriteLog("ThirdPic �߳��˳�!ThreadRun %d",m_iCarNum);
	return 0;
}

void CThirdPicJMQ::ThirdPicInit(int ikch, CString path,int wMSG,HWND hwndz)
{
	InitDC(ikch,3,path,wMSG,hwndz);
	CString temp;
	unsigned short StcharArr[256];
	
	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\tbk.skin",path);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	ImgBK=Image::FromFile(StcharArr);

	memset(StcharArr,0x0,sizeof(StcharArr));
	temp.Format("%s\\hgorbhg.skin",path);
	swprintf((wchar_t *)StcharArr,L"%s",temp.AllocSysString());
	ImgHGBHG=Image::FromFile(StcharArr);
	
	CString m_strInEdit,configfile;
	configfile.Format("%s\\config.ini",path);
	GetPrivateProfileString("SQLLINK","ServerPZ","",m_strInEdit.GetBuffer(MAX_PATH),MAX_PATH,configfile);
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
	m_Connstr.Format("%s",m_strInEdit);
	textstr[0].Format("����%d Init . . .",ikch);
	textstr[1].Format("��ʼ�����");
	m_iMtype =m_iMtype | MTYPE1;
	m_iMtype =m_iMtype | MTYPE3;
	iRunNum=5;
}

void CThirdPicJMQ::SetSleepTime(DWORD dwTime)
{
	m_SleepTime=dwTime;
}

int CThirdPicJMQ::CreateAdoConn()
{
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize(NULL) δ��ʼ��
	{
		TRACE("��ʼ�����Ӷ���ʧ��\n");
		return -1;
	}
	HRESULT res=-1;
	try
	{
		res = m_pConn->Open((_bstr_t)m_Connstr,"","",0);
		if(res!=S_OK)//0
		{
			return -2;
		}
	}
	catch(_com_error e)//��׽�쳣
	{
		return -3;
	}
	return 1;
}

void CThirdPicJMQ::DrawHbHg(Graphics *graphics, int x, int y)
{
	if (m_izt ==1 || m_izt==2)
	{
		int tempw,temph;
		tempw=ImgHGBHG->GetWidth()/2;
		temph=ImgHGBHG->GetHeight();
		graphics->DrawImage(ImgHGBHG,Rect(x,y,tempw,temph),tempw*(m_izt-1), 0,tempw,temph,UnitPixel);
	}
}



void CThirdPicJMQ::On17C51(CString zkzmbh, int ikscs, int idrcs)
{
	m_iMtype =m_iMtype | MTYPE1;
	m_iMtype =m_iMtype | MTYPE2;
	m_iMtype=m_iMtype & (~ MTYPE6);	
	m_zkzmbh.Format("%s",zkzmbh);
	m_idrcs=idrcs;
	m_ikscs=ikscs;
	iRunNum=3;
	SetEvent(hThreadEvent);
}

void CThirdPicJMQ::On17C52(CString zkzmbh)
{
	if (zkzmbh !=m_zkzmbh)
	{
		On17C51(zkzmbh,0,0);
	}
	else
	{
		SetEvent(hThreadEvent);
	}
}

void CThirdPicJMQ::On17C56(int izt,int kscj)
{
	m_iMtype =m_iMtype | MTYPE6;
	m_izt=izt;
	iRunNum=3;
	SetEvent(hThreadEvent);
}

BOOL CThirdPicJMQ::ReadPhotoFromDB(CDC *pDC, CString sCard,CRect rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
	
	CString temp;
	try
	{		
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		strSQL.Format("SELECT ��Ƭ FROM StudentPhoto WHERE ׼��֤����� = '%s'",sCard);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
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
					//pImg->Render(*pDC,242,10,100,126,0,hmHeight,hmWidth,-hmHeight,NULL);//242 10
					pImg->Render(*pDC,rc.left,rc.top,rc.right,rc.bottom,0,hmHeight,hmWidth,-hmHeight,NULL);//242 10
				}
				else
				{
					//AfxMessageBox("��ӡ��Ƭʧ��!");
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
		}
		
//		pRecordset->Close();
//		pRecordset.Release();
		
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

BOOL CThirdPicJMQ::ReadMJPhotoFromDB(CDC *pDC, CString sCard,CRect rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
	
	CString temp;
	try
	{		
		pRecordset.CreateInstance("ADODB.Recordset");
		pRecordset->CursorLocation = adUseClient;
		CString strSQL;	
		strSQL.Format("SELECT �Ž���Ƭ FROM StudentPhoto WHERE ׼��֤����� = '%s'",sCard);
		pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)m_pConn, true), 
			adOpenDynamic, adLockOptimistic, adCmdText);
		
		if(pRecordset->RecordCount != 1)
		{
			return FALSE;
		}		
		char* pbuf = NULL;
		
		long lDataSize = pRecordset->GetFields()->GetItem("�Ž���Ƭ")->ActualSize; //���ݿ���ͼ�����ݳ���
		if (lDataSize > 0)
		{
			_variant_t varBLOB;
			varBLOB = pRecordset->GetFields()->GetItem("�Ž���Ƭ")->GetChunk(lDataSize);
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
					//pImg->Render(*pDC,272,140,80,100,0,hmHeight,hmWidth,-hmHeight,NULL);//30+242 130+10
					pImg->Render(*pDC,rc.left,rc.top,rc.right,rc.bottom,0,hmHeight,hmWidth,-hmHeight,NULL);//30+242 130+10
				}
				else
				{
					//AfxMessageBox("��ӡ��Ƭʧ��!");
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
		}
		
//		pRecordset->Close();
//		pRecordset.Release();
		
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

void CThirdPicJMQ::RealseAdoConn()
{
	if (m_pConn->GetState() == adStateOpen)
	{
		m_pConn->Close();
	}
	if (m_pConn)
	{
		m_pConn.Release();
		m_pConn = NULL;
	}
}

int CThirdPicJMQ::GetDrcs()
{
	int idrcs=0;
	try
	{
		CString sqltemp;
		sqltemp.Format("SELECT ���մ��� FROM Studentinfo WHERE ׼��֤�����='%s' ",m_zkzmbh);
		VARIANT cnt;
		cnt.vt = VT_INT;
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{
			vat =pSet->GetCollect("���մ���");
			idrcs=vat.intVal;
		}
		pSet->Close();
		pSet.Release();
	}
	catch (_com_error e) 
	{
		TRACE(_T("Warning: GetDrcs; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
		return 0;
	}
	return idrcs;
}

bool CThirdPicJMQ::GetStuMSG()
{
	try
	{
		CString sqltemp,MSGstr;
		VARIANT cnt;
		cnt.vt = VT_INT;
		sqltemp.Format("%s'%s' ",sqlExstr,m_zkzmbh);
		TRACE(sqltemp+"\n");
		_RecordsetPtr pSet =m_pConn->Execute((_bstr_t)sqltemp,&cnt,adCmdUnknown);
		_variant_t vat;
		for (int i=0;i<9;i++)
		{
			textstr[i]=_T("");
		}
		if(pSet != NULL)
		{
			while(!pSet->adoEOF)
			{
				for (int i=0;i<9;i++)
				{
					MSGstr.Format("MSG%d",i);
					vat =pSet->GetCollect(_variant_t(MSGstr));
					if (vat.vt != VT_NULL)
					{
						textstr[i].Format("%s",(LPCSTR)_bstr_t(vat));	
					}					
				}
				pSet->MoveNext();
			}
			pSet->Close();
			pSet.Release();	
		}//if		
	}
	catch (_com_error e) 
	{
		TRACE(_T("Warning: GetStuMSG; �ļ�: %s; ��: %d\n"), __FILE__, __LINE__);
		WriteLog("��ѯ������Ϣʧ��!%s",e.ErrorMessage());
		RealseAdoConn();//�Ͽ�ADO����
		return false;
	}
	return true;
}

void CThirdPicJMQ::SetSQLOracle(UINT itype)
{
	if(1==itype)
	{	
		sqlExstr.Format("SELECT ������+'-'+SysCfg.��ע+'-'+���Գ��� as MSG0,���� as MSG1,�Ա� as MSG2,(Select CONVERT(varchar(100), GETDATE(), 23)) as MSG3,��ˮ�� as MSG4,���֤����� as MSG5,��У���� as MSG6,����Ա1+(CASE ����ԭ��	WHEN 'A' THEN ' A-����'	WHEN 'B' THEN ' B-����' WHEN 'F' THEN ' F-����ѧϰ' WHEN 'D' THEN ' D-����' ELSE ' ����ԭ��:δ֪' END) as MSG7,׼��֤����� as MSG8 FROM StudentInfo LEFT JOIN SchoolInfo ON ������=��У��� LEFT JOIN SysCfg ON ������=��Ŀ WHERE ׼��֤�����=");
		WriteLog("SQL�汾 %d",itype);
	}
	else
	{
		//sqlExstr.Format("SELECT ������||'-'||SysCfg.��ע||'-'||���Գ���||'-·��'||SysCfg.����·�� as MSG0,���� as MSG1,�Ա� as MSG2,to_char(sysdate,'yyyy-mm-dd')  as MSG3,��ˮ�� as MSG4,���֤����� as MSG5,��У���� as MSG6,����Ա1 ||'   ����Ա:'|| ����Ա as MSG7,׼��֤�����||'_'||���մ��� as MSG8 FROM StudentInfo LEFT JOIN SchoolInfo ON ������=��У��� JOIN SysCfg ON ������=��Ŀ WHERE ׼��֤�����=");
		sqlExstr.Format("SELECT ������||'-'||SysCfg.��ע||'-'||���Գ���||'-·��'||SysCfg.����·�� as MSG0,���� as MSG1,�Ա� as MSG2,to_char(sysdate,'yyyy-mm-dd')  as MSG3,��ˮ�� as MSG4,���֤����� as MSG5,��У���� as MSG6,����Ա1 as MSG7,׼��֤�����||'_'||���մ��� as MSG8 FROM StudentInfo LEFT JOIN SchoolInfo ON ������=��У��� JOIN SysCfg ON ������=��Ŀ WHERE ׼��֤�����=");
		WriteLog("Oracle�汾 %d",itype);
	}
}