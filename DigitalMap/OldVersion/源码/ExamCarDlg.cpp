// ExamCarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cbkdigitalmap.h"
#include "ExamCarDlg.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKDigitalMapApp theApp;
extern unsigned long g_ulProcessId;
#define ExamCarN0 10
#define ExamCarN0Sub 1000



UINT SerachOnLineExamCar_thd(LPVOID p);  //�����жϴ�ʱ�����߿��� ����״�б������ӻ�ɾ������
UINT RefreshCDDDlg_thd(LPVOID p);  //ˢ�¿�����Ϣ��ĵĶԻ���
UINT TrackExamCar_thd(LPVOID p);  //׷�ٿ���
/////////////////////////////////////////////////////////////////////////////
// CExamCarDlg dialog


CExamCarDlg::CExamCarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExamCarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExamCarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CExamCarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExamCarDlg)
	DDX_Control(pDX, IDC_TREE_EXAM, m_exam);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExamCarDlg, CDialog)
	//{{AFX_MSG_MAP(CExamCarDlg)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_EXAM, OnSelchangedTreeChan)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExamCarDlg message handlers

BOOL CExamCarDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_exam.MoveWindow(1,1,260,388);  
	m_exam.DeleteAllItems();
// 	m_FirstDevItem = NULL;
// 	iExamCarCount = 0;
	CurTrackExamCar = -1;  //��ǰ�����ٵĿ����� Ĭ��Ϊ-1
	
	InitMytree();  
	//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	int i = 0;
	for (i = 1;i < MAX_ExamCar ;i++)
	{
		ArryExamCarPopbox[i] = 0;//�����жϴ�ʱ����ʾ����û�п���  ���Ϊ1 ���� ���Ϊ0 ��û�п���
	}
	for (i = 1;i < MAX_ExamCar; i++)
	{
		
		if(((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].ExamCarState) //���Ϊ����
		{	
			CString tempCar;
			tempCar.Format("����%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
			m_hDevItem = m_exam.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
			m_exam.SetItemData(m_hDevItem,(ExamCarN0 * i) + 10);
			HTREEITEM ExamCarItem = m_exam.InsertItem("����׷��",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 1);   //Data��Ӧͨ���������е�����
			ExamCarItem = m_exam.InsertItem("������ʾ��",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 2);   //Data��Ӧͨ���������е�����

			/////////////
			ExamCarItem = m_exam.InsertItem("��������ʻ��Ƶ",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 3);   //Data��Ӧͨ���������е�����
			ExamCarItem = m_exam.InsertItem("��������ʻ��Ƶ",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 4);   //Data��Ӧͨ���������е�����
			ExamCarItem = m_exam.InsertItem("����������Ƶ",0,1,m_hDevItem,TVI_LAST);
			m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * i) + 5);   //Data��Ӧͨ���������е�����
		}
	}
	
	iExamCarVideo = -1; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
	iExamCarVideot = -1; 
	iExamCarVideos = -1; 

	// ���߳� �������ߵĿ�����
	AfxBeginThread(&SerachOnLineExamCar_thd, this);
	// ���߳� ˢ�¿�����Ϣ�����ʾ��
	AfxBeginThread(&RefreshCDDDlg_thd, this);  
	//׷�ٿ���
	AfxBeginThread(&TrackExamCar_thd, this);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CExamCarDlg::OnSelchangedTreeChan(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	UpdateData(true);
	CString node_name;
	node_name=m_exam.GetItemText(pNMTreeView->itemNew.hItem);  //��ȡ�ı���Ϣ

	
	HTREEITEM hSelected = m_exam.GetSelectedItem();
//	BOOL SetItemText( HTREEITEM hItem, LPCTSTR lpszItem );

	
	
	//δѡ��
	if(NULL == hSelected)
		return;
	DWORD itemData = m_exam.GetItemData(hSelected);
	HTREEITEM hParent = NULL;
	int ExamCarNumber1 = itemData/1000;    // �ڼ�������  
	int ExamCarNumber = 0;    // ��ĳ���������̺߳�  
	int ExamCarNumberiIndex = itemData%1000;  //�ڼ�������������
	CCBKDigitalMapDlg *pCBDigitalMaoDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////////    ���ҿ�������Ӧ���̺߳�  ////////////////////
	int j = 0;
	for (j = 1;j < MAX_ExamCar;j++)
	{
		if (ExamCarNumber1 == pCBDigitalMaoDlg->SockToCar1.find(j)->second)
		{
			//˵���ҵ��� 
			ExamCarNumber = j;
			break;
		}
	}
	/////////////////////////////////////////////////////////////
	switch(ExamCarNumber)
	{
	case 1://��һ������
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}

					ChangeTrack(hSelected);  
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
				{
					//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
					
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}

				break;
			
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 2:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 3:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 4:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 5:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);

						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 6:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 7:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 8:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 9:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
					
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 10:  //��10������
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
					
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 11:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 12:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 13:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 14:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 15:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 16:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 17:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 18:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 19:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 20:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 21:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 22:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 23:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 24:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 25:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 26:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 27:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 28:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 29:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 30:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 31:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 32:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 33:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 34:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 35:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 36:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 37:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 38:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	case 39:
	case 40:
	case 41:
	case 42:
	case 43:
	case 44:
	case 45:
	case 46:
	case 47:
	case 48:
	case 49:
	case 50:
	case 51:
	case 52:
	case 53:
	case 54:
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
		{
			switch(ExamCarNumberiIndex)
			{
			case 1: //����׷��
				if (node_name == "����׷��")  //˵����ʱ�Ѿ�����׷����
				{
					if(CurTrackExamCar > 0)
					{
						pCCBKDigitalMapDlg->m_Button[CurTrackExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					ChangeTrack(hSelected);
					m_exam.SetItemText(hSelected,"ȡ��׷��");
					CurTrackExamCar = ExamCarNumber;
				}
				else if (node_name == "ȡ��׷��")  //˵����ʱ�Ѿ�����׷����
				{
					m_exam.SetItemText(hSelected,"����׷��");
					CurTrackExamCar = -1;
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->m_Button[ExamCarNumber].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
				}
				break;
			case 2: //������ʾ��
				if (node_name == "������ʾ��")  //˵����ʱ�Ѿ�������
				{
					m_exam.SetItemText(hSelected,"ȡ����ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 1;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = TRUE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				else if (node_name == "ȡ����ʾ��")
				{
					m_exam.SetItemText(hSelected,"������ʾ��");
					ArryExamCarPopbox[ExamCarNumber] = 0;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].m_bHover = FALSE;
					pCBDigitalMaoDlg->m_Button[ExamCarNumber].Invalidate(FALSE);
				}
				break;
			case 3: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						ChangeTrackVideoPlay(hSelected);
						m_exam.SetItemText(hSelected,"�ر�����ʻ��Ƶ");
						if(iExamCarVideo == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideo(ExamCarNumber,1);
							
						}
						iExamCarVideo = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�ر�����ʻ��Ƶ")
					{
						CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideo = -1;
					}
				}
				break;	
			case 4: //��������ʻ��Ƶ
				{
					if (node_name == "��������ʻ��Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayT(hSelected);
						m_exam.SetItemText(hSelected,"�رո���ʻ��Ƶ");
						if(iExamCarVideot == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoT(ExamCarNumber,2);

						}
						iExamCarVideot = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رո���ʻ��Ƶ")
					{
						CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"��������ʻ��Ƶ"); 
						iExamCarVideot = -1;
					}
				}
				break;	
			case 5: //��������ʻ��Ƶ
				{
					if (node_name == "����������Ƶ")  //˵����ʱ�Ѿ�������   m_bHover =TRUE;	Invalidate(FALSE);
					{
						//	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
						
						ChangeTrackVideoPlayS(hSelected);
						m_exam.SetItemText(hSelected,"�رճ�����Ƶ");
						if(iExamCarVideos == -1)
						{
							//˵��ֻҪ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);
						}
						else
						{	
							//�ȹر���Ƶ�����Ƶ
							CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
							OpenExamCarVideoS(ExamCarNumber,3);

						}
						iExamCarVideos = ExamCarNumber; //��ǿ�����Ƶ��û�п��� ���Ϊ-1���ʾû�п��� Ϊ������ִ�������
					}
					else if (node_name == "�رճ�����Ƶ")
					{
						CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						m_exam.SetItemText(hSelected,"����������Ƶ"); 
						iExamCarVideos = -1;
					}
				}
				break;	
			}
		}
		break;
	default:
		break;
		
	}
	//////////////////    ����ѡ�񱻸��ٵĿ��� ʹ�俼���ñ����ɫ��ʾ      /////////////////////////
	int i = 0;
	for (i = 1;i < MAX_ExamCar;i++)
	{
		pCBDigitalMaoDlg->m_Button[i].bTrack = FALSE;	
	}
	if (CurTrackExamCar != -1)  //˵���п���������
	{
		pCBDigitalMaoDlg->m_Button[CurTrackExamCar].bTrack = TRUE;
	}
	
	//////////////////         ����       /////////////////////////
	
	hParent = m_exam.GetParentItem(hSelected);
	m_exam.SelectItem(hParent);
//	m_exam.SetItemState(hSelected, NULL, NULL);
	UpdateData(false);
	
	*pResult = 0;
}



