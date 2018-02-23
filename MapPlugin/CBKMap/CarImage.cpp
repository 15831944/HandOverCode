// CarImage.cpp: implementation of the CCarImage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CarImage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCarImage::CCarImage()
{
	m_CRotate=0.0f;
	m_CenterPt.X=0;
	m_CenterPt.Y=0;
	m_mapPT.X=0;
	m_mapPT.Y=0;
	isOnLine=false;
	m_iCarState=0;
	idrawimg=1;
	m_pImgTipBox=NULL;
}

CCarImage::~CCarImage()
{
//	delete m_pImageCar;
}

void CCarImage::InitCar(CString strKchp,Image *pImage)
{
	m_pImageCar=pImage->Clone();
	m_ImgSize.cx=m_pImageCar->GetWidth();
	m_ImgSize.cy=m_pImageCar->GetHeight();
 	memset(tcharArr,0x0,sizeof(tcharArr));
 	swprintf((wchar_t *)tcharArr,L"%s",strKchp.AllocSysString());
	//////////////////////////////////////////////////////////////////////////		
	format.SetAlignment(StringAlignmentCenter);//Ĭ�϶���Ӱ�촹ֱ 
	format.SetLineAlignment(StringAlignmentCenter);// ��SetAlignment��Ӧ��Ϊ��ֱ������
	Pen tempPen(Color(255,40,176,255),5);//, 255));
	EllipsePen =tempPen.Clone();
}

void CCarImage::DrawCar(Graphics *graphics)
{	
// 	if (idrawimg ==1 || idrawimg ==2 )//�����߿�
// 	{
// 		graphics->TranslateTransform(m_CenterPt.X,m_CenterPt.Y);
// 		graphics->RotateTransform(m_CRotate);
// 		//�ָ���ͼƽ����ˮƽ�ʹ�ֱ�����ƽ��
// 		graphics->TranslateTransform(-m_CenterPt.X, -m_CenterPt.Y);
// 		//����ͼƬ
// 		graphics->DrawImage(m_pImageCar,m_picRCF);
// 		graphics->ResetTransform();
// 	}
// 	if (idrawimg ==2 || idrawimg ==3)
// 	{
// 		Image image(L"tipbox.png");
// 
// 		graphics->DrawImage(&image,m_CenterPt.X,m_CenterPt.Y);
// 	}

	//////////////////////////////////////////////////////////////////////////
	if (idrawimg >=0 )
	{
 		RectF temprf(m_CenterPt.X-10,m_CenterPt.Y-10,20,20);
		graphics->DrawEllipse(EllipsePen, temprf);
	}
	if (idrawimg==1)
	{
		graphics->DrawImage(m_pImageCar,m_CenterPt.X,m_CenterPt.Y);
		Font myFont(L"����",14);		
		SolidBrush blackBrush(Color(250,250,255,7));
		graphics->DrawString(
			tcharArr,
			wcslen(tcharArr),
			&myFont,
			m_picRCF,
			&format,
		&blackBrush);
	}
	else if (idrawimg==2)
	{
		graphics->DrawImage(m_pImgTipBox,m_CenterPt.X,m_CenterPt.Y);
		RectF destRect(m_CenterPt.X+52,m_CenterPt.Y+56,142,20);
		Font myFont(L"����",10);		
		SolidBrush blackBrush(Color(250,250,255,7));
		
		DrawStringz(graphics,m_strsd,myFont,blackBrush,destRect,format);
		destRect.Offset(0,20);
		DrawStringz(graphics,m_strlc,myFont,blackBrush,destRect,format);
	}
	else
	{

	}
}

void CCarImage::SetXYRotate(float cx,float cy,float cr)
{
	m_mapPT.X =cx;
	m_mapPT.Y =cy;
	m_CRotate =cr;
	m_iCarState=10;
//	isOnLine =true;
//	TRACE("%d=(%f,%f)\n",m_CarNum,m_mapPT.X,m_mapPT.Y);
}

BOOL CCarImage::IsPtinScreen(CRect src, float fzl)
{	
	if (isOnLine==false)
	{
		return FALSE;
	}
	CPoint temppt;
	temppt.x =(int)m_mapPT.X;
	temppt.y =(int)m_mapPT.Y;
	if (!src.PtInRect(temppt))
	{//������Ļ������
		return FALSE;
	}
	m_CenterPt.X=(m_mapPT.X-src.left)/fzl;
	m_CenterPt.Y=(m_mapPT.Y-src.top)/fzl;
	m_picRCF.X=m_CenterPt.X+6;
	m_picRCF.Y=m_CenterPt.Y+4;
	m_picRCF.Width =(float)m_ImgSize.cx-6;
	m_picRCF.Height =(float)m_ImgSize.cy-4;
	if(m_iCarState>0)
	{
		switch (m_iCarState)
		{
		case 1:
			EllipsePen->SetColor(Color(255,240,150,9));
			break;
		case 5:
			EllipsePen->SetColor(Color(200,30,176,250));
			break;
		case 9:
			EllipsePen->SetColor(Color(255,30,176,250));
			break;
		default:
			break;
		}
		m_iCarState--;
	}
	return TRUE;
}
int CCarImage::GetCarNum()
{
	return 1;
}

void CCarImage::SetEllipsePenColor(UINT iset)
{
	switch (iset)
	{
	case 1:
		EllipsePen->SetColor(Color(255,240,150,9));
		break;
	case 2:
		EllipsePen->SetColor(Color(255,255,0,151));
		break;
	case 3:
		EllipsePen->SetColor(Color(255,51,153,51));
		break;
	default:
		break;
	}
}

void CCarImage::SetDrawimg(int drawi)
{
	idrawimg =drawi;
}

void CCarImage::SetStuZkzm(CString strStuzkzm)
{
	if (!strStuzkzm.IsEmpty())
	{
		memset(tcharArrzkzm,0x0,sizeof(tcharArrzkzm));
 		swprintf((wchar_t *)tcharArrzkzm,L"%s",strStuzkzm.AllocSysString());
		if (m_pImgTipBox!=NULL)
		{
			delete m_pImgTipBox;
			m_pImgTipBox=NULL;
		}
		m_pImgTipBox =Gdiplus::Image::FromFile(tcharArrzkzm);
	}	
}

void CCarImage::SetGnssMsg(GNSSMSGz gnssmsg)
{
	m_mapPT.X =(float)gnssmsg.cx;
	m_mapPT.Y =(float)gnssmsg.cy;
	m_CRotate =(float)gnssmsg.cr;

	m_strsd.Format("%.2f km/h",gnssmsg.sd);
	m_strlc.Format("%.2f m",gnssmsg.lc);

	m_iCarState=10;
}
