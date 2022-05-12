#include "stdafx.h"
#include "SavePoint.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"

CSavePoint::CSavePoint()
{
}


CSavePoint::~CSavePoint()
{
}

void CSavePoint::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/SavePoint/SavePoint1.bmp", L"SavePoint1");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/SavePoint/SavePoint2.bmp", L"SavePoint2");


	//게이지

	m_tInfo.iCX = 128;
	m_tInfo.iCY = 128;

	m_tInfo.iCollisionCX = 128;
	m_tInfo.iCollisionCY = 128;

	m_pFrameKey = L"SavePoint1";

	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iPosX = 0;
	m_tFrame.iPosY = 0;
	m_tFrame.iSizeX = 128;
	m_tFrame.iSizeY = 128;

	m_eGroup = GROUPID::BACKGROUND;

}

int CSavePoint::Update()
{
	return OBJ_NOEVENT;
}

void CSavePoint::Late_Update()
{
	// 충돌 시 저장됨
}

void CSavePoint::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bLineOn)
	{
		HPEN hpen = CreatePen(PS_SOLID, 2, DEEPGREEN);
		HGDIOBJ holdpen = SelectObject(_DC, hpen);
		HBRUSH myBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(_DC, myBrush);
		Rectangle(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY, m_tRect.right + iScrollX, m_tRect.bottom + iScrollY);
		Rectangle(_DC, m_tCollisionRect.left + iScrollX, m_tCollisionRect.top + iScrollY, m_tCollisionRect.right + iScrollX, m_tCollisionRect.bottom + iScrollY);
		SelectObject(_DC, oldBrush);
		DeleteObject(myBrush);
		DeleteObject(hpen);
	}

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);
	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iPosX, m_tFrame.iPosY
		, m_tFrame.iSizeX, m_tFrame.iSizeY
		, RGB(240, 240, 240));
}

void CSavePoint::Release()
{
}
