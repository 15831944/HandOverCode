// JMQCarz.h: interface for the CJMQCarz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JMQCARZ_H__7D783AED_D50E_4CDA_A1C3_6D94A1D0F311__INCLUDED_)
#define AFX_JMQCARZ_H__7D783AED_D50E_4CDA_A1C3_6D94A1D0F311__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CJMQCarz  
{
public:		
	CJMQCarz();
	virtual ~CJMQCarz();
private:
	int m_iKCH;
	LONG m_lUserID;//�豸��¼ID
	BYTE m_deChannl[4];//��������Ӧ�Ľ���ͨ����
	CWriteLog logz;//��־
	DWORD slogid;//JMQSuccess.log
	DWORD flogid;//JMQError.log
public:
	void OnGnssData(LPVOID msgz);
	void On17C56(int itype,int ikscj);
	void On17C55(int ic,CString msgz);
	void On17C53(CString timestr,CString msgz,int ikcfs);
	void On17C52(CString zkzm,int ic,CString msgz);
	void On17C51(CString zkzm,int idrcs,int ikscs);
	bool StartPassive(int iwnd,LONG &lph);//������������
	bool StopPassive(int iwnd);//ֹͣ��������
	bool StartDynamic(STerminalPZ tpz,int iwnd);//������̬����(STerminalPZ���ýṹ,���� 0 ���� 1��Ŀ)
	void StopDynamic(int iwnd);//ֹͣ��̬����(���� 0 ���� 1��Ŀ)
	//////////////////////////////////////////////////////////////////////////
	void InitCar(int iKch, LONG lUserID, BYTE (&deChan) [4],DWORD sid,DWORD fid,CString mPath);//��ʼ��
};

#endif // !defined(AFX_JMQCARZ_H__7D783AED_D50E_4CDA_A1C3_6D94A1D0F311__INCLUDED_)
