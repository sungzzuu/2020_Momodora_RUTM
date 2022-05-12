#include "stdafx.h"
#include "RectMgr.h"
#include "MyRect.h"
#include "ScrollMgr.h"

CRectMgr* CRectMgr::m_pInstance = nullptr;

CRectMgr::CRectMgr()
	:m_eNowChannel(STAGE::STAGE1_1), m_iChannel(1)
{
	ZeroMemory(&m_tRect, sizeof(RECT));
}


CRectMgr::~CRectMgr()
{
	Release();
}

void CRectMgr::initialize()
{
}

int CRectMgr::Update()
{
	Up_MovingTile();
	return OBJ_NOEVENT;
}

void CRectMgr::Late_Update()
{
}

void CRectMgr::Render(HDC _DC)
{
	for (auto& pRect : m_listRect[m_eNowChannel])
		pRect->Render(_DC);
}

void CRectMgr::Release()
{
	for (int i = 0; i < STAGE::END; i++)
	{
		for_each(m_listRect[i].begin(), m_listRect[i].end(), Safe_Delete<CMyRect*>);
		m_listRect[i].clear();
	}
}

void CRectMgr::Next_Channel()
{
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_2;
		m_iChannel = 2;
	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_3;
		m_iChannel = 3;
	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_4;
		m_iChannel = 4;
	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_5;
		m_iChannel = 5;
	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_6;
		m_iChannel = 6;
	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_7;
		m_iChannel = 7;
	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_8;
		m_iChannel = 8;
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_9;
		m_iChannel = 9;
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_10;
		m_iChannel = 10;
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_11;
		m_iChannel = 11;
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_12;
		m_iChannel = 12;
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_13;
		m_iChannel = 13;
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_14;
		m_iChannel = 14;
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_15;
		m_iChannel = 15;
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_16;
		m_iChannel = 16;
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_1;
		m_iChannel = 1;
	}
	Load_Rect(m_eNowChannel);
}

void CRectMgr::Previous_Channel()
{
	if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_1;
		m_iChannel = 1;
	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_2;
		m_iChannel = 2;
	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_3;
		m_iChannel = 3;
	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_4;
		m_iChannel = 4;
	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_5;
		m_iChannel = 5;
	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_6;
		m_iChannel = 6;
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_7;
		m_iChannel = 7;
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_8;
		m_iChannel = 8;
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_9;
		m_iChannel = 9;
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_10;
		m_iChannel = 10;
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_11;
		m_iChannel = 11;
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_12;
		m_iChannel = 12;
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_13;
		m_iChannel = 13;
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_14;
		m_iChannel = 14;
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		m_eNowChannel = STAGE::STAGE1_15;
		m_iChannel = 15;
	}
	Load_Rect(m_eNowChannel);
}

void CRectMgr::Emplace_Rect(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType)
{
	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int	iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());
	_pt.x -= iScrollX;
	_pt.y -= iScrollY;
	if (!m_tRect.left && !m_tRect.top)
	{
		m_tRect.left = _pt.x / TILECX * TILECX;
		m_tRect.top = _pt.y / TILECY * TILECY;
	}
	else
	{
		m_tRect.right = _pt.x / TILECX * TILECX + TILECX;
		m_tRect.bottom = _pt.y / TILECY * TILECY + TILECY;

		CMyRect* pRect = new CMyRect(m_tRect, _eType);
		m_listRect[_eChannel].emplace_back(pRect);

		m_tRect.left = 0;
		m_tRect.top = 0;
	}

}

void CRectMgr::Erase_Rect(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType)
{
	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int	iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());
	_pt.x -= iScrollX;
	_pt.y -= iScrollY;

	for (auto& iter = m_listRect[_eChannel].begin(); iter != m_listRect[_eChannel].end(); ++iter)
	{
		
		if (PtInRect(&(*iter)->Get_Rect(), _pt))
		{
			iter = m_listRect[_eChannel].erase(iter);
			return;
		}
	}
}

void CRectMgr::Delete_Rect(STAGE::CHANNEL _eChannel)
{
	for_each(m_listRect[_eChannel].begin(), m_listRect[_eChannel].end(), Safe_Delete<CMyRect*>);
	m_listRect[_eChannel].clear();
}

bool CRectMgr::Collision_Ladder(RECT _rc,float* _x, bool _bUp)
{
	for (auto& pRect : m_listRect[m_eNowChannel])
	{
		if (TILE::LADDER == pRect->Get_Type())
		{
			RECT rc = {};
			RECT playerRect = _rc;
			playerRect.bottom += 20;
			if (IntersectRect(&rc, &pRect->Get_Rect(), &playerRect))
			{
				if (rc.bottom >= playerRect.bottom || _bUp)
				{
					*_x = pRect->Get_Rect().left + ((pRect->Get_Rect().right - pRect->Get_Rect().left) >> 1);
					return true;
				}
			}
				
		}
	}
	return false;
}

void CRectMgr::Save_Rect(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;

	TCHAR* Filename = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_1.dat";

	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_2.dat";

	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_3.dat";

	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_4.dat";

	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_5.dat";

	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_6.dat";

	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_7.dat";
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_8.dat";
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_9.dat";
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_10.dat";
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_11.dat";
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_12.dat";
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_13.dat";
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_14.dat";
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_15.dat";
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_16.dat";
	}

	HANDLE hFile = CreateFile(Filename, GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	for (auto& pRect : m_listRect[m_eNowChannel])
	{
		WriteFile(hFile, &pRect->Get_Rect(), sizeof(LINEINFO), &dwByte, NULL);
		WriteFile(hFile, &pRect->Get_Type(), sizeof(TILE::TYPE), &dwByte, NULL);
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CRectMgr::Load_Rect(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;

	TCHAR* Filename = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_1.dat";

	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_2.dat";

	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_3.dat";

	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_4.dat";

	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_5.dat";

	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_6.dat";

	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_7.dat";
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_8.dat";
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_9.dat";
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_10.dat";
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_11.dat";
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_12.dat";
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_13.dat";
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_14.dat";
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_15.dat";
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		Filename = L"../Data/Rect/STAGE1_16.dat";
	}

	HANDLE hFile = CreateFile(Filename, GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	Delete_Rect(m_eNowChannel);

	DWORD dwByte = 0;

	RECT tTemp = {};
	TILE::TYPE eType = TILE::END;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(RECT), &dwByte, nullptr);
		ReadFile(hFile, &eType, sizeof(TILE::TYPE), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		m_listRect[m_eNowChannel].emplace_back(new CMyRect(tTemp, eType));
	}

	CloseHandle(hFile);
			//MessageBox(g_hWnd, L"불러오기 성공", L"성공", MB_OK);
}

void CRectMgr::Set_Line_On(bool _bLineOn)
{
	for (auto& pRect : m_listRect[m_eNowChannel])
		pRect->Set_Line_On(_bLineOn);

}

void CRectMgr::Up_MovingTile()
{
	if (m_iMovingCount > 50)
		return;
	if (!m_iMovingCount)
		return;
	if (m_dwMovingTile + 200 < GetTickCount())
	{
		for (auto& pRect :m_listRect[m_eNowChannel])
		{
			if (pRect->Get_Type() == TILE::MOVING)
				pRect->Set_Rect(4.f);
		}
		m_iMovingCount++;
		m_dwMovingTile = GetTickCount();
	}
}
