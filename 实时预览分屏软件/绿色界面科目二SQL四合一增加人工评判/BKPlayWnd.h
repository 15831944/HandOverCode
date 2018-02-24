// BKPlayWnd.h: interface for the CBKPlayWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BKPLAYWND_H__5D547FA8_6194_48CD_A46D_D23749941A6E__INCLUDED_)
#define AFX_BKPLAYWND_H__5D547FA8_6194_48CD_A46D_D23749941A6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HCNetSDK.h"
#pragma comment(lib,"HCNetSDK.lib")

class CBKPlayWnd  
{
public:
	DWORD dwlogid;
	CWriteLog logz;
	BOOL CloseVoiceCom();
	BOOL VoiceIntercom();
	BOOL CloseVoice();
	BOOL OpenVoice();
	BOOL DvrStopRecord();
	BOOL DvrStartRecord(CString str);
	BOOL ISPlaywndd;
	void Drawztext(CString zstr);
	BOOL DvrRealPlay(LONG lChannel);
	BOOL DvrInitLogin(HWND hWnd,char *sDVRIP,char *sUserName,char *sPassword,WORD wDVRPort);
	BOOL DvrLoginout();
	CBKPlayWnd();
	virtual ~CBKPlayWnd();

private:
	void GetLastErrorz();
	HWND m_zhwnd;
	UINT m_wNum;
	LONG m_lPlayHandle;//�������ž��
	LONG m_lVocieComHandle;//�����Խ����
	LONG m_byStartDTalkChan;//��ʼ�Խ�ͨ����
	LONG m_lChannel;//IPͨ������
    LONG m_lUserID;//��¼����ֵID
	BOOL m_VoiceState;
	BOOL m_bVoiceCom;//�����Խ�״̬
	NET_DVR_DEVICEINFO_V30 struDeviceInfo;
	BOOL m_bIsRecording;
};

#endif // !defined(AFX_BKPLAYWND_H__5D547FA8_6194_48CD_A46D_D23749941A6E__INCLUDED_)
