#include "stdafx.h"
#include "Player.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "LineMgr.h"
#include "ObjMgr.h"
#include "Leaf.h"
#include "SceneMgr.h"
#include "RectMgr.h"
#include "MonkeyMonster.h"
#include "Arrow.h"
#include "SoundMgr.h"

CPlayer::CPlayer()
	:m_dwAttak(GetTickCount()), m_pLeaf(nullptr), m_bFallOn(false), m_bThinTileOn(false),
	 m_bAttack(false), m_bThinCheck(false), m_bItemOn(false),m_iItemCount(0), m_bGodContinue(false)
{
}

CPlayer::~CPlayer()

{
	Release();
}

void CPlayer::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/Player240.bmp", L"Player_RIGHT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/Stretch.bmp", L"StretchPlayer");
	m_eCurState = IDLE;
	m_ePreState = IDLE;

	m_tFrame.bEnd = true;
	m_tFrame.iFrameStart = 0;
	m_tFrame.iFrameEnd = 5;
	m_tFrame.iPosX = 1;
	m_tFrame.iPosY = 2150;
	m_tFrame.iSizeX = 48;
	m_tFrame.iSizeY = 48;
	m_tFrame.dwFrameSpeed = 200;
	m_tFrame.dwFrameTime = GetTickCount();
	m_pFrameKey = L"Player_RIGHT";

	m_tInfo.fX = 100.f;
	m_tInfo.fY = 300.f;
	m_tInfo.iCX = 120;
	m_tInfo.iCY = 120;
	m_tInfo.iCollisionCX = 50;
	m_tInfo.iCollisionCY = 70;
	m_fSpeed = 4.f;

	m_bJump = false;
	m_fJumpPower = 9.f;
	m_fJumpAccel = 0.f;
	m_fSecondJump = 0.7f;
	m_bJumpMax = false;
	m_bGod = false;

	float fY = 0.f;
	if (CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY + (m_tInfo.iCY >> 1), &fY, &m_bThinCheck))
		m_tInfo.fY = fY - (m_tInfo.iCY >> 1);
	//게이지
	m_tGauge.iHp = 800;
	m_tGauge.iGold = 0;
	m_tGauge.iMaxHp = 800;
	m_tGauge.iAtt = 100;

	m_eGroup = GROUPID::PLAYER;
	m_dwArrowCol = GetTickCount();

	m_iAlpha = 150;
}
	
int CPlayer::Update()
{
	
	if (FROMPRAYING == m_eCurState)
	{
		//저장되었습니다 출력!!
		// 선택한 슬롯에 플레이어 저장
		CObjMgr::Get_Instance()->Player_Save();
	}
	if (ATTACK1 != m_eCurState && ATTACK2 != m_eCurState && ATTACK3 != m_eCurState)
	{
		if(m_pLeaf)
			CObjMgr::Get_Instance()->Delete_Leaf();
	}
	if (m_eCurState != HURT && m_eCurState != DEATH)
	{
		Key_Check();
		Jumping();
	}

	OffSet();
	Animation_Flow();
	Frame_Move();
	Update_Leaf();
	Scene_Change();
	Check_Player_Pos();
	if (m_dwGod + 3000 < GetTickCount())
	{
		if(!m_bGodContinue)
			m_bGod = false;

	}

	if (DEATH == m_eCurState && m_tFrame.iFrameStart == m_tFrame.iFrameEnd)
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_DEATHMENU);
		return OBJ_NOEVENT;
	}

	if (m_tGauge.iHp <= 0 && m_eCurState != DEATH)
	{
		m_eCurState = DEATH;
		CSoundMgr::Get_Instance()->StopAll();
		CSoundMgr::Get_Instance()->PlaySound(L"kahoHurt.wav", CSoundMgr::PLAYER);
	}

	return OBJ_NOEVENT;
}

