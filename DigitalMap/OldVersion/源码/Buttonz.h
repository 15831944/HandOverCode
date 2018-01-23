#if !defined(AFX_BUTTONZ_H__8665D7B3_3FE2_462C_A7D0_6168DCFB6A65__INCLUDED_)
#define AFX_BUTTONZ_H__8665D7B3_3FE2_462C_A7D0_6168DCFB6A65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Buttonz.h : header file
//
//#include "DD.h"
/////////////////////////////////////////////////////////////////////////////
// CButtonz window

class  CButtonz : public CBitmapButton
{
// Construction
public:
	CButtonz();	
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonz)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	int CurCar_Xpoint; //��ǰ�Ŀ������꾭γ��
	int CurCar_Ypoint;
	UINT CurCar_ZT;
	UINT CurCar_EID;

	UINT ThreadNumber;  //������Ӧ���̺߳�  ������������� UINT examCarNumber;  //������  �������涨�壬Ч���᲻һ��
	//  ���ֵ�Ч����ˢ�µ�ǰ�Ŀ����Ż᲻��ʾ����  ������Ͳ�����  ��֪��Ϊʲô
	int isCircle;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
	int ExamCarState;  //����״̬  ���ߣ�1�� �������ߣ�0��
	DOUBLE Cur_yPoint;  //��ǰ��ťx����
	DOUBLE Cur_xPoint;  //��ǰ��ťy����


	DOUBLE Cur_yPointRelyOnParent;  //��ǰ��ťx����  ����ڸ��Ի���
	DOUBLE Cur_xPointRelyOnParent;  //��ǰ��ťy����
	BOOL bTrack;//�������������Ƿ񱻸��� TRUE ������ FALSE û������

	UINT examCarNumber;  //������
	
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	BOOL LoadBitmap(LPCTSTR lpszName);
	BOOL LoadBitmap(UINT bitmapid);
	virtual ~CButtonz();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonz)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl* m_pToolTip;
	CString		  m_tooltext;

	CSize m_ButtonSize;//��ť�ߴ�
	CBitmap mybitmap;  //����ͼƬ�ı���

public:	
	void WriteLog(CString szERR);
	//��ť��״̬
	BOOL m_bTracking;  //��ť�Ƿ���ٵ����
	BOOL m_bHover;     //ָʾ����Ƿ��ڰ�ť����
	// 	BOOL m_bSelected;	//��ť�������Ǹ�ֵΪtrue
	// 	BOOL m_bFocus;	    //��ťΪ��ǰ��������ʱ��ֵΪtrue
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONZ_H__8665D7B3_3FE2_462C_A7D0_6168DCFB6A65__INCLUDED_)
