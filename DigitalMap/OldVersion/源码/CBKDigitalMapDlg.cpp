// CBKDigitalMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBKDigitalMap.h"
#include "CBKDigitalMapDlg.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern CCBKDigitalMapApp theApp;
extern CPoint temPoint;  //��ǰ�Ի����ƶ�����λ��

//����ʻ��Ƶ

BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam);
unsigned long g_ulProcessId;
//����ʻ��Ƶ

BOOL CALLBACK EnumWndProcT(HWND hWnd, LPARAM lParam);
unsigned long g_ulProcessIdT;
//������Ƶ

BOOL CALLBACK EnumWndProcS(HWND hWnd, LPARAM lParam);
unsigned long g_ulProcessIdS;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

UINT ParsePackage_thd(LPVOID p);
UINT SendCurCarMsg_thd(LPVOID p);  //���ڷ�����Ϣ�õ� ��ǰ���߿�����
UINT ExamCarGPSLocate_thd(LPVOID p); //���ڶ�Ӧ���ȳ˿ʹ����Ӧ��ʵʱ����GPS��Ϣ
UINT WakeUpExamCar_thd(LPVOID p);// ����һ���߳�ר���������ѿ����߳�
UINT HideExamCar_thd(LPVOID p);// ����һ���߳�ר���������ؿ��� �������Ҫ����Ϊ��������Ѿ������ˣ�
									//������;���ߣ����������������һЩ����û�г����꣬���Ծ�����ʾ�����ˣ��������߳̽����رյ�

UINT HideOtherCar_thd(LPVOID p);//��һ���߳���������п��������ٵ�ʱ�򣬱�Ŀ���������

UINT UpdateMap_thd(LPVOID p);//ר����һ���̶߳�ʱ��ˢ�¿����͵�ͼ

UINT ButtonUp_thd(LPVOID p);//ר��������������ƶ�����ı�־λ

UINT ListenExamCar_thd(LPVOID p);//ר�������������߿�����  ��Ҫ�����ڿ��������õ� �����Զ������Ѿ���socket���ﴦ����
UINT ListenExamCarOnline_thd(LPVOID p);//ר�������������߿�����  ��Ҫ�����ڿ��������õ� �����Զ������Ѿ���socket���ﴦ����

UINT ExamCarHideDown_thd(LPVOID p);//ר���������ÿ������ߵ�ʱ�����ؿ���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
//zsz
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapDlg dialog

CCBKDigitalMapDlg::CCBKDigitalMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCBKDigitalMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCBKDigitalMapDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCBKDigitalMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	int i = 0;
	//{{AFX_DATA_MAP(CCBKDigitalMapDlg)
	DDX_Control(pDX, IDC_STATIC_TOTALCAR, m_TotalCar);
	DDX_Control(pDX, IDC_STATIC_BOOT, m_boot);

	DDX_Control(pDX, IDC_BUTTON0, m_Button[0]);
	DDX_Control(pDX, IDC_BUTTON1, m_Button[1]);
	DDX_Control(pDX, IDC_BUTTON2, m_Button[2]);
	DDX_Control(pDX, IDC_BUTTON3, m_Button[3]);
	DDX_Control(pDX, IDC_BUTTON4, m_Button[4]);
	DDX_Control(pDX, IDC_BUTTON5, m_Button[5]);
	DDX_Control(pDX, IDC_BUTTON6, m_Button[6]);
	DDX_Control(pDX, IDC_BUTTON7, m_Button[7]);
	DDX_Control(pDX, IDC_BUTTON8, m_Button[8]);
	DDX_Control(pDX, IDC_BUTTON9, m_Button[9]);

	DDX_Control(pDX, IDC_BUTTON10, m_Button[10]);
	DDX_Control(pDX, IDC_BUTTON11, m_Button[11]);
	DDX_Control(pDX, IDC_BUTTON12, m_Button[12]);
	DDX_Control(pDX, IDC_BUTTON13, m_Button[13]);
	DDX_Control(pDX, IDC_BUTTON14, m_Button[14]);
	DDX_Control(pDX, IDC_BUTTON15, m_Button[15]);
	DDX_Control(pDX, IDC_BUTTON16, m_Button[16]);
	DDX_Control(pDX, IDC_BUTTON17, m_Button[17]);
	DDX_Control(pDX, IDC_BUTTON18, m_Button[18]);
	DDX_Control(pDX, IDC_BUTTON19, m_Button[19]);
	DDX_Control(pDX, IDC_BUTTON20, m_Button[20]);
 
 	DDX_Control(pDX, IDC_BUTTON21, m_Button[21]);
 	DDX_Control(pDX, IDC_BUTTON22, m_Button[22]);
 	DDX_Control(pDX, IDC_BUTTON23, m_Button[23]);
 	DDX_Control(pDX, IDC_BUTTON24, m_Button[24]);
 	DDX_Control(pDX, IDC_BUTTON25, m_Button[25]);
 	DDX_Control(pDX, IDC_BUTTON26, m_Button[26]);
 	DDX_Control(pDX, IDC_BUTTON27, m_Button[27]);
 	DDX_Control(pDX, IDC_BUTTON28, m_Button[28]);
 	DDX_Control(pDX, IDC_BUTTON29, m_Button[29]);
 	DDX_Control(pDX, IDC_BUTTON30, m_Button[30]);
 
 	DDX_Control(pDX, IDC_BUTTON31, m_Button[31]);
 	DDX_Control(pDX, IDC_BUTTON32, m_Button[32]);
 	DDX_Control(pDX, IDC_BUTTON33, m_Button[33]);
 	DDX_Control(pDX, IDC_BUTTON34, m_Button[34]);
 	DDX_Control(pDX, IDC_BUTTON35, m_Button[35]);
 	DDX_Control(pDX, IDC_BUTTON36, m_Button[36]);
 	DDX_Control(pDX, IDC_BUTTON37, m_Button[37]);
 	DDX_Control(pDX, IDC_BUTTON38, m_Button[38]);
 	DDX_Control(pDX, IDC_BUTTON39, m_Button[39]);
 
 	DDX_Control(pDX, IDC_BUTTON40, m_Button[40]);
 	DDX_Control(pDX, IDC_BUTTON41, m_Button[41]);
 	DDX_Control(pDX, IDC_BUTTON42, m_Button[42]);
 	DDX_Control(pDX, IDC_BUTTON43, m_Button[43]);
 	DDX_Control(pDX, IDC_BUTTON44, m_Button[44]);
 	DDX_Control(pDX, IDC_BUTTON45, m_Button[45]);
 	DDX_Control(pDX, IDC_BUTTON46, m_Button[46]);
 	DDX_Control(pDX, IDC_BUTTON47, m_Button[47]);
 	DDX_Control(pDX, IDC_BUTTON48, m_Button[48]);
 	DDX_Control(pDX, IDC_BUTTON49, m_Button[49]);

	DDX_Control(pDX, IDC_BUTTON50, m_Button[50]);
	DDX_Control(pDX, IDC_BUTTON51, m_Button[51]);
	DDX_Control(pDX, IDC_BUTTON52, m_Button[52]);
	DDX_Control(pDX, IDC_BUTTON53, m_Button[53]);
	DDX_Control(pDX, IDC_BUTTON54, m_Button[54]);
	DDX_Control(pDX, IDC_BUTTON55, m_Button[55]);
	DDX_Control(pDX, IDC_BUTTON56, m_Button[56]);
	DDX_Control(pDX, IDC_BUTTON57, m_Button[57]);
	DDX_Control(pDX, IDC_BUTTON58, m_Button[58]);
 	DDX_Control(pDX, IDC_BUTTON59, m_Button[59]);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCBKDigitalMapDlg, CDialog)
	//{{AFX_MSG_MAP(CCBKDigitalMapDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_MESSAGE(WM_BTN_MSGZ,OnBtnMsgz)  
	ON_MESSAGE(WM_BTN_MSGZLeft,OnBtnMsgzLeft)
	ON_WM_CTLCOLOR()  //���ؼ��ı�����Ϊ͸��
	ON_MESSAGE(WM_BTN_MSG_CAR,OnUpdataCurCar)  
	ON_WM_MOUSEWHEEL() //����м�Ļ���
	ON_WM_LBUTTONDOWN() //����������
	ON_WM_LBUTTONUP()  // ����������
	ON_WM_MOUSEMOVE() //����ƶ�
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCBKDigitalMapDlg message handlers

BOOL CCBKDigitalMapDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	DeleteLogData();
	
	ShowWindow(SW_SHOWMAXIMIZED);// ȫ��

	int Pi;
	for(Pi = 0;Pi < MAX_ExamCar;Pi++)
	{
		bMouseOn[Pi] = 0;  //����������ж������û��������������  TRUEΪ���⿼���� FALSE û����������������
	}
	
	iISCreate = 0; //0��ʾ��û�д�������ֽ��� 1��ʾ�Ѿ�����
	iISCreateT = 0; //0��ʾ��û�д�������ֽ��� 1��ʾ�Ѿ�����
	iISCreateS = 0; //0��ʾ��û�д�������ֽ��� 1��ʾ�Ѿ�����
	iStateDlgShow = 0;  //��һ����־λ�������Ի����쳤���ջ�ȥ��ʱ���õ�  ���Ϊ1 ˵�������쳤���ջ�ȥ Ϊ0��������״̬
	isRunningUpdate = 0;
	iStateLBD = 0;//����������������� �϶�ʱΪ1 ����״̬��Ϊ0

	//����Static����  
    m_Font.CreatePointFont(200, "Arial", NULL);  
    m_TotalCar.SetFont(&m_Font,true);  
