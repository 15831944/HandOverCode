// SOCKET.h: interface for the CSOCKET class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKET_H__2A559AC2_EEE2_4A35_9BC1_5F25569D2267__INCLUDED_)
#define AFX_SOCKET_H__2A559AC2_EEE2_4A35_9BC1_5F25569D2267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock.h"
#pragma comment(lib,"wsock32")
#include <string>
#include <deque>
#include <map>
#include <iostream>


#define MAX_ExamCar 60

using namespace std;

struct PackageHead_st
{
	UINT PackageType; //�������� 1 ��������Ϣ�İ�
};
typedef struct PackageHead_st PackageHead_ST;

struct ExamCar_st
{
	UINT ExamCarNumber;//������
	UINT ThreadNumber;//�̺߳�
	DOUBLE x_Gps;  // ���͹�����GPS�� ��γ�� X����
	DOUBLE y_Gps;  // ���͹�����GPS�� �ľ��� y����
	UINT zt;//״̬
	UINT ErrorID;//������
};
typedef struct ExamCar_st ExamCar_ST;

struct Package_st
{
	PackageHead_ST pPackageHead_ST;
	ExamCar_ST pExamCar_ST;
};
typedef struct Package_st Package_ST;

class CSOCKET  
{
public:
	BOOL dequeOK;  //���ڶ��е�ѹ���ȡ�� TRUE ���� FALSE ������
	void InitSocket();
	SOCKET m_hSocket;
	void WriteLog(CString szERR);
//	void WriteLog(CString szERR,CString dataFile);
	CSOCKET();
	virtual ~CSOCKET();
	deque<Package_ST *> pPackage_ST;
	map<int,UINT> SockToCar;
	
	UINT TotalCar;  //��ǰ������������
	int iC_Dlg_F;//�����Ի������
	


};



#endif // !defined(AFX_SOCKET_H__2A559AC2_EEE2_4A35_9BC1_5F25569D2267__INCLUDED_)
