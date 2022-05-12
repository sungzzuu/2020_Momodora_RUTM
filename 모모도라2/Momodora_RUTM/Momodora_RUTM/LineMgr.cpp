#include "stdafx.h"
#include "LineMgr.h"
#include "Line.h"
#include "TileMgr.h"
#include "ScrollMgr.h"
CLineMgr* CLineMgr::m_pInstance = nullptr;

CLineMgr::CLineMgr()
	:m_iChannel(1), m_eNowChannel(STAGE::STAGE1_1)
{
}


CLineMgr::~CLineMgr()
{
	Release();
}

void CLineMgr::Initialize()
{
	m_iMovingCount = 0;

}

void CLineMgr::Update()
{
	Up_MovingTile();
}

void CLineMgr::Late_Update()
{
}

void CLineMgr::Render(HDC _DC)
{
	for (auto& pLine : m_listLine[m_eNowChannel])
		pLine->Render(_DC);
}

void CLineMgr::Release()
{
	for (int i = 0; i < STAGE::END; i++)
	{
		for_each(m_listLine[i].begin(), m_listLine[i].end(), Safe_Delete<CLine*>);
		m_listLine[i].clear();
	}
	
}

void CLineMgr::Emplace_Line(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType)
{
	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int	iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());
	_pt.x -= iScrollX;
	_pt.y -= iScrollY;
	if (!m_tLinePos[0].fX && !m_tLinePos[0].fY)
	{
		m_tLinePos[0].fX = _pt.x / TILECX * TILECX;
		m_tLinePos[0].fY = _pt.y / TILECY * TILECY;
	}
	else
	{
		m_tLinePos[1].fX = _pt.x / TILECX * TILECX + TILECX;
		m_tLinePos[1].fY = _pt.y / TILECY * TILECY;

		CLine* pLine = new CLine(m_tLinePos[0], m_tLinePos[1]);
		pLine->Set_Type(_eType);
		m_listLine[_eChannel].emplace_back(pLine);

		m_tLinePos[0].fX = 0.f;
		m_tLinePos[0].fY = 0.f;
	}
	
}

void CLineMgr::Erase_Line(POINT _pt, STAGE::CHANNEL _eChannel, TILE::TYPE _eType)
{
	int iScrollX = (int)(CScrollMgr::Get_Instance()->Get_ScrollX());
	int	iScrollY = (int)(CScrollMgr::Get_Instance()->Get_ScrollY());
	_pt.x -= iScrollX;
	_pt.y -= iScrollY;

	for (auto& iter = m_listLine[_eChannel].begin();  iter != m_listLine[_eChannel].end();++iter)
	{
		RECT rc = {
			(*iter)->Get_Info().tLeftPos.fX,
			(*iter)->Get_Info().tLeftPos.fY,
			(*iter)->Get_Info().tRightPos.fX,
			(*iter)->Get_Info().tLeftPos.fY + TILECY };
		if (PtInRect(&rc, _pt))
		{
			iter = m_listLine[_eChannel].erase(iter);
			return;
		}
	}
}

void CLineMgr::Delete_Line(STAGE::CHANNEL _eChannel)
{
	for_each(m_listLine[_eChannel].begin(), m_listLine[_eChannel].end(), Safe_Delete<CLine*>);
	m_listLine[_eChannel].clear();

}

void CLineMgr::Next_Channel()
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
	Load_Line(m_eNowChannel);
}

void CLineMgr::Previous_Channel()
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
	Load_Line(m_eNowChannel);
}

