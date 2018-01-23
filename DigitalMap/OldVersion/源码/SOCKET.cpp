// SOCKET.cpp: implementation of the CSOCKET class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SOCKET.h"
#include "CBKDigitalMap.h"
#include "CBKDigitalMapDlg.h"

extern CCBKDigitalMapApp theApp;

#define MAX_BUF_SIZE 4096

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//#pragma warning disable:C4786
//#pragma diag_suppress C4786

UINT server_thd(LPVOID p);

CSOCKET::CSOCKET()
{
	dequeOK = TRUE; //Ĭ������
	TotalCar = 0;  //��ǰ��������
	iC_Dlg_F = 0;//�����Ի��򴴽����
//	InitSocket();
	AfxBeginThread(&server_thd, this);

}

CSOCKET::~CSOCKET()
{

}

/**********************************8
����˵����д������Ϣ���������ļ�
************************************/
void CSOCKET::WriteLog(CString szERR)
{
	CTime curTime =CTime::GetCurrentTime();
	CString Data=curTime.Format("%Y-%m-%d %H:%M:%S");
	FILE *fp=fopen("datalog","a+");
	fprintf(fp,"[%s]%s.\n",Data,szERR);
	fclose(fp);
}

// void CSOCKET::WriteLog(CString szERR,CString dataFile)
// {
// 
// 	FILE *fp=fopen(dataFile,"a+");
// 	fprintf(fp,szERR);
// 	fclose(fp);
// }


void CSOCKET::InitSocket()
{
	//
	//��ʼ��Winsock
	WSADATA wsaDATA;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaDATA);
	if(ret != 0)
	{
		CString err;
		err.Format("SOCKET��ʼ��ʧ��");
		WriteLog(err);
	}
	
	SOCKADDR_IN m_addr;
	

	
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	m_addr.sin_port = htons(9800);

	//�����µ��׽��֣������Ǵ����������͵��׽���
	m_hSocket = socket(AF_INET, SOCK_STREAM,0);	

	/////////////////////

	int on = 1000;
	if(setsockopt(m_hSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&on, sizeof(on)) < 0)

//	int timeout = 3000; //3s
	
//	int ret1=setsockopt(m_hSocket,SOL_SOCKET,SO_SNDTIMEO,(const char *)&timeout,sizeof(timeout));
	
 //  int ret2=setsockopt(m_hSocket,SOL_SOCKET,SO_RCVTIMEO,(const char *)&timeout,sizeof(timeout));
	/////////////////////


	//��һ���׽��ֵ������ĵ�ַ
	ret = 0;
	int error = 0;
	ret = bind(m_hSocket, (struct sockaddr*)&m_addr, sizeof(SOCKADDR_IN));
	if(ret == SOCKET_ERROR)
	{
		CString err;
		err.Format("SOCKET bindʧ��");
		WriteLog(err);
	}
	
	//��ʼһ���������̣��ȴ��û�������
	ret = listen(m_hSocket, 20);
	if(ret == SOCKET_ERROR)
	{
		CString err;
		err.Format("SOCKET listenʧ��");
		WriteLog(err);
	}


	
}


