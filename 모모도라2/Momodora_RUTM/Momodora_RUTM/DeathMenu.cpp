#include "stdafx.h"
#include "DeathMenu.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "ObjMgr.h"
#include "Player.h"

CDeathMenu::CDeathMenu()
{
}


CDeathMenu::~CDeathMenu()
{
}

void CDeathMenu::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/DeathMenu/Destroy.bmp", L"Destroy");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/DeathMenu/GoBackStart.bmp", L"GoBackStart");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/DeathMenu/Retry.bmp", L"Retry");
	m_pFrameKey = L"Retry";
}

void CDeathMenu::Update()
{
	if (CKeyMgr::Get_Instance()->Key_Down(VK_DOWN))
	{
		if (L"Retry" == m_pFrameKey)
			m_pFrameKey = L"GoBackStart";
		else if (L"GoBackStart" == m_pFrameKey)
			m_pFrameKey = L"Destroy";
		else
			m_pFrameKey = L"Retry";
	}
	if (CKeyMgr::Get_Instance()->Key_Down(VK_UP))
	{
		if (L"Retry" == m_pFrameKey)
			m_pFrameKey = L"Destroy";
		else if (L"GoBackStart" == m_pFrameKey)
			m_pFrameKey = L"Retry";
		else
			m_pFrameKey = L"GoBackStart";
	}
	if (CKeyMgr::Get_Instance()->Key_Down('A'))
	{
		if (L"Retry" == m_pFrameKey)
		{
			CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE);
			CObjMgr::Get_Instance()->Get_Player()->Initialize();
		}
		else if (L"GoBackStart" == m_pFrameKey)
		{
			CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_LOGO);
			CObjMgr::Get_Instance()->Get_Player()->Initialize();

		}
		else
			DestroyWindow(g_hWnd);
	}
}

void CDeathMenu::Late_Update()
{
}

void CDeathMenu::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CDeathMenu::Release()
{
}