BOOL CExamCarDlg::InitMytree()
{
	//�ڵ��ͼ��
	int i=0;
	int i_count=2;
	//����ͼ��
	HICON icon[4];
	icon[0] = AfxGetApp()->LoadIcon (IDI_ICON1);
	icon[1] = AfxGetApp()->LoadIcon (IDI_ICON2);
	
	//����ͼ���б�ؼ�
	m_imagelist=new CImageList;
	m_imagelist->Create(16,16,0,7,7);
	m_imagelist->SetBkColor (RGB(255,255,255));
	for(int n=0;n<2;n++)
	{
		m_imagelist->Add(icon[n]); //��ͼ������ͼ���б�ؼ�
	}
	m_exam.SetImageList(m_imagelist,TVSIL_NORMAL); //Ϊm_mytree����һ��ͼ���б�ʹCtreeCtrl�Ľڵ���ʾ��ͬ��ͼ�� 
	m_exam.SetBkColor(RGB(160,210,230));//����m_mytree�ı���ɫ
	
	return true;
}

UINT SerachOnLineExamCar_thd(LPVOID p)  //ʵʱˢ�����߿���
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CExamCarDlg *pExamCarDlg = (CExamCarDlg *)p;
	
	int i = 0;
	for (i = 0;i < MAX_ExamCar; i++) //��ʼ������״̬Ϊ����  ���߿���Ϊ1
	{
		pExamCarDlg->ArryExamCar[i] = FALSE;
	}
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);

	while (pCCBKDigitalMapDlg->iContrl)
	{
		for (i = 1; i < MAX_ExamCar;i++)
		{
			if (pCCBKDigitalMapDlg->m_Button[i].ExamCarState != pExamCarDlg->ArryExamCar[i]) //˵�����߿�����ˢ��
			{
				//�ж��ǿ������߻�������
				if (FALSE == pExamCarDlg->ArryExamCar[i]) //˵���ǿ�������
				{
					//����״�б���ӿ���
					CString tempCar;
					tempCar.Format("����%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
					pExamCarDlg->m_hDevItem = pExamCarDlg->m_exam.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(pExamCarDlg->m_hDevItem,(ExamCarN0 * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)));
					HTREEITEM ExamCarItem = pExamCarDlg->m_exam.InsertItem("����׷��",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 1);   //Data��Ӧͨ���������е�����
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("������ʾ��",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 2);   //Data��Ӧͨ���������е�����

					/////////
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("��������ʻ��Ƶ",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 3);   //Data��Ӧͨ���������е�����
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("��������ʻ��Ƶ",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 4);   //Data��Ӧͨ���������е�����
					ExamCarItem = pExamCarDlg->m_exam.InsertItem("����������Ƶ",0,1,pExamCarDlg->m_hDevItem,TVI_LAST);
					pExamCarDlg->m_exam.SetItemData(ExamCarItem,(ExamCarN0Sub * (pCCBKDigitalMapDlg->m_Button[i].examCarNumber)) + 5);   //Data��Ӧͨ���������е�����
					/////////

					pExamCarDlg->ArryExamCar[i] = TRUE;

				}
				else
				{
					//˵����������
					//����״�б���ɾ������
					// Ҫ���ҵ�������Ϣ������
					HTREEITEM pCurItem;
					HTREEITEM temSub;
					CString szItemText;
					CString tmpItemText;
 					szItemText.Format("����%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
 					pCurItem = pExamCarDlg->m_exam.GetFirstVisibleItem( );
 					tmpItemText = pExamCarDlg->m_exam.GetItemText(pCurItem );
 					while(szItemText != tmpItemText)
 					{
 						pCurItem = pExamCarDlg->m_exam.GetNextSiblingItem(pCurItem);
 						tmpItemText = pExamCarDlg->m_exam.GetItemText(pCurItem );
 					}
 					temSub = pExamCarDlg->m_exam.GetChildItem( pCurItem );
 					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
 					CString szText;
 					szText = pExamCarDlg->m_exam.GetItemText(temSub);
 					if (szText == "ȡ����ʾ��")
 					{
 						pExamCarDlg->m_exam.SetItemText(temSub,"������ʾ��");
 					}
 					pExamCarDlg->ArryExamCarPopbox[i] = 0;
					
// 					pCCBKDigitalMapDlg->m_Button[i].m_bHover = FALSE;
// 					pCCBKDigitalMapDlg->m_Button[i].Invalidate(FALSE);
					pCCBKDigitalMapDlg->dlgha[i].ShowWindow(SW_HIDE);
//					pCCBKDigitalMapDlg->Invalidate(FALSE);  ����ע��
/*					Sleep(500);*/

					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
 					szText = pExamCarDlg->m_exam.GetItemText(temSub);
					if (szText == "�ر�����ʻ��Ƶ")
					{
						pExamCarDlg->m_exam.SetItemText(temSub,"��������ʻ��Ƶ");
						//����һ����Ϣ
						pExamCarDlg->CloseExamCarVideo();  //�ر�ʵʱԤ����Ƶ
						
						pExamCarDlg->iExamCarVideo = -1;
 					}

					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
					szText = pExamCarDlg->m_exam.GetItemText(temSub);
					if (szText == "�رո���ʻ��Ƶ")
					{
						pExamCarDlg->m_exam.SetItemText(temSub,"��������ʻ��Ƶ");
						//����һ����Ϣ
						pExamCarDlg->CloseExamCarVideoT();  //�ر�ʵʱԤ����Ƶ
						
						pExamCarDlg->iExamCarVideot = -1;
 					}

					temSub = pExamCarDlg->m_exam.GetNextSiblingItem( temSub );
					szText = pExamCarDlg->m_exam.GetItemText(temSub);
					if (szText == "�رճ�����Ƶ")
					{
						pExamCarDlg->m_exam.SetItemText(temSub,"����������Ƶ");
						//����һ����Ϣ
						pExamCarDlg->CloseExamCarVideoS();  //�ر�ʵʱԤ����Ƶ
						
						pExamCarDlg->iExamCarVideos = -1;
 					}


					pExamCarDlg->m_exam.DeleteItem(pCurItem);
					pExamCarDlg->ArryExamCar[i] = FALSE;
					if (pExamCarDlg->CurTrackExamCar == i)
					{
						pExamCarDlg->CurTrackExamCar = -1;
						pCCBKDigitalMapDlg->isRunningUpdate = 0; //�����Ҫ���ߵĿ������Ǳ����ٵĿ��� ��Ҫ������һ������ ֹͣˢ��
					}
					pCCBKDigitalMapDlg->m_Button[i].m_bHover = FALSE;
					pExamCarDlg->ArryExamCarPopbox[i] = 0;
				}
			}
		}
		Sleep(300);
	}
	pCCBKDigitalMapDlg->WriteLog("���� SerachOnLineExamCar_thd");
	return 0;
}