UINT server_thd(LPVOID p)
{
	Sleep(5000);
	CCBKDigitalMapDlg *pCCBKDigitalMapDlg = (CCBKDigitalMapDlg *)(theApp.m_pMainWnd);
	map<int,UINT>::iterator it;

	while (!pCCBKDigitalMapDlg->pSocket.iC_Dlg_F)
		Sleep(1000);
	pCCBKDigitalMapDlg->pSocket.InitSocket();

	//�Ƕ����ȴ��ͻ�������
	SOCKADDR_IN client_addr;
	int iaddrSize = sizeof(SOCKADDR_IN);
	CSOCKET *pSocket = (CSOCKET *)p;
	char chData[MAX_BUF_SIZE];
	CFile destFile;
	CString strIni;	
	CString strlon;	
	CString strlat;
	CString strZT,strErrid;
	DOUBLE tplon = 0;
	DOUBLE tplat = 0;

	int nread;
//	char buff[100];
//	Package_ST pPackage_ST;
	Package_ST p_Package_ST[MAX_ExamCar];

	int iCountPackage = 0;
	CString R2;
	
	int Maxfd = 0;
	int fd_Num = 0; //�ж���������������
	SOCKET sock;                                                  //�����������һ������
	fd_set rfd,copyrfd;                                //�������� ���������������û��һ�����õ�����  ���ݶ��ļ���
	struct timeval timeout;
	int client[FD_SETSIZE];		//������ſͻ�����������
	FD_ZERO(&rfd);                                                //�������������һ����������
	FD_ZERO(&copyrfd);
	timeout.tv_sec = 20;                                          //����select�õ����
	timeout.tv_usec = 0;
	u_long ul = 1;
	ioctlsocket(pSocket->m_hSocket, FIONBIO, &ul);                         //�÷�����������
		
	//���ڿ�ʼ��select
	FD_SET(pSocket->m_hSocket, &copyrfd);                                      //��sock����Ҫ���Ե��������� ����˵��sock������rfd���� ������һ������select��rfd����
	Maxfd = pSocket->m_hSocket;//��ȡ��ǰ�����������ֵ
		
	int i = 0;
	for(i = 0; i < FD_SETSIZE; i++)
	client[i] = -1;
	struct timeval _timeout;
	_timeout.tv_sec = 5;
	_timeout.tv_usec = 0;
	while(pCCBKDigitalMapDlg->iContrl)
	{
		rfd = copyrfd;  //�����ݵļ��ϳ�Ա�����ļ���
//			FD_SET(sock, &rfd);                                   //��sock�������������
//			FD_SET(sock, &wfd);                                   //��sock����д��������
		fd_Num = select(Maxfd + 1, &rfd, NULL, 0, &_timeout);
		if (fd_Num == 0)
		{
			//��·����û�ж���  ��������
			pCCBKDigitalMapDlg->WriteLog("��������");
			continue;
		}
		if(SOCKET_ERROR != fd_Num)       //������������������,������ʱ ����rfdֻ�������Զ� wfdֻ��������д
		{//û�д���

				if(FD_ISSET(pSocket->m_hSocket, &rfd))
				{//��һ��������׼������

					sock = accept(pSocket->m_hSocket,(struct sockaddr *)&client_addr, &iaddrSize);                           //һ���������Զ� һ����������д
					if(sock == SOCKET_ERROR)
					{
						CString err;
						err.Format("SOCKET ����ʧ��");
						pSocket->WriteLog(err);
					}
			//		pSocket->TotalCar = pSocket->TotalCar + 1;  //��ǰ���߿�����
					for(i = 0; i < MAX_ExamCar; i++)
					{	if(client[i] < 0)
						{
							client[i] = sock;
							
							CString PPP;
							PPP.Format("%d %d ���������������ˣ�",i,sock);
							pSocket->WriteLog(PPP);

							break;
						}
					}
					if(sock > Maxfd)
						Maxfd = sock; 
					FD_SET(sock,&copyrfd);
					fd_Num --;
					if (fd_Num <= 0)
					{
						continue;
					}
					
					//		FD_ZERO(&rfd);
					//		FD_ZERO(&wfd);
				}

				//������FD�����ɶ��¼�
				
				for(i = 0; i < MAX_ExamCar; i++)  // FD_SETSIZE
				{		 
					
					if(FD_ISSET(client[i],&rfd))
					{
						sock = client[i];
						memset(chData,0,(sizeof(chData)/sizeof(char)));	
						
						nread = recv(sock, chData, MAX_BUF_SIZE, 0); // MSG_PEEK   MSG_OOB
						if(nread == -1 || nread == SOCKET_ERROR )
						{
							client[i] = -1; //�ͻ��˹ر�
							FD_CLR(sock,&copyrfd);
							
							fd_Num --;
							pCCBKDigitalMapDlg->WriteLog("����������");
							
						//	it = pSocket->SockToCar.find(sock);
							

			
							/*

							memset(&pPackage_ST,0,sizeof(Package_ST));
							pPackage_ST.pPackageHead_ST.PackageType = 10;// ����ر��������
							pPackage_ST.pExamCar_ST.ExamCarNumber = pSocket->SockToCar.find(sock)->second;
							//����������������ﴦ��ģ��������������˾�ֻ�������ﴦ����
							//  ���������ǣ�����кܶ࿼�����ߣ�Ȼ���п������ߣ����������������ﲻ���� �ڽ�����ﴦ���ˣ�
							//  �������û���յ�
							pCCBKDigitalMapDlg->m_Button[pPackage_ST.pExamCar_ST.ExamCarNumber].ExamCarState = 0;//��������
							pCCBKDigitalMapDlg->Mutex_Self[pPackage_ST.pExamCar_ST.ExamCarNumber].Lock();// ���������õģ���Ҫ���ڿ�������
							while(!SuspendThread(pCCBKDigitalMapDlg->ThreadHandle[pPackage_ST.pExamCar_ST.ExamCarNumber]));  //�����߳�
							Sleep(10);
							pCCBKDigitalMapDlg->Mutex_Self[pPackage_ST.pExamCar_ST.ExamCarNumber].Unlock();// ���������õģ���Ҫ���ڿ�������
							pCCBKDigitalMapDlg->WriteLog("�����˳� ����   0");
							pCCBKDigitalMapDlg->pMutexForPG.Lock();
							CString XXX;
							XXX.Format("�����˳� ���� %d",pPackage_ST.pExamCar_ST.ExamCarNumber);
							pCCBKDigitalMapDlg->WriteLog(XXX);
					//		while (pSocket->dequeOK == FALSE)
					//			;
							//	Sleep(10);
					//		pSocket->dequeOK = FALSE;
							pSocket->pPackage_ST.push_back((Package_ST *)&pPackage_ST);
						//	Sleep(2);
					//		pSocket->dequeOK = TRUE;
							pCCBKDigitalMapDlg->pMutexForPG.Unlock();
							pCCBKDigitalMapDlg->WriteLog("�����˳� ��ȥ");



							*/
// 							it=mymap.find('b');
// 							mymap.erase (it); 
						//	pSocket->SockToCar.erase (it);  ��仰Ϊʲô���ã�
							if(fd_Num <= 0)
							{
							//	AfxMessageBox("�ر�һ���ͻ���");
								break;	
							}

							closesocket(sock);
							
						}
						else
						{
						//	pSocket->WriteLog(chData,"��������.txt");

							client[i] = -1; //�ͻ��˹ر�
							FD_CLR(sock,&copyrfd);
					//		pSocket->TotalCar = pSocket->TotalCar - 1;
							
							destFile.Open(".\\��������.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::modeNoTruncate );	
							//	destFile.Write("[GPSInfo]\r\n", 11); // Write it
							destFile.SeekToEnd();
							destFile.Write(chData, nread); // Write it	
							destFile.Close();
							/*
							pCCBKDigitalMapDlg->WriteLog(chData);
							
							CStdioFile file;
							if(file.Open(_T("��������.txt"), CFile::modeWrite | CFile::typeText , NULL))		
							{		
								file.SeekToEnd();		
								file.WriteString(chData);		
								file.Close();		
							}
							*/

							destFile.Open(".\\Recv.sh", CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);	
						//	destFile.Write("[GPSInfo]\r\n", 11); // Write it
							destFile.Write(chData, nread); // Write it	
							destFile.Close();
							
							for(i = 1; i <= MAX_ExamCar; i++)
							{
								//���
								strIni = "";	
								strlon = "";	
								strlat = "";
									
							//	WaitForSingleObject(g_RecvFileEvent.m_hObject, 100);
								strIni.Format("CAR%dlon", i);			
								GetPrivateProfileString("GPSInfo",strIni.GetBuffer(MAX_PATH), NULL,strlon.GetBuffer(MAX_PATH),MAX_PATH,".\\Recv.sh");
								strIni.Format("CAR%ddla", i);
								GetPrivateProfileString("GPSInfo",strIni.GetBuffer(MAX_PATH), NULL,strlat.GetBuffer(MAX_PATH),MAX_PATH,".\\Recv.sh");
								strZT.Format("ZT%d",i);
								strErrid.Format("ErrID%d",i);
								UINT zt=GetPrivateProfileInt("GPSInfo",strZT.GetBuffer(MAX_PATH),1,".\\Recv.sh");
								UINT errid=GetPrivateProfileInt("GPSInfo",strErrid.GetBuffer(MAX_PATH),0,".\\Recv.sh");
							//	g_RecvFileEvent.SetEvent();
								//AfxMessageBox(strlon);
								//AfxMessageBox(strlat);
								
								if(strlat == "" || strlon == "")
								{
									TRACE("%d", strlon.GetLength());
								//	str.Format("strlat.IsEmpty   %d", strlat.GetLength());
									//AfxMessageBox(str);
									continue;
								}
							//	pCCBKDigitalMapDlg->WriteLog("qwerty1");
							//	iOnline++;
								tplon = atof(strlon);
								tplat = atof(strlat);

								if(tplon == 0 || tplat == 0)
								{
									continue;
								}


								////////   ������Ҫ�жϵ��� ����п����м����ȥ��ʱ���ҪҪ�ÿ����ж�Ӧ���߳�  /////////
								//	pCCBKDigitalMapDlg    pSocket

								/*
								if(iS == (pCCBKDigitalMapDlg->pSocket.TotalCar + 1) && pCCBKDigitalMapDlg->m_Button[iS - 1].examCarNumber != i)
								{
											
								//	pCCBKDigitalMapDlg->m_Button[i].examCarNumber = pCCBKDigitalMapDlg->SockToCar1.find(i)->second;
									pCCBKDigitalMapDlg->m_Button[iS].examCarNumber = i;
									pCCBKDigitalMapDlg->m_Button[iS].ThreadNumber = pCCBKDigitalMapDlg->m_Button[iS - 1].ThreadNumber++;
									pCCBKDigitalMapDlg->m_Button[iS].ExamCarState == 1;

									int iValue = 0;
									for (iValue = 1;iValue <= MAX_ExamCar;iValue++)
									{
										if (pCCBKDigitalMapDlg->SockToCar1.find(iValue)->second == -1)
									//	if(m_Button[iValue].examCarNumber == 0)
										{
											pCCBKDigitalMapDlg->SockToCar1[iValue] = i;
										//	m_Button[iValue].examCarNumber = _ttoi(c_TmpCarNumber);
											break;
										}
										
									}
									pCCBKDigitalMapDlg->pSocket.TotalCar++;
									CString tmpOnLineCar;
									tmpOnLineCar.Format("��ǰ������Ϊ��%d",pCCBKDigitalMapDlg->pSocket.TotalCar);
									pCCBKDigitalMapDlg->m_TotalCar.SetWindowText(tmpOnLineCar);  
								}

								////////////////////////////////////////////////////*/


								memset(p_Package_ST+i,0,sizeof(Package_ST));
								int i_Find = 1;
						
								while (pCCBKDigitalMapDlg->m_Button[i_Find].examCarNumber != i)
								{
									i_Find ++;								
								}
								(p_Package_ST+i)->pPackageHead_ST.PackageType = 1;// 
								(p_Package_ST+i)->pExamCar_ST.ExamCarNumber = i;
								(p_Package_ST+i)->pExamCar_ST.y_Gps = tplat;//
								(p_Package_ST+i)->pExamCar_ST.x_Gps = tplon;//
								(p_Package_ST+i)->pExamCar_ST.zt =zt; //zzz
								(p_Package_ST+i)->pExamCar_ST.ErrorID=errid;//zzz
								(p_Package_ST+i)->pExamCar_ST.ThreadNumber = i_Find; //������̺߳�
								
								
								//  ThreadNumber
	

								//�������ݣ���������Ϊ�������Լ���ǰGPS��ֵ
								
		//						pSocket->SendString("192.168.0.62",MSG_GPS,temp.GetBuffer(0));
								pCCBKDigitalMapDlg->pMutexForPG.Lock();

								pSocket->pPackage_ST.push_back(p_Package_ST+i);
						//		Sleep(2);
						//		pSocket->dequeOK = TRUE;
								iCountPackage++;
								pCCBKDigitalMapDlg->pMutexForPG.Unlock();
						//		pCCBKDigitalMapDlg->WriteLog("���� ��ȥ");
								
								
							//	pSocket->SockToCar[sock] = pPackage_ST.pExamCar_ST.ExamCarNumber;
								//������û�д���

							}
							
							closesocket(sock);

							

							fd_Num --;
							if(fd_Num <= 0)
							{
								break;	
							}
						}
					}
				}
			

		}

	}
	R2.Format("�հ�����%d",iCountPackage);
	pCCBKDigitalMapDlg->WriteLog(R2);
	pCCBKDigitalMapDlg->WriteLog("Socket StretchDlg_thd");
	return 0;
}