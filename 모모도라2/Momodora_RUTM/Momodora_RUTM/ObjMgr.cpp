#include "stdafx.h"
#include "ObjMgr.h"
#include "Obj.h"
#include "CollisionMgr.h"
#include "TileMgr.h"
#include "Tile.h"
#include "MonkeyMonster.h"
#include "JumpingMonster.h"
#include "BombMonster.h"
#include "ShieldMonster.h"
#include "Player.h"
#include "LineMgr.h"
#include "RectMgr.h"
#include "ScrollMgr.h"
#include "JumpingMonster.h"
#include "MonkeyMonster.h"
#include "BombMonster.h"
#include "ShieldMonster.h"
#include "Chest.h"
#include "BigPlant.h"
#include "SavePoint.h"
#include "Item.h"
#include "Fennel.h"

CObjMgr* CObjMgr::m_pInstance = nullptr;

CObjMgr::CObjMgr()
	:m_eNowChannel(STAGE::STAGE1_1)
{
}


CObjMgr::~CObjMgr()
{
	Release();
}

void CObjMgr::Update()
{
	STAGE::CHANNEL _eChannel = STAGE::END;

	for (int i = 0; i < OBJID::END; ++i)
	{
		if (0 == i)
			_eChannel = STAGE::STAGE1_1;
		else
			_eChannel = m_eNowChannel;

		auto& iter = m_listObj[i][_eChannel].begin();
		for (; iter != m_listObj[i][_eChannel].end();)
		{
			int iEvent = (*iter)->Update();
			if (OBJ_DEAD == iEvent)
			{
				Safe_Delete<CObj*>(*iter);
				iter = m_listObj[i][_eChannel].erase(iter);
			}
			else if ((*iter)->Get_ChannelChange())
			{
				(*iter)->Set_ChannelChange();
				return;

			}
			else
				++iter;
		}
	}

	//CCollisionMgr::Collision_Rect(m_listObj[OBJID::MONSTER], m_listObj[OBJID::BULLET]);
	//CCollisionMgr::Collision_RectEx(m_listObj[OBJID::PLAYER], m_listObj[OBJID::MONSTER]);
	//CCollisionMgr::Collision_RectEx(m_listObj[OBJID::MONSTER], m_listObj[OBJID::PLAYER]);
	//CCollisionMgr::Collision_Rect(m_listObj[OBJID::BULLET], m_listObj[OBJID::MONSTER]);
	//CCollisionMgr::Collision_Rect(m_listObj[OBJID::MOUSE], m_listObj[OBJID::MONSTER]);
	//CCollisionMgr::Collision_Sphere(m_listObj[OBJID::MOUSE], m_listObj[OBJID::MONSTER]);
	//CCollisionMgr::Collision_RectEx(m_listObj[OBJID::MONSTER], m_listObj[OBJID::PLAYER]);
	Collision_Check();
	Collision_Orb_Arrow();

}

void CObjMgr::Late_Update()
{
	STAGE::CHANNEL _eChannel = STAGE::END;
	for (int i = 0; i < OBJID::END; ++i)
	{
		if (0 == i)
			_eChannel = STAGE::STAGE1_1;
		else
			_eChannel = m_eNowChannel;
		
		for (auto& pObj : m_listObj[i][_eChannel])
		{
			if (!pObj)
				return;
			pObj->Late_Update();
			if (m_listObj[i][_eChannel].empty())
				break;

		}
	}
	
	
}

void CObjMgr::Render(HDC _DC)
{
	STAGE::CHANNEL _eChannel = STAGE::END;
	

	for (int i = OBJID::END; i >= 0; --i)
	{
		if (0 == i || 1 == i)
			_eChannel = STAGE::STAGE1_1;
		else
			_eChannel = m_eNowChannel;
		for (auto& pObj : m_listObj[i][_eChannel])
			pObj->Render(_DC);
	}
}

