#include "stdafx.h"
#include "JumpingMonster.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "Leaf.h"
#include "Sword.h"
#include "Gold.h"
#include "SoundMgr.h"

CJumpingMonster::CJumpingMonster()
	:m_eType(MONSTER::JUMPING)

{
}


CJumpingMonster::~CJumpingMonster()
{
	Release();
}

void CJumpingMonster::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/GirlMonster/Girl_RIGHT.bmp", L"Girl_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Monster/GirlMonster/Stretch.bmp", L"StretchGirl");

	m_tInfo.fX = 400.f;
	m_tInfo.fY = 300.f;
	m_tInfo.iCX = 80;
	m_tInfo.iCY = 80;
	m_tInfo.iCollisionCX = 50;
	m_tInfo.iCollisionCY = 30;
	m_eCurState = JUMP;
	m_ePreState = JUMP;

	m_pFrameKey = L"Girl_RIGHT";
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 0;
	m_tFrame.iPosX = 2;
	m_tFrame.iPosY = 233;
	m_tFrame.iSizeX = 32;
	m_tFrame.iSizeY = 32;
	m_tFrame.dwFrameSpeed = 300;
	m_tFrame.dwFrameTime = GetTickCount();
	m_fSpeed = 1.f;

	m_fJumpPower = 3.f;
	m_bJump = true;
	m_fJumpAccel = 0.f;

	m_dwAttack = GetTickCount();
	m_dwTrunTime = GetTickCount();
	float fY = 0.f;
	bool _bCheck = false;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY), &fY, &_bCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);

	//∞‘¿Ã¡ˆ
	m_tGauge.iHp = 300;
	m_tGauge.iGold = 10;
	m_tGauge.iMaxHp = 300;
	m_tGauge.iAtt = 50;


	m_eGroup = GROUPID::OBJECT;

}

int CJumpingMonster::Update()
{
	float fY = 0.f;
	bool _bCheck = false;
	
	if (m_dwCollision + 300 < GetTickCount())
		m_bAttack = false;

	if(m_eCurState != HURT && m_eCurState != THROW)
		Jumping();
	Frame_Move();
	Update_Rect();
	Scene_Change();
	if (m_dwTrunTime + 300 < GetTickCount())
	{
		if (abs(m_tInfo.fX - CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX) <400)
		{
			if (m_tInfo.fX >= CObjMgr::Get_Instance()->Get_Player()->Get_Info().fX)
				m_pFrameKey = L"Girl_LEFT";
			else
				m_pFrameKey = L"Girl_RIGHT";
			m_dwTrunTime = GetTickCount();
		}
		
	}
	

	if (m_pFrameKey == L"Girl_RIGHT")
		m_tInfo.fX += m_fSpeed;
	else
		m_tInfo.fX -= m_fSpeed;

	if (m_dwAttack + 3000 < GetTickCount())
	{
		m_eCurState = THROW;
		
		m_dwAttack = GetTickCount();
		
	}

	if (m_tGauge.iHp <= 0)
	{
		for (int i = 0; i < m_tGauge.iGold; i++)
		{
			int _ran = rand() % 2;
			if (0 == _ran)
				_ran = -1;
			else
				_ran = 1;
			
			CObj* pObj = CAbstractFactory<CGold>::Create(m_tInfo.fX + i*_ran * 20, m_tInfo.fY - (m_tInfo.iCY >> 1));
			CObjMgr::Get_Instance()->Add_Object(OBJID::GOLD, pObj);
		}
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::MONSTER);
		CSoundMgr::Get_Instance()->PlaySound(L"sndImpDeath.wav", CSoundMgr::MONSTER);
		return OBJ_DEAD;
	}
	return OBJ_NOEVENT;
}

void CJumpingMonster::Late_Update()
{
	if (CObjMgr::Get_Instance()->Collision_Monster_Weapon(this, &m_bAttack))
	{
		m_eCurState = HURT;
		m_tFrame.iFrameStart = 0;
		m_dwCollision = GetTickCount();

	}
}

void CJumpingMonster::Render(HDC _DC)
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
	if (!lstrcmp(L"Girl_LEFT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Girl_RIGHT");
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(L"StretchGirl");
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

void CJumpingMonster::Release()
{
	
}

void CJumpingMonster::Jumping()
{
	float fY = 0.f;
	bool bLineCol = CLineMgr::Get_Instance()->Collision_Line(m_tInfo.fX, &fY);

	if (m_bJump)
	{
		float fJumpsize = m_fJumpPower * m_fJumpAccel - GRAVITY * m_fJumpAccel * m_fJumpAccel * 0.5f;
		m_tInfo.fY -= fJumpsize;
		m_fJumpAccel += 0.08f;
		if (fJumpsize < 0)
			m_eCurState = FALL;
		if (m_tInfo.fY > fY-(m_tInfo.iCY>>1))
		{
			m_bJump = false;
			m_fJumpAccel = 0.f;
			m_eCurState = JUMP;
			m_tInfo.fY = fY - (m_tInfo.iCY>>1);
		}
	}
	else if (bLineCol)
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);

}
void CJumpingMonster::Scene_Change()
{
	if (m_eCurState != m_ePreState)
	{
		switch (m_eCurState)
		{
		case JUMP:
			m_bJump = true;
			m_fJumpPower = rand() % 5 + 6;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 233;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 500;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case FALL:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 286;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case HURT:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 74;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 600;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case THROW:
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iPosX = 2;
			m_tFrame.iPosY = 339;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			m_tFrame.dwFrameSpeed = 40;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}
}

void CJumpingMonster::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		++m_tFrame.iFrameStart;
		if (THROW == m_eCurState && 5== m_tFrame.iFrameStart)
		{
			CObj* pObj = CAbstractFactory<CSword>::Create(m_tInfo.fX, m_tInfo.fY + 10.f);
			CObjMgr::Get_Instance()->Add_Object(OBJID::MONSTER_WEAPON, pObj);
			pObj->Set_FrameKey(m_pFrameKey);
		}
		m_tFrame.dwFrameTime = GetTickCount();
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			
			m_eCurState = JUMP;
			m_tFrame.iFrameStart = 0;
			
		}
	}
}
