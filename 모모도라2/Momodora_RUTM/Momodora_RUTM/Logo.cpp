#include "stdafx.h"
#include "Logo.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"

CLogo::CLogo()
	:m_dwTime(GetTickCount())
{
}


CLogo::~CLogo()
{
	Release();
}

void CLogo::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Logo/Logo.bmp", L"Logo");
	
}

void CLogo::Update()
{
	
}

void CLogo::Late_Update()
{
	if (m_dwTime + 500 < GetTickCount())
	{
		CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_START);
		m_dwTime = GetTickCount();
		return;
	}


}

void CLogo::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Logo");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CLogo::Release()
{
}
