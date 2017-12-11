// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__C6BB11AB_A4C5_404D_98DE_4FF91601913F__INCLUDED_)
#define AFX_STDAFX_H__C6BB11AB_A4C5_404D_98DE_4FF91601913F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "AVIThreadz.h"
#pragma comment(lib,"CTranCodeBase.lib")
//海康SDK
#include "../../hikLibDll/HLIB/HCNetSDK.h"
#pragma comment(lib,"../../hikLibDll/HLIB/HCNetSDK.lib")

#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
//gdi
#include "C:\\gdiplus\\Include\\gdiplus.h"
using namespace Gdiplus;  
#pragma comment(lib, "C:\\gdiplus\\lib\\gdiplus.lib")
//忽略ADO的警告
#pragma warning(disable: 4146)
#pragma warning(disable: 4244)
#pragma warning(disable: 4786)//忽略4786警告
//ADO需要导入的库
#import "C:\Program Files\Common Files\SYSTEM\ADO\msado15.dll" no_namespace \
	rename("EOF","adoEOF") \
	rename("DataTypeEnum","adoDataTypeEnum") \
	rename("FieldAttributeEnum","adoFielAttributeEnum") \
	rename("EditModeEnum","adoEditModeEnum") \
	rename("LockTypeEnum","adoLockTypeEnum") \
	rename("RecordStatusEnum","adoRecordStatusEnum") \
rename("ParameterDirectionEnum","adoParameterDirectionEnum")
//////////////////////////////////////////////////////////////////////////
#define MTYPE0 0x0000
#define MTYPE1 0x0001 
#define MTYPE2 0x0002
#define MTYPE3 0x0004
#define MTYPE4 0x0008
#define MTYPE5 0x0010
#define MTYPE6 0x0020
#define MTYPE7 0x0040
#define MTYPE8 0x0080
#define MTYPE9 0x0100
#define MTYPE10 0x0200
#define MTYPE11 0x0400
#define MTYPE12 0x0800
#define MTYPE13 0x1000
#define MTYPE14 0x2000
#define MTYPE15 0x4000
#define MTYPE16 0x8000

#ifndef _SAVE_PNGFILE
#define _SAVE_PNGFILE
#endif
//////////////////////////////////////////////////////////////////////////
typedef struct _gnssmsg
{
	double	gnssX;//经度
	double	gnssY;//纬度
	float	gnssR;//方向角
	float	gnssSD;//速度
	float	gnssLC;//里程
	_gnssmsg()
	{
		gnssX=0.0;
		gnssY=0.0;
		gnssR=0.0f;
		gnssSD=0.0f;
		gnssLC=0.0f;
	}
}GNSSMSG;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__C6BB11AB_A4C5_404D_98DE_4FF91601913F__INCLUDED_)
