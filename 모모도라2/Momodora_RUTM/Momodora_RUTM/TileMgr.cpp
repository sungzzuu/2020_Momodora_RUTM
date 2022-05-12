#include "stdafx.h"
#include "TileMgr.h"
#include "Tile.h"
#include "ScrollMgr.h"
#include "KeyMgr.h"
#include "CollisionMgr.h"
#include "ObjMgr.h"
#include "LineMgr.h"
#include "SoundMgr.h"
CTileMgr* CTileMgr::m_pInstance = nullptr;
CTileMgr::CTileMgr()
	:m_eNowChannel(STAGE::STAGE1_1)
{
}


CTileMgr::~CTileMgr()
{
	Release();
}

void CTileMgr::Initialize()
{
	m_iMovingCount = 0;
}

void CTileMgr::Update()
{
	for (auto& pTile : m_listTile[m_eNowChannel])
		pTile->Update();
	Up_MovingTile();
}

void CTileMgr::Late_Update()
{
	
}

void CTileMgr::Render(HDC _DC)
{
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	int iCullX = abs(iScrollX / TILECX);
	int iCullY = abs(iScrollY / TILECY);

	int iCullEndX = iCullX + WINCX / TILECX + 2;
	int iCullEndY = iCullY + WINCY / TILECY + 2;

	for (auto& pTile : m_listTile[m_eNowChannel])
	{
		if (pTile->Get_Info().fX >= iCullX*TILECX && pTile->Get_Info().fY >= iCullY*TILECY
			&& pTile->Get_Info().fX <= iCullEndX*TILECX &&  pTile->Get_Info().fY <= iCullEndY*TILECY)
		{
			pTile->Render(_DC);
		}
		
	}
}

void CTileMgr::Release()
{
	for (int i = 0; i < STAGE::END; ++i)
	{
		for_each(m_listTile[i].begin(), m_listTile[i].end(), Safe_Delete<CObj*>);
		m_listTile[i].clear();
	}
	
}


void CTileMgr::Picking_Tile(POINT _pt, int _iDrawID_X, int _iDrawID_Y, int _iOption, TILE::TYPE _eType, STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	_pt.x -= iScrollX;
	_pt.y -= iScrollY;

	// 삭제
	
	if (_eType == TILE::END)
	{
		list<CObj*>::iterator iter = m_listTile[m_eNowChannel].begin();
		for (;iter != m_listTile[m_eNowChannel].end(); iter++)
		{
			if (PtInRect(&(*iter)->Get_Rect(), _pt))
			{
				m_listTile[m_eNowChannel].erase(iter);
				return;

			}
		}
		return;
	}

	// 이미 그자리에 타일이 있는 경우
	list<CObj*>::iterator iter = m_listTile[m_eNowChannel].begin();
	for (; iter != m_listTile[m_eNowChannel].end(); iter++)
	{
		if (PtInRect(&(*iter)->Get_Rect(), _pt))
		{
			if (TILE::LADDER == dynamic_cast<CTile*>(*iter)->Get_Type() && _eType == TILE::THIN
				|| TILE::THIN == dynamic_cast<CTile*>(*iter)->Get_Type() && _eType == TILE::LADDER)
				continue;
			if (TILE::OBJECT != dynamic_cast<CTile*>(*iter)->Get_Type() && _eType == TILE::OBJECT
				|| TILE::OBJECT == dynamic_cast<CTile*>(*iter)->Get_Type() && _eType != TILE::OBJECT)
				continue;
			m_listTile[m_eNowChannel].erase(iter);
			break;
		}
	}

	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	//int iIdx = y * TILEX + x;

	CObj* pObj = CAbstractFactory<CTile>::Create((float)(x*TILECX+(TILECX>>1)), (float)(y*TILECY+(TILECY >> 1)));
	m_listTile[m_eNowChannel].emplace_back(pObj);

	//if (0 > iIdx || TILEX*TILEY <= iIdx)
	//	return;

	dynamic_cast<CTile*>(m_listTile[m_eNowChannel].back())->Set_DrawID_X(_iDrawID_X);
	dynamic_cast<CTile*>(m_listTile[m_eNowChannel].back())->Set_DrawID_Y(_iDrawID_Y);
	dynamic_cast<CTile*>(m_listTile[m_eNowChannel].back())->Set_Type(_eType);
	dynamic_cast<CTile*>(m_listTile[m_eNowChannel].back())->Set_Option(_iOption);
}
void CTileMgr::Up_MovingTile()
{
	if (m_iMovingCount > 50)
		return;
	if (!m_iMovingCount)
		return;
	if (m_dwMovingTile + 200 < GetTickCount())
	{
		for (auto& pTile : m_listTile[m_eNowChannel])
		{
			if (dynamic_cast<CTile*>(pTile)->Get_Type() == TILE::MOVING)
				pTile->Set_PosY(-4.f);
			//CSoundMgr::Get_Instance()->StopSound(CSoundMgr::EFFECT);
			if(STAGE::STAGE1_14 != m_eNowChannel)
				CSoundMgr::Get_Instance()->PlaySound(L"ChainMoving.wav", CSoundMgr::EFFECT);

		}
		m_iMovingCount++;
		m_dwMovingTile = GetTickCount();
	}

}
bool CTileMgr::Collision_BounceBall_Player()
{
	CObj* pPlayer = CObjMgr::Get_Instance()->Get_Player();
	RECT rc = {};
	for (auto& iter = m_listTile[m_eNowChannel].begin(); iter != m_listTile[m_eNowChannel].end(); ++iter)
	{
		if (TILE::BOUNCE != dynamic_cast<CTile*>(*iter)->Get_Type())
			continue;
		if (IntersectRect(&rc, &pPlayer->Get_Rect(), &(*iter)->Get_CollisionRect()))
		{
			return true;
		}
	}

	return false;
}
CObj * CTileMgr::Get_Orb()
{
	for (auto& pTile : m_listTile[m_eNowChannel])
	{
		if (TILE::ORB == dynamic_cast<CTile*>(pTile)->Get_Type())
			return pTile;
	}
}

