#include "stdafx.h"
#include "PlayerUI.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "Player.h"
#include "ObjMgr.h"

CPlayerUI::CPlayerUI()
	:m_bFirst(false)
{
}


CPlayerUI::~CPlayerUI()
{
}

void CPlayerUI::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/HpUI.bmp", L"HpUI");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/EmptySlot.bmp", L"EmptySlot");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/ItemSlot.bmp", L"ItemSlot");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/HpDown.bmp", L"HpDown");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Player/MoneyUI.bmp", L"MoneyUI");

	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Boss/BossHealth.bmp", L"BossHealth");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Boss/BossMinus.bmp", L"BossMinus");

}

int CPlayerUI::Update()
{
	if (!m_bFirst)
	{

		if (!lstrcmp(L"HpUI", m_pFrameKey))
		{
			m_tInfo.iCX = 400;
			m_tInfo.iCY = 36;
			m_tFrame.iSizeX = 400;
			m_tFrame.iSizeY = 36;

			//	Check_File();

		}
		else if (!lstrcmp(L"MoneyUI", m_pFrameKey))
		{
			m_tInfo.iCX = 40;
			m_tInfo.iCY = 40;
			m_tFrame.iSizeX = 32;
			m_tFrame.iSizeY = 32;
			//	Check_File();
		}
		else if(!lstrcmp(L"EmptySlot", m_pFrameKey))
		{
			m_tInfo.iCX = 100;
			m_tInfo.iCY = 100;
			m_tFrame.iSizeX = 72;
			m_tFrame.iSizeY = 72;
			//	Check_File();
		}
		else if (!lstrcmp(L"BossHealth", m_pFrameKey))
		{
			m_tInfo.iCX = 650;
			m_tInfo.iCY = 50;
			m_tFrame.iSizeX = 229;
			m_tFrame.iSizeY =15;
			//	Check_File();
		}
		m_bFirst = true;
	}
	Update_Rect();
	return OBJ_NOEVENT;
}

void CPlayerUI::Late_Update()
{
}

void CPlayerUI::Render(HDC _DC)
{
	if (!lstrcmp(L"BossHealth", m_pFrameKey) && !CObjMgr::Get_Instance()->Get_Fennel())
		return;


	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);
	if (!lstrcmp(L"EmptySlot", m_pFrameKey))
	{
		if (CObjMgr::Get_Instance()->Get_Player())
		{
			if(dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_ItemOn())
				hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"ItemSlot");
		}
	}
	
	GdiTransparentBlt(_DC
		, m_tRect.left, m_tRect.top
		, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, 0
		, m_tFrame.iSizeX, m_tFrame.iSizeY
		, RGB(240, 240, 240));

	if (!lstrcmp(L"HpUI", m_pFrameKey))
	{
		//플레이어 Hp받아서 남색바 그리자
		int _iHp = CObjMgr::Get_Instance()->Get_Player()->Get_Hp();
		int _iMaxHp = CObjMgr::Get_Instance()->Get_Player()->Get_MaxHp();
		
		HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"HpDown");
		GdiTransparentBlt(_DC
			, m_tRect.right- (_iMaxHp - _iHp) / 2, m_tRect.top
			, (_iMaxHp - _iHp) / 2, m_tInfo.iCY
			, hMemDC2
			, 400- (_iMaxHp - _iHp)/2, 0
			, (_iMaxHp - _iHp) / 2, m_tFrame.iSizeY
			, RGB(240, 240, 240));
	}
	else if (!lstrcmp(L"BossHealth", m_pFrameKey))
	{
		//Boss Hp받아와서 그리기
		if (CObjMgr::Get_Instance()->Get_Fennel())
		{
			int _iHp = CObjMgr::Get_Instance()->Get_Fennel()->Get_Hp();
			int _iMaxHp = CObjMgr::Get_Instance()->Get_Fennel()->Get_MaxHp();

			HDC hMemDC2 = CBmpMgr::Get_Instance()->Find_Bmp(L"BossMinus");

			for (int i = 0; i < (_iMaxHp - _iHp)/8; i++)
			{
				if (i > 650)
					break;
				GdiTransparentBlt(_DC
					, m_tRect.right - i, m_tRect.top
					, i, m_tInfo.iCY
					, hMemDC2
					, 0, 0
					, 1, 11
					, RGB(240, 240, 240));
			}
	
		}
		

	}
	AddFontResourceA("../Font/모리스9.ttf");
	
	SetBkMode(_DC, 1); 
	SetTextColor(_DC, RGB(255, 255, 255));
	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGUL_CHARSET, 0, 0, 0, VARIABLE_PITCH || FF_ROMAN, TEXT("모리스9"));
	HFONT oldFont = (HFONT)SelectObject(_DC, hFont);

	TCHAR	szBuff[32] = L"";
	swprintf_s(szBuff, L"%03d", CObjMgr::Get_Instance()->Get_Player()->Get_Gold());
	TextOut(_DC, 170, 75, szBuff, lstrlen(szBuff));

	if (dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_ItemOn())
	{
		swprintf_s(szBuff, L"%d", dynamic_cast<CPlayer*>(CObjMgr::Get_Instance()->Get_Player())->Get_ItemCount());
		TextOut(_DC,80, 75, szBuff, lstrlen(szBuff));
	}
	if (L"BossHealth" == m_pFrameKey)
	{
		TCHAR	szBuff[32] = L"";
		swprintf_s(szBuff, L"속죄자 페넬");
		TextOut(_DC, 70, 500, szBuff, lstrlen(szBuff));
	}

	SelectObject(_DC, oldFont);
	DeleteObject(hFont);
}

void CPlayerUI::Release()
{
}
