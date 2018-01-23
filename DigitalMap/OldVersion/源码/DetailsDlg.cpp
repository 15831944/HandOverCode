// DetailsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "cbkdigitalmap.h"
#include "DetailsDlg.h"
#include "ExamCarDlg.h"
#include "CBKDigitalMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCBKDigitalMapApp theApp;
#define ExamCarN0 10
#define ExamCarN0Sub 1000

char zkzh[100][30];//���׼��֤��

UINT UpdateExamCarInfo_thd(LPVOID p);  //ˢ�����߿�����ʵʱ��Ϣ

/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg dialog


CDetailsDlg::CDetailsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDetailsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailsDlg)
	DDX_Control(pDX, IDC_TREE_DETAILS, m_details);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailsDlg, CDialog)
	//{{AFX_MSG_MAP(CDetailsDlg)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailsDlg message handlers

BOOL CDetailsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_details.MoveWindow(1,1,260,388);  
	m_details.DeleteAllItems();
	
	InitMytree(); 


	AfxBeginThread(&UpdateExamCarInfo_thd, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDetailsDlg::InitMytree()
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
	m_details.SetImageList(m_imagelist,TVSIL_NORMAL); //Ϊm_mytree����һ��ͼ���б�ʹCtreeCtrl�Ľڵ���ʾ��ͬ��ͼ�� 
	m_details.SetBkColor(RGB(160,210,230));//����m_mytree�ı���ɫ
	
	return true;
}



