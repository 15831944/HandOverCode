#if !defined(AFX_EXAMCARINFODLG_H__1CA81F05_06E4_47FD_BD05_170D47B07A8D__INCLUDED_)
#define AFX_EXAMCARINFODLG_H__1CA81F05_06E4_47FD_BD05_170D47B07A8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExamCarInfoDlg.h : header file
//
#include "ExamCarDlg.h"
#include "DetailsDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CExamCarInfoDlg dialog

class CExamCarInfoDlg : public CDialog
{
// Construction
public:
	int MouseIsThisRect;  //�ж�����Ƿ������������ Ĭ��Ϊ�����������0��  ����������������Ϊ1
	int ShowDlgState;  //��ʾ�Ի����״̬��־λ  Ϊ1ʱ ���Ի���ȫ����ʾ����(������ʾ����) Ϊ0ʱ���Ի���ֻ��ʾһ����(�����ջ�ȥ) �൱��ֻ��ʾ�ױ� ��Ϊ2ʱ��ʾ���Ż��������� 
	CPoint StartDlgPoint; //��ʼ��ʾ�ĵ� 
	CPoint EndDlgPoint;//������ʾ�ĵ�
	CBitmap bmpBackground;
	CExamCarInfoDlg(CWnd* pParent = NULL);   // standard constructor
	CDetailsDlg DetailsDlg; //������
	CExamCarDlg ExamCarDlg;//������  ExamCarInfo.ExamCarDlg

// Dialog Data
	//{{AFX_DATA(CExamCarInfoDlg)
	enum { IDD = IDD_DIALOG_ONLINECARINFO };
	CTabCtrl	m_ctrlTab;
	CStatic	m_Time;
	CStatic	m_Date;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExamCarInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExamCarInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnSelchangeTabOption(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonmain();
	afx_msg void OnButtoncopilot();
	afx_msg void OnButtonoutside();
	afx_msg void OnButtonall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	CBitmap m_bmpMap;
	CDC	 m_dcMem;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMCARINFODLG_H__1CA81F05_06E4_47FD_BD05_170D47B07A8D__INCLUDED_)