UINT RefreshCDDDlg_thd(LPVOID p)  //ʵʱˢ�����߿�������Ϣ��ʾ��
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CExamCarDlg *pExamCarDlg = (CExamCarDlg *)p;
	BOOL ArryExamCar[MAX_ExamCar];
	int i = 0;
	for (i = 0;i < MAX_ExamCar; i++) //��ʼ������״̬Ϊ����  ���߿���Ϊ1
	{
		ArryExamCar[i] = FALSE;
	}
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	
	while (pCCBKDigitalMapDlg->iContrl)
	{
		for (i = 1; i < MAX_ExamCar;i++)
		{
			//�������һ������Ľ�� ���� ���ǵ��Ŵ���ƶ�����ʱ�������ʱĳ�������Ѿ��Ѿ�������ʾ��Ȼ�󿼳����Ƴ�����Ļ֮��
			// ���Ǹ��Ի����������� ���˼·Ϊ�жϸõ���û������Ļ�⣬����о���ʱ�ر� ��ʾ�� ˢ�� �ڿ��� ��ˢ��
			if (pExamCarDlg->ArryExamCarPopbox[i])  //�����ʱ������ʾ��
			{
				if (pCCBKDigitalMapDlg->MAPLeft > pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps || pCCBKDigitalMapDlg->MAPRight < pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps || pCCBKDigitalMapDlg->MAPTop < pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps || pCCBKDigitalMapDlg->MAPBotom > pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps )
				{
					//˵����ʱ�Ѿ��ÿ����Ѿ�����Ļ֮����
// 					pCCBKDigitalMapDlg->m_Button[i].m_bHover = FALSE;
// 					pCCBKDigitalMapDlg->m_Button[i].ShowWindow(SW_HIDE);
// 					pCCBKDigitalMapDlg->m_Button[i].Invalidate(FALSE);
					pCCBKDigitalMapDlg->dlgha[i].ShowWindow(SW_HIDE);
//					pCCBKDigitalMapDlg->Invalidate(FALSE);
				}

				if (pCCBKDigitalMapDlg->m_Button[i].m_bHover == FALSE)
				{
					pCCBKDigitalMapDlg->m_Button[i].m_bHover = TRUE;
				}
				pCCBKDigitalMapDlg->m_Button[i].Invalidate(FALSE);
			}
			Sleep(20);
		}
	//	Sleep(0);
	}
	pCCBKDigitalMapDlg->WriteLog("���� RefreshCDDDlg_thd");
	return 0;
}


