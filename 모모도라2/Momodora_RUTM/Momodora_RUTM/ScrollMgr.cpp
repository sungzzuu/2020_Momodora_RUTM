#include "stdafx.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "SceneMgr.h"

CScrollMgr*	CScrollMgr::m_pInstance = nullptr;
CScrollMgr::CScrollMgr()
	: m_fScrollX(0.f), m_fScrollY(0.f)
{
}


CScrollMgr::~CScrollMgr()
{
	
}
void CScrollMgr::Scroll_Lock()
{
	if (0 <= m_fScrollX)
		m_fScrollX = 0.f;
	if (0 <= m_fScrollY)
		m_fScrollY = 0.f;

	float fX = 0.f;
	float fY = 0.f;

	CTileMgr::Get_Instance()->Get_End_Tile(&fX, &fY);
	if (!CSceneMgr::Get_Instance()->Get_CurScene_MapTool())
	{
		if (WINCX <= fX)
		{
			if (fX - WINCX <= -m_fScrollX)
				m_fScrollX = WINCX - fX;
		}
		if (WINCY - fY >= m_fScrollY)
			m_fScrollY = (WINCY - fY);
	}
	
	
}
