// CBKFourPicKM2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CBKFourPicKM2.h"
#include "CBKFourPicKM2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCBKFourPicKM2App

BEGIN_MESSAGE_MAP(CCBKFourPicKM2App, CWinApp)
	//{{AFX_MSG_MAP(CCBKFourPicKM2App)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKFourPicKM2App construction

CCBKFourPicKM2App::CCBKFourPicKM2App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCBKFourPicKM2App object

CCBKFourPicKM2App theApp;

/////////////////////////////////////////////////////////////////////////////
// CCBKFourPicKM2App initialization

BOOL CCBKFourPicKM2App::InitInstance()
{
	AfxEnableControlContainer();
	//////////////////////////////////////////////////////////////////////////
	dllHandle = LoadLibrary(TEXT("CBKVideoSkin.dll")); 
	if (dllHandle == NULL) 
	{ 
		AfxMessageBox("����CBKVideoSkin.dllʧ��!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE; 
	}  
	
    zbmpmessagebox = (MessageBoxbmp) GetProcAddress(dllHandle,TEXT("MessageBoxbmp"));
	lpfnDrawBkgnd = (DRAWBKGND) GetProcAddress(dllHandle,TEXT("DrawImage"));
	//////////////////////////////////////////////////////////////////////////

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	CoInitialize(NULL);
	SetDevMode();
	if (!OnInitSQL())
	{
		return FALSE;
	}
	try
	{		
		CCBKFourPicKM2Dlg dlg;
		m_pMainWnd = &dlg;
		int nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with OK
		}
		else if (nResponse == IDCANCEL)
		{
			// TODO: Place code here to handle when the dialog is
			//  dismissed with Cancel
		}
	}
	catch (...)
	{
		
	}
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CCBKFourPicKM2App::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();
	return CWinApp::ExitInstance();
}

BOOL CCBKFourPicKM2App::OnInitSQL()
{
	//////////////////////////////////////////////////////////////////////////
	_ConnectionPtr m_pConn;
	m_pConn.CreateInstance("ADODB.Connection");
	if(m_pConn==NULL)//CoInitialize��NULL��δ��ʼ��
	{
		TRACE("��ʼ�����Ӷ���ʧ��\n");
		return FALSE;
	}
	HRESULT res=-1;
	try
	{
		//�������ݿ�����
		CString Sqlstr;
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
		Sqlstr=m_strInEdit;
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
		ShellExecute(NULL,"open",".\\sqllink.exe","config.ini",NULL,SW_SHOW); 
		return FALSE;
	}
	m_pConn->Close();
	m_pConn.Release();
	m_pConn=NULL;
	return TRUE;
}
void CCBKFourPicKM2App::SetDevMode()
{
	//����ԭʼ�ֱ����˳���ԭ
	SM_CXSCREEN_x=GetSystemMetrics(SM_CXSCREEN);
	SM_CYSCREEN_y=GetSystemMetrics(SM_CYSCREEN);
	if ( SM_CYSCREEN_y !=900 || SM_CXSCREEN_x !=1440)
	{
		zbmpmessagebox("��ʾ,��ѷֱ���Ϊ 1440*900 ,��������");
		DEVMODE DevModez;
		EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&DevModez);//��ȡ��ǰ������
		DevModez.dmPelsWidth =1440;
		DevModez.dmPelsHeight=900;
		ChangeDisplaySettings(&DevModez,CDS_UPDATEREGISTRY);//������Ч
	}
}