//׷�ٿ���
UINT TrackExamCar_thd(LPVOID p) 
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CExamCarDlg *pExamCarDlg = (CExamCarDlg *)p;
//	int i;
	DOUBLE x_TOLeft = 0;
	DOUBLE y_ToTop = 0;
	DOUBLE Left_Latitude = 0;//��Ե�γ��
	DOUBLE Top_Long = 0;//��Եľ���
	DOUBLE Right_Latitude = 0;//��Ե�γ��
	DOUBLE Bottom_Long = 0;//��Եľ���
	
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	int tmpExamCar = -1;
	DOUBLE x_Pixel = pCCBKDigitalMapDlg->x_pixel;  //�����ʱ�ĵ�ͼ���� ��Ҫ���������ͼ�Ŵ����Сʱ���¶�λ����������
	DOUBLE X_Left = pCCBKDigitalMapDlg->MAPLeft;//��ŵ�ǰ��Ļ��ߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
	DOUBLE X_Right = pCCBKDigitalMapDlg->MAPRight;//��ŵ�ǰ��Ļ�ұߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
	DOUBLE Y_Top = pCCBKDigitalMapDlg->MAPTop;//��ŵ�ǰ��Ļ�ϱߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
	DOUBLE Y_Bottom = pCCBKDigitalMapDlg->MAPBotom;//��ŵ�ǰ���±ߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
	int xState = 0;  //������Ļ�϶�ʱ 0 ��Ļû���϶� 1 ��Ļ���϶�
	int xState1 = 0;// �����������Ǵ�ʱ�����ֶ��϶���ͼ�����Զ��ƶ���ͼ�� ���Ϊ1 �Զ��ƶ���ͼ 0���ֶ��ƶ���ͼ

	DOUBLE OriginalLeft; //��ŵ�ǰ׷�ٺ����Ļ�ľ��Ⱥ�γ��
	DOUBLE OriginalRight;
	DOUBLE OriginalTop;
	DOUBLE OriginalBottom;
	static CBrush brush(RGB(255,0,0));
	SetClassLong(pCCBKDigitalMapDlg->m_hWnd,GCL_HBRBACKGROUND,(LONG)(HBRUSH)brush);

	while (pCCBKDigitalMapDlg->iContrl)
	{
	//	CString PPP1;
	//	PPP1.Format("%d  followed",pExamCarDlg->CurTrackExamCar);
	//	pCCBKDigitalMapDlg->WriteLog(PPP1);
		if (pExamCarDlg->CurTrackExamCar != -1)  //˵���п���Ҫ������
		{
			/*
			CString PPP;
			PPP.Format("%d  examination of car being followed",pExamCarDlg->CurTrackExamCar);
			pCCBKDigitalMapDlg->WriteLog(PPP);
			*/
			if (tmpExamCar != pExamCarDlg->CurTrackExamCar)
			{
				xState1 = 0;
			}
			//�����жϴ�ʱ�ĵ�ͼ�ǲ���ԭͼ �����ԭͼ�Ͳ���������������������
			if (pCCBKDigitalMapDlg->MAPLeft != pCCBKDigitalMapDlg->OriginalMAPLeft || pCCBKDigitalMapDlg->MAPRight != pCCBKDigitalMapDlg->OriginalMAPRight)
			{
				pCCBKDigitalMapDlg->WriteLog("PPP2");
				if (X_Left != pCCBKDigitalMapDlg->MAPLeft || X_Right != pCCBKDigitalMapDlg->MAPRight || Y_Top != pCCBKDigitalMapDlg->MAPTop || Y_Bottom != pCCBKDigitalMapDlg->MAPBotom)
				{
					if (xState1 == 0)
					{
						X_Left = pCCBKDigitalMapDlg->MAPLeft;//��ŵ�ǰ��Ļ��ߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
						X_Right = pCCBKDigitalMapDlg->MAPRight;//��ŵ�ǰ��Ļ�ұߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
						Y_Top = pCCBKDigitalMapDlg->MAPTop;//��ŵ�ǰ��Ļ�ϱߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
						Y_Bottom = pCCBKDigitalMapDlg->MAPBotom;//��ŵ�ǰ���±ߵ�γ��ֵ ��Ҫ���������ͼ������϶��򴥷����¶�λ
						xState = 1;
					}

				}
				//˵����ͼ�зŴ�
				//�ж��ǲ�������׷�ٵĿ��� ������������½��ÿ�����������Ļ�м俿 
				if (tmpExamCar != pExamCarDlg->CurTrackExamCar || x_Pixel != pCCBKDigitalMapDlg->x_pixel || xState == 1) //˵����������׷�ٵĿ��� ���� ��ʱ�ĵ�ͼ�б��Ŵ����С��
				{
					xState = 0;
					tmpExamCar = pExamCarDlg->CurTrackExamCar;
					x_Pixel = pCCBKDigitalMapDlg->x_pixel;
					//�����Ľ���������Ļ�����ƶ�
					//�����ֵ�ľ���γ��
					Left_Latitude = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].x_Gps - ((pCCBKDigitalMapDlg->CurrentScrWidth) / 2) * pCCBKDigitalMapDlg->x_pixel;  //�������Ե���Ļ���γ��ֵ
					Top_Long = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].y_Gps + ((pCCBKDigitalMapDlg->CurrentScrHeight) / 2) * pCCBKDigitalMapDlg->y_pixel;
					Right_Latitude = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].x_Gps + ((pCCBKDigitalMapDlg->CurrentScrWidth) / 2) * pCCBKDigitalMapDlg->x_pixel;  //�������Ե���Ļ���γ��ֵ
					Bottom_Long = pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].y_Gps - ((pCCBKDigitalMapDlg->CurrentScrHeight) / 2) * pCCBKDigitalMapDlg->y_pixel;
					if (Left_Latitude >= pCCBKDigitalMapDlg->OriginalMAPLeft && Right_Latitude <= pCCBKDigitalMapDlg->OriginalMAPRight) //���ڵ�ͼ�ķ�Χ�� x����������м���ʾ
					{
						pCCBKDigitalMapDlg->MAPLeft = Left_Latitude;
						pCCBKDigitalMapDlg->MAPRight = Right_Latitude;
						X_Left = pCCBKDigitalMapDlg->MAPLeft;
					}
					else
					{
						if (Left_Latitude >= pCCBKDigitalMapDlg->OriginalMAPLeft) //˵���ұ��Ѿ������� ���ұ�ȷ��x������Ļ��ֵ
						{
							pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->OriginalMAPRight;
							pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPRight - pCCBKDigitalMapDlg->CurrentScrWidth *  pCCBKDigitalMapDlg->x_pixel;
						}
						else //˵��������Ѿ�������
						{
							pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->OriginalMAPLeft;
							pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPLeft + pCCBKDigitalMapDlg->CurrentScrWidth * pCCBKDigitalMapDlg->x_pixel;
						}
					}

					pCCBKDigitalMapDlg->WriteLog("PPP3");
					if (Bottom_Long >= pCCBKDigitalMapDlg->OriginalMAPBotom && Top_Long <= pCCBKDigitalMapDlg->OriginalMAPTop) //���ڵ�ͼ�ķ�Χ�� y����������м���ʾ
					{
						pCCBKDigitalMapDlg->MAPBotom = Bottom_Long;
						pCCBKDigitalMapDlg->MAPTop = Top_Long;
					}
					else
					{
						if (Bottom_Long >= pCCBKDigitalMapDlg->OriginalMAPBotom) //˵���ϱ��Ѿ������� ���ϱ�ȷ��y������Ļ��ֵ
						{
							pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->OriginalMAPTop;
							pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->CurrentScrHeight * pCCBKDigitalMapDlg->y_pixel;
						}
						else //˵�����±��Ѿ�������
						{
							pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->OriginalMAPBotom;
							pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPBotom + pCCBKDigitalMapDlg->CurrentScrHeight * pCCBKDigitalMapDlg->y_pixel;
						}
					}

					pCCBKDigitalMapDlg->x_StartBit = (pCCBKDigitalMapDlg->MAPLeft - pCCBKDigitalMapDlg->OriginalMAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
					pCCBKDigitalMapDlg->y_StartBit = (pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->MAPTop) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
					pCCBKDigitalMapDlg->x_BitWidth = (pCCBKDigitalMapDlg->MAPRight - pCCBKDigitalMapDlg->MAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
					pCCBKDigitalMapDlg->y_BitWidth = (pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->MAPBotom) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
					OriginalLeft = pCCBKDigitalMapDlg->MAPLeft;
					OriginalRight = pCCBKDigitalMapDlg->MAPRight;
					OriginalTop = pCCBKDigitalMapDlg->MAPTop;
					OriginalBottom = pCCBKDigitalMapDlg->MAPBotom;
					/*////////////////////////////////////////////////////////
					while (pCCBKDigitalMapDlg->isRunningUpdate);
					/////////////////////////////////////////////////////////*/
					Sleep(50);
					
					pCCBKDigitalMapDlg->isRunningUpdate = 1;
				//	pCCBKDigitalMapDlg->Suspend_Thread(); //�����߳�
//					Sleep(30);
//					pCCBKDigitalMapDlg->WriteLog("PPP4");
//					pCCBKDigitalMapDlg->Invalidate(FALSE);
//					Sleep(30); 
					pCCBKDigitalMapDlg->isRunningUpdate = 0;
					pCCBKDigitalMapDlg->WriteLog("PPP5");

				}
				else
				{
					//˵����ʱû�жԵ�ͼ�Ŵ� �� �����Լ�������������
					//���´�ʱ�õ�����Ļ�ϵ�λ��  ������û�г�����Ļ�İٷ�֮��ʮ
					pCCBKDigitalMapDlg->WriteLog("PPP2A");
					int x_Cur = 0;  //��ǰ����Ļ������
					int y_Cur = 0;
					int iLeft,iRight,iTop,iBottom;
					DOUBLE x_Latitude1; // γ��Ҫƫ�Ƶ����ֵ
					DOUBLE y_Long1; // ����Ҫƫ�Ƶ����ֵ
					iLeft = pCCBKDigitalMapDlg->CurrentScrWidth * 0.5;
					iRight = pCCBKDigitalMapDlg->CurrentScrWidth - pCCBKDigitalMapDlg->CurrentScrWidth * 0.5;
					iTop = pCCBKDigitalMapDlg->CurrentScrHeight * 0.5;
					iBottom = pCCBKDigitalMapDlg->CurrentScrHeight - pCCBKDigitalMapDlg->CurrentScrHeight * 0.5;
					x_Cur = (pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
					y_Cur = (pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->on_ExamCar[tmpExamCar].y_Gps) / pCCBKDigitalMapDlg->y_pixel;
					if (x_Cur < iLeft) //˵����ʱҪ�ѻ������ұ��ƶ� 
					{
						//�����ʱ������Ѿ�����������򲻴��� ���������ұ��ƶ�
						if (pCCBKDigitalMapDlg->MAPLeft > pCCBKDigitalMapDlg->OriginalMAPLeft)
						{
							//���������ƶ�  �ƶ���ֵΪ ������ ��һ�����ƶ����Ƶ��˵�ͼ������� ��һ����û���ƶ��������
							x_Latitude1 = (iLeft - x_Cur) * pCCBKDigitalMapDlg->x_pixel;
							if (pCCBKDigitalMapDlg->MAPLeft - x_Latitude1 >= pCCBKDigitalMapDlg->OriginalMAPLeft) //˵�����������ƶ�
							{
					//			pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPLeft - x_Latitude1;
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPRight - x_Latitude1;
							}
							else //˵���ƶ�������ô���ˣ���Ϊ�Ѿ����������
							{
					//			pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								x_Latitude1 = pCCBKDigitalMapDlg->MAPLeft - pCCBKDigitalMapDlg->OriginalMAPLeft;
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->OriginalMAPLeft;
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPRight - x_Latitude1;
							}
						}
					}
					if (x_Cur > iRight) //˵����ʱҪ�ѻ���������ƶ� 
					{
						//�����ʱ���ұ��Ѿ��������ұ��򲻴��� ������������ƶ�
						if (pCCBKDigitalMapDlg->MAPRight < pCCBKDigitalMapDlg->OriginalMAPRight)
						{
							//���������ƶ�  �ƶ���ֵΪ ������ ��һ�����ƶ����Ƶ��˵�ͼ�����ұ� ��һ����û���ƶ��������
							x_Latitude1 = (x_Cur - iRight) * pCCBKDigitalMapDlg->x_pixel;
							if (pCCBKDigitalMapDlg->MAPRight + x_Latitude1 <= pCCBKDigitalMapDlg->OriginalMAPRight) //˵�����������ƶ�
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->MAPRight + x_Latitude1;
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPLeft + x_Latitude1;
							}
							else //˵���ƶ�������ô���ˣ���Ϊ�Ѿ��������ұ�
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								x_Latitude1 = pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->MAPRight;
								pCCBKDigitalMapDlg->MAPRight = pCCBKDigitalMapDlg->OriginalMAPRight;
								pCCBKDigitalMapDlg->MAPLeft = pCCBKDigitalMapDlg->MAPLeft + x_Latitude1;
							}
						}
					}
					if (y_Cur > iBottom) //˵����ʱҪ�ѻ������ϱ��ƶ� 
					{
						//�����ʱ���±��Ѿ��������±��򲻴��� ���������ϱ��ƶ�
						if (pCCBKDigitalMapDlg->MAPBotom > pCCBKDigitalMapDlg->OriginalMAPBotom)
						{
							//���������ƶ�  �ƶ���ֵΪ ������ ��һ�����ƶ����Ƶ��˵�ͼ�����±� ��һ����û���ƶ������±�
							y_Long1 = (y_Cur - iBottom) * pCCBKDigitalMapDlg->y_pixel;
							if (pCCBKDigitalMapDlg->MAPBotom - y_Long1 >= pCCBKDigitalMapDlg->OriginalMAPBotom) //˵�����������ƶ�
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPBotom - y_Long1;
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPTop - y_Long1;
							}
							else //˵���ƶ�������ô���ˣ���Ϊ�Ѿ��������ұ�
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								y_Long1 = pCCBKDigitalMapDlg->MAPBotom - pCCBKDigitalMapDlg->OriginalMAPBotom;
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->OriginalMAPBotom;
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPTop - y_Long1;
							}
						}
					}
					if (y_Cur < iTop) //˵����ʱҪ�ѻ������±��ƶ� 
					{
						//�����ʱ���±��Ѿ��������ϱ��򲻴��� ���������±��ƶ�
						if (pCCBKDigitalMapDlg->MAPTop < pCCBKDigitalMapDlg->OriginalMAPTop)
						{
							//���������ƶ�  �ƶ���ֵΪ ������ ��һ�����ƶ����Ƶ��˵�ͼ�����ϱ� ��һ����û���ƶ������ϱ�
							y_Long1 = (iTop - y_Cur) * pCCBKDigitalMapDlg->y_pixel;
							if (pCCBKDigitalMapDlg->MAPTop + y_Long1 <= pCCBKDigitalMapDlg->OriginalMAPTop) //˵�����������ƶ�
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->MAPTop + y_Long1;
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPBotom + y_Long1;
							}
							else //˵���ƶ�������ô���ˣ���Ϊ�Ѿ��������ұ�
							{
						//		pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
								y_Long1 = pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->MAPTop;
								pCCBKDigitalMapDlg->MAPTop = pCCBKDigitalMapDlg->OriginalMAPTop;
								pCCBKDigitalMapDlg->MAPBotom = pCCBKDigitalMapDlg->MAPBotom + y_Long1;
							}
						}
					}
					pCCBKDigitalMapDlg->WriteLog("PPP2B");
					if (OriginalLeft != pCCBKDigitalMapDlg->MAPLeft || OriginalRight != pCCBKDigitalMapDlg->MAPRight || OriginalTop != pCCBKDigitalMapDlg->MAPTop || OriginalBottom != pCCBKDigitalMapDlg->MAPBotom)
					{
						xState1 = 1;
						pCCBKDigitalMapDlg->x_StartBit = (pCCBKDigitalMapDlg->MAPLeft - pCCBKDigitalMapDlg->OriginalMAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
						pCCBKDigitalMapDlg->y_StartBit = (pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->MAPTop) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
						pCCBKDigitalMapDlg->x_BitWidth = (pCCBKDigitalMapDlg->MAPRight - pCCBKDigitalMapDlg->MAPLeft) / ((pCCBKDigitalMapDlg->OriginalMAPRight - pCCBKDigitalMapDlg->OriginalMAPLeft) / pCCBKDigitalMapDlg->x_OriginalBit);
						pCCBKDigitalMapDlg->y_BitWidth = (pCCBKDigitalMapDlg->MAPTop - pCCBKDigitalMapDlg->MAPBotom) / ((pCCBKDigitalMapDlg->OriginalMAPTop - pCCBKDigitalMapDlg->OriginalMAPBotom) / pCCBKDigitalMapDlg->y_OriginalBit);
					
						pCCBKDigitalMapDlg->WriteLog("PPP6");
						/*/////////////////һ�´��뱾�����еģ������ֳ���һֱ��ѭ����������� ���ԾͰ���ȥ��
						
						while (pCCBKDigitalMapDlg->isRunningUpdate);
						
						////////////////////////////////////////////////*/
						pCCBKDigitalMapDlg->WriteLog("PPP7");

						Sleep(50);
						pCCBKDigitalMapDlg->isRunningUpdate = 1;
					//	pCCBKDigitalMapDlg->Suspend_Thread(); //�����߳�
//						Sleep(30);
//						pCCBKDigitalMapDlg->Invalidate(FALSE);
//						Sleep(50);
						pCCBKDigitalMapDlg->isRunningUpdate = 0;
						OriginalLeft = pCCBKDigitalMapDlg->MAPLeft;
						OriginalRight = pCCBKDigitalMapDlg->MAPRight;
						OriginalTop = pCCBKDigitalMapDlg->MAPTop;
						OriginalBottom = pCCBKDigitalMapDlg->MAPBotom;
						Sleep(20);
				//		Sleep(1000);
						
					}
					if (pCCBKDigitalMapDlg->MAPLeft == pCCBKDigitalMapDlg->OriginalMAPLeft || pCCBKDigitalMapDlg->MAPRight == pCCBKDigitalMapDlg->OriginalMAPRight || pCCBKDigitalMapDlg->MAPBotom == pCCBKDigitalMapDlg->OriginalMAPBotom || pCCBKDigitalMapDlg->MAPTop == pCCBKDigitalMapDlg->OriginalMAPTop)
					{
						pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}
					else
					{
						pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 1;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����
					}

					pCCBKDigitalMapDlg->WriteLog("PPP2C");
					if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar != -1)
					{
/*						
						for (i = 1;i < MAX_ExamCar;i++)
						{
							if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.CurTrackExamCar != -1)
							{
								if (pCCBKDigitalMapDlg->m_Button[i].ExamCarState == 1) //˵����������
								{
									pCCBKDigitalMapDlg->m_Button[i].Cur_xPointRelyOnParent = (pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel;
									pCCBKDigitalMapDlg->m_Button[i].Cur_yPointRelyOnParent = pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel);
									if (pCCBKDigitalMapDlg->m_Button[i].isCircle == 1)//˵��������
									{
										pCCBKDigitalMapDlg->m_Button[i].MoveWindow(pCCBKDigitalMapDlg->CurrentScrWidth / 2, pCCBKDigitalMapDlg->CurrentScrHeight / 2,20,20);
									}
									else
									{

										while (pCCBKDigitalMapDlg->isRunningUpdate);
										pCCBKDigitalMapDlg->isRunningUpdate = 1;
										Sleep(50);
										pCCBKDigitalMapDlg->m_Button[i].MoveWindow(int((pCCBKDigitalMapDlg->on_ExamCar[i].x_Gps - pCCBKDigitalMapDlg->MAPLeft) / pCCBKDigitalMapDlg->x_pixel),int(pCCBKDigitalMapDlg->CurrentScrHeight - ((pCCBKDigitalMapDlg->on_ExamCar[i].y_Gps - pCCBKDigitalMapDlg->MAPBotom) / pCCBKDigitalMapDlg->y_pixel)),20,20);
										Sleep(50);
										pCCBKDigitalMapDlg->isRunningUpdate = 0;

									}
								}
							}
							else
							{
								break;
							}
						}
*/
					//	pCCBKDigitalMapDlg->WakeUp_Thread();  //�����߳�
						
					}


										
				}
			}
		}
 		else
 		{
 			pCCBKDigitalMapDlg->m_Button[tmpExamCar].isCircle = 0;//�Ƿ������ĵ� 1 Ϊ�� 0 Ϊ����  ����������
			Sleep(100);
 		}
		Sleep(10);
	}
	pCCBKDigitalMapDlg->WriteLog("���� TrackExamCar_thd");
	return 0;
}

