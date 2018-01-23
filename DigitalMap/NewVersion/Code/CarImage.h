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
	void SetXY(int gx,int gy);
	void SetCarNum(int inum);
	void SetStuZkzm(CString zkzm);
	void SetDrawimg(int drawi);
	void SetEllipsePenColor(UINT iset);//����Բ����ɫ
	bool isOnLine;//��������
	BOOL IsPtinScreen(CRect src,float fzl, float fzlY);//�Ƿ�����Ļ������
	void SetXYRotate(double cx,double cy,double cr);//����XY����ת�Ƕ�
	void DrawCar(Graphics* graphics);//����
	void InitCar(int cnum);//��ʼ������
	int GetCarNum();//��ȡ������
private:
	int idrawimg;
	float m_CRotate;//��ת�Ƕ�
	int m_CarNum;//������
//	Image *m_pImageCar;//����ͼ
//	CSize m_ImgSize;//ͼ���С
	PointF m_CenterPt,m_mapPT;//����
	RectF m_picRCF;//m_CenterPtΪ���ĵľ��� �����ż������
	Pen *EllipsePen;
	//////////////////////////////////////////////////////////////////////////
	unsigned short tcharArr[10];//������
	unsigned short tcharArrzkzm[50];//׼��֤�����
	StringFormat format;
};

#endif // !defined(AFX_CARIMAGE_H__34280621_6E08_427A_BBF9_B41D849FBFC1__INCLUDED_)
