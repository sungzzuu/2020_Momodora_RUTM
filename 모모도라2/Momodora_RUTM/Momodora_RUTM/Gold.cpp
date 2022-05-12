#include "stdafx.h"
#include "Gold.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include  "LineMgr.h"


CGold::CGold()
{
}


CGold::~CGold()
{
}

void CGold::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Money/Gold.bmp", L"Gold");

	//°ÔÀÌÁö

	m_tInfo.iCX = 50;
	m_tInfo.iCY = 50;


	m_tInfo.iCollisionCX = 2;
	m_tInfo.iCollisionCY = 2;

	m_pFrameKey = L"Gold";


	m_fJumpPower = 3.f;
	m_fJumpAccel = 0;
	m_bJump = true;


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 3;
	m_tFrame.iPosX = 0;
	m_tFrame.iPosY =0;
	m_tFrame.iSizeX = 16;
	m_tFrame.iSizeY = 16;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	
	
}

int CGold::Update()
{
	Update_Rect();
	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);
	//Jumping();
	Frame_Move();
	return OBJ_NOEVENT;
}

void CGold::Late_Update()
{
}

void CGold::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);
	GdiTransparentBlt(_DC
		, m_tRect.left + iScrollX, m_tRect.top + iScrollY
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, m_tFrame.iFrameStart*m_tFrame.iSizeX, 0
		, m_tFrame.iSizeX, m_tFrame.iSizeY
		, RGB(240, 240, 240));
}

void CGold::Release()
{
}

void CGold::Jumping()
{
	float fY = 0.f;
	bool bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		m_tInfo.fY -= m_fJumpPower * m_fJumpAccel - 10.8f * m_fJumpAccel * m_fJumpAccel * 0.5f;
		m_fJumpAccel += 0.2f;

		if (m_tInfo.fY > fY)
		{
			m_bJump = false;
			m_fJumpAccel = 0.f;
			m_tInfo.fY = fY;
		}
	}
	else if (bLineCol)
		m_tInfo.fY = fY;
}

void CGold::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_tFrame.iFrameStart = 0;
		}
	}
}