void CPlayer::Render(HDC _DC)
{
	Update_Rect();
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	if (m_bLineOn)
	{
		HPEN hpen = CreatePen(PS_SOLID, 2, BLACK);
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
	if (!lstrcmp(L"Player_LEFT", m_pFrameKey))
	{
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"Player_RIGHT");
		HDC hStretch = CBmpMgr::Get_Instance()->Find_Bmp(L"StretchPlayer");
		HDC hAlphaBlend = CBmpMgr::Get_Instance()->Find_Bmp(L"StretchPlayer");
	

		StretchBlt(hStretch
			, 0, 0
			, m_tInfo.iCX, m_tInfo.iCY
			, hMemDC2
			, m_tFrame.iPosX + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX) + m_tFrame.iSizeX - 1, m_tFrame.iPosY + m_tFrame.iSizeY
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
		HDC hAlphaBlend = CBmpMgr::Get_Instance()->Find_Bmp(L"StretchPlayer");
		if (m_bGod)
		{
			m_BlendFunction.BlendOp = AC_SRC_OVER;
			m_BlendFunction.BlendFlags = 0;
			m_BlendFunction.AlphaFormat = 0;


			m_BlendFunction.SourceConstantAlpha = m_iAlpha;

			AlphaBlend(hAlphaBlend, 0, 0
				, m_tInfo.iCX, m_tInfo.iCY,
				hMemDC
				, m_tFrame.iPosX + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX), m_tFrame.iPosY + m_tFrame.iSizeY
				, m_tFrame.iSizeX, m_tFrame.iSizeY,
				m_BlendFunction);

			GdiTransparentBlt(_DC
				, m_tRect.left + iScrollX, m_tRect.top + iScrollY
				, m_tInfo.iCX, m_tInfo.iCY
				, hAlphaBlend
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
				, m_tFrame.iPosX + m_tFrame.iFrameStart * (m_tFrame.iSizeX + m_tFrame.iPosX), m_tFrame.iPosY + m_tFrame.iSizeY
				, m_tFrame.iSizeX, m_tFrame.iSizeY
				, RGB(240, 240, 240));
		}
		
	}
	

}

void CPlayer::Release()
{
	
}

void CPlayer::Check_Player_Pos()
{
	int iScrollX = (float)(CScrollMgr::Get_Instance()->Get_ScrollX());
	float fX = 0.f;
	float fY = 0.f;
	STAGE::CHANNEL _eChannel = STAGE::END;
	CTileMgr::Get_Instance()->Get_End_Tile(&fX, &fY);
	_eChannel = CTileMgr::Get_Instance()->Get_Channel();
	if (STAGE::STAGE1_10 == _eChannel)
	{
		if (m_tInfo.fY >= fY)
		{

			CTileMgr::Get_Instance()->Next_Channel();
			CLineMgr::Get_Instance()->Next_Channel();
			CRectMgr::Get_Instance()->Next_Channel();
			CObjMgr::Get_Instance()->Next_Channel();
			m_tInfo.fY = 0.f;
			m_eCurState = IDLE;
			m_bChannelChange = true;
		}
	}
	else if (STAGE::STAGE1_11 == _eChannel)
	{
		if (m_tInfo.fX < 0)
		{

			CTileMgr::Get_Instance()->Next_Channel();
			CLineMgr::Get_Instance()->Next_Channel();
			CRectMgr::Get_Instance()->Next_Channel();
			CObjMgr::Get_Instance()->Next_Channel();
			m_tInfo.fX = 800.f;;
			m_tInfo.fY = 300.f;
			m_eCurState = IDLE;
			m_bChannelChange = true;
		}
	}
	else if (STAGE::STAGE1_13 == CTileMgr::Get_Instance()->Get_Channel())
	{
		if (m_tInfo.fX >= fX)
		{
			
			CTileMgr::Get_Instance()->Next_Channel();
			CLineMgr::Get_Instance()->Next_Channel();
			CRectMgr::Get_Instance()->Next_Channel();
			CObjMgr::Get_Instance()->Next_Channel();
			m_tInfo.fX = m_tInfo.iCX >> 1;
			
		
			m_eCurState = IDLE;
			m_bChannelChange = true;
		}
	}
	else if (STAGE::STAGE1_15 == CTileMgr::Get_Instance()->Get_Channel())
	{
		if (m_tInfo.fY >= fY)
		{

			CTileMgr::Get_Instance()->Next_Channel();
			CLineMgr::Get_Instance()->Next_Channel();
			CRectMgr::Get_Instance()->Next_Channel();
			CObjMgr::Get_Instance()->Next_Channel();
			//보스 몬스터 생성
			m_tInfo.fY = 0.f;
			m_eCurState = IDLE;
			m_bChannelChange = true;
		}
	}
	else if (m_tInfo.fX <= 0.f && STAGE::STAGE1_16 != CTileMgr::Get_Instance()->Get_Channel())
	{
		if (STAGE::STAGE1_1 != CTileMgr::Get_Instance()->Get_Channel())
		{

			CTileMgr::Get_Instance()->Previous_Channel();
			CLineMgr::Get_Instance()->Previous_Channel();
			CRectMgr::Get_Instance()->Previous_Channel();
			CTileMgr::Get_Instance()->Get_End_Tile(&fX, &fY);
			CObjMgr::Get_Instance()->Previous_Channel();

			m_tInfo.fX = fX - (m_tInfo.iCollisionCX >> 1);
			m_eCurState = IDLE;
			m_bChannelChange = true;

		}
		else
			m_tInfo.fX = 0.f;
	}
	else 
	{
		if (m_tInfo.fX >= fX)
		{

			CTileMgr::Get_Instance()->Next_Channel();
			CLineMgr::Get_Instance()->Next_Channel();
			CRectMgr::Get_Instance()->Next_Channel();
			CObjMgr::Get_Instance()->Next_Channel();
			m_tInfo.fX = m_tInfo.iCX >> 1;
			m_eCurState = IDLE;
			m_bChannelChange = true;
		}
	}
	
	
}