//	m_TotalCar.MoveWindow(100,100,400,200);
	ReadOnlineCarFromDB();//��ǰ������
	CString tmpOnLineCar;
	tmpOnLineCar.Format("��ǰ������Ϊ��%d",pSocket.TotalCar);
    m_TotalCar.SetWindowText(tmpOnLineCar);  

	
	m_bmpMap.m_hObject = LoadImage(NULL,".\\GNSSmap.dat",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	pBitmapDlg.m_hObject = LoadImage(NULL,".\\boxtip2.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (m_bmpMap.m_hObject==NULL)
	{
		AfxMessageBox("���ص�ͼʧ��!"); 
		TerminateProcess(GetCurrentProcess(),0); 
		return FALSE;
	}
	
	CDC *pDC = this->GetDC();   //����CWnd�ͻ������豸����
	CRect ClientRect; //�ͻ�������
	GetClientRect(ClientRect);

	
	////////////     ��ǰ������ƵʵʱԤ��    /////////////////////////
	
	g_ulProcessId = 0UL;
	g_ulProcessIdT = 0UL;
	g_ulProcessIdS = 0UL;
	


	////////////                 ����        /////////////////////////

	///////////////////////////
	//  IDB_BITMAP1
	pBitmap.m_hObject = LoadImage(NULL,".\\TT.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE);

	m_Cmem.CreateCompatibleDC(pDC); // ����һ�����豸���ݵ��ڴ�
	m_Cbuf.CreateCompatibleDC(pDC);
	m_CbufDlg.CreateCompatibleDC(pDC);
	pBG.CreateCompatibleBitmap(pDC,ClientRect.Width(),ClientRect.Height()); //�ú���������ָ���豸���ݵĵ�λͼ
	m_Cmem.SelectObject(pBG);
	
	////////////////////////////


	//��ȡ�����ļ��е�MAPCONFIGSTATE ��״̬ �Ƿ�Ϊ1 ���������������һ������
	CString State;
	GetPrivateProfileString("MAPCONFIGSTATE","State","",State.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	if (atoi(State) != 1)  //���������������һ������
	{
		//ShellExecute(NULL,"open",".\\CBKTestVideoPlayBack.exe","playz save",NULL,SW_SHOW); 
		ShellExecute(NULL,"open",".\\ReadConfig.exe","",NULL,SW_SHOW); 
		CCBKDigitalMapDlg::OnCancel();
	}

	
	//��ȡ�����ļ�������� ���ϵ� �� ���µĵ�
	CString left,right,top,bottom;
	GetPrivateProfileString("Point","Left","",left.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	GetPrivateProfileString("Point","Right","",right.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	GetPrivateProfileString("Point","Top","",top.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	GetPrivateProfileString("Point","Bottom","",bottom.GetBuffer(MAX_PATH),MAX_PATH,".\\DATA.dll");
	MAPRight = (atof)(right);
	MAPLeft = (atof)(left);
	MAPTop = (atof)(top);
	MAPBotom = (atof)(bottom);

	OriginalMAPBotom = MAPBotom;//����ԭʼGPSֵ 
	OriginalMAPLeft = MAPLeft;//����ԭʼGPSֵ 
	OriginalMAPTop = MAPTop;//����ԭʼGPSֵ 
	OriginalMAPRight = MAPRight;//����ԭʼGPSֵ 

	x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
	y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��
	CurrentScrHeight = ClientRect.bottom - ClientRect.top;  //��ǰ��Ļ�߶�
	CurrentScrWidth = ClientRect.right - ClientRect.left;  //��ǰ��Ļ���

	x_Originalpixel = x_pixel;// ԭʼ��x
	y_Originalpixel = y_pixel;// ԭʼ��y

	
	
	BITMAP bitmap; 
	CBitmap	m_bmpTmp;
	m_dcMem.CreateCompatibleDC(pDC);  // CDC	 m_dcMem;   ������pDCָ���豸���ݵ��豸�������ڴ�
	m_bmpMap.GetBitmap(&bitmap);  // ���������ڲ鿴Cbitmap�������Ϣ�����ص���Ϣ�����bitmapָ���BITMAP�ṹ��
	y_BitWidth = bitmap.bmHeight; //��ȡͼƬ�ĸ߶�
	x_BitWidth = bitmap.bmWidth;  //��ȡͼ�̵Ŀ��
	y_OriginalBit = bitmap.bmHeight; //��ȡͼƬ�ĸ߶�  ����ԭ��ͼƬ�Ĵ�С
	x_OriginalBit = bitmap.bmWidth; //��ȡͼƬ�Ŀ��  ����ԭ��ͼƬ�Ĵ�С
	x_BitWidth = ClientRect.Width();
	y_BitWidth = ClientRect.Height();
	m_bmpTmp.CreateCompatibleBitmap(pDC,bitmap.bmWidth,bitmap.bmHeight);  //��ʼ��һ����pDCָ�����豸�����ļ��ݵ�λͼ
	m_dcMem.SelectObject(m_bmpTmp);  // Ӧ��ֻ�ܽ�λͼѡ���ڴ��豸��������
	this->ReleaseDC(pDC);  // �ͷ��豸�������Թ�����Ӧ�ó���ʹ��
	m_dcMem.SelectObject(m_bmpMap);

	//���㵱ǰ��һ�����ش�����پ�γ��
	x_pixel = ((MAPRight-MAPLeft) / x_OriginalBit);
	y_pixel = ((MAPTop-MAPBotom) / y_OriginalBit);
	MAPRight = MAPLeft + x_pixel * ClientRect.Width();
	MAPBotom = MAPTop - y_pixel * ClientRect.Height();

	//��ʼ��50����Ŀ���Ŀ���  ��ʼ��ת��Ϊ��û�п�ʼ����
	int i = 0;
	for (i = 0;i < MAX_ExamCar; i ++)
	{
		ExamCarState[i] = FALSE;
	}

	//�ƶ���ʾ��--����������ʾ
	m_boot.SetFont(&m_Font,true);  
	m_boot.MoveWindow(CurrentScrWidth / 2 - 150,CurrentScrHeight / 2 - 25,300,50);
	CString bootText;
	bootText.Format("���������У����Ե�...");
	m_boot.SetWindowText(bootText);
	///////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////


	//��ʼ�� ������ǰGPS�����Ϣ
	memset(on_ExamCar,0,sizeof(ExamCar_ST) * MAX_ExamCar);

	for (i = 0;i < MAX_ExamCar;i++)
	{
		m_Button[i].LoadBitmap(IDB_BMP_BTNBK);//���ذ�ťͼƬ
		m_Button[i].MoveWindow(-20,-20,20,20);
		m_Button[i].ShowWindow(SW_HIDE);
		m_Button[i].examCarNumber = 0; //�������Ŵ���ȥ  �Ѿ������������ŵ������ReadOnlineCarFromDB��������
	}
	for(i = 1;i <= pSocket.TotalCar;i++)
	{
		m_Button[i].examCarNumber = SockToCar1.find(i)->second;
		m_Button[i].ThreadNumber = i;
	}

	iNowZoomRunning = 0;//���ڷŴ���Сˢ�� 1 ����ˢ�� 0 û����ˢ��
	ZoomLevel = 20;// �Ŵ�ȼ�����
	ZoomLevel1 = 0;
	x_StartBit = 0;//��ʼ�ػ���ͼƬ����ʼ����x��
	y_StartBit = 0;//��ʼ�ػ���ͼƬ����ʼ����y��
	PixState = 0;  //ԭͼ��־ ����Ѿ���С��ԭͼ״̬ ����0��ʾ ����Ŵ� ����1 ��ʾ

	LButtonDown = FALSE; //�������϶������� ��ΪTRUEʱ����갴�¿��Կ�ʼ�϶����� FALSEʱ�������ͷŲ������϶�����
	HadWakeUp = 0;//��ʾ�߳��Ѿ�������  0 ��ʾ�����߳��Ѿ��������� 1 �����߳���Ҫ����
	iCountFlushMap = 0;//����ͳ�Ƶ�ͼˢ�µĴ��� ��Ϊ0��ʱ��ſ���ˢ�°�ť 
	iCountFlushExamCar = 0;//����ͳ�ƿ���ˢ�µĴ��� ��Ϊ0��ʱ��ſ���ˢ�µ�ͼ��

	iContrl = 1;

//	m_TotalCar.MoveWindow(10,10,200,60);
//	CString title;
//	title.Format("��������Ϊ��0");
//	m_TotalCar.SetCaption(title);
//	m_TotalCar.SetBkMode(TRANSPARENT);// ����͸��


	SetTimer(GPS_Dlg,100,NULL);
	
	/*
	for (i = 0;i < MAX_ExamCar;i++)
	{
		dlgha[i].Create(IDD_DIALOG1,this);
		dlgha[i].ShowWindow(SW_HIDE);
	}
	*/


	//�������߿����ĶԻ��� 
	ExamCarInfo.Create(IDD_DIALOG_ONLINECARINFO,this);
	ExamCarInfo.ShowWindow(SW_SHOW);
	ExamCarInfo.MoveWindow(700,-637,280,640);
	

	// ���߳� �������������������Ϣ
//	CWinThread *tmpCwinThread;
	AfxBeginThread(&ParsePackage_thd, this);

//	AfxBeginThread(&SendCurCarMsg_thd, this);  //ˢ�ɵ�ǰ�����߿�����  ���ڲ����� ��Ϊһ��ʼ�Ͷ�ȡ�˵�ǰ���߿�����
	AfxBeginThread(&WakeUpExamCar_thd, this); // ר���������ѿ����߳�
	AfxBeginThread(&HideExamCar_thd, this); // ����һ���߳�ר���������ؿ��� �������Ҫ����Ϊ��������Ѿ������ˣ�
											//������;���ߣ����������������һЩ����û�г����꣬���Ծ�����ʾ�����ˣ��������߳̽����رյ�

	
	AfxBeginThread(&HideOtherCar_thd, this); //�����п���������ʱ����Ŀ�������
	AfxBeginThread(&UpdateMap_thd, this); //ר����һ���߳���ˢ�µ�ͼ�Ϳ���

	AfxBeginThread(&ListenExamCar_thd, this);
	AfxBeginThread(&ListenExamCarOnline_thd, this);
	 
	
//	SuspendThread(PP); //�����߳�
//	ResumeThread(PP);  //�����߳�

/********************************************************
��������Ǵ��������߳�
***********************************************************/

/*
	ExamCarInfoToThread_ST pExamCarInfoToThread_ST;
	pExamCarInfoToThread_ST = (ExamCarInfoToThread_ST)malloc(sizeof(struct ExamCarInfoToThread));
	

	for (i = 0;i < MAX_ExamCar;i++)
	{
		memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
		pExamCarInfoToThread_ST->pMapDlg = this;
		pExamCarInfoToThread_ST->iExamCarNumber = i;
		tmpCwinThread = AfxBeginThread(&ExamCarGPSLocate_thd, pExamCarInfoToThread_ST);
		ThreadHandle[i] = tmpCwinThread->m_hThread;
		if (i == MAX_ExamCar -1)
		{
			memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
			pExamCarInfoToThread_ST->pMapDlg = this;
			pExamCarInfoToThread_ST->iExamCarNumber = 0; //�����ĵ�һ���߳� ����ΪʲôҪ��ô����Ҳ��֪�� 
		}
		Sleep(500);
		SuspendThread(ThreadHandle[i]);  //�����߳�
	}
	
	free(pExamCarInfoToThread_ST);

*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCBKDigitalMapDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCBKDigitalMapDlg::OnPaint() 
{
	CPaintDC dc(this);
	m_TotalCar.MoveWindow(10,10,240,50);

	SetStretchBltMode(dc.m_hDC,STRETCH_HALFTONE);

	CFont pFont;
	pFont.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");                 // lpszFacename

//	pFont.CreatePointFont(100, "Arial", NULL);  
	

	if (IsIconic())
	{
		 // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
//		CPaintDC dc(this); 
		CRect rect; 
//		m_dcMem.SelectObject(m_bmpMap);

		BITMAP bitmap; 
		m_bmpMap.GetBitmap(&bitmap);  // ���������ڲ鿴Cbitmap�������Ϣ�����ص���Ϣ�����bitmapָ���BITMAP�ṹ��
		GetClientRect(&rect); 
	//	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,0,0,bitmap.bmWidth,bitmap.bmHeight,SRCCOPY); 
		// 1��Ҫ������λͼ��λ�ò�����ֱ�Ӷ�λ 0,0,bitmap.bmWidth,bitmap.bmHeight  
		//    Ӧ��ͨ����������ȥ
		// 2�����Ƶ�ͼƬ�Ŀ��Ҳ����ֱ�Ӵ���ȥ��Ӧ��ͨ������ֱ�Ӵ���ȥ

		/////////////////////////////////////////////////
		/*
		m_Cbuf.SelectObject(pBG1);
		m_Cmem.BitBlt(0,0,1000,800,&m_Cbuf,0,0,SRCCOPY);
		m_Cbuf.SelectObject(pBitmap);
	
		m_Cmem.BitBlt(m_x,m_y,26,26,&m_Cbuf,26,0,SRCAND);  // ���������� 
		m_Cmem.BitBlt(m_x,m_y,26,26,&m_Cbuf,0,0,SRCPAINT);//���л�����
		*/
	//	m_Cbuf.SelectObject(pBG1);

		SetStretchBltMode(m_Cmem.m_hDC,STRETCH_HALFTONE);//�ú�����������ָ���豸�����е�λͼ����ģʽ
		m_Cmem.StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,x_StartBit,y_StartBit,x_BitWidth,y_BitWidth,SRCCOPY); 
		m_Cbuf.SelectObject(pBitmap);

	
//zzz
 		int Pi = 0;
		if(iStateLBD == 0)
		{
			
			for(Pi = 0; Pi < MAX_ExamCar;Pi++)
			{
				// int((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel),
				// int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel) - 10)
				if(m_Button[Pi].CurCar_Xpoint > 0 && m_Button[Pi].CurCar_Ypoint > 0 )
				{
					CRect RC1(0,0,20,20);
					CString szCarNumber;
					szCarNumber.Format("%d",m_Button[Pi].examCarNumber);
					
					
					if(ExamCarInfo.ExamCarDlg.CurTrackExamCar == Pi)
					{
						RC1.SetRect(20,0,40,20);
// 						m_Cbuf.SelectObject(pBitmap);
						m_Cbuf.DrawText("   ",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						m_Cbuf.DrawText(szCarNumber,RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						//m_Cbuf.DrawText("abc",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,20,SRCAND);  // ���������� 
						m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,20,0,SRCPAINT);//���л�����
						if (m_Button[Pi].CurCar_EID !=1000)
						{
							//playvoice here
							m_Button[Pi].CurCar_EID=1000;
						}
					}
					else
					{	
						
// 						m_Cbuf.SelectObject(pBitmap);
						m_Cbuf.DrawText("    ",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						m_Cbuf.DrawText(szCarNumber,RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						//m_Cbuf.DrawText("dd",RC1,DT_CENTER | DT_SINGLELINE | DT_VCENTER  );
						if (m_Button[Pi].CurCar_ZT ==1)//״̬=1
						{							
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,20,SRCAND);  // ���������� 
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,0,SRCPAINT);//���л�����
						}
						else
						{							
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,0,20,SRCAND);  // ���������� 
							m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint-10,m_Button[Pi].CurCar_Ypoint,20,20,&m_Cbuf,40,0,SRCPAINT);//���л�����
						}
						//////////////////////////////////////////////////////////////////////////zzzzz

					}

				}
			}
		}
		/////////////////////////////////////////////////

		//////////////////////////////////////////////////
		m_Cbuf.SelectObject(pBitmapDlg);
		CBitmap bmpBackground; 
		bmpBackground.LoadBitmap(IDB_BMPPHOBK); 
		BITMAP bitmap2; 
		bmpBackground.GetBitmap(&bitmap2);
		CBitmap *pbmpOld=m_CbufDlg.SelectObject(&bmpBackground);
		CRect rt(0,0,bitmap2.bmWidth,bitmap2.bmHeight);
	//	ReadPhotoFromDB(&m_CbufDlg,"520100713081",&rt);
	
		CFont* def_font = m_Cbuf.SelectObject(&pFont);
		
	//	m_Cbuf.BitBlt(240,15,bitmap.bmWidth,bitmap.bmHeight,&m_CbufDlg,0,0,SRCCOPY); //����ǿ��Ե�
	//	m_Cbuf.BitBlt(0,0,350,150,&m_CbufDlg,0,0,SRCCOPY);  //����ǿ��Ե�



		for(Pi = 1; Pi < MAX_ExamCar;Pi++)
		{
			// int((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel),
			// int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel) - 10)
			if(  m_Button[Pi].CurCar_Xpoint > 0 && m_Button[Pi].CurCar_Ypoint > 0 && (m_Button[Pi].m_bHover == 1 || bMouseOn[Pi] == 1))
			{
				//////  ��ȡ��Ϣ���һЩ��Ϣ  ////
				::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSGZ,m_Button[Pi].examCarNumber,m_Button[Pi].ThreadNumber);
				//////                        ////
				m_CbufDlg.SelectObject(&bmpBackground);

				CRect RC(100,10,200,35);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].CSCAR,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				RC.SetRect(100,45,200,70);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].szExamineeName,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				RC.SetRect(100,80,200,105);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].strzkzm,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				RC.SetRect(100,115,200,140);
				m_Cbuf.DrawText("                            ",RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );
				m_Cbuf.DrawText(dlgha[m_Button[Pi].ThreadNumber].strCarModel,RC,DT_LEFT | DT_SINGLELINE | DT_VCENTER  );

				ReadPhotoFromDB(&m_CbufDlg,dlgha[m_Button[Pi].ThreadNumber].strzkzm,&rt); //���׼��֤Ҫ��ô��
				m_Cbuf.BitBlt(240,12,bitmap.bmWidth,bitmap.bmHeight,&m_CbufDlg,0,0,SRCCOPY);
			//	m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+20,m_Button[Pi].CurCar_Ypoint+20,350,150,&m_Cbuf,0,0,SRCCOPY);
				//////////////    ������Ҫ�ж����ĸ�����    ///////////////////
				//������Ҫ�ж�4��������� ��ͬ��������ز�ͬ�ı���ͼƬ
				CRect ClientRect; //�ͻ�������
				GetClientRect(ClientRect);
				if (m_Button[Pi].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
				{
					//�Ի���ļ�ͷ�������½�
					
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+10,m_Button[Pi].CurCar_Ypoint-160,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				else if (m_Button[Pi].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
				{	//�Ի���ļ�ͷ�������Ͻ�
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+10,m_Button[Pi].CurCar_Ypoint+10,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				else if (m_Button[Pi].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
				{	//�Ի���ļ�ͷ�������½�
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+20,m_Button[Pi].CurCar_Ypoint+10,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				else if (m_Button[Pi].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[Pi].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
				{	//�Ի���ļ�ͷ�������Ͻ�
					m_Cmem.BitBlt(m_Button[Pi].CurCar_Xpoint+10,m_Button[Pi].CurCar_Ypoint-160,350,150,&m_Cbuf,0,0,SRCCOPY);
				}
				////////////////////////////////////////////////////
			}
		}



		m_Cbuf.SelectObject(def_font);
		pFont.DeleteObject();
		//////////////////////////////////////////////////

		
	//	dc.StretchBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,x_StartBit,y_StartBit,x_BitWidth,y_BitWidth,SRCCOPY); 

		dc.BitBlt(0,0,rect.Width(),rect.Height(),&m_Cmem,0,0,SRCCOPY);

		
		iNowZoomRunning = 0;//û�зŴ����С ��仰������д������� ���ڸ�д��OnPaint����β�� ˵��ˢ������
		HadWakeUp = 1;//��ʾ�߳��Ѿ�������  0 ��ʾ�����߳��Ѿ��������� 1 �����߳���Ҫ����
		if (iCountFlushMap > 0)
		{
			iCountFlushMap--;//����ͳ�Ƶ�ͼˢ�µĴ��� ��Ϊ0��ʱ��ſ���ˢ�°�ť 
		}
		
//		WakeUp_Thread();  //����������̵߳Ĺ�����OnPaint�����β��ʵ��
// 		CString ff;
// 		ff.Format("%d %d %d %d",x_StartBit,y_StartBit,x_BitWidth,y_BitWidth);
// 		WriteLog(ff);
	
	//	return TRUE;

		//��ֹGDIй¶
		bmpBackground.DeleteObject();  //��ֹGDIй¶
		pbmpOld->DeleteObject();
		CDialog::OnPaint();
	}
	
}



// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCBKDigitalMapDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CCBKDigitalMapDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return CDialog::OnEraseBkgnd(pDC);
}

/**********************************8
����˵����д������Ϣ���������ļ�
************************************/
void CCBKDigitalMapDlg::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

/**********************************8
����˵������������Ӧ��GPS������ӵ���Ӧ���ļ�
����˵����pExamCar_ST ��Ҫд��Ŀ�����Ϣ�İ�
************************************/
void CCBKDigitalMapDlg::WriteGPSPoint(ExamCar_ST *pExamCar_ST)
{
	/*
	ExamCar_ST *ExamCar;
	ExamCar = (ExamCar_ST *) malloc (sizeof(ExamCar_ST));
	memset(ExamCar,0,sizeof(ExamCar_ST));
	memcpy(ExamCar,pExamCar_ST,sizeof(ExamCar_ST));

	if (ExamCarState[ExamCar->ExamCarNumber] == FALSE) //˵���ǿ�ʼ���һ�ο�ʼд��GPS��
	{
		CTime curTime =CTime::GetCurrentTime();
		CString Data=curTime.Format("����%d %Y%m%d%H%M%S",ExamCar->ExamCarNumber);
		FILE *fp=fopen(Data,"a+");

		fprintf(fp,"[%s]%s.\n",Data,szERR);
		fclose(fp);
	}
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
//	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
	*/
}

UINT ParsePackage_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	Package_ST *pPackage_ST = NULL;
	pPackage_ST = (Package_ST *) malloc (sizeof(Package_ST));
	memset(pPackage_ST,0,sizeof(Package_ST));

// 	CString ff1;
// 	ff1.Format("pExamCar_ST = %p, pPackage_ST = %p",pExamCar_ST,pPackage_ST);
// 	pCCBKDigitalMapDlg->WriteLog(ff1);


//	BOOL bExamCarState[MAX_ExamCar];  �����������������Ҫ�����ŵ�������ȥ
	CString OO;
	CString ert;
	int i = 0;
	for(i = 0;i < MAX_ExamCar;i++)
	{
		pCCBKDigitalMapDlg->bExamCarState[i] = FALSE;
	}
	int iCount_Package = 0;
	int iTmpj = 0;
	int iTmpc = 0;
	int iTmpDefault = 0;
	CString R1;
//	while (1)  
	while (pCCBKDigitalMapDlg->iContrl)
	{
		while (!pCCBKDigitalMapDlg->pSocket.pPackage_ST.empty() && pCCBKDigitalMapDlg->iContrl)
		{
		//	pCCBKDigitalMapDlg->WriteLog("��� ����  0");
			memset(pPackage_ST,0,sizeof(Package_ST));
			pCCBKDigitalMapDlg->pMutexForPG.Lock();
		//	while(pCCBKDigitalMapDlg->pSocket.dequeOK == FALSE)
		//		;
		//	Sleep(10);
		//	pCCBKDigitalMapDlg->WriteLog("��� ����");
		//	pCCBKDigitalMapDlg->pSocket.dequeOK = FALSE;
			
	//		Package_ST *pPackage_ST11 = NULL;
	//		pPackage_ST11 = (Package_ST *)pCCBKDigitalMapDlg->pSocket.pPackage_ST.front();  //ȡ���ȴ��ȥ��
			memcpy(pPackage_ST,pCCBKDigitalMapDlg->pSocket.pPackage_ST.front(),sizeof(Package_ST));
			Sleep(2);
/*
			if (pPackage_ST->pExamCar_ST.ExamCarNumber>38 || pPackage_ST->pExamCar_ST.ExamCarNumber <1)
			{
				Sleep(5);
				Sleep(5);
			}
*/
			pCCBKDigitalMapDlg->pSocket.pPackage_ST.pop_front();  //ɾ����һ��Ԫ��
			Sleep(2);
		//	pCCBKDigitalMapDlg->pSocket.dequeOK = TRUE;
			iCount_Package ++;
			pCCBKDigitalMapDlg->pMutexForPG.Unlock();
		//	pCCBKDigitalMapDlg->WriteLog("��� ��ȥ");
		//	CString CarNumber;
			switch(pPackage_ST->pPackageHead_ST.PackageType)
			{
			case 1: 
				//Ϊ���ؿ�������Ϣ ���������ź�GPS����
				// ��ȡGPS�ĵ㣬ͬʱ���㷢�͵���Ӧ�Ŀ��������´�ʱ��GPS���� ͬʱ���õ�д���Ӧ���ļ�
		//		memcpy(pExamCar_ST,pPackage_ST+8,sizeof(ExamCar_ST));
		//		pExamCar_ST = (ExamCar_ST *)(pPackage_ST + sizeof(PackageHead_ST));
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].ExamCarNumber = pPackage_ST->pExamCar_ST.ExamCarNumber;
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].x_Gps = pPackage_ST->pExamCar_ST.x_Gps;
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].y_Gps = pPackage_ST->pExamCar_ST.y_Gps;
				//////////////////////////////////////////////////////////////////////////
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].zt = pPackage_ST->pExamCar_ST.zt;
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].ErrorID = pPackage_ST->pExamCar_ST.ErrorID;
				//////////////////////////////////////////////////////////////////////////
				pCCBKDigitalMapDlg->on_ExamCar[pPackage_ST->pExamCar_ST.ThreadNumber].ThreadNumber = pPackage_ST->pExamCar_ST.ThreadNumber;
				//�����ʱGPSλ����Ļ������		
//zzz

				iTmpj++;

				// ExamCarState
				if (pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ThreadNumber] == FALSE)
				{
					pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ThreadNumber].ExamCarState = 1;//��������
			//		pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ThreadNumber].ShowWindow(SW_SHOW);
					// ������ʱ�� ����GPS��������ƶ�
					//		pCCBKDigitalMapDlg->SetTimer(GPS_CarTimer,10,NULL); ������ĳɻ����߳�
				//	ResumeThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ExamCarNumber]);
					while(ResumeThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ThreadNumber]));
					pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ThreadNumber] = TRUE;
				}
			//	pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ExamCarState = 1;//��������
			//	pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ShowWindow(SW_SHOW);
				// ������ʱ�� ����GPS��������ƶ�
		//		pCCBKDigitalMapDlg->SetTimer(GPS_CarTimer,10,NULL); ������ĳɻ����߳�
			//	ResumeThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ExamCarNumber]);
				break;
			case 10:
				//����رտ���
				iTmpc++;
				if (pPackage_ST->pExamCar_ST.ExamCarNumber > 0 && pPackage_ST->pExamCar_ST.ExamCarNumber < MAX_ExamCar)
				{
					pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ExamCarState = 0;//��������
					pCCBKDigitalMapDlg->m_Button[pPackage_ST->pExamCar_ST.ExamCarNumber].ShowWindow(SW_HIDE);
				//	SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ExamCarNumber]);//�����߳�
					while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ThreadNumber]));
					pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ExamCarNumber] = FALSE;
					CString ff;
					ff.Format("%d �����ر�",pPackage_ST->pExamCar_ST.ExamCarNumber);
					pCCBKDigitalMapDlg->WriteLog(ff);
				}
				break;
			default:
				iTmpDefault++;
				break;
			}
		}
		Sleep(100);
	}
	Sleep(5000);

