#include "stdafx.h"
#include "Start.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

CStart::CStart()
	:m_wParam(0)
{
}


CStart::~CStart()
{
}

void CStart::Initialize()
{
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Images/Start/Start.bmp", L"Start");
	

}

void CStart::Update()
{
}

void CStart::Late_Update()
{
	for (int i = 0; i < VK_MAX; i++)
	{
		if (CKeyMgr::Get_Instance()->Key_Down(i))
		{
			CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MENU);
			return;
		}
	}

	
}

void CStart::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(L"Start");
	BitBlt(_DC, 0, 0, WINCX, WINCY, hMemDC, 0, 0, SRCCOPY);
}

void CStart::Release()
{
}
