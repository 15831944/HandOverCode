#if !defined(AFX_CBKMAPCTL_H__65924283_3C55_43FF_A8FE_25315D71C49D__INCLUDED_)
#define AFX_CBKMAPCTL_H__65924283_3C55_43FF_A8FE_25315D71C49D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CBKMapCtl.h : Declaration of the CCBKMapCtrl ActiveX Control class.
#include <objsafe.h> //IE��ȫ�ӿ� һ
#include "CarImage.h"
#include <map>
using namespace std;
typedef map<int,int> MAP_INT_INT;
#define MAX_CAR 100
#include "ToolzDlg.h"
#include "Mmsystem.h"//; include Windows.h.
#pragma comment(lib,"Winmm.lib")

//�Ҽ��˵��Զ�����Ϣ
#define		VIDEO_MENU_BASE				WM_USER + 1800  
#define		VIDEO_MENU_FULLSCREEN		WM_USER + 1801

#define		VIDEO_MENU_END				WM_USER + 1806
/////////////////////////////////////////////////////////////////////////////
// CCBKMapCtrl : See CBKMapCtl.cpp for implementation.

class CCBKMapCtrl : public COleControl
{	

	DECLARE_DYNCREATE(CCBKMapCtrl)
	//ISafeObject
	DECLARE_INTERFACE_MAP()
		
	BEGIN_INTERFACE_PART(ObjSafe, IObjectSafety)
	STDMETHOD_(HRESULT, GetInterfaceSafetyOptions) (
		REFIID riid,
		DWORD __RPC_FAR *pdwSupportedOptions,
		DWORD __RPC_FAR *pdwEnabledOptions
		);
	
	STDMETHOD_(HRESULT, SetInterfaceSafetyOptions) (
		REFIID riid,
		DWORD dwOptionSetMask,
		DWORD dwEnabledOptions
		);
	END_INTERFACE_PART(ObjSafe);
	//ISafeObject
// Constructor
public:
	CCBKMapCtrl();
	ULONG_PTR gdiplusToken;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKMapCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnSetClientSite();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CCBKMapCtrl();

	DECLARE_OLECREATE_EX(CCBKMapCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCBKMapCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCBKMapCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCBKMapCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCBKMapCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnToolzDlgMsg(WPARAM wParam,LPARAM lParam);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnVideoMenu(UINT nID);//z
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CCBKMapCtrl)
	afx_msg void LoadMapFile(LPCTSTR strkcdm, LPCTSTR strUDate);
	afx_msg void PlayBackMap(LPCTSTR strzkzm, short ikscs, short idrcs);
	afx_msg void SetTongJi(short izxkc, short iykrs, short iwkrs, float fhgr);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCBKMapCtrl)
	void FireGetTongJi(LPCTSTR strKcdm)
		{FireEvent(eventidGetTongJi,EVENT_PARAM(VTS_BSTR), strKcdm);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	void DrawPlayBack(Graphics *grapz);
	CWriteLog logz;
	DWORD dwLogid;
	void DrawTongJi(Graphics *grapz);
	CString m_promptStr;
	void SetpromptText(LPCTSTR pstrFormat, ... );
	

	enum {
	//{{AFX_DISP_ID(CCBKMapCtrl)
	dispidLoadMapFile = 1L,
	dispidPlayBackMap = 2L,
	dispidSetTongJi = 3L,
	eventidGetTongJi = 1L,
	//}}AFX_DISP_ID
	};
private:
	CWnd* pWndTask;	//Windows����������
	CRect rect;
	HWND m_hWndParent;
	void OnDrawPBBK(CString str);
	CString m_strKCDM;
	UINT m_uMapMode;
	//////////////////////////////////////////////////////////////////////////
	int m_iykrs,m_iwkrs; //�ѿ����� δ������
	float m_fhgr;		 //�ϸ���
	int m_izxkc;
	//////////////////////////////////////////////////////////////////////////
	int m_iCarNum;
	BOOL m_bFullScreen;
	DWORD m_dwPromptBoxTime,m_dwTongJiTime;
	bool m_bDrawPromptBox;
	void DrawPrompt(Graphics *grapz);
	MAP_INT_INT m_iCarMap;
	CString m_TitleText;

	CToolzDlg m_ToolzDlg;
	CRect m_imgRc;
	bool isRunNowGPSData;
	CCarImage m_CarImg[MAX_CAR];
	bool m_isLoading;
	//////////////////////////////////////////////////////////////////////////
	CSize m_promptBoxSize;
	CSize m_initPicLT;
	CSize m_imageSize;
	//////////////////////////////////////////////////////////////////////////
	Image * m_pImageMap;
	Image * m_TitleBar;
	Image * m_TSKchp;
	Image * m_ImgpromptBox;
	Image * m_ImgTongJi;
	//
	Image * m_ImgPlayBackBk;
	Image * m_ImgPBTemp;
	Image * m_ImgXMP;
	//////////////////////////////////////////////////////////////////////////
	CDC	 m_pbDC;
	CBitmap m_pbBkbmp;
	CDC	 m_dcMem;
	CBitmap m_bmpMap;
	CRect ClientRect;//���ڴ�С
	
	int m_LBx,m_LBy;//�����������
	bool m_isLBDown;//����Ƿ���
	
	double m_Mapx,m_Mapy,m_bs;	//��γ�� �ͱ���
	float m_ZoomLevel,m_ZoomLevelOld,m_MinZoomLevel; //���ż���
	float m_ImageL,m_ImageT,m_ImageW,m_ImageH,m_ImgLOld,m_ImgTOld;
	float m_ShowAllL,m_ShowAllT,m_ShowAllW,m_ShowAllH;
	float m_DestL,m_DestT,m_DestW,m_DestH;
	void AssertImageLT();
	BOOL LoadImageFromIDR(UINT nID, Image * &pImg);

	void SetFullScreen(BOOL bFlag);
	WINDOWPLACEMENT m_OldWndPlacement; //��������ԭ����λ�� 
	CRect m_FullScreenRect; //��ʾȫ����ʾʱ�Ĵ���λ��
	//	ȫ��ʱ����ԭ������Ϣ,�����ָ�����
//	WINDOWPLACEMENT _temppl;		//window's placement
	CWnd *			_tempparent;	//window's parent
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKMAPCTL_H__65924283_3C55_43FF_A8FE_25315D71C49D__INCLUDED)
