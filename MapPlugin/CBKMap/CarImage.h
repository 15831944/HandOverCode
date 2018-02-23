// CarImage.h: interface for the CCarImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CARIMAGE_H__34280621_6E08_427A_BBF9_B41D849FBFC1__INCLUDED_)
#define AFX_CARIMAGE_H__34280621_6E08_427A_BBF9_B41D849FBFC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCarImage  
{
public:	
	CCarImage();
	virtual ~CCarImage();
public:	
	void SetGnssMsg(GNSSMSGz gnssmsg);
	void InitCar(CString strKchp,Image *pImage);//��ʼ������
	void SetStuZkzm(CString strStuzkzm);
	void SetDrawimg(int drawi);
	void SetEllipsePenColor(UINT iset);//����Բ����ɫ
	bool isOnLine;//��������
	BOOL IsPtinScreen(CRect src,float fzl);//�Ƿ�����Ļ������
	void SetXYRotate(float cx,float cy,float cr);//����XY����ת�Ƕ�
	void DrawCar(Graphics* graphics);//����	
	int GetCarNum();//��ȡ������
private:
	CString m_strlc;
	CString m_strsd;
	int m_iCarState;
	int idrawimg;
	float m_CRotate;//��ת�Ƕ�
//	int m_CarNum;//������
	Image *m_pImageCar;//����ͼ
	Image *m_pImgTipBox;
	CSize m_ImgSize;//ͼ���С
	PointF m_CenterPt,m_mapPT;//����
	RectF m_picRCF;//m_CenterPtΪ���ĵľ��� �����ż������
	Pen *EllipsePen;
	//////////////////////////////////////////////////////////////////////////
	unsigned short tcharArr[20];//������
	unsigned short tcharArrzkzm[256];//׼��֤�����
	StringFormat format;
};

#endif // !defined(AFX_CARIMAGE_H__34280621_6E08_427A_BBF9_B41D849FBFC1__INCLUDED_)