void CObjMgr::Release()
{
	STAGE::CHANNEL _eChannel = STAGE::END;
	for (int c = 0; c < STAGE::END; c++)
	{
		for (int i = 0; i < OBJID::END; ++i)
		{

			for_each(m_listObj[i][c].begin(), m_listObj[i][c].end(), Safe_Delete<CObj*>);
			m_listObj[i][c].clear();
		}
	}
	
}

void CObjMgr::Add_Object(OBJID::ID _eID, CObj* _pObj)
{
		m_listObj[_eID][m_eNowChannel].emplace_back(_pObj);
}

void CObjMgr::Delete_ID(OBJID::ID _eID)
{
	STAGE::CHANNEL _eChannel = STAGE::END;
	if (OBJID::PLAYER == _eID)
		_eChannel = STAGE::STAGE1_1;
	else
		_eChannel = m_eNowChannel;
	for (auto& pObj : m_listObj[_eID][_eChannel])
		Safe_Delete(pObj);
	m_listObj[_eID][_eChannel].clear();
}

void CObjMgr::Delete_Leaf()
{
	for (auto& iter = m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel].begin(); iter != m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel].end(); ++iter)
	{
		if (L"Leaf" == (*iter)->Get_Name())
		{
			Safe_Delete(*iter);
			iter = m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel].erase(iter);
			return;
		}
	}
}

CObj* CObjMgr::Get_Target(CObj* _pDst, OBJID::ID _eID)
{

	if (m_listObj[_eID][m_eNowChannel].empty())
		return nullptr;

	CObj* pTarget = nullptr;
	float fDis = 0.f;

	for (auto& _Src : m_listObj[_eID][m_eNowChannel])
	{
		float fX = abs(_pDst->Get_Info().fX - _Src->Get_Info().fX);
		float fY = abs(_pDst->Get_Info().fY - _Src->Get_Info().fY);
		float fDis2 = sqrtf(fX * fX + fY * fY);

		if (fDis > fDis2 || !pTarget)
		{
			pTarget = _Src;
			fDis = fDis2;
		}
	}

	return pTarget;
}

CObj * CObjMgr::Get_Fennel()
{
	if (m_listObj[OBJID::MONSTER][STAGE::STAGE1_16].empty())
		return nullptr;
	return m_listObj[OBJID::MONSTER][STAGE::STAGE1_16].front();
}

void CObjMgr::Picking_Obj(POINT _pt, int _iOption,int _iType, STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;
	int iScrollX = (int)CScrollMgr::Get_Instance()->Get_ScrollX();
	int iScrollY = (int)CScrollMgr::Get_Instance()->Get_ScrollY();

	_pt.x -= iScrollX;
	_pt.y -= iScrollY;

	// 삭제

	if (-1 == _iOption)
	{
		if (0 == _iType)
		{
			list<CObj*>::iterator iter = m_listObj[OBJID::MONSTER][m_eNowChannel].begin();
			for (; iter != m_listObj[OBJID::MONSTER][m_eNowChannel].end(); iter++)
			{
				if (PtInRect(&(*iter)->Get_Rect(), _pt))
				{
					m_listObj[OBJID::MONSTER][m_eNowChannel].erase(iter);
					return;

				}
			}
		}
		else if (1 == _iType)
		{
			list<CObj*>::iterator iter = m_listObj[OBJID::SAVEPOINT][m_eNowChannel].begin();
			for (; iter != m_listObj[OBJID::SAVEPOINT][m_eNowChannel].end(); iter++)
			{
				if (PtInRect(&(*iter)->Get_Rect(), _pt))
				{
					m_listObj[OBJID::SAVEPOINT][m_eNowChannel].erase(iter);
					return;

				}
			}
		}
		else if (2 == _iType)
		{
			list<CObj*>::iterator iter = m_listObj[OBJID::ITEM][m_eNowChannel].begin();
			for (; iter != m_listObj[OBJID::ITEM][m_eNowChannel].end(); iter++)
			{
				if (PtInRect(&(*iter)->Get_Rect(), _pt))
				{
					m_listObj[OBJID::ITEM][m_eNowChannel].erase(iter);
					return;

				}
			}
		}
		return;
	}

	int x = _pt.x / TILECX;
	int y = _pt.y / TILECY;

	if (1 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CJumpingMonster>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);

		m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
	}
	else if (2 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CBombMonster>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);

		m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
	}
	else if (3 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CShieldMonster>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);

		m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
	}
	else if (4 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CMonkeyMonster>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);

		m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
	}
	else if (5 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CBigPlant>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);

		m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
	}
	else if (6 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CChest>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);

		m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
	}
	else if (7 == _iOption || 8 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CSavePoint>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);
		m_listObj[OBJID::SAVEPOINT][m_eNowChannel].emplace_back(pObj);
	}
	else if (9 == _iOption)
	{
		CObj* pObj = CAbstractFactory<CItem>::Create((float)(x*TILECX + (TILECX >> 1)), (float)(y*TILECY + (TILECY >> 1)));
		pObj->Set_ObjOption(_iOption);
		m_listObj[OBJID::ITEM][m_eNowChannel].emplace_back(pObj);
	}
}