void CPlayer::Key_Check()
{
	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();

	if (CKeyMgr::Get_Instance()->Key_Down(VK_F2))
	{
		m_bGod = true;
		m_bGodContinue = true;
	}
	if (CKeyMgr::Get_Instance()->Key_Down('D'))
	{

		if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
			m_eCurState = CROUCHBOW;
		else if (m_eCurState != CROUCHBOW)
			m_eCurState = BOW;
		

	}
	else if (CKeyMgr::Get_Instance()->Key_Down('W'))
	{
		if (m_iItemCount)
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"Healing.wav", CSoundMgr::PLAYER);
			--m_iItemCount;
			m_tGauge.iHp += 100;
			m_eCurState = ITEM;

		}
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('S'))
	{
		if (BOW == m_eCurState || CROUCHBOW == m_eCurState)
			return;
		if (m_eCurState == ATTACK1 && m_bAttack == false)
			m_bAttack = true;
		else if (m_eCurState != ATTACK2 && m_eCurState != ATTACK3)
		{
			m_eCurState = ATTACK1;
			CObjMgr::Get_Instance()->Delete_Leaf();
			Add_Leaf();
			dynamic_cast<CLeaf*>(m_pLeaf)->Set_State(1);
			m_pLeaf->Set_FrameKey(m_pFrameKey);
			m_bAttack = false;
		}
		else
		{
			m_bAttack = true;
		}
	}
	else if (CKeyMgr::Get_Instance()->Key_Down('Q'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"sndRoll.wav", CSoundMgr::PLAYER);
		m_eCurState = ROLL;
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		float fX = 0.f;

		if (CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect, &fX, 0))
		{
			m_tInfo.fX = fX;
			m_eCurState = LADDERDOWN;

		}
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_DOWN))
	{
		float fX = 0.f;
		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"sndJump.wav", CSoundMgr::PLAYER);
			m_bThinTileOn = true;

		}
		else if (CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect,&fX,0))
		{
			if (LADDERUP == m_eCurState)
				m_eCurState = LADDERDOWN;
			else if (LADDERDOWN == m_eCurState)
			{
				m_eCurState = LADDERDOWN;
				if (m_eCurState != LANDSOFT)
				{
					m_tInfo.fY += 5.f;
					m_tInfo.fX = fX;
				}
			}
			else
				m_eCurState = LADDERDOWN;
			
		}
		else
		{
			if(CROUCHBOW != m_eCurState && BOW != m_eCurState)
				m_eCurState = CROUCH;

		}
	}
	else if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_UP))
	{
		float fX = 0.f;
		if (CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect, &fX, 1))
		{
			float fY = 0.f;
			CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tInfo.fY, &fY, &m_bThinCheck);
			if (!m_bThinCheck)
			{
				if (LADDERDOWN == m_eCurState)
					m_eCurState = LADDERUP;
				else if (LADDERUP == m_eCurState)
				{
					m_eCurState = LADDERUP;
					m_tInfo.fY -= 3.f;
					m_tInfo.fX = fX;
				}
				else
					m_eCurState = LADDERUP;
			}
			else
			{
				m_tInfo.fY = fY -(m_tInfo.iCY>>1);
				if(m_eCurState == LADDERUP)
					m_eCurState = LANDSOFT;

			}
			
		}
	}
	
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LEFT) && (m_eCurState == RUN || m_eCurState == IDLE || m_eCurState == JUMP || m_eCurState == FALL || m_eCurState == LANDSOFT || m_eCurState == BRAKE))
	{

		m_tInfo.fX -= m_fSpeed;
		if ((float)m_tRect.left <= fScrollX - (WINCX >> 1) + 65.f)
			m_tInfo.fX += m_fSpeed;

		m_pFrameKey = L"Player_LEFT";
		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"sndJump.wav", CSoundMgr::PLAYER);
			Check_Second_Jump();
			if (!m_bJumpMax)
			{
				m_eCurState = JUMP;
				m_bJump = true;
				Jumping();
			}
		}
		else if (m_eCurState == JUMP)
		{

		}
		else if (m_eCurState == FALL)
		{
			
		}
		else
			m_eCurState = RUN;

	}
	else if (CKeyMgr::Get_Instance()->Key_Up(VK_LEFT) && m_ePreState == RUN)
		m_eCurState = BRAKE;
	else if (CKeyMgr::Get_Instance()->Key_Up(VK_DOWN))
	{
		float fX = 0.f;
		if (CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect,&fX, 0))
		{
			

		}
		else if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
		{
			m_eCurState = ATTACK1;
		}
	
		else if (ATTACK1 != m_eCurState && ATTACK1 != m_eCurState && ATTACK1 != m_eCurState)
			m_eCurState = RISE;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RIGHT) && (m_eCurState == RUN || m_eCurState == IDLE || m_eCurState == JUMP || m_eCurState == FALL || m_eCurState == LANDSOFT || m_eCurState == BRAKE))
	{
		
		m_tInfo.fX += m_fSpeed;
		m_pFrameKey = L"Player_RIGHT";
		if (CKeyMgr::Get_Instance()->Key_Down('A'))
		{
			CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
			CSoundMgr::Get_Instance()->PlaySound(L"sndJump.wav", CSoundMgr::PLAYER);
			Check_Second_Jump();
			if (!m_bJumpMax)
			{
				m_eCurState = JUMP;
				m_bJump = true;
				Jumping();
			}
			
		}
		else if (m_eCurState == JUMP)
		{

		}
		else if (m_eCurState == FALL)
		{

		}
		else
			m_eCurState = RUN;
	}
	else if (CKeyMgr::Get_Instance()->Key_Up(VK_RIGHT) && m_ePreState == RUN)
		m_eCurState = BRAKE;
	else if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"sndJump.wav", CSoundMgr::PLAYER);
		Check_Second_Jump();
		if (!m_bJumpMax)
		{
			m_eCurState = JUMP;
			m_bJump = true;
			Jumping();
		}
	}
	else
	{
		if (m_tFrame.bEnd)
			m_eCurState = IDLE;
	}
	Update_Rect();
}

