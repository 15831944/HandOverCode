// FourthPicJMQ.h: interface for the CFourthPicJMQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOURTHPICJMQ_H__7970E164_2015_4B34_9E91_763BC138A42E__INCLUDED_)
#define AFX_FOURTHPICJMQ_H__7970E164_2015_4B34_9E91_763BC138A42E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
class CFourthPicJMQ  : public CAVIThreadz
{
public:	
	UINT uDituPy;
	int iRunNum;//�����м���
	//////////////////////////////////////////////////////////////////////////
	void FourthPicInit(int ikch, CString path,int wMSG,HWND hwndz);//��ʼ��
	void LoadMapCfg(CString path);		//���ص�ͼ�ļ� û����Ĭ������Ŀ�ư汾
	void SetSleepTime(DWORD dwTime);	//Ĭ��1000����
	//////////////////////////////////////////////////////////////////////////
	void On17C51(CString zkzmbh,int ikscs,int idrcs);		//׼��֤�����,���Դ���,���մ���
	void On17C52(CString zkzmbh,int icode,CString smsg);	//׼��֤�����,��Ŀ���,��Ϣ
	void On17C53(CString strTime,CString smsg,int ikcfs);	//�۷�ʱ��,�۷�ԭ��,�۳�����
	void On17C55(int icode,CString smsg);					//��Ŀ���,��Ϣ
	void On17C56(int itype,int kscj);						//����״̬1�ϸ�2���ϸ�,���Գɼ�
	void OnGnssData(LPVOID msgz);							//��γ���ٶ���̵Ƚṹ��
	//////////////////////////////////////////////////////////////////////////	
public:
	void SetNowDataPath(CString path,UINT uKSKM);
	CFourthPicJMQ();
	virtual ~CFourthPicJMQ();
protected:
	DWORD ThreadMethod();//�߳�
private:
	UINT m_uKSKM;
	CString stuDataName,m_NowDataPath;
	void WriteStuData(LPCTSTR pstrFormat, ...);//����ط�����
	//////////////////////////////////////////////////////////////////////////
	bool m_DrawMap;				//�Ƿ񻭵�ͼ
	void DrawXMListTM();		//��Ŀ�б��߳�
	void DrawMapTM();			//��̬��ͼ�߳�
	//////////////////////////////////////////////////////////////////////////
	int m_type;
	int m_i52type,m_i55type;
	int GetMType(int &type);
	void SetMtype(int &type,int idrxmp);
	void DrawXMList(Graphics *graphics, Image *img);
	//////////////////////////////////////////////////////////////////////////
	bool m_DrawCar;				//�Ƿ���ʾ����
	POINT m_mayWH;				//��ͼ����
	double m_Mapx,m_Mapy,m_bs;	//��γ�� �ͱ���
	int m_imapx,m_imapy;		//���� γ�� ƽ�������
	//////////////////////////////////////////////////////////////////////////
	int ikscj;					//���Գɼ�	
	int iDrXMP;					//����Ŀ��
	int m_ErrorLine;			//������
	CString m_ErrorMSG[3][3];	//9��������Ϣ
	CString nowztstr;			//��ǰ״̬
	CTime m_StartTime;			//���Կ�ʼʱ��	
	CString m_kscssj;
	GNSSMSG m_GnssMsg;			//GNSS����
	int GetXMPi(int icode);
	void DrawTime(Graphics *graphics,int x,int y);//����ʱ��
	//////////////////////////////////////////////////////////////////////////
	Image *ImgMark;			//���ֲ�
	Image *ImgMap;			//��ͼ�ļ�
	Image *ImgTime;			//ʱ��
	Image *ImgXmp;			//��Ŀ��
	Image *ImgCAR;			//����
	//////////////////////////////////////////////////////////////////////////
	DWORD m_SleepTime;		//�߳����м��ʱ��
	HANDLE hThreadEvent;	//�߳̾��
	bool threadRun;			//�߳��Ƿ�����

	//huangqiwei
	CString m_sExePath;	//����·��
	CString m_sMapPath;	//��ͼ·��
	bool m_bNineMaps;		//�Ƿ���þŹ���ģʽ
	bool m_bBigCar;	//��
	Image *ImgSignal;			//�ź�
	Image *ImgXMList;		//��ͼģʽ��ҲҪչʾ��Ŀ��
	Image *ImgXMListMark;		//��ͼģʽ��ҲҪչʾ��Ŀ��,����ͼ����Ŀ����
	bool m_bDrawSignal;	//�Ƿ�����ź�
	bool m_bSignalAqd;	//��ȫ���ź�
	bool m_bSignalJs;	//��ɲ�ź�
	bool m_bSignalSs;	//��ɲ�ź�
	bool m_bSignalFs;	//��ɲ�ź�
	bool m_bSignalLb;	//�����ź�
	bool m_bSignalZzx;	//��ת���ź�
	bool m_bSignalYzx;	//��ת���ź�
	bool m_bSignalYgd;	//Զ����ź�
	bool m_bSignalJgd;	//������ź�
	bool m_bSignalXh;	//Ϩ���ź�
	bool m_bSignalKgm;	//�������ź�
	bool m_bSignalLh;	//����ź�
	bool m_bSignalDw;	//��λ�ź�
	void DrawSignal(Graphics *graphics);//ʵʱ�ź�
	void DrawNineMaps(Graphics *graphics, int carX, int carY);//���Ź����ͼ
};

#endif // !defined(AFX_FOURTHPICJMQ_H__7970E164_2015_4B34_9E91_763BC138A42E__INCLUDED_)