bool CObjMgr::Collision_Player_Weapon()
{
	float fX = 0.f;
	float fY = 0.f;

	CObj* _pPlayer = m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].front();
	for (auto& iter = m_listObj[OBJID::MONSTER_WEAPON][m_eNowChannel].begin(); iter != m_listObj[OBJID::MONSTER_WEAPON][m_eNowChannel].end();)
	{
		if ((*iter)->Get_Dead())
		{
			++iter;
			continue;

		}
		if (CCollisionMgr::Check_Rect(*iter, _pPlayer, &fX, &fY))
		{
			if (fX > fY)
			{
				if ((*iter)->Get_Info().fY < _pPlayer->Get_Info().fY)
					_pPlayer->Set_PosY(fY);
				else
					_pPlayer->Set_PosY(-fY);
			}
			else
			{
				if ((*iter)->Get_Info().fX < _pPlayer->Get_Info().fX)
					_pPlayer->Set_PosX(fX);
				else
					_pPlayer->Set_PosX(-fX);
			}
			_pPlayer->Set_Hp((*iter)->Get_Att());
			SAFE_DELETE(*iter);
			iter = m_listObj[OBJID::MONSTER_WEAPON][m_eNowChannel].erase(iter);
			return true;
		}
		else
			++iter;
	}
	return false;
}

bool CObjMgr::Collision_Player_Monster()
{
	float fX = 0.f;
	float fY = 0.f;
	CObj* _pPlayer = m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].front();
	for (auto& pMonster : m_listObj[OBJID::MONSTER][m_eNowChannel])
	{
		if (CCollisionMgr::Check_Rect(pMonster, _pPlayer, &fX, &fY))
		{
			//if (fX > fY)
			//{
			//	if (pMonster->Get_Info().fY < _pPlayer->Get_Info().fY)
			//		_pPlayer->Set_PosY(fY);
			//	else
			//		_pPlayer->Set_PosY(-fY);
			//}
			//else
			//{
			//	if (pMonster->Get_Info().fX < _pPlayer->Get_Info().fX)
			//		_pPlayer->Set_PosX(fX);
			//	else
			//		_pPlayer->Set_PosX(-fX);
			//}
			_pPlayer->Set_Hp(pMonster->Get_Att());
			return true;
		}
	}
	return false;
}