void CExamCarDlg::ChangeTrack(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //��ǰ����ĸ���
	HTREEITEM temSub; //�����ȡ
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //��õ�һ��
	while (tem != NULL) //������ؿ�˵���Ѿ�����β����  �˳�
	{
		if (tem != tem1) //��ʱ��ȡ�ĸ����ѡ����Ǹ�����
		{
			//�鿴��ʱ׷�ٵ�״̬�ǿ�������ȡ��
			//��ȡ����ĵ�һ��
			temSub = m_exam.GetChildItem( tem );
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "ȡ��׷��")
			{
				m_exam.SetItemText(temSub,"����׷��");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}


}
//
BOOL CExamCarDlg::OpenExamCarVideo(UINT ExamCarNo,UINT VideoAdd)
{	
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	if(pCbkdlg->iISCreate == 0)
	{
		//˵����û�д���
			BOOL bRET = FALSE;
			bRET = pCbkdlg->CreateVideoDlg();
			if(bRET == FALSE)
				return FALSE;

			/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
			WriteLog("�ɹ���1");
			pCbkdlg->SendMessageToVideoDlg(ExamCarNo,VideoAdd);
			pCbkdlg->ExamCarInfo.GetDlgItem(IDC_BUTTONMAIN)->SetWindowText("������");
			//////////////////////////////////
	}
	else
	{
		/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
		pCbkdlg->SendMessageToVideoDlg(ExamCarNo,VideoAdd);
		//////////////////////////////////
	}

	return TRUE;
}

BOOL CExamCarDlg::CloseExamCarVideo()    
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
	pCbkdlg->SendMesageToVideoDlgClose();
	//////////////////////////////////

	return TRUE;
}