bool CLineMgr::Collision_Surface(float _x, float _y, float* _pY, bool* _bThinTileOn)
{
	if (m_listLine[m_eNowChannel].empty())
		return false;
	CLine* pTargetLine = nullptr;
	for (auto& pLine : m_listLine[m_eNowChannel])
	{	
		if (TILE::CEILING == pLine->Get_Type())
			continue;
		//라인의 범위에 들었을때
		if (pLine->Get_Info().tLeftPos.fX <= _x
			&& pLine->Get_Info().tRightPos.fX >= _x)
		{
			if (pLine->Get_Info().tLeftPos.fY >= _y)
			{
				if(!pTargetLine)
					pTargetLine = pLine;
			
			
				if(pTargetLine->Get_Info().tLeftPos.fY > pLine->Get_Info().tLeftPos.fY)
					pTargetLine = pLine;

			}
		}
	}
	if (!pTargetLine)
		return false;
	if (pTargetLine->Get_Info().tLeftPos.fY < _y)
		return false;
	

	// 직선의 방정식에서 기울기가 0이면 그 선의 y좌표와 
	// 플레이어의 y좌표는 같아진다 ->라인태우기
	float x1 = pTargetLine->Get_Info().tLeftPos.fX;
	float y1 = pTargetLine->Get_Info().tLeftPos.fY;
	float x2 = pTargetLine->Get_Info().tRightPos.fX;
	float y2 = pTargetLine->Get_Info().tRightPos.fY;

	if (TILE::THIN == pTargetLine->Get_Type())
		*_bThinTileOn = true;
	else
		*_bThinTileOn = false;
	*_pY = ((y2 - y1) / (x2 - x1))*(_x - x1) + y1;
	// 충돌시에 라인과 맞게 플레이어가 움지여야함!!
	return true;
}
bool CLineMgr::Collision_Ceiling(float _x, float _y, float * _pY)
{
	if (m_listLine[m_eNowChannel].empty())
		return false;

	CLine* pTargetLine = nullptr;

	for (auto& pLine : m_listLine[m_eNowChannel])
	{
		if (TILE::CEILING == pLine->Get_Type())
		{
			if (pLine->Get_Info().tLeftPos.fY > _y+TILECY)
				continue;
			if (pLine->Get_Info().tLeftPos.fX <= _x
				&& pLine->Get_Info().tRightPos.fX >= _x)
			{
				if (pLine->Get_Info().tLeftPos.fY >= _y)
					pTargetLine = pLine;
			}
		}
		
	}
	if (!pTargetLine)
		return false;
	*_pY = pTargetLine->Get_Info().tLeftPos.fY;
	return true;
}
bool CLineMgr::Collision_Line(float _x, float * _y)
{
	if (m_listLine[m_eNowChannel].empty())
		return false;

	CLine* pTargetLine = nullptr;

	for (auto& pLine : m_listLine[m_eNowChannel])
	{
		if (pLine->Get_Info().tLeftPos.fX <= _x
			&& pLine->Get_Info().tRightPos.fX >= _x)
			pTargetLine = pLine;
	}

	if (!pTargetLine)
		return false;

	float x1 = pTargetLine->Get_Info().tLeftPos.fX;
	float y1 = pTargetLine->Get_Info().tLeftPos.fY;
	float x2 = pTargetLine->Get_Info().tRightPos.fX;
	float y2 = pTargetLine->Get_Info().tRightPos.fY;

	*_y = ((y2 - y1) / (x2 - x1)) * (_x - x1) + y1;

	return true;
}
void CLineMgr::Set_Line(bool _bLineOn)
{
	for (auto& pLine : m_listLine[m_eNowChannel])
	{
		pLine->Set_LineOn(_bLineOn);
	}
}

void CLineMgr::Load_Line(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;

	TCHAR* Filename = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_1.dat";

	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_2.dat";

	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_3.dat";

	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_4.dat";

	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_5.dat";

	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_6.dat";

	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_7.dat";
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_8.dat";
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_9.dat";
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_10.dat";
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_11.dat";
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_12.dat";
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_13.dat";
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_14.dat";
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_15.dat";
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_16.dat";
	}

	HANDLE hFile = CreateFile(Filename, GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
		return;
	}

	Delete_Line(m_eNowChannel);

	DWORD dwByte = 0;

	LINEINFO tTemp = {};
	TILE::TYPE eType = TILE::END;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(LINEINFO), &dwByte, nullptr);
		ReadFile(hFile, &eType, sizeof(TILE::TYPE), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		m_listLine[m_eNowChannel].emplace_back(new CLine(tTemp, eType));
	}

	CloseHandle(hFile);
//		MessageBox(g_hWnd, L"불러오기 성공", L"성공", MB_OK);
}

void CLineMgr::Save_Line(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;

	TCHAR* Filename = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_1.dat";

	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_2.dat";

	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_3.dat";

	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_4.dat";

	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_5.dat";

	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_6.dat";

	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_7.dat";
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_8.dat";
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_9.dat";
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_10.dat";
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_11.dat";
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_12.dat";
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_13.dat";
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_14.dat";
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_15.dat";
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		Filename = L"../Data/Line/STAGE1_16.dat";
	}

	HANDLE hFile = CreateFile(Filename, GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	for (auto& pLine : m_listLine[m_eNowChannel])
	{
		WriteFile(hFile, &pLine->Get_Info(), sizeof(LINEINFO), &dwByte, NULL);
		WriteFile(hFile, &pLine->Get_Type(), sizeof(TILE::TYPE), &dwByte, NULL);
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CLineMgr::Up_MovingTile()
{
	if (m_iMovingCount > 50)
		return;
	if (!m_iMovingCount)
		return;
	if (m_dwMovingTile + 200 < GetTickCount())
	{
		for (auto& pLine : m_listLine[m_eNowChannel])
		{
			if (pLine->Get_Type() == TILE::MOVING)
				pLine->Set_LineY(4.f);
		}
		m_iMovingCount++;
		m_dwMovingTile = GetTickCount();
	}
}