bool CObjMgr::Collision_Monster_Weapon(CObj * _pMonster, bool* _bCol)
{
	if (*_bCol == true)
		return false;

	float fX = 0.f;
	float fY = 0.f;
	//부딪히면 무기삭제
	// 값을 바꾸면 자꾸 날라감
	for (auto& iter = m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel].begin(); iter != m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel].end(); ++iter)
	{
		if (CCollisionMgr::Check_Rect(*iter, _pMonster, &fX, &fY))
		{
			//if (fX > fY)
			//{
			//	if (pWeapon->Get_Info().fY < _pMonster->Get_Info().fY)
			//		_pMonster->Set_PosY(fY);
			//	else
			//		_pMonster->Set_PosY(-fY);
			//}
			//else
			//{
			//	if (pWeapon->Get_Info().fX < _pMonster->Get_Info().fX)
			//		_pMonster->Set_PosX(fX);
			//	else
			//		_pMonster->Set_PosX(-fX);
			//}
			_pMonster->Set_Hp((*iter)->Get_Att());
			*_bCol = true;
			if (!lstrcmp(L"Arrow",(*iter)->Get_Name()))
			{
				SAFE_DELETE(*iter);
				iter = m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel].erase(iter);

			}
				
			return true;
		}
	}
	return false;

	
}

bool CObjMgr::Collision_SavePoint_Player()
{
	CObj* pPlayer = m_listObj[OBJID::PLAYER][m_eNowChannel].front();
	RECT rc = {};
	for (auto& pSavePoint : m_listObj[OBJID::SAVEPOINT][m_eNowChannel])
	{
		if (IntersectRect(&rc, &pPlayer->Get_Rect(), &pSavePoint->Get_Rect()))
		{
			return true;
		}
	}
	return false;
}

bool CObjMgr::Collision_Orb_Arrow()
{
	if (!CTileMgr::Get_Instance()->Get_Orb())
		return false;
	RECT rc = {};
	for (auto& pWeapon : m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel])
	{
		if (IntersectRect(&rc, &CTileMgr::Get_Instance()->Get_Orb()->Get_Rect(), &pWeapon->Get_Rect()))
		{
			if (dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Orb())->Get_Option() == 15)
				return false;
			dynamic_cast<CTile*>(CTileMgr::Get_Instance()->Get_Orb())->Set_Option(15);
			CTileMgr::Get_Instance()->Set_dwMovingTime();
			CRectMgr::Get_Instance()->Set_dwMovingTime();
			CLineMgr::Get_Instance()->Set_dwMovingTime();
			return true;
		}

			
	}
	return false;
}

bool CObjMgr::Collision_Player_Gold()
{

	CObj* pPlayer = m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].front();
	RECT rc = {};
	for (auto& iter = m_listObj[OBJID::GOLD][m_eNowChannel].begin(); iter != m_listObj[OBJID::GOLD][m_eNowChannel].end();++iter)
	{
		if (IntersectRect(&rc, &pPlayer->Get_Rect(), &(*iter)->Get_CollisionRect()))
		{
			SAFE_DELETE(*iter);
			iter = m_listObj[OBJID::GOLD][m_eNowChannel].erase(iter);
			return true;
		}
	}

	return false;
}

bool CObjMgr::Collision_Flower_Player()
{
	CObj* pPlayer = m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].front();
	RECT rc = {};
	for (auto& iter = m_listObj[OBJID::ITEM][m_eNowChannel].begin(); iter != m_listObj[OBJID::ITEM][m_eNowChannel].end(); ++iter)
	{
		if (IntersectRect(&rc, &pPlayer->Get_Rect(), &(*iter)->Get_CollisionRect()))
		{
			SAFE_DELETE(*iter);
			iter = m_listObj[OBJID::ITEM][m_eNowChannel].erase(iter);
			dynamic_cast<CPlayer*>(pPlayer)->Set_ItemOn();
			dynamic_cast<CPlayer*>(pPlayer)->Set_ItemCount();

			return true;
		}
	}

	return false;
}