void CExamCarDlg::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

void CExamCarDlg::ChangeTrackVideoPlay(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //��ǰ����ĸ���
	HTREEITEM temSub; //�����ȡ
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //��õ�һ��
	while (tem != NULL) //������ؿ�˵���Ѿ�����β����  �˳�
	{
		if (tem != tem1) //��ʱ��ȡ�ĸ����ѡ����Ǹ�����
		{
			//�鿴��ʱ׷�ٵ�״̬�ǿ�������ȡ��
			//��ȡ����ĵ�һ��
			temSub = m_exam.GetChildItem( tem );
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "�ر�����ʻ��Ƶ")
			{
				m_exam.SetItemText(temSub,"��������ʻ��Ƶ");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}


}

BOOL CExamCarDlg::OpenExamCarVideoT(UINT ExamCarNo, UINT VideoAdd)
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	if(pCbkdlg->iISCreateT == 0)
	{
		//˵����û�д���
			BOOL bRET = FALSE;
			bRET = pCbkdlg->CreateVideoDlgT();
			if(bRET == FALSE)
				return FALSE;

			/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
			WriteLog("�ɹ���1");
			pCbkdlg->SendMessageToVideoDlgT(ExamCarNo,VideoAdd);
			pCbkdlg->ExamCarInfo.GetDlgItem(IDC_BUTTONCOPILOT)->SetWindowText("�ظ���");
			//////////////////////////////////
	}
	else
	{
		/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
		pCbkdlg->SendMessageToVideoDlgT(ExamCarNo,VideoAdd);
		//////////////////////////////////
	}

	return TRUE;
}