//	free(pPackage_ST);  //Ϊʲô����ͷŲ���

	int xxx = 0;
 	if (!pCCBKDigitalMapDlg->pSocket.pPackage_ST.empty())
 	{
// 		memcpy(pPackage_ST,pCCBKDigitalMapDlg->pSocket.pPackage_ST.front(),sizeof(Package_ST));
// 		Sleep(5);
// 		R1.Format("�������%d  %d  %d  %d   ����Ϊ��%d  car  %d  %d %lf  %lf",iCount_Package,iTmpj,iTmpc,iTmpDefault,\
// 			(int) pCCBKDigitalMapDlg->pSocket.pPackage_ST.size(),pPackage_ST->pPackageHead_ST.PackageType,pPackage_ST->pExamCar_ST.ExamCarNumber,\
// 			pPackage_ST->pExamCar_ST.x_Gps,pPackage_ST->pExamCar_ST.y_Gps);
// 			pCCBKDigitalMapDlg->WriteLog(R1);
		xxx = 100;
 	}
	R1.Format("�������%d  %d  %d  %d   ����Ϊ��%d   %d",iCount_Package,iTmpj,iTmpc,iTmpDefault,(int) pCCBKDigitalMapDlg->pSocket.pPackage_ST.size(),xxx);
	pCCBKDigitalMapDlg->WriteLog(R1);
	
	pCCBKDigitalMapDlg->WriteLog("�� ParsePackage_thd");
	return 0;
}

/*
����˵����������ͳһ���߿������ģ�
����е�ǰ�����߿������б仯ʱ���ᴥ��һ����Ϣ
  ���ڲ����� ����Ϊһ��ʼ�Ͷ�ȡ�����߿�����  ����仯
*/

UINT SendCurCarMsg_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{
		Sleep(2000);		
		::PostMessage(AfxGetMainWnd()->m_hWnd,WM_BTN_MSG_CAR,pCCBKDigitalMapDlg->pSocket.TotalCar,0);
		/*CurCar = pCCBKDigitalMapDlg->pSocket.TotalCar;*/
	}
	pCCBKDigitalMapDlg->WriteLog("�� SendCurCarMsg_thd");
	Sleep(2000);
	return 0;
}





//ר���������ѿ����߳�
UINT WakeUpExamCar_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
 	while (pCCBKDigitalMapDlg->iContrl)
 	{   // HadWakeUp;//��ʾ�߳��Ѿ�������  0 ��ʾ�����߳��Ѿ��������� 1 �����߳���Ҫ����
		if (0 == pCCBKDigitalMapDlg->iNowZoomRunning && /*pCCBKDigitalMapDlg->iCountFlushMap == 0 &&*/ pCCBKDigitalMapDlg->HadWakeUp == 1)
 		{
 			Sleep(150);
 
 			pCCBKDigitalMapDlg->WakeUp_Thread();
 			pCCBKDigitalMapDlg->HadWakeUp = 0;
 		}
 		else
 		{
 			Sleep(100);
 		}
 	}
	pCCBKDigitalMapDlg->WriteLog("�� WakeUpExamCar_thd");
	return 0;
}
 

/*
 ʵʱ��λ���߿�����GPS�� �����Ӧ�Ŀ��������߾͹��� �����Ӧ�Ŀ������߾ͻ���
*/
UINT ExamCarGPSLocate_thd(LPVOID p)
{
//	memset(pExamCarInfoToThread_ST,0,sizeof(ExamCarInfoToThread_ST));
	 //  (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = ((ExamCarInfoToThread_ST)p)->pMapDlg;
//	pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	int iExamCarNumber = ((ExamCarInfoToThread_ST)p)->iExamCarNumber;
	int tmpX = 0;
	int tmpY = 0;
	int preX = 0;
	int preY = 0;
	Sleep(1000);
	//�������Ѿ��ж�Ӧ�Ŀ����ź����Ի���ָ��
	CString car;
	CString szContent;
	car.Format("%d",iExamCarNumber);

	

	while (pCCBKDigitalMapDlg->iContrl)
	{		
		
 //		if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)  //˵����ǰû�б����ٵĿ���
 //		{
 		//	if (pCCBKDigitalMapDlg->m_Button[iExamCarNumber].ExamCarState == 1) //˵����������
 		//	{
			if (pCCBKDigitalMapDlg->iNowZoomRunning == 1)
			{
				
				Sleep(50);
				continue;
			}
			else if (pCCBKDigitalMapDlg->iNowZoomRunning == 0 && pCCBKDigitalMapDlg->isRunningUpdate == 0)//���ڷŴ����С)  �Ϳ���׷��ʱ����û��ˢ��
			{
				int iResult = 0;
				pCCBKDigitalMapDlg->Mutex_Self[iExamCarNumber].Lock();// ���������õģ���Ҫ���ڿ�������
				iResult = pCCBKDigitalMapDlg->pMutex.Lock();
				//zzz
 				pCCBKDigitalMapDlg->m_Button[iExamCarNumber].Cur_xPointRelyOnParent = (pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
 				pCCBKDigitalMapDlg->m_Button[iExamCarNumber].Cur_yPointRelyOnParent = pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel);
 				pCCBKDigitalMapDlg->iCountFlushExamCar++;//����ͳ�ƿ���ˢ�µĴ��� ��Ϊ0��ʱ��ſ���ˢ�µ�ͼ
				if (pCCBKDigitalMapDlg->m_Button[iExamCarNumber].isCircle == 1)//˵��������
 				{
 					pCCBKDigitalMapDlg->m_Button[iExamCarNumber].MoveWindow(pCCBKDigitalMapDlg->CurrentScrWidth / 2, pCCBKDigitalMapDlg->CurrentScrHeight / 2,20,20);
				}
 				else	
				{
					if (pCCBKDigitalMapDlg->iNowZoomRunning == 1)
					{
						Sleep(50);
						continue;
					}
					
					/*/////////////////////////////    ��ط��������е�         //////////////////////////////
 					while (pCCBKDigitalMapDlg->isRunningUpdate);
					//////////////////////////////                  /////////////////////////////*/
					
 					pCCBKDigitalMapDlg->isRunningUpdate = 1;
 			//		Sleep(100);
					


					tmpX = (pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
					tmpY = pCCBKDigitalMapDlg->CurrentScrHeight - (pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel;
	
					if (pCCBKDigitalMapDlg->m_Button[iExamCarNumber].ExamCarState != 0)
					{
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Xpoint = int((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel);
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Ypoint = int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel) - 10);

						//zzzz fuzi1
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_ZT =pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].zt;
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_EID =pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].ErrorID;
						//////////////////////////////////////////////////////////////////////////
 						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].MoveWindow(pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Xpoint,pCCBKDigitalMapDlg->m_Button[iExamCarNumber].CurCar_Ypoint,20,20);
						pCCBKDigitalMapDlg->m_Button[iExamCarNumber].Invalidate(FALSE);
					}

						preX = tmpX;
						preY = tmpY;
			//		}
					
 			//		Sleep(100);
 					pCCBKDigitalMapDlg->isRunningUpdate = 0; 
 				}
				
				iResult = pCCBKDigitalMapDlg->pMutex.Unlock();
				pCCBKDigitalMapDlg->Mutex_Self[iExamCarNumber].Unlock();// ���������õģ���Ҫ���ڿ�������
				
				szContent.Format("�������� x = %lf,y = %lf",pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].x_Gps,pCCBKDigitalMapDlg->on_ExamCar[iExamCarNumber].y_Gps);
				pCCBKDigitalMapDlg->WriteLog(szContent,car+".clog");
				Sleep(50);
			}
				
 		//	}
 //		}
 			
 	Sleep(100);		
	}
	Sleep(5000);
	pCCBKDigitalMapDlg->WriteLog("�� ExamCarGPSLocate_thd");
	return 0;
}