void CTileMgr::Save_Tile(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;
	
	TCHAR* Filename = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_1.dat";
	
	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_2.dat";
	
	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_3.dat";

	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_4.dat";

	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_5.dat";

	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_6.dat";

	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_7.dat";
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_8.dat";
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_9.dat";
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_10.dat";
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_11.dat";
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_12.dat";
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_13.dat";
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_14.dat";
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_15.dat";
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		Filename = L"../Data/STAGE1_16.dat";
	}

	HANDLE hFile = CreateFile(Filename, GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	for (auto& pTile : m_listTile[m_eNowChannel])
	{
		WriteFile(hFile, &pTile->Get_Info(), sizeof(INFO), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_DrawID_X(), sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_DrawID_Y(), sizeof(int), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_Type(), sizeof(TILE::TYPE), &dwByte, NULL);
		WriteFile(hFile, &dynamic_cast<CTile*>(pTile)->Get_Option(), sizeof(int), &dwByte, NULL);
	}

	CloseHandle(hFile);
	//MessageBox(g_hWnd, L"저장 성공", L"성공", MB_OK);
}

void CTileMgr::Set_LineOn(bool _bLineOn)
{
	for (auto& pLine : m_listTile[m_eNowChannel])
	{
		pLine->Set_LineOn(_bLineOn);
	}
}

void CTileMgr::Next_Channel()
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
	Load_Tile(m_eNowChannel);
}

void CTileMgr::Previous_Channel()
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
	Load_Tile(m_eNowChannel);
}

void CTileMgr::Delete_Tile(STAGE::CHANNEL _eChannel)
{
	for_each(m_listTile[_eChannel].begin(), m_listTile[_eChannel].end(), Safe_Delete<CObj*>);
	m_listTile[_eChannel].clear();
}

void CTileMgr::Get_Begin_Tile(float * _x)
{
	float fX = -1.f;
	for (auto& pTile : m_listTile[m_eNowChannel])
	{
		if (-1.f == fX)
			fX = pTile->Get_Rect().left;
		else
		{
			if (fX > pTile->Get_Rect().left)
				fX = pTile->Get_Rect().left;
		}
	}
	*_x = fX;
}

void CTileMgr::Get_End_Tile(float * _x, float * _y)
{
	
	float fX = -1.f;
	float fY = -1.f;

	for (auto& pTile : m_listTile[m_eNowChannel])
	{
		pTile->Update();
		if (-1.f == fX)
			fX = pTile->Get_Rect().right;
		else
		{
			if (fX < pTile->Get_Rect().right)
				fX = pTile->Get_Rect().right;
		}

		if (-1.f == fY)
			fY = pTile->Get_Rect().bottom;
		else
		{
			if (TILE::MOVING == dynamic_cast<CTile*>(pTile)->Get_Type())
				continue;
			if (fY < pTile->Get_Rect().bottom)
				fY = pTile->Get_Rect().bottom;
		}
	}

	*_x = fX;
	*_y = fY;
}

bool CTileMgr::Get_ThinTile()
{
	return false;
}

void CTileMgr::Load_Tile(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;
	HANDLE hFile = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_1.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_2.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_3.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_4.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_5.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_6.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_7.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_8.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_9.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_10.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_11.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_12.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_13.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_14.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_15.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		hFile = CreateFile(L"../Data/STAGE1_16.dat", GENERIC_READ
			, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	//if (INVALID_HANDLE_VALUE == hFile)
	//{
	//	MessageBox(g_hWnd, L"불러오기 실패", L"실패", MB_OK);
	//	return;
	//}

	Delete_Tile(m_eNowChannel);

	DWORD dwByte = 0;
	INFO tTemp = {};
	int iDrawID_X = 0, iDrawID_Y =0,  iOption = 0;
	TILE::TYPE eType = TILE::END;

	while (true)
	{
		ReadFile(hFile, &tTemp, sizeof(INFO), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID_X, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &iDrawID_Y, sizeof(int), &dwByte, nullptr);
		ReadFile(hFile, &eType, sizeof(TILE::TYPE), &dwByte, nullptr);
		ReadFile(hFile, &iOption, sizeof(int), &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CObj* pTile = CAbstractFactory<CTile>::Create();
		dynamic_cast<CTile*>(pTile)->Load_Tile(tTemp);
		dynamic_cast<CTile*>(pTile)->Set_DrawID_X(iDrawID_X);
		dynamic_cast<CTile*>(pTile)->Set_DrawID_Y(iDrawID_Y);
		dynamic_cast<CTile*>(pTile)->Set_Type(eType);
		dynamic_cast<CTile*>(pTile)->Set_Option(iOption);
		m_listTile[m_eNowChannel].emplace_back(pTile);
	}

	CloseHandle(hFile);
//	MessageBox(g_hWnd, L"불러오기 성공", L"성공", MB_OK);

}