void CPlayer::Jumping()
{
	float fY = 0.f;
	float fX = 0.f;
	bool bLineCol = CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX, m_tCollisionRect.bottom, &fY, &m_bThinCheck);
	if (m_bJump)
	{
		if (LADDERUP == m_eCurState)
			return;
		float fJumpSize = m_fJumpPower * m_fJumpAccel - GRAVITY * m_fJumpAccel * m_fJumpAccel * 0.5f;
		m_tInfo.fY -= fJumpSize;
		if (m_eCurState == JUMP && m_tFrame.iFrameStart < 2)
			m_fJumpAccel += 0.2f;
		else
			m_fJumpAccel += 0.05f;
		//fY가 0이면 라인 없어서 위로 ㅈㄴ뜸,,
		if (m_tInfo.fY + (m_tInfo.iCY>>1) > fY && m_eCurState != JUMP && fY != 0.f)
		{
			m_bJump = false;
			m_fJumpAccel = 0.f;
			m_tInfo.fY = fY - (m_tInfo.iCY >> 1);
			m_bJumpMax = false;
			m_eCurState = LANDSOFT;
		}

	}
	else if (bLineCol)
	{
		float fX = 0.f;
		if (m_eCurState == LADDERDOWN && CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect,&fX, 0))
		{

		}
		else if (m_eCurState == LADDERUP && CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect, &fX, 1))
		{

		}
		else if (m_bThinTileOn && m_bThinCheck)
		{
			m_bFallOn = true;
			m_eCurState = FALL;
			m_tInfo.fY += 8.f;
			m_bThinTileOn = false;
		}
		else
		{

			if (m_tInfo.fY < (fY - (m_tInfo.iCY >> 1)))
			{
				float fX = 0.f;
				if (!CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect, &fX, 0))
				{
					m_bFallOn = true;
					m_eCurState = FALL;
					m_tInfo.fY += 8.f;
				}
			
			}
			if (m_tInfo.fY >= (fY - (m_tInfo.iCY >> 1)))
			{
				m_tInfo.fY = (fY - (m_tInfo.iCY >> 1));
				m_bFallOn = false;
			}
			m_bThinTileOn = false;
		}
	
	}
	Update_Rect();
	bLineCol = CLineMgr::Get_Instance()->Collision_Ceiling(m_tInfo.fX, m_tCollisionRect.top, &fY);
	if (bLineCol)
	{
		m_bJump = false;
		m_eCurState = FALL;
		m_tInfo.fY = fY + (m_tInfo.iCollisionCY >> 1);

	}
}

