// VideoInterFace.cpp: implementation of the VideoInterFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CBKDigitalMap.h"
#include "VideoInterFace.h"
#include "HCNetSDK.h"
#pragma comment(lib,"DsSdk.lib")
#pragma comment(lib,"GdiPlus.lib")
#pragma comment(lib,"HCNetSDK.lib")
#pragma comment(lib,"PlayCtrl.lib")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



struct RIFF_HEADER//�ļ�����
{
	char szRiffID[4]; // 'R','I','F','F'
	DWORD dwRiffSize;//DWORD��������4GB���ļ���С=FILE_LENGTH-8(����ȥ�����ݽṹ��С��
	char szRiffFormat[4]; // 'W','A','V','E'
};
struct WAVE_FORMAT
{
	WORD wFormatTag;
	WORD wChannels;
	DWORD dwSamplesPerSec;
	DWORD dwAvgBytesPerSec;
	WORD wBlockAlign;
	WORD wBitsPerSample;
};
struct FMT_BLOCK//��ʽ
{
	char szFmtID[4]; // 'f','m','t',' ' 
	DWORD dwFmtSize;//////////////һ�������Ϊ16�����и�����ϢΪ18
	WAVE_FORMAT wavFormat;
};
struct FACT_BLOCK//������ һ��ɲ���
{
	char szFactID[4]; // 'f','a','c','t'
	DWORD dwFactSize;
};
struct DATA_BLOCK//����ͷ
{
	char szDataID[4]; // 'd','a','t','a'
	DWORD dwDataSize;//���ݳ���
};


FILE* file = NULL;	
RIFF_HEADER riff;
WAVE_FORMAT wform;
FMT_BLOCK fmt;
FACT_BLOCK fact;
DATA_BLOCK data;
BYTE buff[1024*256];//��������

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VideoInterFace::VideoInterFace()
{
	m_lPlayHandle_record = -1;  //��NET_DVR_StopRealPlay�Ⱥ����ľ������
	m_iCurChanIndex = 0;		//ѡ�е�ͨ����
	VideoRunning = FALSE;		//�Ƿ����������Ƶ
	isRecording = FALSE;		//�Ƿ�����¼����Ƶ
}

VideoInterFace::~VideoInterFace()
{

}


/*************************************************
��������:��½�豸
����˵��
DVRIP IP��ַ
Port  �˿ں�
Name  �û��� 
Password ����
pHwnd  ���
����ֵ true ��½�ɹ�
		false ��½ʧ��
**************************************************/
BOOL VideoInterFace::NET_DVR_LOGIN(char *DVRIP, WORD pPort, char *pName, char *Password, HWND pHwnd)
{
	this->DVRIP = new char[strlen(DVRIP) + 1];
	strcpy(this->DVRIP,DVRIP);
	this->UserName = new char[strlen(pName) + 1];
	strcpy(this->UserName,pName);
	this->Password = new char[strlen(Password) + 1];
	strcpy(this->Password,Password);
	this->Port = pPort;
	this->pHwnd = pHwnd;

	//��ʼ��
	NET_DVR_Init();
	//��������ʱ��������ʱ�� 
	NET_DVR_SetConnectTime(2000, 1); 
	NET_DVR_SetReconnect(10000, true); 
	
	//�����豸
	char *pAddress = "192.168.0.173";
	WORD Port =8000 ;
	char Name[20] = {"admin"};
	char password[20] = {"12345"};
//	NET_DVR_DEVICEINFO_V30 struLocalDeviceInfo;
	memset(&struLocalDeviceInfo,0,sizeof(struLocalDeviceInfo));
	lUserId = NET_DVR_Login_V30(DVRIP,(WORD)Port,Name,password,&struLocalDeviceInfo);
	
	if (lUserId == NET_DVR_NOERROR) //���ӳɹ�
	{
		
		//	SetDlgItemText(IDC_BUTTON_Login,"123");
		MessageBox("���ӳɹ�");
		return true;
	}
	else
	{
		GetLastErrorz();
//		CString err;
//		err.Format("������룺%d",lUserId);
//		MessageBox(err);
		return false;
	}

	
}



