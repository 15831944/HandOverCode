// VideoInterFace.h: interface for the VideoInterFace class.
//
//////////////////////////////////////////////////////////////////////
#include "HCNetSDK.h"  //SDK��ͷ�ļ�  ��Ҫ����NET_DVR_DEVICEINFO_V30�������

#if !defined(AFX_VIDEOINTERFACE_H__46728AF6_F0B8_4370_91EC_4C41250F717C__INCLUDED_)
#define AFX_VIDEOINTERFACE_H__46728AF6_F0B8_4370_91EC_4C41250F717C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



typedef void(CALLBACK *fRealDataCallBack_V30)(
											  LONG      lRealHandle,
											  DWORD     dwDataType,
											  BYTE      *pBuffer,
											  DWORD     dwBufSize,
											  void      *pUser
											  );




class VideoInterFace : public CDialog 
{
public:
	int CloseSound(DWORD dwMode);
	int OpenSound(DWORD dwMode, WORD wVolume);
	int InitNET_DVR_AUDIOSTREAMDATA();
	void Drawztext(CString zstr);  //������ʾ�쳣����
	BOOL stopvideotape();
	int stopplay();
	BOOL startvideotape();  //��ʼ¼��
	BOOL NET_DVR_LOGIN(char *DVRIP,WORD Port,char *Name,char *Password,HWND pHwnd);
	int play(HWND pHwnd,int iChannel);
	LONG lUserId;
	char Dev_cfg[256];
	VideoInterFace();
	virtual ~VideoInterFace();

private:
	void GetLastErrorz();  // �����쳣��

	int m_iCurChanIndex; //��ǰͨ����
	BOOL VideoRunning;	//true �����������Ƶ false ��û���ڲ�����Ƶ
	HWND pHwnd;
	char * Password;
	char * UserName;
	WORD Port;
	char * DVRIP;
	LONG m_lPlayHandle_record;  //��NET_DVR_StopRealPlay�Ⱥ����ľ������
	BOOL isRecording; //����¼����Ƶ
	NET_DVR_DEVICEINFO_V30 struLocalDeviceInfo;
};






#endif // !defined(AFX_VIDEOINTERFACE_H__46728AF6_F0B8_4370_91EC_4C41250F717C__INCLUDED_)

