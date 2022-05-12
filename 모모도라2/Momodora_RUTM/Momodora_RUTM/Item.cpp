#include "stdafx.h"
#include "Item.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"

CItem::CItem()
{
}


CItem::~CItem()
{
}

void CItem::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/Item/Flower.bmp", L"Flower");

	//게이지

	m_tInfo.iCX = 72;
	m_tInfo.iCY = 100;
	m_eCurState = LIVE;

	m_tInfo.iCollisionCX = 40;
	m_tInfo.iCollisionCY = 100;

	m_pFrameKey = L"Flower";


	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iPosX = 0;
	m_tFrame.iPosY = 0;
	m_tFrame.iSizeX = 72;
	m_tFrame.iSizeY = 100;
	m_eGroup = GROUPID::BACKGROUND;

}

int CItem::Update()
{
	Update_Rect();
	//CObjMgr::Get_Instance()->Collision_Monster_Weapon(this, &m_bAttack);

	//조사하면 DIe하고 인벤으로 들어감
	return OBJ_NOEVENT;
}

void CItem::Late_Update()
{

}

void CItem::Render(HDC _DC)
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

void CItem::Release()
{
}

void CItem::Scene_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case LIVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 0;
			m_tFrame.iPosY = 0;
			m_tFrame.iSizeX = 72;
			m_tFrame.iSizeY = 100;
			break;
		case DIE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 72;
			m_tFrame.iPosY = 0;
			m_tFrame.iSizeX = 72;
			m_tFrame.iSizeY = 100;
			break;
		}
		m_ePreState = m_eCurState;
	}
}

void CItem::Frame_Move()
{
}
