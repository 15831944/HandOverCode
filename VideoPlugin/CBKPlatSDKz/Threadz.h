// Threadz.h: interface for the CThreadz class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_)
#define AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//�̻߳��� 
class AFX_CLASS_EXPORT CThreadz  
{
public:
	DWORD Start(DWORD dwCreationFlags = 0);
	DWORD Stop (WORD timeout);			//�ȴ�һ��ʱ��
	DWORD Stop (bool bForceKill = false);
	DWORD Resume();						//�ָ��߳�
	DWORD Suspend();					//�����߳�
	int GetPriority();					//��ȡ�߳����ȼ�
	BOOL SetPriority(int priority);		//�����߳����ȼ�
	CThreadz();
	virtual ~CThreadz();
protected:
	virtual DWORD ThreadMethod() = 0;
private:
	static DWORD WINAPI EntryPoint(LPVOID pArg);//��̬��,���������ʵ�ʹ�������
	HANDLE	m_hThread;
	DWORD	m_dwTID;                    //Thread ID�߳�ID
	LPVOID	m_pParent;                  //this pointer of the parent CThread object
	DWORD	m_dwExitCode;               //Exit Code of the thread �߳��˳���   
protected:
    LPTHREAD_START_ROUTINE    m_pThreadFunction;    //�����߳�ָ��
    BOOL	m_runthread;                //�߳��Ƿ�������еı�־
};

#endif // !defined(AFX_THREADZ_H__1888E1D6_2C1F_4DF6_B343_B7FD6100E9A9__INCLUDED_)