UINT HideExamCar_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;

	int i = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{
	//	for (i = 1;i < pCCBKDigitalMapDlg->pSocket.TotalCar;i++)
		for (i = 1;i < MAX_ExamCar;i++)
		{
			if (pCCBKDigitalMapDlg->m_Button[i].ExamCarState == 0)//��������
			{
				pCCBKDigitalMapDlg->m_Button[i].MoveWindow(-20,-20,20,20);
				pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
			//	while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[i]));  //����Ƶ���SCOKET �����뿪������
				Sleep(2000);  //������ʱ2s����Ҫԭ�������������ߵ�������Щ������Ϣ�İ���û��ȫ��������������𵽵ȴ�������
				pCCBKDigitalMapDlg->bExamCarState[i] = FALSE;
			}			
		}
		Sleep(2000);
	}
	Sleep(4000);

	pCCBKDigitalMapDlg->WriteLog("�� HideExamCar_thd");
	return 0;
}


/*//////////////////////////////
�п��������ٵ�ʱ���Ŀ���������
//////////////////////////////*/
UINT HideOtherCar_thd(LPVOID p)
{
	
	Sleep(10000);
	int tmpCurTrackCar = -1;//������ʱ��ű����ٵĿ���
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
	int i = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{
		if(tmpCurTrackCar != pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar) //˵����ǰ��׷�ٵĿ��������仯
		{
			if(pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1) //˵��û�п���������
			{
				
				tmpCurTrackCar = -1;
				//�õ�ǰ���ߵĿ���������ʾ����
				for(i = 0;i < MAX_ExamCar;i++)
				{  
					pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
				}
			}
			else
			{
				tmpCurTrackCar = pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar;
				//ֻ��ʾ��׷�ٵĿ���
				for(i = 0;i < MAX_ExamCar;i++)
				{
					pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
				}
				pCCBKDigitalMapDlg->m_Button[tmpCurTrackCar].ShowWindow(SW_HIDE);
			}
		}
		else
		{
			Sleep(500);
		}


		
	}
	pCCBKDigitalMapDlg->WriteLog("�� HideOtherCar_thd");
	Sleep(2000);
	return 0;
}


/*//////////////////////////////
���ڶ���ˢ�����߿����͵�ͼ
//////////////////////////////*/
UINT UpdateMap_thd(LPVOID p)
{
	int tmpCurTrackCar = -1;//������ʱ��ű����ٵĿ���
	Sleep(10000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;
	int CurCar = 0;
	int i = 0;
	while (pCCBKDigitalMapDlg->iContrl)
	{	
		
		pCCBKDigitalMapDlg->HasPlayInvalite();
		Sleep(500);
	}
	pCCBKDigitalMapDlg->WriteLog("�� UpdateMap_thd");
	Sleep(2000);
	return 0;
}


/*
���ڼ������߿�������û������
ListenExamCar_thd(LPVOID p);//ר�������������߿�����  ��Ҫ�����ڿ��������õ� �����Զ������Ѿ���socket���ﴦ����
*/
UINT ListenExamCar_thd(LPVOID p)
{
	int tmpTotalCar = 0;//������ʱ��ű����ٵĿ���
	Sleep(10000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;

	while (pCCBKDigitalMapDlg->iContrl)
	{	

		pCCBKDigitalMapDlg->ReadOnlineCarFromDB1();

		
		Sleep(500);
	}

	pCCBKDigitalMapDlg->WriteLog("�� ListenExamCar_thd");
	Sleep(2000);
	return 0;
}


/*
���ڼ������߿�������û������
ListenExamCarOnline_thd(LPVOID p);//ר�������������߿�����  ��Ҫ�����ڿ��������õ� �����Զ������Ѿ���socket���ﴦ����
*/
UINT ListenExamCarOnline_thd(LPVOID p)
{
	int tmpTotalCar = 0;//������ʱ��ű����ٵĿ���
	Sleep(10000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)p;

	while (pCCBKDigitalMapDlg->iContrl)
	{	
		pCCBKDigitalMapDlg->ReadOnlineCarFromDB2();
		
		Sleep(1000);
	}

	pCCBKDigitalMapDlg->WriteLog("�� ListenExamCarOnline_thd");
	Sleep(2000);
	return 0;
}


UINT ButtonUp_thd(LPVOID p)
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	Sleep(100);
	pCCBKDigitalMapDlg->iStateLBD = 0;
	return 0;
}


/*
ר���������ÿ������ߵ�ʱ�����ؿ���
*/
UINT ExamCarHideDown_thd(LPVOID p)
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	CButtonz *pButton = (CButtonz *)p;
	Sleep(2000);

	pButton->MoveWindow(-20,-20,20,20);
//	pButton->examCarNumber = 0;
	
	pButton->CurCar_Xpoint = 0;
	pButton->CurCar_Ypoint = 0;
	pButton->CurCar_ZT = 0;
	pButton->CurCar_EID=1000;
	
	pButton->ExamCarState = 0;
	
	CString tmpOnLineCar;
	tmpOnLineCar.Format("��ǰ������Ϊ��%d",pCCBKDigitalMapDlg->pSocket.TotalCar);
	pCCBKDigitalMapDlg->m_TotalCar.SetWindowText(tmpOnLineCar);  

//	while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST->pExamCar_ST.ThreadNumber]));
//	pCCBKDigitalMapDlg->bExamCarState[pPackage_ST->pExamCar_ST.ExamCarNumber] = FALSE;


	pCCBKDigitalMapDlg->WriteLog("�� ExamCarHideDown_thd");
	return 0;
}

void CCBKDigitalMapDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	int i = 0;
	
	switch(nIDEvent)
	{
	case GPS_CarTimer:
		{
			if (ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)
			{
			
				for (i = 1;i < MAX_ExamCar;i++)
				{
					if (ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)
					{
						if (m_Button[i].ExamCarState == 1) //˵����������
						{//zzz
							m_Button[i].Cur_xPointRelyOnParent = (on_ExamCar[i].x_Gps - MAPLeft) / x_pixel;
							m_Button[i].Cur_yPointRelyOnParent = CurrentScrHeight - ((on_ExamCar[i].y_Gps - MAPBotom) / y_pixel);
							if (m_Button[i].isCircle == 1)//˵��������
							{
								m_Button[i].MoveWindow(this->CurrentScrWidth / 2, this->CurrentScrHeight / 2,20,20);
							}
							else
							{
								while (isRunningUpdate);
								isRunningUpdate = 1;
							//	Sleep(50);
								m_Button[i].MoveWindow(int((on_ExamCar[i].x_Gps - MAPLeft) / x_pixel),int(CurrentScrHeight - ((on_ExamCar[i].y_Gps - MAPBotom) / y_pixel)),20,20);
							//	Sleep(50);
								isRunningUpdate = 0;
							}
						}
					}
					else
					{
						break;
					}
				}
					
			}
		//	m_Button[12].MoveWindow((on_ExamCar[12].x_Gps - MAPLeft) / x_pixel,CurrentScrHeight - ((on_ExamCar[12].y_Gps - MAPBotom) / y_pixel),20,20);
		}
		break;
	case GPS_Dlg:
		KillTimer(GPS_Dlg);
		CreateDlgha();//������ťҲ���ǿ�������Ϣ��
		pSocket.iC_Dlg_F = 1;
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}



LRESULT CCBKDigitalMapDlg::OnBtnMsgz(WPARAM wParam, LPARAM lParam)
{
	CString str;
	CString Card;
	CString temp;
	UINT ExamCarNumber = wParam;
	temp.Format("%d",wParam);
	Card.Format("����");
	Card += temp;
	dlgha[lParam].ReadExamInfoFromDB(NULL,Card,NULL);
	str.Format("����%d",wParam);
/*	str += dlgha.szExamineeName;*/
	dlgha[lParam].CSCAR = str;


	
/*
	if (wParam > 0 && wParam < MAX_ExamCar)
	{

	
		//������Ҫ�ж�4��������� ��ͬ��������ز�ͬ�ı���ͼƬ
		CRect ClientRect; //�ͻ�������
		GetClientRect(ClientRect);
		if (m_Button[lParam].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
		{
			//�Ի���ļ�ͷ�������½�
			
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent + 20,m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
		else if (m_Button[lParam].Cur_xPointRelyOnParent + 20 <= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
		{	//�Ի���ļ�ͷ�������Ͻ�
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent + 20,m_Button[lParam].Cur_yPointRelyOnParent + 20 ,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
		else if (m_Button[lParam].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight >= 0)
		{	//�Ի���ļ�ͷ�������½�
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent - SubDlgForInfoWidth,m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
		else if (m_Button[lParam].Cur_xPointRelyOnParent + 20 >= ClientRect.right -SubDlgForInfoWidth && m_Button[lParam].Cur_yPointRelyOnParent - SubDlgForInfoHeight < 0)
		{	//�Ի���ļ�ͷ�������Ͻ�
			dlgha[lParam].MoveWindow(m_Button[lParam].Cur_xPointRelyOnParent - SubDlgForInfoWidth,m_Button[lParam].Cur_yPointRelyOnParent + 20,SubDlgForInfoWidth,SubDlgForInfoHeight);
			dlgha[lParam].ShowWindow(SW_SHOW);
		}
	}

*/
	
	return TRUE;
}

LRESULT CCBKDigitalMapDlg::OnBtnMsgzLeft(WPARAM wParam, LPARAM lParam)
{
	CString str;
	UINT ExamCarNumber = wParam;
	if (wParam > 0 && wParam < MAX_ExamCar)
	{
		str.Format("����%d",wParam);
		dlgha[ExamCarNumber].CSCAR = str;
		dlgha[ExamCarNumber].MoveWindow(-500,-300,350,150);
		dlgha[ExamCarNumber].ShowWindow(SW_HIDE);
	}

	return TRUE;
}

HBRUSH CCBKDigitalMapDlg::OnCtlColor(CDC *pDC, CWnd *pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TOTALCAR)
	{
		pDC->SetTextColor(RGB(255,0,255));  //����������ɫ
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_BOOT)
	{
		pDC->SetTextColor(RGB(255,0,255));  //����������ɫ
	}
	if   (nCtlColor   ==   CTLCOLOR_STATIC)        //���ÿؼ�Ϊ͸��
	{     
		pDC->SetBkMode(TRANSPARENT);     
		hbr=(HBRUSH)GetStockObject(NULL_BRUSH);
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;

}




LRESULT CCBKDigitalMapDlg::OnUpdataCurCar(WPARAM wParam, LPARAM lParam)
{
	CString str;	
	str.Format("��ǰ������Ϊ��%d",wParam);
	m_TotalCar.SetWindowText(str);
	CRect pCRect(10,10,240,50);
	InvalidateRect(pCRect,false); //ǿ�Ƹ��������ڵĽ��� 
//	Invalidate();//ǿ��ˢ������Ի�������
	return TRUE;
}