UINT UpdateExamCarInfo_thd(LPVOID p)  //ʵʱˢ�����߿�����Ϣ
{   //	((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->MAPBotom;
	Sleep(5000);
	CDetailsDlg *pDetailsDlg = (CDetailsDlg *)p;
	//  ����������ˢ���б��ֵ  ��һ�鵽ʱ����ܻ����߳�������
	pDetailsDlg->m_details.DeleteAllItems(); //����б�ֵ
	// Ȼ��������߿����ҵ�����ʲô��Ŀ �������ݿ�
	// BOOL ArryExamCar[MAX_ExamCar]; //���߿��� 1 ���� 0 ������
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	int i = 0;
	BOOL OnlineExamCar[MAX_ExamCar];

	memset(zkzh,0,100*30);
	for (i = 0;i < MAX_ExamCar;i++) //��ʼ�����߿���Ϊ������
	{
		OnlineExamCar[i] = FALSE;
	}
	while (pCCBKDigitalMapDlg->iContrl)
//	while (1)
	{
		for (i = 1;i < MAX_ExamCar;i++)
		{
			if (pCCBKDigitalMapDlg->ExamCarInfo.ExamCarDlg.ArryExamCar[i] == TRUE)
			{
				// ���ж�����������û������
				if (OnlineExamCar[i] == FALSE)
				{
					OnlineExamCar[i] = TRUE;
				}
				//˵���������� �������ݿ�Ȼ��һЩ�����е���Ϣ��ʾ����
				CString tempCar;
				int iCar;
			//	tempCar.Format("����%d",i);
				tempCar.Format("����%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
				iCar = ((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber;
				pDetailsDlg->ReadExamInfoFromDB(NULL, tempCar, NULL,zkzh[iCar]);
			}
			else
			{
				// ˵�������Ѿ������߻��ߴ�û�����߹� 
				// ���ж�������������û�����߹� ����о͸������� ͬʱ����״�ؼ��е��������
				if (OnlineExamCar[i] == TRUE) //˵�����������߹�
				{
					// �����¼ ����Ϊ����
					OnlineExamCar[i] = FALSE;
			//		pDetailsDlg->m_details.DeleteItem(pCurItem);


					//����״�б���ɾ������
					// Ҫ���ҵ�������Ϣ������
 					HTREEITEM pCurItem;
 					HTREEITEM temSub;
 					CString szItemText;
 					CString tmpItemText;
			//		szItemText.Format("����%d",i);
					szItemText.Format("����%d",((CCBKDigitalMapDlg *)(theApp.m_pMainWnd))->m_Button[i].examCarNumber);
			//		GetChildItem( HTREEITEM hItem );
					pCurItem = pDetailsDlg->m_details.GetChildItem(TVI_ROOT); 
			//		pCurItem = pDetailsDlg->m_details.GetFirstVisibleItem( );
					if (pCurItem != NULL)
					{
						tmpItemText = pDetailsDlg->m_details.GetItemText(pCurItem );
						while(szItemText != tmpItemText)
						{
							pCurItem = pDetailsDlg->m_details.GetNextSiblingItem(pCurItem);
							if (pCurItem == NULL)
							{
								break;
							}
							tmpItemText = pDetailsDlg->m_details.GetItemText(pCurItem );
						}
						if(szItemText == tmpItemText)
						{
							pDetailsDlg->m_details.DeleteItem(pCurItem);
						}
						


					}
					pCurItem = NULL;
					temSub = NULL;
					szItemText = _T("");
 					tmpItemText = _T("");
				}
			}
		}
		Sleep(1000);
	}
	Sleep(5000);
	pCCBKDigitalMapDlg->WriteLog("�� UpdateExamCarInfo_thd");
	return 0;
}



void CDetailsDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDetailsDlg::ReadExamInfoFromDB(CDC *pDC, CString sCard, CRect *rc,char *pzkzh)
{
//	LPVOID m_pBMPBuffer;
	CString temp;
	CString szExamineeName;  //����
	CString strzkzm;  //׼��֤�����
	CString strCarModel;  //���Գ���
	CString szPointsType;  //�۷�����
	CString szDeductionPoint;  //�۳�����

	char bufzkzh[30];
	memset(bufzkzh,0,30);
	strcpy(bufzkzh,pzkzh);

	int j = 0;

	//////
	CTime pTime = CTime::GetCurrentTime();
	CString szTime;
	szTime.Format("%d-%d-%d",pTime.GetYear(),pTime.GetMonth(),pTime.GetDay());
	////////

	try
	{
	//	BOOL sInit=theApp.OnInitSQL();
	//	if (sInit)
	//	{
			CString strSQL;	
			VARIANT cnt;
			cnt.vt = VT_INT;
			strSQL.Format("SELECT * FROM StudentInfo WHERE ������ = '%s' and ״̬ = '3' and Ԥ������ = '%s'",sCard,szTime);
			// 			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
			// 				adOpenDynamic, adLockOptimistic, adCmdText);
			
			_RecordsetPtr pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
			_variant_t vat;
			if(pSet != NULL && (!pSet->adoEOF))
			{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
				while(!pSet->adoEOF)
				{
					vat =pSet->GetCollect("����");
					if (vat.vt != VT_NULL)
					{
						szExamineeName = (LPCSTR)_bstr_t(vat);
					}
					vat =pSet->GetCollect("׼��֤�����");
					if (vat.vt != VT_NULL)
					{
						strzkzm = (LPCSTR)_bstr_t(vat);
					}
					vat =pSet->GetCollect("���Գ���");
					if (vat.vt != VT_NULL)
					{
						strCarModel = (LPCSTR)_bstr_t(vat);
					}
					
					pSet->MoveNext();
					break;
				}

				/////      �����ж��ǲ���ԭ�����Ǹ�����   ///////
				if(strcmp(strzkzm,bufzkzh)) 
				{
					//˵��������ͬ��׼��֤��
					if(!strcmp(bufzkzh,""))
					{
						//˵��׼��֤��Ϊ��

					}
					else
					{
						//˵����Ϊ�� ��ʱ�Ѿ�������һ�������ˣ�Ҫ��֮ǰ�ļ�¼��ɾ��
						HTREEITEM pCurItem2;
						HTREEITEM temSub2;
						CString tmpItemText2;
						CString tempCar2;
						CString szText2;  //�ҵ����һ�����������
						szText2 = _T("");
						tempCar2 = sCard;
						pCurItem2 = m_details.GetChildItem(TVI_ROOT); 
						if (pCurItem2 != NULL)
						{
							tmpItemText2 = m_details.GetItemText(pCurItem2 );
								

							while(tempCar2 != tmpItemText2)
							{
								pCurItem2 = m_details.GetNextSiblingItem(pCurItem2);
								if (pCurItem2 == NULL)
								{

									break;
								}
								tmpItemText2 = m_details.GetItemText(pCurItem2);
							}

							///// ����Ҫ�����ǰ�֮ǰ��ȫ����ɾ���� 
							temSub2 = m_details.GetChildItem( pCurItem2 );
							while (temSub2 != NULL)
							{
								//����Ҫ���ǵ���ôɾ��
								m_details.DeleteItem( temSub2);
								temSub2 = m_details.GetChildItem( pCurItem2 );

						//		szText2 = m_details.GetItemText(temSub2);
						//		temSub2 = m_details.GetNextSiblingItem( temSub2 );
						//		m_details.DeleteItem( temSub2);
							}
						}
					}
					strcpy(pzkzh,strzkzm);

				}
				/////////////////////////////////////////////////
				


				/////////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////////
				//������Ҫ��ӵ�����֮�� ��Ҫ���뵽��״�ؼ���Ӧ����������
				
				//˵���������� �������ݿ�Ȼ��һЩ�����е���Ϣ��ʾ����
				CString tempCar;
				tempCar = sCard;
				// �Ȳ�������������û������״�ؼ����� ����оͼ���������� û�оͲ���Ȼ���������
				
				HTREEITEM pCurItem;
				HTREEITEM temSub;
				
				CString tmpItemText;
				int i;
				i = atoi(tempCar.Right(1));
				
			//	pCurItem = m_details.GetFirstVisibleItem( );
				// չ����һ���ڵ�    
				// HTREEITEM hChildItem = m_treeCtrlList.GetChildItem(TVI_ROOT); 
				pCurItem = m_details.GetChildItem(TVI_ROOT); 
				if (pCurItem != NULL)
				{
					tmpItemText = m_details.GetItemText(pCurItem );
					    

					while(tempCar != tmpItemText)
					{
						pCurItem = m_details.GetNextSiblingItem(pCurItem);
						if (pCurItem == NULL)
						{
							//��ӿ����ĸ���
							m_hDevItem = m_details.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
							m_details.SetItemData(m_hDevItem,(ExamCarN0 * i) + 10);
							break;
						}
						tmpItemText = m_details.GetItemText(pCurItem );
					}
				}
				else
				{
					//��ӿ����ĸ���
					m_hDevItem = m_details.InsertItem(tempCar,0,1,TVI_ROOT,TVI_LAST);
					m_details.SetItemData(m_hDevItem,(ExamCarN0 * i) + 10);
				}
				
				//�ж��������������û�����ֵ ����оͼ���������һ�� ���û�о����
				// ���ҵ����һ�� 
				CString szText;  //�ҵ����һ�����������
				szText = _T("");
				if (pCurItem != NULL)
				{
					temSub = m_details.GetChildItem( pCurItem );
					
					while (temSub != NULL)
					{
						szText = m_details.GetItemText(temSub);
						temSub = m_details.GetNextSiblingItem( temSub );
						
					}
				}
				

				strSQL.Format("SELECT * FROM ErrorRecordInDetail WHERE ׼��֤����� = '%s' order by ����ʱ�� asc",strzkzm );
				// 			pRecordset->Open((_variant_t)_bstr_t(strSQL), _variant_t((IDispatch*)theApp.m_pConn, true), 
				// 				adOpenDynamic, adLockOptimistic, adCmdText);
				
				pSet =theApp.m_pConn->Execute((_bstr_t)strSQL,&cnt,adCmdUnknown);
			//	_variant_t vat;
				if(pSet != NULL && (!pSet->adoEOF))
				{//	CString m_strTitle,m_strStuname,m_strStucx,m_strStuqs,m_strStuzkzh,m_strStushzh;
					while(!pSet->adoEOF)
					{
						vat =pSet->GetCollect("�۷�����");
						if (vat.vt != VT_NULL)
						{
							szPointsType = (LPCSTR)_bstr_t(vat);
						}
						vat =pSet->GetCollect("�۳�����");
						if (vat.vt != VT_NULL)
						{
							szDeductionPoint = (LPCSTR)_bstr_t(vat);
						}	
						
						//�ж��б���û������ û�о�������� ����оͲ�����
					//	HTREEITEM ExamCarItem;

						if (szPointsType.GetLength() > 20)
						{
							szPointsType = szPointsType.Left(20);
						}

						if (szText == _T(""))
						{
							// ˵��������ӵĸ���
							if (szText != szPointsType)
							{
								
								HTREEITEM ExamCarItem = m_details.InsertItem(szPointsType,0,1,m_hDevItem,TVI_LAST);
								m_details.SetItemData(ExamCarItem,(ExamCarN0Sub * j) + 1);   //Data��Ӧͨ���������е�����
								j++;
							}
							pSet->MoveNext();
						}
						else  //˵�����Ѿ����ڵĸ���
						{
							if (szText == szPointsType) //��û���ҵ��Ǹ�������ļ�¼ֵ
							{
								//�Ѿ��ҵ���������¼ֵ  ��������¼֮��Ķ������ȥ

								pSet->MoveNext();
								while(!pSet->adoEOF)
								{
									vat =pSet->GetCollect("�۷�����");
									if (vat.vt != VT_NULL)
									{
										szPointsType = (LPCSTR)_bstr_t(vat);
									}
									vat =pSet->GetCollect("�۳�����");
									if (vat.vt != VT_NULL)
									{
										szDeductionPoint = (LPCSTR)_bstr_t(vat);
									}	
									
									//�ж��б���û������ û�о�������� ����оͲ�����
									//	HTREEITEM ExamCarItem;
									
									if (szPointsType.GetLength() > 20)
									{
										szPointsType = szPointsType.Left(20);
									}
									HTREEITEM ExamCarItem = m_details.InsertItem(szPointsType,0,1,m_hDevItem,TVI_LAST);
									m_details.SetItemData(ExamCarItem,(ExamCarN0Sub * j) + 1);   //Data��Ӧͨ���������е�����
									j++;
									pSet->MoveNext();
								}								
							}
							else
							{
								pSet->MoveNext();
							}
						}

						
	
					//	break;
					}
				}

			}
			
			
	//	}
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
