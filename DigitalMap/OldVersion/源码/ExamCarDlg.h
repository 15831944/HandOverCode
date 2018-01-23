#if !defined(AFX_EXAMCARDLG_H__92DB6E8D_CB73_4F7B_9B54_E10E2A9AF27A__INCLUDED_)
#define AFX_EXAMCARDLG_H__92DB6E8D_CB73_4F7B_9B54_E10E2A9AF27A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define MAX_ExamCar 20
// ExamCarDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExamCarDlg dialog
//������ �����ڶԻ�������ʹ�õ�
class CExamCarDlg : public CDialog
{
// Construction
public:
	void ChangeTrackVideoPlayS(HTREEITEM pVCurItem);
	void ChangeTrackVideoPlayT(HTREEITEM pVCurItem);
	BOOL CloseExamCarVideoS();
	BOOL CloseExamCarVideoT();
	BOOL OpenExamCarVideoS(UINT ExamCarNo, UINT VideoAdd);
	BOOL OpenExamCarVideoT(UINT ExamCarNo,UINT VideoAdd);
	void ChangeTrackVideoPlay(HTREEITEM pVCurItem);
	void WriteLog(CString szERR);
	BOOL CloseExamCarVideo();
	BOOL OpenExamCarVideo(UINT ExamCarNo,UINT VideoAdd);
	int iExamCarVideo; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������  ����ʻ��Ƶ
	int iExamCarVideot; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������  ����ʻ��Ƶ
	int iExamCarVideos; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������  ������Ƶ
	BOOL ArryExamCar[MAX_ExamCar]; //���߿��� 1 ���� 0 ������
	int CurTrackExamCar;  //��ǰ�����ٵĿ�����
	int ArryExamCarPopbox[MAX_ExamCar];  //������Ŵ�ʱ��������ʾ����û�п��� ��������� Ϊ 1 ���û�п�����Ϊ0
	void ChangeTrack(HTREEITEM pVCurItem);
	CExamCarDlg(CWnd* pParent = NULL);   // standard constructor
	HTREEITEM m_hDevItem;
// 	HTREEITEM m_FirstDevItem; //�洢��һ��������ָ��
// 	int iExamCarCount;  //���浱ǰ���ߵĿ�����
	CImageList *m_imagelist;
	BOOL InitMytree();
// Dialog Data
	//{{AFX_DATA(CExamCarDlg)
	enum { IDD = IDD_DIALOG_EXAM };
	CTreeCtrl	m_exam;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExamCarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExamCarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeChan(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXAMCARDLG_H__92DB6E8D_CB73_4F7B_9B54_E10E2A9AF27A__INCLUDED_)