//����м���ֵ���Ϣ
//����˵����zDelta ��������� ���� ��ǰ���� ����Ǹ��� ˵���Ǹ��� ����������  
// pt ����ǰ��������ֵ
BOOL CCBKDigitalMapDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	short iDelta = zDelta; //ȷ����������
	x_CurScr = pt.x; // ��ǰ�������Ļ�ϵ�����
	y_CurScr = pt.y;
	
	CRect ClientRect; //�ͻ�������
	GetClientRect(ClientRect);





	if (iDelta > 0) //˵����Ҫ���зŴ�
	{
		int x = 0; //ClientRect.Width(); //��Ļ�Ŀ��  ��һ�����ڷŴ��
		int y = 0;//ClientRect.Height();//��Ļ�ĸ߶�   ��һ�����ڷŴ��
		DOUBLE x_CurLatitude = 0;//��ǰγ��
		DOUBLE y_CurLong = 0;// ��ǰ����
		if (ZoomLevel >= 24)
		{
			return TRUE;
		}
		x = (ClientRect.Width() / 1.2);
		y = (ClientRect.Height() / 1.2);
		// 1��Ҫ�ж��Ƿ���4���ŵĵط����Ǵ���4���ߵĵط� �������м� 9������
		// 2��ȷ����ʱ����Ļ�еľ���
		// 3�������ʱ���Ͻź����½ǵľ���γ�� ͬʱ�����ʵ����Ļ��1��������ռ�ľ���γ�� 
		//    ���� MAPBotom MAPTop  MAPLeft  MAPRight  x_pixel  y_pixel
		// 4����Ӧ��ԭ���Ļ���λ��
		// 5 ���Զ���һ����Ϣ ˢ����Ļ
		if (x_CurScr <= (x / 2) && y_CurScr <= (y / 2)) //˵���������Ͻǵ�����
		{  //  x_StartBit,y_StartBit,x_BitWidth,y_BitWidth
			// x_StartBit,y_StartBit,x_BitWidth,y_BitWidth
			// ����һ������  x_StartBit  y_StartBit �Ĵ���  �ѽ��

			MAPLeft = MAPLeft;
			MAPTop = MAPTop;
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			MAPRight = x_pixel * x + MAPLeft;  
			MAPBotom = MAPTop - y_pixel * y;
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��
			
		}
		else if (x_CurScr <= (x / 2) && ((y_CurScr + (y / 2)) >= ClientRect.bottom )) //˵���������½ǵ�����
		{	
			DOUBLE X_GPS = (MAPRight - MAPLeft) * 0.1;  //��ǰ��ĻGPSֵҪ�����  ���Ȳ�ֵ
			DOUBLE Y_GPS = (MAPTop - MAPBotom) * 0.1;  //��ǰ��ĻGPSֵҪ�����  γ�Ȳ�ֵ
			MAPLeft = MAPLeft;
			MAPBotom = MAPBotom;
			MAPRight = x_pixel * x + MAPLeft;  
			MAPTop = MAPBotom + y_pixel * y;

			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��
		}
		else if (x_CurScr <= (x / 2) && ((y_CurScr + (y / 2)) < ClientRect.bottom ))  //˵��������ߵ��м�����
		{
			DOUBLE X_GPS = (MAPRight - MAPLeft) * 0.1;  //��ǰ��ĻGPSֵҪ�����  ���Ȳ�ֵ
			DOUBLE Y_GPS = (MAPTop - MAPBotom) * 0.1;  //��ǰ��ĻGPSֵҪ�����  γ�Ȳ�ֵ
			DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			MAPLeft = MAPLeft;	
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
			iY = (ClientRect.Height() / 1.2 );

			MAPTop = y_CurLong + y_pixel * (iY / 2);
			MAPBotom = y_CurLong - y_pixel * (iY / 2);
			MAPRight = MAPLeft + x_pixel * iX;

			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��

		}
		else if (((x_CurScr + (x / 2)) < ClientRect.right) && y_CurScr <= (y / 2)) //˵�����ϱߵ��м�����
		{
			DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			MAPTop = MAPTop;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
			iY = (ClientRect.Height() / 1.2 );
			MAPLeft = x_CurLatitude - x_pixel * (iX / 2);
			MAPRight = x_CurLatitude + x_pixel * (iX / 2);
			MAPBotom = MAPTop - y_pixel * iY;
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��

		}
		else if (((x_CurScr + (x / 2)) >= ClientRect.right) && y_CurScr <= (y / 2))  //˵�������Ͻ�
		{
			DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			MAPRight = MAPRight;
			MAPTop = MAPTop;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
			iY = (ClientRect.Height() / 1.2 );
			
			MAPBotom = MAPTop - y_pixel * iY;
			MAPLeft = MAPRight - x_pixel * iX;
			
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��

		}
		else if (((x_CurScr + (x / 2)) >= ClientRect.right) && ((y_CurScr + (y / 2)) < ClientRect.bottom )) //˵�����ұߵ��м�����
		{
			DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			MAPRight = MAPRight;	
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
			iY = (ClientRect.Height() / 1.2 );
			
			MAPTop = y_CurLong + y_pixel * (iY / 2);
			MAPBotom = y_CurLong - y_pixel * (iY / 2);
			MAPLeft = MAPRight - x_pixel * iX;
			
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��

		}
		else if (((x_CurScr + (x / 2)) >= ClientRect.right) && ((y_CurScr + (y / 2)) >= ClientRect.bottom ))//˵�������½�
		{
			DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			MAPRight = MAPRight;
			MAPBotom = MAPBotom;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
			iY = (ClientRect.Height() / 1.2 );
			
			MAPTop = MAPBotom + y_pixel * iY;
			MAPLeft = MAPRight - x_pixel * iX;
			
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��
		}
		else if (((x_CurScr + (x / 2)) < ClientRect.right) && ((y_CurScr + (y / 2)) >= ClientRect.bottom )) //˵�����±ߵ��м�����
		{
			DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			MAPBotom = MAPBotom;
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
			iY = (ClientRect.Height() / 1.2 );
			MAPLeft = x_CurLatitude - x_pixel * (iX / 2);
			MAPRight = x_CurLatitude + x_pixel * (iX / 2);
			MAPTop = MAPBotom + y_pixel * iY;
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��

		}
		else //���м�����
		{
			DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
			x_CurLatitude = MAPLeft + x_pixel * pt.x;
			y_CurLong = MAPTop - y_pixel * pt.y;
			iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
			iY = (ClientRect.Height() / 1.2 );
			MAPLeft = x_CurLatitude - x_pixel * (iX / 2);
			MAPRight = x_CurLatitude + x_pixel * (iX / 2);
			MAPTop = y_CurLong + y_pixel * (iY / 2);
			MAPBotom = y_CurLong - y_pixel * (iY / 2);
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);	
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_pixel = (DOUBLE)((MAPRight-MAPLeft) / (ClientRect.right - ClientRect.left)); // ��õ���һ��x�������ش�����پ���
			y_pixel = (DOUBLE)((MAPTop-MAPBotom) / (ClientRect.bottom - ClientRect.top)); // ��õ���һ��y�������ش������γ��

		}

		switch(ZoomLevel)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
			{
				if (ZoomLevel == 0)
				{
					ZoomLevel = ZoomLevel1;
				}
				ZoomLevel = ZoomLevel + 1;

				


				PixState = 1;  //�Ѿ��Ŵ�
				Suspend_Thread();
				iNowZoomRunning = 1;//���ڷŴ����С
				iCountFlushMap ++; //��ͼ��ˢ�´�����һ
				while (iCountFlushExamCar)
				{
					Sleep(200);
					iCountFlushExamCar = 0;
				}
			//	Sleep(100);
			//	Invalidate(FALSE);
				HasPlayInvalite();
//				while (iNowZoomRunning);
	//			WakeUp_Thread();  ����������̵߳Ĺ�����OnPaint�����β��ʵ��
				
			}

			break;
		case 24:  //�����ٽ��зŴ��� 
			break;
		default:
			break;
		}
	}
	else  //˵��Ҫ������С
	{
		// ��С��˼· 
		// ������õ�ľ��� γ��
		// 1���Ƚ�ȫ����ͼƬ��СΪ��һ�ηŴ�֮ǰ��ͼƬ��С  ���ѵ�ǰ����Ļ ���� 1.2 ͬʱ���¼��� x_pixel ��y_pixel
		// 2������ͼƬ����߾�γ����ͼƬ����߼��� ��Ļ�Ŀ�� ��ȥ �Ŵ�ǰ�Ŀ�� �ٳ���2 ����һ��Ҫ��С����ʾ�Ŀ�� ͬʱ�������
		// 3�������ʱ�ľ���С�ڵ���ԭʼ�ľ��ȣ�˵���Ѿ���������ͼ��ԭʼͼ�˾�ԭͼ��ʾ������ԭʼͼ��
		//  ����о�˵�� �Ѿ���С��ԭͼ�ˣ�û�о��Ըõ�Ϊ���Ľ�������
		int x = 0; //ClientRect.Width(); //��Ļ�Ŀ��  ��һ��������С
		int y = 0;//ClientRect.Height();//��Ļ�ĸ߶�   ��һ��������С��
		DOUBLE iX = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
		DOUBLE iY = 0; //���ڷŴ�֮����С�����ǲ��Ǵ���0
		if (ZoomLevel <= 0)
		{
			return TRUE;
		}

		////////    �������Щ���������������ԭͼ�Ļ���������С�� �������һЩ��ʱ����  /////////
		
		DOUBLE itmpMAPLeft = 0;
		DOUBLE itmpMAPRight = 0;
		DOUBLE itmpMAPBotom = 0;
		DOUBLE itmpMAPTop = 0;

		DOUBLE itmpx_pixel = 0;
		DOUBLE itmpy_pixel = 0;
		
		itmpMAPLeft = MAPLeft;
		itmpMAPRight = MAPRight;
		itmpMAPBotom = MAPBotom;
		itmpMAPTop = MAPTop;

		itmpx_pixel = x_pixel;
		itmpy_pixel = y_pixel;


		/////////////////
		iX = (ClientRect.Width() / 1.2 ) ;  //  iX / 2 �������ͼƬ�Ŀ��һ��
		iY = (ClientRect.Height() / 1.2 );
		x = iX;
		y = iY;
		int x_Offset = 0;
		int y_Offset = 0;
		x_Offset = (ClientRect.Width() - (ClientRect.Width() / 1.2 )) / 2;  //γ�ȵ�ƫ����
		y_Offset = (ClientRect.Height() - (ClientRect.Height() / 1.2)) /2;  //���ȵ�ƫ����

		// �����ǰ�ľ��� γ��
		DOUBLE x_CurLatitude = 0;//��ǰγ��
		DOUBLE y_CurLong = 0;// ��ǰ����
		x_CurLatitude = MAPLeft + x_pixel * pt.x;
		y_CurLong = MAPTop - y_pixel * pt.y;

		x_pixel = (DOUBLE)((MAPRight-MAPLeft) / iX); // ��õ���һ��x�������ش�����پ��� ���»�ȡ����
		y_pixel = (DOUBLE)((MAPTop-MAPBotom) / iY); // ��õ���һ��y�������ش������γ��

		MAPRight = x_CurLatitude + x_pixel *  (iX/2);
		MAPBotom = y_CurLong - y_pixel * (iY/2);
		MAPLeft = x_CurLatitude - x_pixel * (iX/2);
		MAPTop = y_CurLong + y_pixel * (iY/2);
		DOUBLE MAPRight1 = MAPRight + x_pixel * ((ClientRect.Width() - iX) / 2);
		DOUBLE MAPBotom1 = MAPBotom - y_pixel * ((ClientRect.Width() - iY) /2) ;
		DOUBLE MAPLeft1 = MAPLeft - x_pixel * ((ClientRect.Width() - iX) / 2);
		DOUBLE MAPTop1 = MAPTop + y_pixel * ((ClientRect.Width() - iY) /2) ;


		if (MAPLeft1 > OriginalMAPLeft || MAPTop1 < OriginalMAPTop || MAPRight1 < OriginalMAPRight || MAPBotom1 > OriginalMAPBotom)  //��û�е�ԭͼ�Ĵ�С
		{
			if (MAPLeft < OriginalMAPLeft)  //˵���������
			{
				MAPLeft = OriginalMAPLeft;
				
				if (MAPTop > OriginalMAPTop)  //˵�������Ͻ�
				{
					
					MAPTop = OriginalMAPTop;
					MAPBotom = MAPTop - y_pixel * iY - y_pixel * y_Offset * 2;
				}
				else if (MAPBotom < OriginalMAPBotom)  //˵�������½�
				{
					MAPBotom = OriginalMAPBotom;
					MAPTop = MAPBotom + y_pixel * iY + y_Offset * y_pixel * 2;
				}
				else
				{
					//˵��������м�
					MAPBotom = y_CurLong - y_pixel * (iY/2) - y_pixel * y_Offset;
					MAPTop = y_CurLong + y_pixel * (iY/2) + y_pixel * y_Offset;
					

					if (MAPBotom < OriginalMAPBotom && MAPTop > OriginalMAPTop)
					{
						MAPLeft = itmpMAPLeft;
						MAPRight = itmpMAPRight;
						MAPBotom = itmpMAPBotom;
						MAPTop = itmpMAPTop;
						
						x_pixel = itmpx_pixel;
						y_pixel = itmpy_pixel;
						if(ZoomLevel != 0)
						{
							ZoomLevel1 = ZoomLevel;
						}
						ZoomLevel = 0;
						
						return 1;
					}
					else
					{
						if (MAPBotom < OriginalMAPBotom)
						{
							DOUBLE itmp = OriginalMAPBotom - MAPBotom + MAPTop;
							if (itmp > OriginalMAPTop) //Ҳ��������С��
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPBotom = OriginalMAPBotom;
								MAPTop = itmp;
							}
						}
						else
						{
							if (MAPTop > OriginalMAPTop)
							{
								DOUBLE itmp = MAPBotom - (MAPTop - OriginalMAPTop);
								if (itmp < OriginalMAPBotom) //Ҳ��������С��
								{
									MAPLeft = itmpMAPLeft;
									MAPRight = itmpMAPRight;
									MAPBotom = itmpMAPBotom;
									MAPTop = itmpMAPTop;
									
									x_pixel = itmpx_pixel;
									y_pixel = itmpy_pixel;
									if(ZoomLevel != 0)
									{
										ZoomLevel1 = ZoomLevel;
									}
									ZoomLevel = 0;
									
									return 1;
								}
								else
								{
									MAPTop = OriginalMAPTop;
									MAPBotom = itmp;
								}
							}
							
						}
					}


				}
				MAPRight = MAPLeft + x_pixel * iX + x_pixel * x_Offset * 2;

				if ((MAPRight > OriginalMAPRight) || (MAPBotom < OriginalMAPBotom) || (MAPTop > OriginalMAPTop))
				{


					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					
					return 1;
				}
			}
			else if (MAPRight > OriginalMAPRight) //˵�������ұ�
			{
				MAPRight = OriginalMAPRight;

				/*
				if ((MAPRight - x_pixel * iX - x_Offset * x_pixel * 2) < OriginalMAPLeft ) //˵����������С�ˣ��˳�
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					return 1;
				}
				*/
					
				
				if (MAPTop > OriginalMAPTop)  //˵�������Ͻ�
				{
					MAPTop = OriginalMAPTop;
					MAPBotom = MAPTop - y_pixel * iY - y_pixel * y_Offset * 2;
				}
				else if (MAPBotom < OriginalMAPBotom)  //˵�������½�
				{
					MAPBotom = OriginalMAPBotom;
					MAPTop = MAPBotom + y_pixel * iY + y_Offset * y_pixel * 2;
				}
				else
				{
					//˵�����ұ��м�
					MAPBotom = y_CurLong - y_pixel * (iY/2) - y_pixel * y_Offset;
					MAPTop = y_CurLong + y_pixel * (iY/2) + y_pixel * y_Offset;

					if (MAPBotom < OriginalMAPBotom && MAPTop > OriginalMAPTop)
					{
						MAPLeft = itmpMAPLeft;
						MAPRight = itmpMAPRight;
						MAPBotom = itmpMAPBotom;
						MAPTop = itmpMAPTop;
						
						x_pixel = itmpx_pixel;
						y_pixel = itmpy_pixel;
						if(ZoomLevel != 0)
						{
							ZoomLevel1 = ZoomLevel;
						}
						ZoomLevel = 0;
						
						return 1;
					}
					else
					{
						if (MAPBotom < OriginalMAPBotom)
						{
							DOUBLE itmp = OriginalMAPBotom - MAPBotom + MAPTop;
							if (itmp > OriginalMAPTop) //Ҳ��������С��
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPBotom = OriginalMAPBotom;
								MAPTop = itmp;
							}
						}
						else
						{
							if (MAPTop > OriginalMAPTop)
							{
								DOUBLE itmp = MAPBotom - (MAPTop - OriginalMAPTop);
								if (itmp < OriginalMAPBotom) //Ҳ��������С��
								{
									MAPLeft = itmpMAPLeft;
									MAPRight = itmpMAPRight;
									MAPBotom = itmpMAPBotom;
									MAPTop = itmpMAPTop;
									
									x_pixel = itmpx_pixel;
									y_pixel = itmpy_pixel;
									if(ZoomLevel != 0)
									{
										ZoomLevel1 = ZoomLevel;
									}
									ZoomLevel = 0;
									
									return 1;
								}
								else
								{
									MAPTop = OriginalMAPTop;
									MAPBotom = itmp;
								}
							}
							
						}
					}
				}
				MAPLeft = MAPRight - x_pixel * iX - x_Offset * x_pixel * 2;	

				if ((MAPLeft < OriginalMAPLeft) || (MAPBotom < OriginalMAPBotom) || (MAPTop > OriginalMAPTop))
				{
				

					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					
					return 1;
				}
			}
			else if (MAPTop > OriginalMAPTop) //˵�����ϱߵ��м� ��Ϊ���� �� �����Ѿ��ų���
			{
				MAPTop = OriginalMAPTop;
				
				MAPBotom = MAPTop - y_pixel * iY - y_pixel * y_Offset * 2;
				MAPLeft = x_CurLatitude - x_pixel * (iX/2) - x_pixel * x_Offset;
				MAPRight = x_CurLatitude + x_pixel *  (iX/2) + x_pixel * x_Offset;

				if (MAPBotom < OriginalMAPBotom)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}

				if (MAPLeft < OriginalMAPLeft && MAPRight > OriginalMAPRight) //ûʲô��˵�ģ���������С��
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
					
				}
				else
				{
					if (MAPLeft < OriginalMAPLeft)
					{
						DOUBLE itmp = OriginalMAPLeft - MAPLeft + MAPRight;
						if (itmp > OriginalMAPRight) //Ҳ��������С��
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;

							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPLeft = OriginalMAPLeft;
							MAPRight = itmp;
						}
					}
					else
					{
						if (MAPRight > OriginalMAPRight)
						{
							DOUBLE itmp = MAPLeft - (MAPRight - OriginalMAPRight);
							if (itmp < OriginalMAPLeft) //Ҳ��������С��
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPRight = OriginalMAPRight;
								MAPLeft = itmp;
							}
						}
						
					}
				}

				
			}
			else if (MAPBotom < OriginalMAPBotom)
			{
				MAPBotom = OriginalMAPBotom;			
				MAPTop = MAPBotom + y_pixel * iY + y_Offset * y_pixel * 2;
				MAPLeft = x_CurLatitude - x_pixel * (iX/2) - x_pixel * x_Offset;
				MAPRight = x_CurLatitude + x_pixel *  (iX/2) + x_pixel * x_Offset;

				if (MAPTop > OriginalMAPTop)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}


				if (MAPLeft < OriginalMAPLeft && MAPRight > OriginalMAPRight)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}
				else
				{
					if (MAPLeft < OriginalMAPLeft)
					{
						DOUBLE itmp = OriginalMAPLeft - MAPLeft + MAPRight;
						if (itmp > OriginalMAPRight) //Ҳ��������С��
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;
							
							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPLeft = OriginalMAPLeft;
							MAPRight = itmp;
						}
					}
					else
					{
						if (MAPRight > OriginalMAPRight)
						{
							DOUBLE itmp = MAPLeft - (MAPRight - OriginalMAPRight);
							if (itmp < OriginalMAPLeft) //Ҳ��������С��
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPRight = OriginalMAPRight;
								MAPLeft = itmp;
							}
						}
						
					}
				}
			}
			else
			{
				MAPLeft = x_CurLatitude - x_pixel * (iX/2) - x_pixel * x_Offset;
				MAPRight = x_CurLatitude + x_pixel *  (iX/2) + x_pixel * x_Offset;
				MAPBotom = y_CurLong - y_pixel * (iY/2) - y_pixel * y_Offset;
				MAPTop = y_CurLong + y_pixel * (iY/2) + y_pixel * y_Offset;
					
				if (MAPLeft < OriginalMAPLeft && MAPRight > OriginalMAPRight)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}
				else
				{
					if (MAPLeft < OriginalMAPLeft)
					{
						DOUBLE itmp = OriginalMAPLeft - MAPLeft + MAPRight;
						if (itmp > OriginalMAPRight) //Ҳ��������С��
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;
							
							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPLeft = OriginalMAPLeft;
							MAPRight = itmp;
						}
					}
					else
					{
						if (MAPRight > OriginalMAPRight)
						{
							DOUBLE itmp = MAPLeft - (MAPRight - OriginalMAPRight);
							if (itmp < OriginalMAPLeft) //Ҳ��������С��
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPRight = OriginalMAPRight;
								MAPLeft = itmp;
							}
						}
						
					}
				}



				if (MAPBotom < OriginalMAPBotom && MAPTop > OriginalMAPTop)
				{
					MAPLeft = itmpMAPLeft;
					MAPRight = itmpMAPRight;
					MAPBotom = itmpMAPBotom;
					MAPTop = itmpMAPTop;
					
					x_pixel = itmpx_pixel;
					y_pixel = itmpy_pixel;
					
					if(ZoomLevel != 0)
					{
						ZoomLevel1 = ZoomLevel;
					}
					ZoomLevel = 0;
					
					return 1;
				}
				else
				{
					if (MAPBotom < OriginalMAPBotom)
					{
						DOUBLE itmp = OriginalMAPBotom - MAPBotom + MAPTop;
						if (itmp > OriginalMAPTop) //Ҳ��������С��
						{
							MAPLeft = itmpMAPLeft;
							MAPRight = itmpMAPRight;
							MAPBotom = itmpMAPBotom;
							MAPTop = itmpMAPTop;
							
							x_pixel = itmpx_pixel;
							y_pixel = itmpy_pixel;
							
							if(ZoomLevel != 0)
							{
								ZoomLevel1 = ZoomLevel;
							}
							ZoomLevel = 0;
							
							return 1;
						}
						else
						{
							MAPBotom = OriginalMAPBotom;
							MAPTop = itmp;
						}
					}
					else
					{
						if (MAPTop > OriginalMAPTop)
						{
							DOUBLE itmp = MAPBotom - (MAPTop - OriginalMAPTop);
							if (itmp < OriginalMAPBotom) //Ҳ��������С��
							{
								MAPLeft = itmpMAPLeft;
								MAPRight = itmpMAPRight;
								MAPBotom = itmpMAPBotom;
								MAPTop = itmpMAPTop;
								
								x_pixel = itmpx_pixel;
								y_pixel = itmpy_pixel;
								if(ZoomLevel != 0)
								{
									ZoomLevel1 = ZoomLevel;
								}
								ZoomLevel = 0;
								
								return 1;
							}
							else
							{
								MAPTop = OriginalMAPTop;
								MAPBotom = itmp;
							}
						}
						
					}
				}


