// CBKDigitalMapDlg.h : header file
//

#if !defined(AFX_CBKDIGITALMAPDLG_H__4B6522BF_C2F2_4FC0_9F70_6AC7D4C06270__INCLUDED_)
#define AFX_CBKDIGITALMAPDLG_H__4B6522BF_C2F2_4FC0_9F70_6AC7D4C06270__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Buttonz.h"
#include "DD.h"
#include "ExamCarInfoDlg.h"
//#include <afxmt.h>


/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapDlg dialog


#define GPS_CarTimer 1
#define GPS_Dlg 2
#define SubDlgForInfoWidth 350
#define SubDlgForInfoHeight 150


class CCBKDigitalMapDlg : public CDialog
{
// Construction
public:
	void DeleteLogData();  //ɾ��֮ǰ�ļ�¼�ļ�
	BOOL ReadOnlineCarFromDB2();
	BOOL ReadOnlineCarFromDB1();
	void SendMesageToVideoDlgCloseS();
	void SendMesageToVideoDlgCloseT();
	void SendMessageToVideoDlgS(UINT ExamCar,UINT VideoAdd);
	void SendMessageToVideoDlgT(UINT ExamCar,UINT VideoAdd);
	BOOL OnButtonopen(); 
	BOOL OnButtonopenS();
	BOOL OnButtonopenT();
	BOOL CreateVideoDlg();
	BOOL CreateVideoDlgT();
	BOOL CreateVideoDlgS();
	void SendMesageToVideoDlgClose();
	void SendMessageToVideoDlg(UINT ExamCar,UINT VideoAdd);  // VideoAdd�ǿ����ϵ���·��Ƶ 1 ������ʻ 2 �Ǹ���ʻ 3 �ǳ���
	HWND GetServerMainWnd();
	HWND GetServerMainWndT();
	HWND GetServerMainWndS();
	int iISCreate; //0��ʾ��û�д�������ֽ��� 1��ʾ�Ѿ�����
	int iISCreateT; //0��ʾ��û�д�������ֽ��� 1��ʾ�Ѿ�����
	int iISCreateS; //0��ʾ��û�д�������ֽ��� 1��ʾ�Ѿ�����
	HANDLE m_hProcess;  //�����ӽ��̵� Ҳ�����Ǵ���Ƶ�Ľ���
	HANDLE m_hProcessT;  //�����ӽ��̵� Ҳ�����Ǵ���Ƶ�Ľ���
	HANDLE m_hProcessS;  //�����ӽ��̵� Ҳ�����Ǵ���Ƶ�Ľ���
	BOOL HasPlayInvalite();  //��������Ĺ����������������Ƶ���ŵĻ�ˢ�µ�����Ҫ��ȥ��Ƶ��һ����
	int iStateDlgShow;  //��һ����־λ�������Ի����쳤���ջ�ȥ��ʱ���õ�  ���Ϊ1 ˵�������쳤���ջ�ȥ Ϊ0��������״̬
	int bMouseOn[MAX_ExamCar];  //����������ж������û��������������  TRUEΪ���⿼���� FALSE û����������������
	BOOL ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc);
	CRect rt;
	int iStateLBD;//����������������� �϶�ʱΪ1 ����״̬��Ϊ0
	CDC m_Cbuf;
	CDC m_Cmem;
	CDC m_CbufDlg;
	CBitmap pBitmap;
	CBitmap pBitmapDlg;
	CBitmap pBG;
//	CBitmap pBG1;
	///////////////////
	BOOL ReadOnlineCarFromDB();//��ȡ���߿�����
	void WriteLog(CString szERR,CString car);
	CCriticalSection  pMutexForPG;//���ڱ�֤����ͽ��հ�����
	int iContrl;//�������Ƶ����������ʱ���ͷ��߳� ��ʼ��Ϊ 1 ������ʱ��Ϊ0 ����������������
	CCriticalSection  pMutex; //������ ���ڿ���GPS����ʾ�Ļ���
	CMutex Mutex_Self[MAX_ExamCar];//���ڿ����Լ������ã���Ҫ�����ڿ���������
	int iCountFlushMap;//����ͳ�Ƶ�ͼˢ�µĴ��� ��Ϊ0��ʱ��ſ���ˢ�°�ť 
	int iCountFlushExamCar;//����ͳ�ƿ���ˢ�µĴ��� ��Ϊ0��ʱ��ſ���ˢ�µ�ͼ
	int HadWakeUp;//��ʾ�߳��Ѿ�������  0 ��ʾ�����߳��Ѿ��������� 1 �����߳���Ҫ����
	void WakeUp_Thread();  //�����߳�
	void Suspend_Thread(); //�����߳�
	int iNowZoomRunning;//���ڷŴ���С��ˢ��  1 ����ˢ�� 0 û����ˢ��
	HANDLE ThreadHandle[MAX_ExamCar+1];
	void CreateDlgha();
	int isRunningUpdate;  //����ˢ�°�ť 1����ˢ�� 0 û����ˢ��
	//CExamCarInfoDlg ��Ҫ�õĳ�Ա�����Ѿ�����
	CExamCarInfoDlg ExamCarInfo;
    // ����
	int y_CurLbuttonDown;  //��ǰ��갴�µ�x����  �����ƶ�������
	int x_CurLbuttonDown;  //��ǰ��갴�µ�y����  �����ƶ�������
	BOOL LButtonDown;  // �������϶������� ��ΪTRUEʱ����갴�¿��Կ�ʼ�϶����� FALSEʱ�������ͷŲ������϶�����
	int PixState;  //ԭͼ��־ ����Ѿ���С��ԭͼ״̬ ����0��ʾ ����Ŵ� ����1 ��ʾ
	int y_OriginalBit;  //ԭʼͼƬ�ĸ߶�
	int x_OriginalBit; //ԭʼͼƬ�Ŀ��
	int y_BitWidth;  //Ӧ��Ҫ���ص�ͼƬ�ĸ߶�
	int x_BitWidth;	//Ӧ��Ҫ���ص�ͼƬ�Ŀ��
	int y_StartBit; //��ʼ�ػ���ͼƬ����ʼ����y�� 
	int x_StartBit;//��ʼ�ػ���ͼƬ����ʼ����x��
	//�Ŵ���С����ѡ�� 1.2
	int ZoomLevel;  //�Ŵ���С�ȼ�  Ϊ 0 ʱ���Կ�ʼ�Ŵ� ������4ʱ���Ͳ����ٷŴ���  ÿ�Ŵ�һ�λ���Сһ�� ���ֵ�ͼ�һ���һ
	// ZoomLevelΪ-20 ��ʱ��˵���Ѿ�����С�˲�������С��
	int ZoomLevel1; //������������洢��Сʱ����ʱ����С����
	int y_CurScr;  //����������м们���ǣ���ǰ����x����ֵ
	int x_CurScr;  //����������м们���ǣ���ǰ����y����ֵ
