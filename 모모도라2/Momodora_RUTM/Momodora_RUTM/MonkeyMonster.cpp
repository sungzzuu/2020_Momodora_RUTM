#include "stdafx.h"
#include "MonkeyMonster.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Leaf.h"
#include "Gold.h"
#include "SoundMgr.h"
CMonkeyMonster::CMonkeyMonster()
	:m_dwAttack(GetTickCount()), m_eType(MONSTER::MONKEY)
{
}


CMonkeyMonster::~CMonkeyMonster()
{
}

void CMonkeyMonster::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/MonkeyMonster/Monkey_RIGHT.bmp", L"Monkey_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/MonkeyMonster/StretchNormal.bmp", L"StretchNormal");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/MonkeyMonster/StretchAttack.bmp", L"StretchAttack");

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 300.f;
	m_tInfo.iCX = 100;
	m_tInfo.iCY = 72;
	m_tInfo.iCollisionCX = 50;
	m_tInfo.iCollisionCY = 30;
	m_eCurState = MOVE;
	m_ePreState = MOVE;
	m_bRight = RIGHT;

	m_pFrameKey = L"Monkey_RIGHT";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iPosX = 2;
	m_tFrame.iPosY = 17;
	m_tFrame.iSizeX = 44;
	m_tFrame.iSizeY = 32;
	m_tFrame.dwFrameSpeed = 100;
	m_tFrame.dwFrameTime = GetTickCount();
	m_fSpeed = 1.f;
	m_iAttackRandom = 0;
	m_iLeafNum = 0;
	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);


	//게이지
	m_tGauge.iHp = 300;
	m_tGauge.iGold = 10;
	m_tGauge.iMaxHp = 300;
	m_tGauge.iAtt = 50;

	m_eGroup = GROUPID::OBJECT;


}

int CMonkeyMonster::Update()
{
	if (m_dwCollision + 300 < GetTickCount())
		m_bAttack = false;
	Frame_Move();
	Update_Rect();
	if (MOVE == m_eCurState)
	{
		if (m_pFrameKey == L"Monkey_RIGHT")
			m_tInfo.fX += m_fSpeed;
		else
			m_tInfo.fX -= m_fSpeed;
	}
	if (m_tInfo.fX > CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX)
	{
		m_pFrameKey = L"Monkey_LEFT";
	}
	else
	{
		m_pFrameKey = L"Monkey_RIGHT";
	}
	if (m_dwAttack + 2000 < GetTickCount())
	{
		m_dwAttack = GetTickCount();
		m_eCurState = ATTACK;
	}


	Scene_Change();

	if (ATTACK == m_eCurState)
	{
		m_tInfo.fY -= 20.f;
		m_tInfo.iCX = 180;
		m_tInfo.iCY = 110;
		m_tInfo.iCollisionCX = 100;
		m_tInfo.iCollisionCY = 30;

		float fY = 0.f;
		bool _bCheck = false;
	}
	else
	{
		m_tInfo.iCX = 100;
		m_tInfo.iCY = 72;
		m_tInfo.iCollisionCX = 50;
		m_tInfo.iCollisionCY = 30;

	}
	// 크기가 커지면 바닥충돌 안된다,,,,
	float fY = -1.f;
	bool _bCheck = false;

	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);

	if (m_tGauge.iHp <= 0)
	{
		for (int i = 0; i < m_tGauge.iGold; i++)
		{
			int _ran = rand() % 2;
			if (0 == _ran)
				_ran = -1;
			else
				_ran = 1;
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
			CSoundMgr::Get_Instance()->PlaySound(L"sndMonsterDeath.wav", CSoundMgr::MONSTER);
			CObj* pObj = CAbstractFactory<CGold>::Create(m_tInfo.fX + i*_ran*20, m_tInfo.fY- (m_tInfo.iCY));
			CObjMgr::Get_Instance()->Add_Object(OBJID::GOLD, pObj);
		}

		return OBJ_DEAD;

	}

	return OBJ_NOEVENT;

}

void CMonkeyMonster::Late_Update()
{
	if (CObjMgr::Get_Instance()->Collision_Monster_Weapon(this, &m_bAttack))
	{
		m_eCurState = HIT;
		m_iAttackRandom = rand() % 2;
		m_tFrame.iFrameStart = m_iAttackRandom;
		m_dwCollision = GetTickCount();
	}
}

void CMonkeyMonster::Render(HDC _DC)
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
	if (!lstrcmp(L"Monkey_LEFT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Monkey_RIGHT");
		TCHAR* FrameKey = nullptr;
		if (m_eCurState == ATTACK)
			FrameKey = L"StretchAttack";
		else
			FrameKey = L"StretchNormal";
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(FrameKey);
		StretchBlt(hStretch
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC2
			, 1 + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX) + m_tFrame.iSizeX, m_tFrame.iPosY
			, -m_tFrame.iSizeX, m_tFrame.iSizeY
			, SRCCOPY);

		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY
			, hStretch
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, RGB(240, 240, 240));
	}
	else
	{
		GdiTransparentBlt(_DC
			, m_tRect.left + iScrollX, m_tRect.top + iScrollY
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC
			, m_tFrame.iPosX + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX), m_tFrame.iPosY
			, m_tFrame.iSizeX, m_tFrame.iSizeY
			, RGB(240, 240, 240));
	}


}

void CMonkeyMonster::Release()
{
}

void CMonkeyMonster::Scene_Change()
{


	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case MOVE:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 17;
			m_tFrame.iSizeX = 44;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case HIT:
			m_tFrame.iFrameStart = m_iAttackRandom;
			m_tFrame.iFrameEnd = m_iAttackRandom;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 70;
			m_tFrame.iSizeX = 44;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 400;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 123;
			m_tFrame.iSizeX = 80;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CMonkeyMonster::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			m_eCurState = MOVE;
			m_tFrame.iFrameStart = 0;
		}
	}
}