/*				if (MAPLeft <= OriginalMAPLeft || MAPTop >= OriginalMAPTop || MAPRight >= OriginalMAPRight || MAPBotom <= OriginalMAPBotom)
				{

					//////////////////////////////////////////////////////
					//////////////////////////////////////////////////////

					WriteLog("bbbb","bbbb");
					//˵����С��ص�ԭͼ��С
					x_StartBit = 0;
					y_StartBit = 0;
					MAPBotom = OriginalMAPBotom;//����ԭʼGPSֵ 
					MAPLeft = OriginalMAPLeft;//����ԭʼGPSֵ 
					MAPTop = OriginalMAPTop;//����ԭʼGPSֵ 
					MAPRight = OriginalMAPRight;//����ԭʼGPSֵ 
					x_pixel = x_Originalpixel;// ԭʼ��x
					y_pixel = y_Originalpixel;// ԭʼ��y
					y_BitWidth = y_OriginalBit; //��ȡͼƬ�ĸ߶�  ����ԭ��ͼƬ�Ĵ�С
					x_BitWidth = x_OriginalBit; //��ȡͼƬ�Ŀ��  ����ԭ��ͼƬ�Ĵ�С
					PixState = 0;//�ص�ԭͼ
					ZoomLevel = 0;
// 					CRect ClientRect; //�ͻ�������
// 					GetClientRect(ClientRect);
// 					InvalidateRect(&ClientRect,FALSE);
					Suspend_Thread();
					iNowZoomRunning = 1;//���ڷŴ����С
					iCountFlushMap ++; //��ͼ��ˢ�´�����һ
					while (iCountFlushExamCar)
					{
						Sleep(200);
						iCountFlushExamCar = 0;
					}
					Sleep(100);
					Invalidate(FALSE);
			//		while (iNowZoomRunning);
			//		WakeUp_Thread();
					
				//	Sleep(50);
				//	MessageBox("123");
				//	Sleep(100);
					iNowZoomRunning = 0;//û�зŴ����С
					return CDialog::OnMouseWheel(nFlags, zDelta, pt);
					
				}
*/
			}

			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);

		}
		else//˵���Ѿ�����ԭͼ�Ĵ�С��
		{
			return 1;
			//˵���Ѿ���С����С��
			x_StartBit = 0;
			y_StartBit = 0;
			MAPBotom = OriginalMAPBotom;//����ԭʼGPSֵ 
			MAPLeft = OriginalMAPLeft;//����ԭʼGPSֵ 
			MAPTop = OriginalMAPTop;//����ԭʼGPSֵ 
			MAPRight = OriginalMAPRight;//����ԭʼGPSֵ 
			x_pixel = x_Originalpixel;// ԭʼ��x
			y_pixel = y_Originalpixel;// ԭʼ��y
			y_BitWidth = y_OriginalBit; //��ȡͼƬ�ĸ߶�  ����ԭ��ͼƬ�Ĵ�С
			x_BitWidth = x_OriginalBit; //��ȡͼƬ�Ŀ��  ����ԭ��ͼƬ�Ĵ�С
			PixState = 0;//�ص�ԭͼ
			WriteLog("����С����ص�����С","bbbb.clog");
		}

		switch(ZoomLevel)
		{
		case 0:  //�����ٽ�����С��
			break;
		
			/*
			//ǿ�Ƹ����ص�ԭͼ
			{
				x_StartBit = 0;
				y_StartBit = 0;
				MAPBotom = OriginalMAPBotom;//����ԭʼGPSֵ 
				MAPLeft = OriginalMAPLeft;//����ԭʼGPSֵ 
				MAPTop = OriginalMAPTop;//����ԭʼGPSֵ 
				MAPRight = OriginalMAPRight;//����ԭʼGPSֵ 
				x_pixel = x_Originalpixel;// ԭʼ��x
				y_pixel = y_Originalpixel;// ԭʼ��y
				y_BitWidth = y_OriginalBit; //��ȡͼƬ�ĸ߶�  ����ԭ��ͼƬ�Ĵ�С
				x_BitWidth = x_OriginalBit; //��ȡͼƬ�Ŀ��  ����ԭ��ͼƬ�Ĵ�С
				PixState = 0;//�ص�ԭͼ
				if (PixState == 0  && ZoomLevel == 0)
				{
					break;
				}
				ZoomLevel = ZoomLevel - 1;
				Suspend_Thread();
				iNowZoomRunning = 1;//���ڷŴ����С
				iCountFlushMap ++; //��ͼ��ˢ�´�����һ
				while (iCountFlushExamCar)
				{
					Sleep(200);
					iCountFlushExamCar = 0;
				}
				Sleep(100);
				Invalidate(FALSE);
		//		while (iNowZoomRunning);
		//		WakeUp_Thread();
				
			}
			
			break;
*/
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
			{
				if (PixState == 0  && ZoomLevel == 0)
				{
					break;
				}
				ZoomLevel = ZoomLevel - 1;

				
// 				CRect ClientRect; //�ͻ�������
// 				GetClientRect(ClientRect);
// 				InvalidateRect(&ClientRect,FALSE);
				Suspend_Thread();
				iNowZoomRunning = 1;//���ڷŴ����С
				iCountFlushMap ++; //��ͼ��ˢ�´�����һ
				while (iCountFlushExamCar)
				{
					Sleep(200);
					iCountFlushExamCar = 0;
				}
			//	Sleep(100);
			//	Invalidate(FALSE);
				HasPlayInvalite();
			//	while (iNowZoomRunning);
			//	WakeUp_Thread();
				
			}
			break;
		default:
			break;
		}
	}
//	Sleep(50);
//	Sleep(100);
	iNowZoomRunning = 0;//û�зŴ����С
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CCBKDigitalMapDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nFlags == MK_LBUTTON )
	{
		//˵���������������
		LButtonDown = TRUE;
		x_CurLbuttonDown = point.x;
		y_CurLbuttonDown = point.y;
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CCBKDigitalMapDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//˵���������������
	LButtonDown = FALSE;
	
	CDialog::OnLButtonUp(nFlags, point);

	//����һ���߳��������� iStateLBD ��־λ  iStateLBD = 0;//����������������� �϶�ʱΪ1 ����״̬��Ϊ0
	AfxBeginThread(&ButtonUp_thd, this); //ר����һ���߳���ˢ�µ�ͼ�Ϳ���  

	
}

void CCBKDigitalMapDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	DOUBLE x_Latitude1 = 0;  //γ�ȵ�ƫ����
	DOUBLE y_Long1 = 0;  //���ȵ�ƫ����
	if (LButtonDown && ExamCarInfo.ExamCarDlg.CurTrackExamCar == -1)//��갴�� ��ʼ�϶�����  ���Ҵ�ʱû�п�����ʵʱ����
	{
		iStateLBD = 1;//����������������� �϶�ʱΪ1 ����״̬��Ϊ0
		if (x_CurLbuttonDown != point.x || y_CurLbuttonDown != point.y)
		{
			// 1���ж���������� ���� ���� ����
			
			if (x_CurLbuttonDown != point.x)
			{
				if (x_CurLbuttonDown - point.x > 0)// ˵��������
				{
					//�����ʱ��γ��ƫ����
					x_Latitude1 = (x_CurLbuttonDown - point.x) * x_pixel;
					//�����ұ���û���Ƶ�ԭͼ���ұ�
					if (MAPRight + x_Latitude1 <= OriginalMAPRight) //˵����û�г���ԭͼ
					{
						MAPRight = MAPRight + x_Latitude1;
						MAPLeft = MAPLeft + x_Latitude1;
					}
					else
					{
						//������ԭͼ���ұ�  ���ֻ���Ƶ�ԭͼ���ұ�
						DOUBLE x_Latitude2 = 0;//��һ�����������µ���ƫ���˶���
						x_Latitude2 = OriginalMAPRight - MAPRight;
						MAPRight = OriginalMAPRight;
						MAPLeft = MAPLeft + x_Latitude2;
					}
				}
				else
				{
					//˵��������
					//�����ʱ��γ��ƫ����
					x_Latitude1 = (point.x - x_CurLbuttonDown) * x_pixel;
					//���������û���Ƶ�ԭͼ�����
					if (MAPLeft - x_Latitude1 >= OriginalMAPLeft) //˵����û�г���ԭͼ
					{
						MAPLeft = MAPLeft - x_Latitude1;
						MAPRight = MAPRight - x_Latitude1;
					}
					else
					{
						//������ԭͼ�����  ���ֻ���Ƶ�ԭͼ�����
						DOUBLE x_Latitude2 = 0;//��һ�����������µ���ƫ���˶���
						x_Latitude2 = MAPLeft - OriginalMAPLeft;
						MAPLeft = OriginalMAPLeft;
						MAPRight = MAPRight - x_Latitude2;
					}
				}
				 
			}

			//�����������Ƶ�
			if (y_CurLbuttonDown != point.y)
			{
				if (y_CurLbuttonDown - point.y > 0)// ˵��������
				{
					//�����ʱ�ľ���ƫ����
					y_Long1 = (y_CurLbuttonDown - point.y) * y_pixel;
					//�����±���û���Ƶ�ԭͼ���±�
					if (MAPBotom - y_Long1 >= OriginalMAPBotom) //˵����û�г���ԭͼ
					{
						MAPBotom = MAPBotom - y_Long1;
						MAPTop = MAPTop - y_Long1;
					}
					else
					{
						//������ԭͼ���±�  ���ֻ���Ƶ�ԭͼ���±�
						DOUBLE y_Long2 = 0;//��һ�����������µ���ƫ���˶���
						y_Long2 = MAPBotom - OriginalMAPBotom;
						MAPBotom = OriginalMAPBotom;
						MAPTop = MAPTop - y_Long2;
					}
				}
				else
				{
					//˵��������
					//�����ʱ��γ��ƫ����
					y_Long1 = (point.y  - y_CurLbuttonDown) * y_pixel;
					//�����ϱ���û���Ƶ�ԭͼ���ϱ�
					if (MAPTop + y_Long1 <= OriginalMAPTop) //˵����û�г���ԭͼ
					{
						MAPTop = MAPTop + y_Long1;
						MAPBotom = MAPBotom + y_Long1;
					}
					else
					{
						//������ԭͼ���ϱ�  ���ֻ���Ƶ�ԭͼ���ϱ�
						DOUBLE y_Long2 = 0;//��һ�����������µ���ƫ���˶���
						y_Long2 = OriginalMAPTop - MAPTop;
						MAPTop = OriginalMAPTop;
						MAPBotom = MAPBotom + y_Long2;
					}
				}
				
			}
			x_StartBit = (MAPLeft - OriginalMAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_StartBit = (OriginalMAPTop - MAPTop) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);
			x_BitWidth = (MAPRight - MAPLeft) / ((OriginalMAPRight - OriginalMAPLeft) / x_OriginalBit);
			y_BitWidth = (MAPTop - MAPBotom) / ((OriginalMAPTop - OriginalMAPBotom) / y_OriginalBit);

			Suspend_Thread();
			iNowZoomRunning = 1;//���ڷŴ����С
			iCountFlushMap ++; //��ͼ��ˢ�´�����һ
			while (iCountFlushExamCar)
			{
				Sleep(200);
				iCountFlushExamCar = 0;
			}
		//	Sleep(2);
		//	Invalidate(FALSE);
			HasPlayInvalite();
			//Sleep(20);
		//	iNowZoomRunning = 0;//û�зŴ����С ��仰������д������� ���ڸ�д��OnPaint����β��
			x_CurLbuttonDown = point.x;
			y_CurLbuttonDown = point.y;
			
		}
	}

	// �ж������û�������������
	if (0 == ExamCarInfo.MouseIsThisRect)
	{
// 		CString temp;
// 		temp.Format("x = %d y = %d",point.x,point.y);
// 		WriteLog(temp);
		//˵����û�������������
		if (point.x >= 700 && point.y <= 15 && point.x <= 980)
		{
			ExamCarInfo.ShowDlgState = 0; //�쳤����
			ExamCarInfo.MouseIsThisRect = 1;
		}
		
	}
	else if(1 == ExamCarInfo.MouseIsThisRect)
	{
		//�����������
		if (point.x < 700 || point.y > 640 || point.x > 980)
		{
			//˵���Ѿ��뿪���Ǹ�����
			ExamCarInfo.ShowDlgState = 1; //��������
			ExamCarInfo.MouseIsThisRect = 0;
		}
	}

	/////////////////////
	// ������һ�δ����������жϴ�ʱ�����û�����ڿ�������  
	//  m_Button[Pi].CurCar_Xpoint > 0 && m_Button[Pi].CurCar_Ypoint > 0
	int i = 0;
	for(i = 1;i < MAX_ExamCar;i++)
	{
		if(point.x >= m_Button[i].CurCar_Xpoint && point.x <= m_Button[i].CurCar_Xpoint + 20 &&  point.y <= m_Button[i].CurCar_Ypoint + 20  && point.y >= m_Button[i].CurCar_Ypoint)
		{
			// ���ð�ť���Ǹ�����  ���Ǹ���־λ��ΪTRUE
//			int((on_ExamCar[Pi].x_Gps - MAPLeft) / x_pixel),int(CurrentScrHeight - ((on_ExamCar[Pi].y_Gps - MAPBotom) / y_pixel) - 10)
			bMouseOn[i] = 1;  //����������ж������û��������������  TRUEΪ���⿼���� FALSE û����������������
		}
		else
		{
			bMouseOn[i] = 0;  //����������ж������û��������������  TRUEΪ���⿼���� FALSE û����������������
		}
	}
	//  ����о͵��ö�Ӧ����ʾ��
	/////////////////////

	CDialog::OnMouseMove(nFlags, point);
}

void CCBKDigitalMapDlg::CreateDlgha()
{
	int i;
	CWinThread *tmpCwinThread;
	//���������߳�
	ExamCarInfoToThread_ST pExamCarInfoToThread_ST;
	pExamCarInfoToThread_ST = (ExamCarInfoToThread_ST)malloc(sizeof(struct ExamCarInfoToThread));
	
	for (i = 0;i < MAX_ExamCar+1;i++)
	{
		memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
		pExamCarInfoToThread_ST->pMapDlg = this;
		pExamCarInfoToThread_ST->iExamCarNumber = i;
		tmpCwinThread = AfxBeginThread(&ExamCarGPSLocate_thd, pExamCarInfoToThread_ST);
		ThreadHandle[i] = tmpCwinThread->m_hThread;
		if (i == MAX_ExamCar )
		{
			memset(pExamCarInfoToThread_ST,0,sizeof(struct ExamCarInfoToThread));
			pExamCarInfoToThread_ST->pMapDlg = this;
			pExamCarInfoToThread_ST->iExamCarNumber = 0; //�����ĵ�һ���߳� ����ΪʲôҪ��ô����Ҳ��֪�� 
		}
		Sleep(100);
		SuspendThread(ThreadHandle[i]);  //�����߳�
	}
	/*
	Sleep(500);
	for (i = 0;i <= MAX_ExamCar;i++)
	{
		SuspendThread(ThreadHandle[i]);  //�����߳�
	}
	*/
	free(pExamCarInfoToThread_ST);

	//������������Ӧ����ʾ��
	for (i = 0;i < MAX_ExamCar;i++)
	{
		dlgha[i].Create(IDD_DIALOG1,this);
		dlgha[i].ShowWindow(SW_HIDE);
	}
//	m_boot.MoveWindow(-200, -200,100,50); (CurrentScrWidth / 2 - 150,CurrentScrHeight / 2 - 25,300,50);
	m_boot.SetWindowText("");
	CRect pCRect(CurrentScrWidth / 2 - 150,CurrentScrHeight / 2 - 25,CurrentScrWidth / 2 + 150,CurrentScrHeight / 2 + 25);
	InvalidateRect(&pCRect);
}

void CCBKDigitalMapDlg::Suspend_Thread() //�����߳�
{
	int i = 0;
	for (i = 1;i < pSocket.TotalCar;i++)
	{
		if (m_Button[i].ExamCarState == 1) //��������
		{
		//	SuspendThread(ThreadHandle[i]);
			while(!SuspendThread(ThreadHandle[i]));
		//	m_Button[i].ShowWindow(SW_HIDE) ;   ����Ч������ �Ŵ����С��ʱ�� ��ť������ְ�ɫ����
			m_Button[i].MoveWindow(-20,-20,20,20);//
		}
	}
}

void CCBKDigitalMapDlg::WakeUp_Thread()  //�����߳�
{
	int i = 0;
	int iCount = 0;
//	int iResult = 0;
//	iResult = pMutex.Unlock();// ���������߳���ʱ��ֻ��һ���̲߳�����֪��Ϊʲô
	for (i = 1;i < pSocket.TotalCar;i++)
	{
		if (m_Button[i].ExamCarState == 1) //��������
		{
			int xx = 0;
			while (xx = ResumeThread(ThreadHandle[i])); //���������û�е�0 �ͼ�������
		//	ResumeThread(ThreadHandle[i]);
		}
	}
}

void CCBKDigitalMapDlg::OnDestroy() 
{
//	int m;
	CString E;
	/*
	for (m = 0;m < MAX_ExamCar;m++)
	{
		E.Format("EXAMCAR = %d x = %lf,y = %lf",on_ExamCar[m].ExamCarNumber, on_ExamCar[m].x_Gps,on_ExamCar[m].y_Gps);
		WriteLog(E);
	}
	*/

	iContrl = 0;

	/////////����һ����Ϣ�ò�����Ƶ�������////////
	if(iISCreate == 1)
	{
		HWND hWnd = GetServerMainWnd();
		if(hWnd == NULL)
		{    
	//		return;
		}
		CWnd *pWnd = CWnd::FromHandle(hWnd);
		pWnd->SendMessage(WM_TEXTMESSAGE, 0, 0);  //��������ڹر��ӽ��̳���
	}

	if(iISCreateT == 1)
	{
		HWND hWnd = GetServerMainWndT();
		if(hWnd == NULL)
		{    
	//		return;
		}
		CWnd *pWnd = CWnd::FromHandle(hWnd);
		pWnd->SendMessage(WM_TEXTMESSAGET, 0, 0);  //��������ڹر��ӽ��̳���
	}

	if(iISCreateS == 1)
	{
		HWND hWnd = GetServerMainWndS();
		if(hWnd == NULL)
		{    
	//		return;
		}
		CWnd *pWnd = CWnd::FromHandle(hWnd);
		pWnd->SendMessage(WM_TEXTMESSAGES, 0, 0);  //��������ڹر��ӽ��̳���
	}
	/////////////////
	Sleep(10000);
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


//��������ESC����
BOOL CCBKDigitalMapDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	int iChoose = 0;
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			//MessageBox(_T("�س�������"));
			return TRUE; //����Enter��
		case VK_ESCAPE:
			//MessageBox(_T("ESC������"));
			iChoose = MessageBox("ȷ��Ҫ�˳�����ô?","�˳���ʾ",MB_YESNO);
			if (iChoose == IDYES)
			{
				//return TRUE;
				//::PostMessage(AfxGetMainWnd()->m_hWnd,WM_CLOSE,0,0);
				//this->SendMessage(WM_CLOSE);
				CDialog::OnCancel();
			}
			else
			{
				return TRUE;
			}
			break;
			//return TRUE;
		default:
			break;
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}




void CCBKDigitalMapDlg::WriteLog(CString szERR, CString car)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen(car,"a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}


BOOL CCBKDigitalMapDlg::ReadOnlineCarFromDB()
{
	
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString c_ExamCarNumber;
	CString c_TmpCarNumber;
	int iLength;
	//////////       ���´���������ʼ��map����           ///////////
	int iValue = 1;
	for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
	{
		SockToCar1[iValue] = -1;
	}
	
	//////////        ��ʼ��map��������          ///////////
	try
	{
		//	BOOL sInit=theApp.OnInitSQL();
		//	if (sInit)
		//	{
		CString strSQL;	
		VARIANT cnt;
		cnt.vt = VT_INT;
		strSQL.Format("SELECT * FROM SysCfg WHERE ״̬ = '1'");
		
		_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
			while(!pSet->adoEOF)
			{
				
				

				vat =pSet->GetCollect("��Ŀ");  //�������ǿ�����
				if (vat.vt != VT_NULL)
				{
					c_ExamCarNumber = (LPCSTR)_bstr_t(vat);
					c_TmpCarNumber = c_ExamCarNumber.Left(4);
					if (c_TmpCarNumber == "����")
					{
						// ��ȡ���������������j
						pSocket.TotalCar++;
						iLength = c_ExamCarNumber.GetLength();
						c_TmpCarNumber = c_ExamCarNumber.Right(iLength - 4);
						for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
						{
							if (SockToCar1.find(iValue)->second == -1)
						//	if(m_Button[iValue].examCarNumber == 0)
							{
								SockToCar1[iValue] = _ttoi(c_TmpCarNumber);
							//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
								break;
							}
							
						}
					}
				}
				
				
				//	pSet->MoveNext();
				
				pSet->MoveNext();
				
			}
			pSet->Close();
			//�ͷ�
			pSet.Release();
			//	}
	//		Sleep(50);
			
			
		}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}
















BOOL CCBKDigitalMapDlg::ReadPhotoFromDB(CDC *pDC, CString sCard, CRect *rc)
{
	LPVOID m_pBMPBuffer;
	_RecordsetPtr pRecordset;
	_variant_t  varBLOB;
   
	CString temp;
	try
	{
	//	BOOL sInit=theApp.OnInitSQL();
	//	if (sInit)
	//	{
			pRecordset.CreateInstance("ADODB.Recordset");
			pRecordset->CursorLocation = adUseClient;
			CString strSQL;	
	//		strSQL.Format("SELECT * FROM StudentPhoto WHERE ׼��֤����� = '%s' and ״̬ = '3'",sCard);
			strSQL.Format("SELECT * FROM StudentPhoto WHERE ׼��֤����� = '%s'",sCard);
			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
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
						pImg->Render(*pDC,rc->left,rc->top,rc->right-rc->left,rc->bottom-rc->top,0,hmHeight,hmWidth,-hmHeight,NULL);
					}
					else
					{
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
				pbuf = NULL;
			}
			else
			{
				pRecordset->Close();
				pRecordset.Release();
			}
	//	}

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



/*
��������Ĺ����������������Ƶ���ŵĻ�ˢ�µ�����Ҫ��ȥ��Ƶ��һ����
*/
BOOL CCBKDigitalMapDlg::HasPlayInvalite()
{
	if(iStateDlgShow == 0)
	{
		if(iStateDlgShow == 1) // ˵�����ʱ��Ի�������show���������ջ�ȥ
		{
			CRect pRect(0,0,700,CurrentScrHeight);
			InvalidateRect(&pRect,FALSE);
			pRect.SetRect(700,temPoint.y,980,CurrentScrHeight);
			InvalidateRect(&pRect,FALSE);
			pRect.SetRect(980,0,CurrentScrWidth,CurrentScrHeight);
			InvalidateRect(&pRect,FALSE);
		}
		else  // ˵�����ʱ��û�жԻ�������show
		{ 
			Invalidate(FALSE);
		}
		
	}
	return TRUE;
}


BOOL CALLBACK EnumWndProc(HWND hWnd, LPARAM lParam)
{
//	TCHAR szWndText[MAX_PATH];
	unsigned long dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if(dwProcId == g_ulProcessId)
	{
		if(NULL == ::GetParent(hWnd))
		{
			*(HWND*)lParam = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}


BOOL CALLBACK EnumWndProcT(HWND hWnd, LPARAM lParam)
{
//	TCHAR szWndText[MAX_PATH];
	unsigned long dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if(dwProcId == g_ulProcessIdT)
	{
		if(NULL == ::GetParent(hWnd))
		{
			*(HWND*)lParam = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CALLBACK EnumWndProcS(HWND hWnd, LPARAM lParam)
{
//	TCHAR szWndText[MAX_PATH];
	unsigned long dwProcId;
	GetWindowThreadProcessId(hWnd, &dwProcId);
	if(dwProcId == g_ulProcessIdS)
	{
		if(NULL == ::GetParent(hWnd))
		{
			*(HWND*)lParam = hWnd;
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CCBKDigitalMapDlg::OnButtonopen() 
{
	// TODO: Add your control notification handler code here
	

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//�õ�ShowPlay.exe����·��
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) 
		return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcess = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessId = pi.dwProcessId;

		iISCreate = 1; //˵���ӽ����Ѿ�����
		return TRUE;
	}
	else
	{
		WriteLog("�������ݽ���ʧ��");
		return FALSE;
	//	AfxMessageBox("��������ʧ��");
	}

		
}

HWND CCBKDigitalMapDlg::GetServerMainWnd()
{
	HWND hWnd;
	if(0 == EnumWindows((WNDENUMPROC)EnumWndProc, (LPARAM)&hWnd))
	{
		return hWnd;
	}
	return NULL;
}

HWND CCBKDigitalMapDlg::GetServerMainWndT()
{
	HWND hWnd;
	if(0 == EnumWindows((WNDENUMPROC)EnumWndProcT, (LPARAM)&hWnd))
	{
		return hWnd;
	}
	return NULL;
}

HWND CCBKDigitalMapDlg::GetServerMainWndS()
{
	HWND hWnd;
	if(0 == EnumWindows((WNDENUMPROC)EnumWndProcS, (LPARAM)&hWnd))
	{
		return hWnd;
	}
	return NULL;
}

void CCBKDigitalMapDlg::SendMessageToVideoDlg(UINT ExamCar,UINT VideoAdd)
{
	HWND hWnd;
	do
	{
		hWnd = GetServerMainWnd();
		Sleep(100);
		WriteLog("һֱѭ��");
	}
	while(hWnd == NULL);//˵����û���ҵ�������
  
WriteLog("�ɹ���2");
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGE, ExamCar, VideoAdd);  //
}

void CCBKDigitalMapDlg::SendMesageToVideoDlgClose()
{
	HWND hWnd = GetServerMainWnd();
	if(hWnd == NULL)
	{    
//		return;
	}
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGE, 0, 3);  //��������ڹر��ӽ��̳���
}

BOOL CCBKDigitalMapDlg::CreateVideoDlg()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//�õ�ShowPlay.exe����·��
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	WriteLog(strFilePath);
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcess = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessId = pi.dwProcessId;

		iISCreate = 1; //˵���ӽ����Ѿ�����
	}
	else
	{
		WriteLog("�������ݽ���ʧ��");
		return FALSE;
	//	AfxMessageBox("��������ʧ��");
	}
	return TRUE;
}


BOOL CCBKDigitalMapDlg::CreateVideoDlgT()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//�õ�ShowPlay.exe����·��
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	WriteLog(strFilePath);
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessT = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdT = pi.dwProcessId;

		iISCreateT = 1; //˵���ӽ����Ѿ�����
//		GetDlgItem(IDC_BUTTON_SHOWPLAY)->SetWindowText("����");
	}
	else
	{
		WriteLog("�������ݽ���ʧ��");
		return FALSE;
	//	AfxMessageBox("��������ʧ��");
	}
	return TRUE;
}



BOOL CCBKDigitalMapDlg::CreateVideoDlgS()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//�õ�ShowPlay.exe����·��
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	WriteLog(strFilePath);
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessS = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdS = pi.dwProcessId;

		iISCreateS = 1; //˵���ӽ����Ѿ�����
//		GetDlgItem(IDC_BUTTON_SHOWPLAY)->SetWindowText("����");
	}
	else
	{
		WriteLog("�����������ʧ��");
		return FALSE;
	//	AfxMessageBox("��������ʧ��");
	}
	return TRUE;
}