//	int iNowTrack;//���ڷŴ���С��ˢ��  1 ����ˢ�� 0 û����ˢ��
	
//	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CDD dlgha[MAX_ExamCar];
	void WriteGPSPoint(ExamCar_ST *pExamCar_ST);
	DOUBLE MAPBotom; //��ͼ�ĵײ�GPSֵ
	DOUBLE MAPTop;  //��ͼ�Ķ���GPSֵ
	DOUBLE MAPLeft;  //��ͼ�����GPSֵ
	DOUBLE MAPRight;  //��ͼ���ұ�GPSֵ

	DOUBLE OriginalMAPBotom; //��ͼ�ĵײ�GPSֵ ԭʼ������ͼƬ��GPSֵ
	DOUBLE OriginalMAPTop;  //��ͼ�Ķ���GPSֵ
	DOUBLE OriginalMAPLeft;  //��ͼ�����GPSֵ
	DOUBLE OriginalMAPRight;  //��ͼ���ұ�GPSֵ
	void WriteLog(CString szERR);
	CCBKDigitalMapDlg(CWnd* pParent = NULL);	// standard constructor
	DOUBLE x_pixel;  //��Ӧ��Ļһ�����ش������γ��
	DOUBLE y_pixel;  //��Ӧ��Ļһ�����ش�����پ���
	DOUBLE x_Originalpixel;  //��Ӧ��Ļһ�����ش������γ��  ԭʼ��x
	DOUBLE y_Originalpixel;  //��Ӧ��Ļһ�����ش�����پ���  ԭʼ��y
	ExamCar_ST on_ExamCar[MAX_ExamCar]; //��ǰ����GPS��������Ϣ

	UINT CurrentScrHeight;  //��ǰ��Ļ�߶�
	UINT CurrentScrWidth;  //��ǰ��Ļ���
	CSOCKET pSocket;//����Socket����
	BOOL ExamCarState[MAX_ExamCar]; //����״̬ TRUE ��ʾ��ʼ���� FALSE ��ʾ���Խ��� �����Ҫ���ڴ�Ÿ�������GPS�� �������50������

	BOOL bExamCarState[MAX_ExamCar];  //�������ڽ������ľֲ�����

	map<int,UINT> SockToCar1; //����������ߵĿ����Ŷ�Ӧ�����߳��е�λ��
	// Dialog Data
	//{{AFX_DATA(CCBKDigitalMapDlg)
	enum { IDD = IDD_CBKDIGITALMAP_DIALOG };
	CStatic	m_TotalCar;
	CStatic	m_boot;
	CButtonz	m_Button[MAX_ExamCar];
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCBKDigitalMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CFont m_Font;  

	// Generated message map functions
	//{{AFX_MSG(CCBKDigitalMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnBtnMsgz(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnBtnMsgzLeft(WPARAM wParam,LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);  //���ؼ��ı�����Ϊ͸��

	afx_msg LRESULT OnUpdataCurCar(WPARAM wParam,LPARAM lParam); //������Ϣ���ڸ��µ�ǰ������
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt); //��������м�Ļ��� ����

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
//	afx_msg void OnButtonopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CBitmap m_bmpMap;
	CDC	 m_dcMem;
};


//�����ﹹ��һ���ṹ�� ��ԱΪ CCBKDigitalMapDlg ��ָ���Լ� ��Ӧ������
struct ExamCarInfoToThread
{
	CCBKDigitalMapDlg *pMapDlg; //���Ի����ָ��
	int iExamCarNumber; //������
};
typedef struct ExamCarInfoToThread * ExamCarInfoToThread_ST;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CBKDIGITALMAPDLG_H__4B6522BF_C2F2_4FC0_9F70_6AC7D4C06270__INCLUDED_)