void CPlayer::OffSet()
{
	int iOffsetX = WINCX >> 1;
	int iOffsetY = WINCY >> 1;

	float fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	if (iOffsetX > m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + fScrollX));
	if (iOffsetX < m_tInfo.fX + fScrollX)
		CScrollMgr::Get_Instance()->Set_ScrollX(iOffsetX - (m_tInfo.fX + fScrollX));

	float fY = 0.f;
	
	 //타일이 위로 올라오면 이게 생김
	if (iOffsetY > m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffsetY - (m_tInfo.fY + fScrollY));
	if (iOffsetY < m_tInfo.fY + fScrollY)
		CScrollMgr::Get_Instance()->Set_ScrollY(iOffsetY - (m_tInfo.fY + fScrollY));
}

void CPlayer::Check_Second_Jump()
{
	if (m_bJump && !m_bJumpMax)
	{
		m_bJumpMax = true;
		m_fJumpAccel = m_fSecondJump;
		if (m_eCurState == JUMP)
			m_tFrame.iFrameStart = 0;
		else
			m_eCurState = JUMP;
	}
}



void CPlayer::Scene_Change()
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case IDLE:
			m_tFrame.bEnd = true;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 2150;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK1:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 75-48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK2:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 138 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ATTACK3:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 201 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case BOW:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 650 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case BRAKE:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 6;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 906 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case TURN:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 969 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CROUCH:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1032 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case CROUCHBOW:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 776 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case RISE:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 1;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1095 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case RUN:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 1;
			m_tFrame.iFrameEnd = 9;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 843 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 80;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case JUMP:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1694 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 70;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case FALL:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1757 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case LANDSOFT:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 3158 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ROLL:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 7;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 3536 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case LADDERUP:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 2800 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 200;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case LADDERDOWN:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 2;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 2861 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case HURT:
			m_tFrame.bEnd = true;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 0;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 2135 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 300;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case DEATH:
			m_tFrame.bEnd = true;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 19;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1158 - 44;
			m_tFrame.iSizeX = 49;
			m_tFrame.iSizeY = 44;
			m_tFrame.dwFrameSpeed = 120;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case PRAYING:
			m_tFrame.bEnd = true;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 3;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1883 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case TOPRAYING:
			m_tFrame.bEnd = true;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 5;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1820 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case FROMPRAYING:
			m_tFrame.bEnd = true;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 8;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 1946 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 100;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		case ITEM:
			m_tFrame.bEnd = false;
			m_tFrame.iFrameStart = 0;
			m_tFrame.iFrameEnd = 10;
			m_tFrame.iPosX = 1;
			m_tFrame.iPosY = 2548 - 48;
			m_tFrame.iSizeX = 48;
			m_tFrame.iSizeY = 48;
			m_tFrame.dwFrameSpeed = 50;
			m_tFrame.dwFrameTime = GetTickCount();
			break;
		}

		m_ePreState = m_eCurState;
	}


}