/*************************************************
����˵�� ʵʱԤ���Ļص�����

*************************************************/
void CALLBACK g_RealDataCallBack_V30(LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer,DWORD 
dwBufSize,void* dwUser) 
{ 
	BOOL bRet = FALSE;
	int i = 0;
	HWND hWnd = NULL;
	CString tmp;
	tmp.Format("datatype = %d, dwbufsize = %d", dwDataType, dwBufSize);
	OutputDebugString(tmp);
	
	switch (dwDataType)
	{
	case NET_DVR_SYSHEAD:

/*
		if (PLAYBYNAME == dwUser)
		{
			PlayM4_GetPort(&g_pMainDlg->m_lPlayPort);
			hWnd = g_pMainDlg->m_FileWnd;
		}
		
		bRet = PlayM4_SetOverlayMode(g_pMainDlg->m_lPlayPort, FALSE, COLORREF(0));//not play on overplay
		bRet = PlayM4_SetStreamOpenMode(g_pMainDlg->m_lPlayPort, STREAME_FILE);
		
		if (dwBufSize > 0)
		{
			bRet = PlayM4_OpenStream(g_pMainDlg->m_lPlayPort, pBuffer, dwBufSize, 600*1000);//SOURCE_BUF_MIN50*1000
			if (bRet)
			{
				PlayM4_Play(g_pMainDlg->m_lPlayPort, hWnd);
			}
		}
*/
		break;
	case NET_DVR_AUDIOSTREAMDATA:

		if (dwBufSize > 0 )
		{
			fwrite((BYTE *)&buff,sizeof(BYTE),dwBufSize,file);//д��Ƶ����
		}
/*
		if (dwBufSize > 0 && g_pMainDlg->m_lPlayPort >=0)
		{
			PlayM4_InputVideoData(g_pMainDlg->m_lPlayPort, pBuffer, dwBufSize);
		}
*/
		break;
/*
	case NET_DVR_STREAMDATA:
		if (dwBufSize > 0 && g_pMainDlg->m_lPlayPort >=0)
		{
			for (i=0; i<1000; i++)
			{
				bRet = PlayM4_InputData(g_pMainDlg->m_lPlayPort, pBuffer, dwBufSize);
				if (!bRet)
				{
					Sleep(1);
				}
				else
				{
					break;
				}
			}
		}
*/
		break;
	}
	return;
} 








/*
for(int i=0;i<256;i++)//д��������ݣ�������
{
	BYTE k=rand()%256;
	for(int j=0;j<1024;j++)
		buff[i*1024+j]=k;
}




fwrite((BYTE *)&buff,sizeof(BYTE),1024*256,file);//д��Ƶ����
fclose(file);

 */

/*�ɼ���Ƶ����ʼ��*/
int VideoInterFace::InitNET_DVR_AUDIOSTREAMDATA()
{
//	fmt.wavFormat.wFormatTag=WAVE_FORMAT_PCM;//1
	fmt.wavFormat.wFormatTag=1;//1
	fmt.wavFormat.dwAvgBytesPerSec=11025;
	fmt.wavFormat.dwSamplesPerSec=11025;
	fmt.wavFormat.wBitsPerSample=8;
	fmt.wavFormat.wBlockAlign=1;
	fmt.wavFormat.wChannels=1;
	
	sprintf(fmt.szFmtID,"%s","fmt ");
	fmt.dwFmtSize=16;//һ�������Ϊ16�����и�����ϢΪ18
	sprintf(data.szDataID,"%s","data");
	data.dwDataSize=1024*256;
	riff.dwRiffSize=1024*256+sizeof(FMT_BLOCK)+sizeof(DATA_BLOCK);
	strncpy(riff.szRiffFormat,"WAVE",4);
	sprintf(riff.szRiffID,"%s","RIFF");
	file=fopen("wavefile.wav","wb");
	fwrite(&riff,sizeof(RIFF_HEADER),1,file);//дRIFF_HEADER 
	fwrite(&fmt,sizeof(FMT_BLOCK),1,file);//дFMT_BLOCK
	fwrite(&data,sizeof(DATA_BLOCK),1,file);//д����ͷ DATA_BLOCK
	return 1;
}


