// ThirdPicJMQ.h: interface for the CThirdPicJMQ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THIRDPICJMQ_H__8F5907CA_486F_4FD0_893F_8F97564C9226__INCLUDED_)
#define AFX_THIRDPICJMQ_H__8F5907CA_486F_4FD0_893F_8F97564C9226__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CThirdPicJMQ  : public CAVIThreadz
{
public:
	int iRunNum;
	void ThirdPicInit(int ikch, CString path,int wMSG,HWND hwndz);//��ʼ��
	void SetSQLOracle(UINT itype);				//�������ݿ�����SQL 1����Oracle 0
	void SetSleepTime(DWORD dwTime);
	void On17C51(CString zkzmbh,int ikscs,int idrcs);	//׼��֤�����,���Դ���,���մ���
	void On17C52(CString zkzmbh);						//׼��֤�����
	void On17C56(int izt,int kscj);						//����״̬1�ϸ�2���ϸ�,���Գɼ�
public:
	CThirdPicJMQ();
	virtual ~CThirdPicJMQ();
protected:
	DWORD ThreadMethod();//�߳�	
private:
	Image *ImgBK;			//����
	Image *ImgHGBHG;		//�ϸ񲻺ϸ�ͼƬ
	DWORD m_SleepTime;		//�߳����м��ʱ��
	HANDLE hThreadEvent;	//�߳̾��
	bool threadRun;			//�߳��Ƿ�����
	CString m_zkzmbh;		//׼��֤�����  17C51
	int m_idrcs,m_ikscs;	//���Դ��� ���մ��� 17C51
	CString textstr[9];	
	int m_iMtype;
	int m_izt;
	
	bool m_bEncrypt;	//���ݿ��ֶ��Ƿ���������

private:
	void DrawHbHg(Graphics *graphics,int x,int y);//17C56ʱ���� �� �ϸ��벻�ϸ�  	
	//////////////////////////////////////////////////////////////////////////
	CString m_Connstr;			//���ݿ������ִ�
	CString sqlExstr;			//��ѯ���
	_ConnectionPtr m_pConn;
	int CreateAdoConn();
	void RealseAdoConn();
	bool GetStuMSG();
	int GetDrcs();
	BOOL ReadPhotoFromDB(CDC *pDC, CString sCard,CRect rc);		//֤����Ƭ
	BOOL ReadMJPhotoFromDB(CDC *pDC, CString sCard,CRect rc); 	//�Ž���Ƭ
	//////////////////////////////////////////////////////////////////////////
};

#endif // !defined(AFX_THIRDPICJMQ_H__8F5907CA_486F_4FD0_893F_8F97564C9226__INCLUDED_)