void CObjMgr::Save_Obj(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;

	TCHAR* Filename = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_1.dat";

	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_2.dat";

	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_3.dat";

	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_4.dat";

	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_5.dat";

	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_6.dat";

	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_7.dat";
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_8.dat";
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_9.dat";
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_10.dat";
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_11.dat";
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_12.dat";
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_13.dat";
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_14.dat";
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_15.dat";
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_16.dat";
	}

	HANDLE hFile = CreateFile(Filename, GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	for (int i = 0; i < OBJID::END; i++)
	{
		for (auto& pObj : m_listObj[i][m_eNowChannel])
		{
			WriteFile(hFile, &pObj->Get_Info().fX, sizeof(float), &dwByte, NULL);
			WriteFile(hFile, &pObj->Get_Info().fY, sizeof(float), &dwByte, NULL);
			WriteFile(hFile, &pObj->Get_ObjOption(), sizeof(int), &dwByte, NULL);

		}
	}
	
	CloseHandle(hFile);
	MessageBox(g_hWnd, L"몬스터 저장 성공", L"성공", MB_OK);
}

void CObjMgr::Load_Obj(STAGE::CHANNEL _eChannel)
{
	m_eNowChannel = _eChannel;
	TCHAR* Filename = nullptr;
	if (STAGE::STAGE1_1 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_1.dat";

	}
	else if (STAGE::STAGE1_2 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_2.dat";

	}
	else if (STAGE::STAGE1_3 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_3.dat";

	}
	else if (STAGE::STAGE1_4 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_4.dat";

	}
	else if (STAGE::STAGE1_5 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_5.dat";

	}
	else if (STAGE::STAGE1_6 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_6.dat";

	}
	else if (STAGE::STAGE1_7 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_7.dat";
	}
	else if (STAGE::STAGE1_8 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_8.dat";
	}
	else if (STAGE::STAGE1_9 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_9.dat";
	}
	else if (STAGE::STAGE1_10 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_10.dat";
	}
	else if (STAGE::STAGE1_11 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_11.dat";
	}
	else if (STAGE::STAGE1_12 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_12.dat";
	}
	else if (STAGE::STAGE1_13 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_13.dat";
	}
	else if (STAGE::STAGE1_14 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_14.dat";
	}
	else if (STAGE::STAGE1_15 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_15.dat";
	}
	else if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		Filename = L"../Data/Obj/STAGE1_16.dat";
	}
	HANDLE hFile = CreateFile(Filename, GENERIC_READ
		, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	Delete_ID(OBJID::MONSTER);
	Delete_ID(OBJID::SAVEPOINT);
	Delete_ID(OBJID::ITEM);
	Delete_ID(OBJID::MONSTER_WEAPON);
	Delete_ID(OBJID::PLAER_WEAPON);

	DWORD dwByte = 0;
	float	fX = 0.f;
	float	fY = 0.f;
	int		_iOption = 0;

	while (true)
	{
		ReadFile(hFile, &fX, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fY, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &_iOption, sizeof(int), &dwByte, nullptr);

		if (0 == dwByte)
			break;

		if (1 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CJumpingMonster>::Create(fX,fY);
			pObj->Set_ObjOption(_iOption);
			m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
		}
		else if (2 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CBombMonster>::Create(fX, fY);
			pObj->Set_ObjOption(_iOption);

			m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
		}
		else if (3 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CShieldMonster>::Create(fX, fY);
			pObj->Set_ObjOption(_iOption);

			m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
		}
		else if (4 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CMonkeyMonster>::Create(fX, fY);
			pObj->Set_ObjOption(_iOption);

			m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
		}
		else if (5 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CBigPlant>::Create(fX, fY);
			pObj->Set_ObjOption(_iOption);

			m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
		}
		else if (6 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CChest>::Create(fX, fY);
			pObj->Set_ObjOption(_iOption);

			m_listObj[OBJID::MONSTER][m_eNowChannel].emplace_back(pObj);
		}
		else if (7 == _iOption || 8 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CSavePoint>::Create(fX, fY);
			pObj->Set_ObjOption(_iOption);
			m_listObj[OBJID::SAVEPOINT][m_eNowChannel].emplace_back(pObj);
		}
		else if (9 == _iOption)
		{
			CObj* pObj = CAbstractFactory<CItem>::Create(fX, fY);
			pObj->Set_ObjOption(_iOption);
			m_listObj[OBJID::ITEM][m_eNowChannel].emplace_back(pObj);
		}
	}

	CloseHandle(hFile);
	//	MessageBox(g_hWnd, L"불러오기 성공", L"성공", MB_OK);

}