void CPlayer::Frame_Move()
{
	if (m_tFrame.dwFrameTime + m_tFrame.dwFrameSpeed < GetTickCount())
	{
		
		++m_tFrame.iFrameStart;
		m_tFrame.dwFrameTime = GetTickCount();
		if (3 >= m_tFrame.iFrameStart && (CROUCHBOW == m_eCurState || BOW == m_eCurState) && m_dwArrowCol + 500 < GetTickCount())
		{
			m_dwArrowCol = GetTickCount();
			Add_Arrow();
		}
		if (m_tFrame.iFrameStart == 3 && m_eCurState == FALL)
			m_tFrame.dwFrameSpeed = 50;
		if (m_tFrame.iFrameStart > m_tFrame.iFrameEnd)
		{
			if (DEATH == m_eCurState)
			{
				//스테이지 종료
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_DEATHMENU);
				return;
			}
			else if (HURT == m_eCurState)
			{
				m_eCurState = FALL;
				
			}
			else if (TOPRAYING == m_eCurState)
			{
				m_eCurState = PRAYING;
			}
			else if (PRAYING == m_eCurState)
			{
				m_eCurState = FROMPRAYING;
			}
			else if (FROMPRAYING == m_eCurState)
			{
				m_tFrame.bEnd = true;
				m_eCurState = IDLE;
			}
			else if (LADDERDOWN == m_eCurState)
			{
				//아직도 사다리면 그대로
				m_tFrame.iFrameStart = 0;
			}
			else if (LADDERUP == m_eCurState)
			{
				m_tFrame.iFrameStart = 0;

			}
			else if (CROUCHBOW == m_eCurState)
			{
				m_tFrame.iFrameStart = 0;
				m_eCurState = CROUCH;
				m_tFrame.bEnd = true;

			}
			else if (CROUCH == m_eCurState)
			{
				m_tFrame.iFrameStart = 2;

			}
			else if (m_eCurState == RUN)
				m_tFrame.iFrameStart = 2;
			else if (m_eCurState == JUMP)
			{
				m_eCurState = FALL;
			}
			else if (m_eCurState == FALL)
			{
				if (m_bJump)
					--m_tFrame.iFrameStart;
				else
					m_eCurState = LANDSOFT;
			}
			else if (m_eCurState == ATTACK1)
			{
				if (m_bAttack)
				{
					CObjMgr::Get_Instance()->Delete_Leaf();
					Add_Leaf();
					dynamic_cast<CLeaf*>(m_pLeaf)->Set_State(2);
					m_pLeaf->Set_FrameKey(m_pFrameKey);
					m_bAttack = false;
					m_eCurState = ATTACK2;
				}
				else
				{
					CObjMgr::Get_Instance()->Delete_Leaf();
					m_eCurState = IDLE;
					m_tFrame.iFrameStart = 0;
					m_tFrame.bEnd = true;
					m_bAttack = false;

				}
				
			}
			else if (m_eCurState == ATTACK2)
			{
				if (m_bAttack)
				{
					CObjMgr::Get_Instance()->Delete_Leaf();
					Add_Leaf();
					dynamic_cast<CLeaf*>(m_pLeaf)->Set_State(3);
					m_pLeaf->Set_FrameKey(m_pFrameKey);
					m_bAttack = false;
					m_eCurState = ATTACK3;
				}
				else
				{
					CObjMgr::Get_Instance()->Delete_Leaf();

					m_eCurState = IDLE;
					m_tFrame.iFrameStart = 0;
					m_tFrame.bEnd = true;
					m_bAttack = false;

				}
			}
			else if (m_eCurState == ATTACK3)
			{
				CObjMgr::Get_Instance()->Delete_Leaf();
				m_bAttack = false;
				m_eCurState = IDLE;
				m_tFrame.iFrameStart = 0;
				m_tFrame.bEnd = true;
			}
			else
			{
				m_eCurState = IDLE;
				m_tFrame.iFrameStart = 0;
				m_tFrame.bEnd = true;
				m_bAttack = false;

			}

		}
	}
}