BOOL CCBKDigitalMapDlg::OnButtonopenT()
{
	// TODO: Add your control notification handler code here
	
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//�õ�ShowPlay.exe����·��
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) 
		return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessT = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdT = pi.dwProcessId;

		iISCreateT = 1; //˵���ӽ����Ѿ�����
		return TRUE;
	}
	else
	{
		WriteLog("�������ݽ���ʧ��");
	//	AfxMessageBox("��������ʧ��");
		return FALSE;
	}

	
}

BOOL CCBKDigitalMapDlg::OnButtonopenS()
{

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(STARTUPINFO));
	memset(&pi, 0, sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOW;
	si.dwFlags = STARTF_USESHOWWINDOW;
	TCHAR szCmd[MAX_PATH + 100];

	//�õ�ShowPlay.exe����·��
	CString strFilePath;
	GetModuleFileName(NULL, strFilePath.GetBuffer(MAX_PATH), MAX_PATH-1);
	strFilePath.ReleaseBuffer();
	int nPos = strFilePath.ReverseFind(_T('\\'));
	if(nPos == -1) 
		return FALSE;
	strFilePath = strFilePath.Left(nPos);
	if(strFilePath.Right(1) != _T("\\")) strFilePath += _T("\\");
	strFilePath += _T("ShowPlay.exe");
	/////////////////////////////////////////

	_tcscpy(szCmd, strFilePath);
	BOOL bSucceed = CreateProcess(NULL, szCmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if(bSucceed)
	{
		m_hProcessS = pi.hProcess;
//		((CButton*)GetDlgItem(IDC_BUTTONOPEN))->EnableWindow(FALSE);
//		((CButton*)GetDlgItem(IDC_BUTTON_MESSAGE))->EnableWindow(TRUE);
//		((CEdit*)GetDlgItem(IDC_EDIT_MESSAGE))->SetReadOnly(FALSE);
		g_ulProcessIdS = pi.dwProcessId;

		iISCreateS = 1; //˵���ӽ����Ѿ�����
		return TRUE;
	}
	else
	{
		WriteLog("�����������ʧ��");
	//	AfxMessageBox("��������ʧ��");
		return FALSE;
	}

}

void CCBKDigitalMapDlg::SendMessageToVideoDlgT(UINT ExamCar, UINT VideoAdd)
{
	HWND hWnd;
	do
	{
		hWnd = GetServerMainWndT();
		Sleep(100);
		WriteLog("һֱѭ��");
	}
	while(hWnd == NULL);//˵����û���ҵ�������
  
	WriteLog("�ɹ���2");
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGET, ExamCar, VideoAdd);  //
}

void CCBKDigitalMapDlg::SendMessageToVideoDlgS(UINT ExamCar, UINT VideoAdd)
{
	HWND hWnd;
	do
	{
		hWnd = GetServerMainWndS();
		Sleep(100);
		WriteLog("һֱѭ��");
	}
	while(hWnd == NULL);//˵����û���ҵ�������
  
	WriteLog("�ɹ���2");
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGES, ExamCar, VideoAdd);  //
}

void CCBKDigitalMapDlg::SendMesageToVideoDlgCloseT()
{
	HWND hWnd = GetServerMainWndT();
	if(hWnd == NULL)
	{    
//		return;
	}
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGET, 0, 3);  //��������ڹر��ӽ��̳���
}

void CCBKDigitalMapDlg::SendMesageToVideoDlgCloseS()
{
	HWND hWnd = GetServerMainWndS();
	if(hWnd == NULL)
	{    
//		return;
	}
	CWnd *pWnd = CWnd::FromHandle(hWnd);
	pWnd->SendMessage(WM_TEXTMESSAGES, 0, 3);  //��������ڹر��ӽ��̳���
}



BOOL CCBKDigitalMapDlg::ReadOnlineCarFromDB1()
{
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString c_ExamCarNumber;
	CString c_TmpCarNumber;
	int iLength;
	/*/////////       ���´���������ʼ��map����           ///////////
	int iValue = 1;
	for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
	{
		SockToCar1[iValue] = -1;
	}
	
	//////////        ��ʼ��map��������          //////////*/
	try
	{
		//	BOOL sInit=theApp.OnInitSQL();
		//	if (sInit)
		//	{
		CString strSQL;	
		VARIANT cnt;
		cnt.vt = VT_INT;
		strSQL.Format("SELECT * FROM SysCfg WHERE ״̬ != '1'");
		
		_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
			while(!pSet->adoEOF)
			{
				
				

				vat =pSet->GetCollect("��Ŀ");  //�������ǿ�����
				if (vat.vt != VT_NULL)
				{
					c_ExamCarNumber = (LPCSTR)_bstr_t(vat);
					c_TmpCarNumber = c_ExamCarNumber.Left(4);
					if (c_TmpCarNumber == "����")
					{
						// ��ȡ���������������j
				//		pSocket.TotalCar++;
						iLength = c_ExamCarNumber.GetLength();
						c_TmpCarNumber = c_ExamCarNumber.Right(iLength - 4);
						int iValue = 0;
						for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
						{
							if (SockToCar1.find(iValue)->second == _ttoi(c_TmpCarNumber))  //˵�����ҵ� Ҳ����˵��������������
						//	if(m_Button[iValue].examCarNumber == 0)
							{
								int tmpCar = SockToCar1[iValue];
								SockToCar1[iValue] = -1;
								pSocket.TotalCar--;
								int iValue1 = 0;
								for(iValue1 = 1; iValue1 < MAX_ExamCar;iValue1 ++)
								{
									if(m_Button[iValue1].examCarNumber == tmpCar)
									{
										m_Button[iValue1].MoveWindow(-20,-20,20,20);
										//���Ҫ�ĳɴ����߳�����ʱ��ı��־λ
										AfxBeginThread(&ExamCarHideDown_thd, &m_Button[iValue1]);

										/*
										m_Button[iValue1].examCarNumber = 0;
			
										m_Button[iValue1].CurCar_Xpoint = 0;
										m_Button[iValue1].CurCar_Ypoint = 0;

										
										m_Button[iValue1].ExamCarState == 0;

										CString tmpOnLineCar;
										tmpOnLineCar.Format("��ǰ������Ϊ��%d",pSocket.TotalCar);
										m_TotalCar.SetWindowText(tmpOnLineCar);
										*/ 
										break;
									}
								}
								
							//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
								break;
								
							}
							
						}
					}
				}
				
				
				//	pSet->MoveNext();
				
				pSet->MoveNext();
				
			}
			pSet->Close();
			//�ͷ�
			pSet.Release();
			//	}
	//		Sleep(50);
			
			
		}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}

//DEL void CCBKDigitalMapDlg::ChangeTrackVideoPlayT(HTREEITEM pVCurItem)
//DEL {
//DEL 
//DEL }

void CCBKDigitalMapDlg::DeleteLogData()
{
	CFileFind fFind;
	//�ж��ļ�MP4�Ƿ����
	//ɾ��MP4
	CString tmpFileName;
	tmpFileName.Format("��������.txt");
	if(fFind.FindFile(tmpFileName)!=0)
	{
		CFile::Remove(tmpFileName);
	}
	tmpFileName.Empty();
	tmpFileName.Format("VideoERRLog");
	if(fFind.FindFile(tmpFileName)!=0)
	{
		CFile::Remove(tmpFileName);
	}
	tmpFileName.Empty();
	tmpFileName.Format("datalog");
	if(fFind.FindFile(tmpFileName)!=0)
	{
		CFile::Remove(tmpFileName);
	}
	BOOL found = fFind.FindFile("*.clog");	
	//ɾ���ǵ������ݿ�
	while(found)
	{
		found = fFind.FindNextFile();
		tmpFileName = fFind.GetFileName();		
		CFile::Remove(tmpFileName);
	}

}

BOOL CCBKDigitalMapDlg::ReadOnlineCarFromDB2()
{
	BOOL bflag = FALSE;//�����־λ�������жϵ���������ʱ����û���ҵ� trueΪû���ҵ� ��Ҫ��� falseΪ���ҵ�����Ҫ���
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString c_ExamCarNumber;
	CString c_TmpCarNumber;
	int iLength;
	/*/////////       ���´���������ʼ��map����           ///////////
	int iValue = 1;
	for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
	{
		SockToCar1[iValue] = -1;
	}
	
	//////////        ��ʼ��map��������          //////////*/
	try
	{
		//	BOOL sInit=theApp.OnInitSQL();
		//	if (sInit)
		//	{
		CString strSQL;	
		VARIANT cnt;
		cnt.vt = VT_INT;
		strSQL.Format("SELECT * FROM SysCfg WHERE ״̬ = '1'");
		
		_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
		_variant_t vat;
		if(pSet != NULL && (!pSet->adoEOF))
		{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
			while(!pSet->adoEOF)
			{
				
				

				vat =pSet->GetCollect("��Ŀ");  //�������ǿ�����
				if (vat.vt != VT_NULL)
				{
					c_ExamCarNumber = (LPCSTR)_bstr_t(vat);
					c_TmpCarNumber = c_ExamCarNumber.Left(4);
					if (c_TmpCarNumber == "����")
					{
						// ��ȡ���������������j
				//		pSocket.TotalCar++;
						iLength = c_ExamCarNumber.GetLength();
						c_TmpCarNumber = c_ExamCarNumber.Right(iLength - 4);
						int iValue = 0;

						bflag = TRUE;
						for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
						{
							if (SockToCar1.find(iValue)->second == _ttoi(c_TmpCarNumber))  //˵�����ҵ� Ҳ����˵��������֮ǰ���Ѿ�������
						//	if(m_Button[iValue].examCarNumber == 0)
							{
								bflag = FALSE;
								break;
							}
						}

						if(bflag)  //˵����Ҫ���
						{
							for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
							{
								if (SockToCar1.find(iValue)->second == -1)
									//	if(m_Button[iValue].examCarNumber == 0)
								{
									SockToCar1[iValue] = _ttoi(c_TmpCarNumber);
									//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
									break;
								}
								
							}
								
							pSocket.TotalCar++;
							int iValue1 = 0;
							for(iValue1 = 1; iValue1 < MAX_ExamCar;iValue1 ++)
							{
								if(m_Button[iValue1].examCarNumber == 0)
								{
									m_Button[iValue1].examCarNumber = _ttoi(c_TmpCarNumber);
		
									m_Button[iValue1].CurCar_Xpoint = 0;
									m_Button[iValue1].CurCar_Ypoint = 0;
									m_Button[iValue1].ExamCarState = 1;


									CString tmpOnLineCar;
									tmpOnLineCar.Format("��ǰ������Ϊ��%d",pSocket.TotalCar);
									m_TotalCar.SetWindowText(tmpOnLineCar);  

									break;
								}
							}														
						}						
					}
				}
				
				
				//	pSet->MoveNext();
				
				pSet->MoveNext();
				
			}
			pSet->Close();
			//�ͷ�
			pSet.Release();
			//	}
	//		Sleep(50);
			
			
		}
	}
	catch (_com_error e) 
	{
// 		if(m_pBMPBuffer)
// 		{
// 			delete m_pBMPBuffer;
// 			m_pBMPBuffer = NULL;
// 		}
		//AfxMessageBox(e.Description());
		return FALSE;
	}
	
	return TRUE;
}
