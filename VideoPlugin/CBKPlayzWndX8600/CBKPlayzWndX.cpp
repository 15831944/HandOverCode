// CBKPlayzWndX.cpp : Implementation of CCBKPlayzWndXApp and DLL registration.

#include "stdafx.h"
#include "CBKPlayzWndX.h"
#include "comcat.h"
#include "Objsafe.h"
//#include "Strsafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCBKPlayzWndXApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xb4917d2c, 0x6063, 0x47fd, { 0x91, 0x4b, 0x84, 0x25, 0x49, 0x27, 0, 0x51 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXApp::InitInstance - DLL initialization
	
BOOL CCBKPlayzWndXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}
	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CCBKPlayzWndXApp::ExitInstance - DLL termination

int CCBKPlayzWndXApp::ExitInstance()
{
	// TODO: Add your own module termination code here.
	return COleControlModule::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
//////////////////////////////////////////////////////////////////////////
const GUID CDECL CLSID_SafeItem =
{ 0xd3b921a4, 0xe5d7, 0x48f4, { 0x80,0x83,0xcf,0xea,0x6e,0x2b,0x64,0x74 }};
// �����������
HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (FAILED(hr))
        return hr;
    // Make sure the HKCR\Component Categories\{..catid...}
    // key is registered.
    CATEGORYINFO catinfo;
    catinfo.catid = catid;
    catinfo.lcid = 0x0409 ; // english
    // Make sure the provided description is not too long.
    // Only copy the first 127 characters if it is.
    int len = (int)wcslen(catDescription);
    if (len>127)
        len = 127;
    wcsncpy(catinfo.szDescription, catDescription, len);
    // Make sure the description is null terminated.
    catinfo.szDescription[len] = '\0';
    hr = pcr->RegisterCategories(1, &catinfo);
	pcr->Release();
    return hr;
}
// ע���������
HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    // Register your component categories information.
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
		// Register this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
    }
    if (pcr != NULL)
        pcr->Release();
    return hr;
}
// ж���������
HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
    ICatRegister* pcr = NULL ;
    HRESULT hr = S_OK ;
    hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
    if (SUCCEEDED(hr))
    {
		// Unregister this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
    }
    if (pcr != NULL)
        pcr->Release();
    return hr;
}
//////////////////////////////////////////////////////////////////////////
STDAPI DllRegisterServer(void)
{	
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	//////////////////////////////////////////////////////////////////////////
	HRESULT hr;
	// ��ǿؼ���ʼ����ȫ.
    // ������ʼ����ȫ�������
    hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data!");
    if (FAILED(hr))
        return hr;
    // ע���ʼ����ȫ
    hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
    if (FAILED(hr))
        return hr;
    // ��ǿؼ��ű���ȫ
    // �����ű���ȫ������� 
    hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls safely scriptable!");
    if (FAILED(hr))
        return hr;
    // ע��ű���ȫ�������
    hr = RegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
    if (FAILED(hr))
        return hr;
	//////////////////////////////////////////////////////////////////////////
	return NOERROR;
}


/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	HRESULT hr;
	// ɾ���ؼ���ʼ����ȫ���.
    hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForInitializing);
    if (FAILED(hr))
        return hr;
    // ɾ���ؼ��ű���ȫ���
    hr=UnRegisterCLSIDInCategory(CLSID_SafeItem, CATID_SafeForScripting);
    if (FAILED(hr))
        return hr;
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	return NOERROR;
}