void CPlayer::Animation_Flow()
{
	float fY = 0.f;
	bool bThinCheck = false;
	float fX = 0.f;
	//사다리
	if (CRectMgr::Get_Instance()->Collision_Ladder(m_tCollisionRect, &fX, 1))
		return;
	// 떨어지는 중인데 바닥에 착지하면
	if (m_eCurState == FALL && CLineMgr::Get_Instance()->Collision_Surface(m_tInfo.fX,m_tCollisionRect.bottom, &fY, &bThinCheck))
	{
		if (fY <= m_tRect.bottom)
		{
			m_eCurState = LANDSOFT;
			m_fJumpAccel = 0.f;
			m_bJump = false;
		}
		
	}// 구르기는 방향에따라 -,+다르다
	if (m_eCurState == ROLL)
	{
		if (m_pFrameKey == L"Player_LEFT")
		{
			if (m_tFrame.iFrameStart < 7 && m_tFrame.iFrameStart > 0)
				m_tInfo.fX -= m_fSpeed + 2.f;
			else if(0 == m_tFrame.iFrameStart)
				m_tInfo.fX -= m_fSpeed;

		}
		else if (m_pFrameKey == L"Player_RIGHT")
		{
			if (m_tFrame.iFrameStart < 7 && m_tFrame.iFrameStart > 0)
				m_tInfo.fX += m_fSpeed + 2.f;
			else if (0 == m_tFrame.iFrameStart)
				m_tInfo.fX += m_fSpeed;

		}

	}

}

void CPlayer::Add_Leaf()
{

	CObj* pObj = CAbstractFactory<CLeaf>::Create(m_tInfo.fX, m_tInfo.fY, m_pFrameKey);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAER_WEAPON, pObj);
	//dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_Leaf(pObj);
	m_pLeaf = pObj;
	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
	CSoundMgr::Get_Instance()->PlaySound(L"sndLeaf.wav", CSoundMgr::PLAYER);

}

void CPlayer::Add_Arrow()
{
	float fY = 0.f;
	if (BOW == m_eCurState)
		fY = m_tInfo.fY;
	else
		fY = m_tInfo.fY + 20.f;
	CObj* pObj = CAbstractFactory<CArrow>::Create(m_tInfo.fX+5.f, fY, m_pFrameKey);
	CObjMgr::Get_Instance()->Add_Object(OBJID::PLAER_WEAPON, pObj);
	pObj->Update();
	//dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Set_Leaf(pObj);

	CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
	CSoundMgr::Get_Instance()->PlaySound(L"화살쏘기.wav", CSoundMgr::PLAYER);
}

void CPlayer::Update_Leaf()
{
	if(!m_pLeaf)
		return;
	if(m_pFrameKey == L"Player_LEFT")
		m_pLeaf->Set_Pos(m_tInfo.fX- (m_pLeaf->Get_Info().iCX>>1), m_tInfo.fY);
	else  if(m_pFrameKey == L"Player_RIGHT")
		m_pLeaf->Set_Pos(m_tInfo.fX + (m_pLeaf->Get_Info().iCX >> 1), m_tInfo.fY);

}

void CPlayer::Late_Update()
{
	if (CObjMgr::Get_Instance()->Collision_Player_Gold())
	{
		++m_tGauge.iGold;
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"sndCharge4.wav", CSoundMgr::PLAYER);
	}
	CObjMgr::Get_Instance()->Collision_Flower_Player();

	if (CTileMgr::Get_Instance()->Collision_BounceBall_Player())
	{
		m_tInfo.fY -= 100.f;
	}
	//플레이어와 몬스터->플레이어의 상태만 변환
	if (ROLL == m_eCurState)
		return;
	if (m_bGod)
		return;
	if (CObjMgr::Get_Instance()->Collision_Player_Monster() || CObjMgr::Get_Instance()->Collision_Player_Weapon())
	{
		CSoundMgr::Get_Instance()->StopSound(CSoundMgr::PLAYER);
		CSoundMgr::Get_Instance()->PlaySound(L"kahoHurt.wav", CSoundMgr::PLAYER);
		m_eCurState = HURT;
		m_bGod = true;
		m_dwGod = GetTickCount();
		// 어느 방향인지 받아오기
	}
	
}