/*************************************************
����˵����������Ƶ
����˵�� 
pHwnd  ��� ����������Ƶ�Ĵ��ھ��
pButtonhwnd  ��ť��ָ��
����ֵ  1 ��½�ɹ�
		0 ��½ʧ��
**************************************************/
int VideoInterFace::play(HWND pHwnd,int iChannel)
{
	NET_DVR_CLIENTINFO clientnfo ={0};
	HWND pWnd = pHwnd; //  GetDlgItem(IDC_STATIC_PLAY)->m_hWnd; //��ô��ڵľ��
	clientnfo.hPlayWnd = pWnd; //��ô��ڵľ��
	clientnfo.lChannel = iChannel;              //Ԥ��ͨ���� 
	clientnfo.lLinkMode = 0;              /*���λ(31)Ϊ 0��ʾ��������Ϊ1 ��ʾ��������0��30λ��ʾ����
						 ��ʽ��0��TCP ��ʽ��1��UDP ��ʽ��2���ಥ��ʽ��3��RTP ��ʽ; 4- RTP over RTSP��5- RTSP over HTTP */ 
	clientnfo.sMultiCastIP = NULL;      //�ಥ��ַ����Ҫ�ಥԤ��ʱ���� 


	m_lPlayHandle_record = NET_DVR_RealPlay_V30(lUserId,&clientnfo,g_RealDataCallBack_V30,NULL,TRUE);
	if(-1 == m_lPlayHandle_record)
	{
		GetLastErrorz();
// 		DWORD err=NET_DVR_GetLastError();  //��ȡ�������
// 		CString m_csErr;
// 		m_csErr.Format("���ų����������%d",err);
// 		MessageBox(m_csErr);
		NET_DVR_Cleanup();
		return 0;
	}
	VideoRunning = true;
	return 1;

}

/*************************************************
����˵����ֹͣ������Ƶ
����˵�� 

����ֵ  1 �ɹ�
		0 ʧ��
**************************************************/
int VideoInterFace::stopplay()
{
	if(m_lPlayHandle_record != -1)
	{
		if(VideoRunning == TRUE)  //����¼����ֹͣ
		{
			// ֹͣ¼��StopRecord();
		}
		NET_DVR_StopRealPlay(m_lPlayHandle_record);
		m_lPlayHandle_record = -1;
		VideoRunning = false;
	//	GetDlgItem(IDC_STATIC_PLAY)->Invalidate();  ������������ã� ָ���Ƿ�Ҫ�������������ڵı���
		return 1;
	}
	else
	{
		return 0;
	}
}



/*************************************************
����˵������ʼ¼����Ƶ
����˵��
����ֵ true ��ʼ¼��
		false ¼��ʧ��
**************************************************/
BOOL VideoInterFace::startvideotape()
{
	char RecName[256] = {0};	
	CTime CurTime = CTime::GetCurrentTime();;
	sprintf(RecName,"%04d%02d%02d%02d%02d%02d_ch.mp4",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());

	 if(!NET_DVR_SaveRealData(m_lPlayHandle_record,RecName))
	 {
		 GetLastErrorz();
		 MessageBox("����¼��ʧ��");
		 return false;
	 }
	 return true;
}



/*************************************
����˵����ֹͣ¼��

*************************************/
BOOL VideoInterFace::stopvideotape()
{
	if(!NET_DVR_StopSaveRealData(m_lPlayHandle_record))
	{
		GetLastErrorz();
		MessageBox("ֹͣ¼��ʧ��");
		return false;
	}
	isRecording = false;
	return true;
}