BOOL CExamCarDlg::OpenExamCarVideoS(UINT ExamCarNo, UINT VideoAdd)
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	if(pCbkdlg->iISCreateS == 0)
	{
		//˵����û�д���
			BOOL bRET = FALSE;
			bRET = pCbkdlg->CreateVideoDlgS();
			if(bRET == FALSE)
				return FALSE;

			/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
			WriteLog("�ɹ���1");
			pCbkdlg->SendMessageToVideoDlgS(ExamCarNo,VideoAdd);
			pCbkdlg->ExamCarInfo.GetDlgItem(IDC_BUTTONOUTSIDE)->SetWindowText("�س���");
			//////////////////////////////////
	}
	else
	{
		/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
		pCbkdlg->SendMessageToVideoDlgS(ExamCarNo,VideoAdd);
		//////////////////////////////////
	}

	return TRUE;
}

BOOL CExamCarDlg::CloseExamCarVideoT()
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
	pCbkdlg->SendMesageToVideoDlgCloseT();
	//////////////////////////////////

	return TRUE;
}

BOOL CExamCarDlg::CloseExamCarVideoS()
{
	CCBKDigitalMapDlg *pCbkdlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	/////////�������Ի��� ʵ�ֵĹ����Ƿ�����Ϣ��������Ƶ�ĶԻ���////////
	pCbkdlg->SendMesageToVideoDlgCloseS();
	//////////////////////////////////

	return TRUE;
}

