#include "stdafx.h"
#include "Tile.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"


CTile::CTile()
	:m_iDrawID_X(0), m_iDrawID_Y(0), m_iOption(0)
{
}


CTile::~CTile()
{
}

void CTile::Initialize()
{
	m_tInfo.iCX = TILECX;
	m_tInfo.iCY = TILECY;
	

}

int CTile::Update()
{
	Update_Rect();

	return OBJ_NOEVENT;
}

void CTile::Late_Update()
{

}

void CTile::Render(HDC _DC)
{
	Update_Rect();

	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (1 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Tile1");
		BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X*TILECX, m_iDrawID_Y*TILECY
			, SRCCOPY);
	}
	else if (2 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"StoneTile");
		BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X*TILECX, m_iDrawID_Y*TILECY
			, SRCCOPY);
	}
	else if (3 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"BounceBall");
		BitBlt(_DC, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X*TILECX, m_iDrawID_Y*TILECY
			, SRCCOPY);
	}
	else if (4 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ThinTile");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, 0, 0
			, TILECX, TILECY
			, RGB(255, 255, 255));
	}
	else if (5 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Ladder");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X*16, m_iDrawID_Y*16
			, 16, 16
			, RGB(255, 255, 255));
	}
	else if (6 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"HardTile");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX*2, TILECY
			, hMemDC
			, 0, 0
			, 80, 40
			, RGB(255, 255, 255));
	}
	else if (7 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"MovingTile");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX*3, TILECY*3
			, hMemDC
			, 0, 0
			, 120, 120
			, RGB(255, 255, 255));
	}
	else if (8 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Elevator");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX*3, TILECY
			, hMemDC
			, 0, 0
			, 120, 40
			, RGB(255, 255, 255));
	}
	else if (9 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"CityLadder");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X * 40, m_iDrawID_Y * 40
			, 40, 40
			, RGB(255, 255, 255));
	}
	else if (10 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"CityTile");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X * 40, m_iDrawID_Y * 40
			, 40, 40
			, RGB(255, 255, 255));
	}
	else if (11 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"CityThinTile");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X * 40, m_iDrawID_Y * 40
			, 40, 40
			, RGB(255, 255, 255));
	}
	else if (12 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Grass");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, m_iDrawID_X * 40, m_iDrawID_Y * 40
			, 40, 40
			, RGB(240, 240, 240));
	}
	else if (13 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Orb");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, 0, 0
			, 40, 40
			, RGB(240, 240, 240));
	
	}
	else if (14 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Thorn");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX * 2, TILECY
			, hMemDC
			, 0, 0
			, 80, 40
			, RGB(0, 0, 0));
	}
	else if (15 == m_iOption)
	{
		HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Orb");
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, TILECX, TILECY
			, hMemDC
			, 120, 0
			, 40, 40
			, RGB(240, 240, 240));
	}
}

void CTile::Release()
{
}