/*********************************
����˵����������ʾ�쳣����
����˵��  zstr Ҫ��ʾ������
*********************************/
void VideoInterFace::Drawztext(CString zstr)
{
	MessageBox(zstr);
// 	HDC hdc=GetDC();
// 	CRect rc;
// 	GetClientRect(pHwnd,&rc);
// 	DrawText(hdc,zstr,strlen(zstr),&rc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

/*********************************
����˵���������쳣��
����˵��  
*********************************/
void VideoInterFace::GetLastErrorz()
{
	switch (NET_DVR_GetLastError())
	{
	case NET_DVR_PASSWORD_ERROR://1
		Drawztext("�û����������,��鿴�����ļ���!");
		break;
	case NET_DVR_NOINIT://3
		Drawztext("SDKδ��ʼ��!");
		break;
	case NET_DVR_OVER_MAXLINK://5
		Drawztext("���ӵ��豸���û������������!");
		break;
	case NET_DVR_NETWORK_FAIL_CONNECT://7
		Drawztext("�����豸ʧ��.�豸�����߻�����ԭ����������ӳ�ʱ!");
		break;
	case NET_DVR_NETWORK_SEND_ERROR://8
		Drawztext("���豸����ʧ��!");
		break;
	case NET_DVR_MAX_USERNUM://52
		Drawztext("��¼�豸���û����ﵽ���!");
		break;
	case NET_DVR_BINDSOCKET_ERROR://72
		Drawztext("���׽���ʧ��!");
		break;
	case NET_DVR_SOCKETCLOSE_ERROR://73
		Drawztext("socket�����ж�,�˴���ͨ�������������жϻ�Ŀ�ĵز��ɴ�!");
		break;
	case NET_DVR_IPCHAN_NOTALIVE://83
		Drawztext("Ԥ��ʱ���IPͨ��������!");
		break;
	case NET_DVR_RTSP_SDK_ERROR://84
		Drawztext("���ر�׼Э��ͨѶ��StreamTransClientʧ��!");
		break;
	case NET_DVR_CONVERT_SDK_ERROR://85
		Drawztext("����ת��װ��ʧ��!");
		break;
	}
}






// 		NET_DVR_TIME struStartTime, struStopTime; 
// 		struStartTime.dwYear      = 2013; 
// 		struStartTime.dwMonth    = 7; 
// 		struStartTime.dwDay        = 15; 
// 		struStartTime.dwHour      = 10; 
// 		struStartTime.dwMinute = 7; 
// 		struStartTime.dwSecond =0; 
// 		struStopTime.dwYear        = 2013; 
// 		struStopTime.dwMonth      = 7; 
// 		struStopTime.dwDay          = 15; 
// 		struStopTime.dwHour        = 10; 
// 		struStopTime.dwMinute    = 15; 
// 		struStopTime.dwSecond    = 0; 
// 
// 
// 		
// 		//��ʱ������ 
// 		 int hPlayback; 
// 		 hPlayback = NET_DVR_GetFileByTime(lUserId, 1, &struStartTime, &struStopTime, "./test.mp4"); 
// 		 if(hPlayback < 0) 
// 		 { 
// 		   printf("NET_DVR_GetFileByTime fail,last error %d\n",NET_DVR_GetLastError()); 
// 		   NET_DVR_Logout(lUserId); 
// 		   NET_DVR_Cleanup();   
// 		   return false; 
// 		 } 
// 
// 
// 		 //��ʼ���� 
// 		 if(!NET_DVR_PlayBackControl(hPlayback, NET_DVR_PLAYSTART, 0, NULL)) 
// 		 { 
// 			 printf("play back control failed [%d]\n",NET_DVR_GetLastError()); 
// 			 NET_DVR_Logout(lUserId); 
// 			 NET_DVR_Cleanup(); 
// 			 return true; 
// 		 }  
// 
// 
// 		 int nPos = 0; 
// 		 for(nPos = 0; nPos < 100&&nPos>=0; nPos = NET_DVR_GetDownloadPos(hPlayback)) 
// 		 { 
// 			 Sleep(5000);    //millisecond 
// 		 } 
// 		 if(!NET_DVR_StopGetFile(hPlayback)) 
// 		 { 
// 			 printf("failed to stop get file [%d]\n",NET_DVR_GetLastError()); 
// 			 NET_DVR_Logout(lUserId); 
// 			 NET_DVR_Cleanup(); 
// 			 return false; 
// 		 } 
// 		 if(nPos<0||nPos>100) 
// 		 { 
// 			 printf("download err [%d]\n",NET_DVR_GetLastError()); 
// 			 NET_DVR_Logout(lUserId); 
// 			 NET_DVR_Cleanup(); 
// 			 return true; 
// 		  } 
//     
// 	   
// 	   
// 	  
// 	   
// 		//����Ԥ�� �����ûص�������
// 		//	LONG IrealPlayHandle;
// 		HWND pHwnd = this->m_hWnd;  //��ȡ���ھ��
// 		NET_DVR_CLIENTINFO clientnfo ={0};
// 		
// 		clientnfo.hPlayWnd = GetDlgItem(IDC_STATIC_PLAY)->m_hWnd; //��ô��ڵľ��
// 		//		ClientInfo->hPlayWnd = pHwnd;              //��ҪSDK����ʱ�����Ϊ��Чֵ����ȡ��������ʱ����Ϊ�� 
// 		clientnfo.lChannel = 1;              //Ԥ��ͨ���� 
// 		 clientnfo.lLinkMode = 0;              /*���λ(31)Ϊ 0��ʾ��������Ϊ1 ��ʾ��������0��30λ��ʾ����
// 		 ��ʽ��0��TCP ��ʽ��1��UDP ��ʽ��2���ಥ��ʽ��3��RTP ��ʽ; 4- RTP over RTSP��5- RTSP over HTTP */ 
// 		 clientnfo.sMultiCastIP = NULL;      //�ಥ��ַ����Ҫ�ಥԤ��ʱ���� 
// 		 
// 		 //		BOOL bPreviewBlock = false;              //�������������Ƿ�������0����1���� 
// 		 //		IrealPlayHandle = NET_DVR_RealPlay_V30(lUserID, &ClientInfo, NULL, NULL, 0);
// 		 
// 		 m_lPlayHandle_record = NET_DVR_RealPlay_V30(lUserId,&clientnfo,NULL,NULL,TRUE);
// 		 if(-1 == m_lPlayHandle_record)
// 		 {
// 			 DWORD err=NET_DVR_GetLastError();
// 			 CString m_csErr;
// 			 m_csErr.Format("���ų����������%d",err);
// 			 MessageBox(m_csErr);
// 			 NET_DVR_Cleanup();
// 			 return false;
// 		 }
// 
// 	 char RecName[256] = {0};
// 	CTime CurTime = CTime::GetCurrentTime();;
// 	sprintf(RecName,"%04d%02d%02d%02d%02d%02d_ch.mp4",CurTime.GetYear(),CurTime.GetMonth(),CurTime.GetDay(), \
// 		CurTime.GetHour(),CurTime.GetMinute(),CurTime.GetSecond());
// 
// 	if(!NET_DVR_SaveRealData(m_lPlayHandle_record,RecName))
// 	 {
// 		 MessageBox("����¼��ʧ��");
// 		 return true;
// 	 }
// 	STARTRECORD = false;
// //	 GetDlgItem(IDC_BUTTON_LOGIN1)->SetWindowText("ֹͣ¼��");
// 		 
// 	}
// 	else if (lUserId < 0)
// 	{
// 		NET_DVR_Cleanup();
// 		return;
// 	}




/*********************************
����˵��������Ԥ������
����˵����dwMode ѡ���ռ���� ��1�� ����������2�� 
wVolume  �������� ȡֵ��Χ[0,0xffff] 
����ֵ �ɹ�����1 ʧ�ܷ���0
*********************************/
int VideoInterFace::OpenSound(DWORD dwMode, WORD wVolume)
{
	//������������ģʽ��
	BOOL Result ;
	Result = NET_DVR_SetAudioMode(dwMode);
	if (Result == 1) //�ɹ�
	{
		switch(dwMode)
		{
		case 1:
			//	��ռ����ģʽ�¿�������
			Result = NET_DVR_OpenSound(m_lPlayHandle_record);
			if (Result == 0)
			{
				return 0;
			}
			break;
		case 2:
			//	��������ģʽ�¿���������
			
			Result = NET_DVR_OpenSoundShare(m_lPlayHandle_record);
			if (Result == 0)
			{
				return 0;
			}
			break;
		default:
			break;
		}
		//	���ڲ���������
		NET_DVR_Volume(m_lPlayHandle_record,wVolume);
	}
	
	return 1;
}

/*********************************
����˵�����ر�Ԥ������
����˵����dwMode ѡ���ռ���� ��1�� ����������2�� 
����ֵ �ɹ�����1 ʧ�ܷ���0
*********************************/
int VideoInterFace::CloseSound(DWORD dwMode)
{
	BOOL Result;
	switch(dwMode)
	{
	case 1:
		//��ռ����ģʽ�¹ر�������		
		Result = NET_DVR_CloseSound();
		if (Result == 0)
		{
			return 0;
		}
		break;
	case 2:
		//��������ģʽ�¹ر�������	
		Result = NET_DVR_CloseSoundShare(m_lPlayHandle_record);
		break;
	default:
		break;
	}
	return 1;
}
