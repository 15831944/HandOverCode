#if !defined(AFX_SCREENPANNEL_H__59976CBA_70D0_4BDA_B9E9_1315B0A1B074__INCLUDED_)
#define AFX_SCREENPANNEL_H__59976CBA_70D0_4BDA_B9E9_1315B0A1B074__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenPannel.h : header file
//
#include "BSWndContainer.h"
#include "PlayWndz.h"

#define WND_MAXNUM 16
#define NAME_MENU_FULLSCREEN "ȫ����ʾ"
//#define NAME_MENU_MULTISCREEN "������ʾ"
#define NAME_MENU_AUTOADJUST "�Զ�����"


//����ָ�����
enum
{
	SPLIT1 = 0,
	SPLIT4,
	SPLIT9,
	SPLIT16,
	SPLIT_TOTAL
};
/////////////////////////////////////////////////////////////////////////////
// CScreenPannel window
typedef void (CALLBACK *CBMessageProcFunc)(UINT nWndID,WPARAM wParam,LPARAM lParam,DWORD dwUser);

class CScreenPannel : public CBSWndContainer
{
// Construction
public:
	CScreenPannel();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenPannel)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CScreenPannel();

	// Generated message map functions
protected:
	//{{AFX_MSG(CScreenPannel)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CPlayWndz m_wndVideo[WND_MAXNUM];	//�Ӵ���
	int m_curScreen;					//��ǰ��ʾ�������
	int m_Split;						//������
public:
//	void SetVoiceSound(int ivoice, bool btype,UINT uWndID);
	void SetPlayWndThiS(LPCTSTR pstrFormat, ... );
	void SetPlayWndState(int playstate,bool bAdd=true);
	int GetPlayWndState();
	void OnVideoMenu(UINT nWndID,WPARAM wParam,LPARAM lParam);
	//////////////////////////////////////////////////////////////////////////
	CBMessageProcFunc	m_pMessageProc;
	DWORD				m_dwMessageUser;
	void SetCallBack(CBMessageProcFunc cbMessageProc, DWORD dwMessageUser);
	//////////////////////////////////////////////////////////////////////////
	int	GetCurWindId();
	int SetShowPlayWin(int nMain);
	void SetCurWindId(int nCuWinID);
	//////////////////////////////////////////////////////////////////////////
	int GetSplit();
	void SetSplit(int split);
	void SwitchMultiWnd(int nSplit); //�����л� 0 ������ 1 �Ļ��� 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCREENPANNEL_H__59976CBA_70D0_4BDA_B9E9_1315B0A1B074__INCLUDED_)
