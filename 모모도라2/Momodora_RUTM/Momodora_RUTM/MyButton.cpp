#include "stdafx.h"
#include "MyButton.h"
#include "BmpMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"

// ���Կ� �̹� ����� ������ ������ �ٸ��� ǥ�õǵ��� �����ؾ���
CMyButton::CMyButton()
	:m_iDrawID(0), m_bFirst(false)
{
}


CMyButton::~CMyButton()
{
	Release();
}

void CMyButton::Initialize()
{

}

int CMyButton::Update()
{
	if (!m_bFirst)
	{
		if (!lstrcmp(L"����1", m_pFrameKey) || !lstrcmp(L"����2", m_pFrameKey))
		{
			m_tInfo.iCX = 700;
			m_tInfo.iCY = 56;
		}
		else if (!lstrcmp(L"����", m_pFrameKey) || !lstrcmp(L"����Ű", m_pFrameKey)
			|| !lstrcmp(L"����", m_pFrameKey))
		{
			m_tInfo.iCX = 200;
			m_tInfo.iCY = 64;
		}

		m_bFirst = true;
	}
	Update_Rect();
	return OBJ_NOEVENT;
}

void CMyButton::Late_Update()
{
	POINT pt = {};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);


	if (PtInRect(&m_tRect, pt))
	{
		if (CKeyMgr::Get_Instance()->Key_Down(VK_LBUTTON))
		{
			if (!lstrcmp(L"����1", m_pFrameKey))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE);
				return;
			}
			else if (!lstrcmp(L"����2", m_pFrameKey))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_STAGE);
				return;
			}
			else if (!lstrcmp(L"����Ű", m_pFrameKey))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_CONTROLTYPE);
				return;
			}
			else if (!lstrcmp(L"����", m_pFrameKey))
			{
				CSceneMgr::Get_Instance()->Scene_Change(CSceneMgr::SCENE_MAPTOOL);
				return;
			}
			else if (!lstrcmp(L"����", m_pFrameKey))
			{
				DestroyWindow(g_hWnd);
				return;
			}
		}
		m_iDrawID = 1;
	}
	else
		m_iDrawID = 0;
}

void CMyButton::Render(HDC _DC)
{
	HDC hMemDC = CBmpMgr::Get_Instance()->Find_Bmp(m_pFrameKey);

	BitBlt(_DC, m_tRect.left, m_tRect.top, m_tInfo.iCX, m_tInfo.iCY
		, hMemDC
		, 0, m_iDrawID * m_tInfo.iCY, SRCCOPY);
}

void CMyButton::Release()
{
}