void CObjMgr::Player_Save()
{
	TCHAR* Filename = nullptr;
	
	if (1 == m_iSlotNum)
		Filename = L"../Data/Player/Player1.dat";
	else
		Filename = L"../Data/Player/Player2.dat";

	HANDLE hFile = CreateFile(Filename, GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return;
	}

	DWORD dwByte = 0;
	CObj* pObj = m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].front();

	WriteFile(hFile, &pObj->Get_Info().fX, sizeof(float), &dwByte, NULL);
	WriteFile(hFile, &pObj->Get_Info().fY, sizeof(float), &dwByte, NULL);
	WriteFile(hFile, &m_eNowChannel, sizeof(STAGE::CHANNEL), &dwByte, NULL);

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"플레이어 저장 성공", L"성공", MB_OK);
}

bool CObjMgr::Player_Load()
{
	TCHAR* Filename = nullptr;

	if (1 == m_iSlotNum)
		Filename = L"../Data/Player/Player1.dat";
	else
		Filename = L"../Data/Player/Player2.dat";

	HANDLE hFile = CreateFile(Filename, GENERIC_WRITE
		, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (INVALID_HANDLE_VALUE == hFile)
	{
		MessageBox(g_hWnd, L"저장 실패", L"실패", MB_OK);
		return false;
	}

	DWORD dwByte = 0;
	float	fX = 0.f;
	float	fY = 0.f;
	STAGE::CHANNEL		_eChannel = STAGE::END;

	while (true)
	{
		ReadFile(hFile, &fX, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &fY, sizeof(float), &dwByte, nullptr);
		ReadFile(hFile, &_eChannel, sizeof(STAGE::CHANNEL), &dwByte, nullptr);

		if (0 == dwByte)
			return false;
		CObj* pObj = CAbstractFactory<CPlayer>::Create(fX, fY);
		m_listObj[OBJID::PLAYER][STAGE::STAGE1_1].emplace_back(pObj);
		m_eNowChannel = _eChannel;
		return true;
	}

	CloseHandle(hFile);
	MessageBox(g_hWnd, L"플레이어 불러오기 성공", L"성공", MB_OK);
}

void CObjMgr::Next_Channel()
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
	Load_Obj(m_eNowChannel);

	if (STAGE::STAGE1_16 == m_eNowChannel)
	{
		CObj* pObj = CAbstractFactory<CFennel>::Create();
		Add_Object(OBJID::MONSTER, pObj);
	}
}

void CObjMgr::Previous_Channel()
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
	Load_Obj(m_eNowChannel);

	
}

void CObjMgr::Collision_Check()
{
	// 렉트와 플레이어
	CCollisionMgr::Collision_RectEx_Rect(m_listObj[OBJID::PLAYER][STAGE::STAGE1_1]);
	
	//무기와 몬스터->몬스터의 상태만 변환

	//몬스터와 렉트
	CCollisionMgr::Collision_RectEx_Rect(m_listObj[OBJID::MONSTER][m_eNowChannel]);

	//검과 렉트 ->박힘!!!
	CCollisionMgr::Collision_RectEx_Rect(m_listObj[OBJID::MONSTER_WEAPON][m_eNowChannel]);

	CCollisionMgr::Collision_RectEx_Rect(m_listObj[OBJID::PLAER_WEAPON][m_eNowChannel]);

	// 플레이어와 가시


}

void CObjMgr::Set_Line(bool _bLineOn)
{
	STAGE::CHANNEL _eChannel = STAGE::END;

	
	for (int i = 0; i < OBJID::END; i++)
	{
		if (0 == i)
			_eChannel = STAGE::STAGE1_1;
		else
			_eChannel = m_eNowChannel;
		for (auto& pObj : m_listObj[i][_eChannel])
		{
			pObj->Set_LineOn(_bLineOn);
		}
	}
}