void CExamCarDlg::ChangeTrackVideoPlayT(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //��ǰ����ĸ���
	HTREEITEM temSub; //�����ȡ
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //��õ�һ��
	while (tem != NULL) //������ؿ�˵���Ѿ�����β����  �˳�
	{
		if (tem != tem1) //��ʱ��ȡ�ĸ����ѡ����Ǹ�����
		{
			//�鿴��ʱ׷�ٵ�״̬�ǿ�������ȡ��
			//��ȡ����ĵ�һ��
			temSub = m_exam.GetChildItem( tem );
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "�رո���ʻ��Ƶ")
			{
				m_exam.SetItemText(temSub,"��������ʻ��Ƶ");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}
}

void CExamCarDlg::ChangeTrackVideoPlayS(HTREEITEM pVCurItem)
{
	HTREEITEM temCurItem = pVCurItem;
	HTREEITEM tem;
	HTREEITEM tem1; //��ǰ����ĸ���
	HTREEITEM temSub; //�����ȡ
	tem1 = m_exam.GetParentItem(temCurItem); 
	tem = m_exam.GetFirstVisibleItem();  //��õ�һ��
	while (tem != NULL) //������ؿ�˵���Ѿ�����β����  �˳�
	{
		if (tem != tem1) //��ʱ��ȡ�ĸ����ѡ����Ǹ�����
		{
			//�鿴��ʱ׷�ٵ�״̬�ǿ�������ȡ��
			//��ȡ����ĵ�һ��
			temSub = m_exam.GetChildItem( tem );
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			temSub = m_exam.GetNextSiblingItem(temSub);
			CString szText;
			szText = m_exam.GetItemText(temSub);
			if (szText == "�رճ�����Ƶ")
			{
				m_exam.SetItemText(temSub,"����������Ƶ");
			}
			
		}
		tem = m_exam.GetNextSiblingItem(tem);
	}
}
