#include "stdafx.h"
#include "CollisionMgr.h"
#include "Obj.h"
#include "RectMgr.h"
#include "MyRect.h"
#include "LineMgr.h"
#include "Player.h"
#include "ObjMgr.h"


CCollisionMgr::CCollisionMgr()
{
}


CCollisionMgr::~CCollisionMgr()
{
}


bool CCollisionMgr::Collision_RectEx(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	float fX = 0.f, fY = 0.f;

	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Rect(Dst, Src, &fX, &fY))
			{
				if (fX > fY)
				{
					if (Dst->Get_Info().fY < Src->Get_Info().fY)
						Src->Set_PosY(fY);
					else
						Src->Set_PosY(-fY);
				}
				else
				{
					if (Dst->Get_Info().fX < Src->Get_Info().fX)
						Src->Set_PosX(fX);
					else
						Src->Set_PosX(-fX);
				}
				return true;
			}
		}
	}


	return false;
}

bool CCollisionMgr::Collision_RectEx(CObj * _Dst, list<CObj*>& _Src)
{
	if (!_Dst)
		return false;
	float fX = 0.f, fY = 0.f;

	for (auto& Src : _Src)
	{
		if (Check_Rect(_Dst, Src, &fX, &fY))
		{
			if (fX > fY)
			{
				if (_Dst->Get_Info().fY < Src->Get_Info().fY)
					Src->Set_PosY(fY);
				else
					Src->Set_PosY(-fY);
			}
			else
			{
				if (_Dst->Get_Info().fX < Src->Get_Info().fX)
					Src->Set_PosX(fX);
				else
					Src->Set_PosX(-fX);
			}
			return true;
		}
	}
	return false;
}

void CCollisionMgr::Collision_Sphere(list<CObj*>& _Dst, list<CObj*>& _Src)
{
	for (auto& Dst : _Dst)
	{
		for (auto& Src : _Src)
		{
			if (Check_Sphere(Dst, Src))
			{
				//Dst->Set_Dead();
				Src->Set_Dead();
			}
		}
	}
}

void CCollisionMgr::Collision_RectEx_Rect(list<CObj*>& _Src)
{
	list<CMyRect*> _pRect = CRectMgr::Get_Instance()->Get_Rect();

	RECT rc = {};

	for (auto& pRect : _pRect)
	{
		for (auto& Src : _Src)
		{
			if (IntersectRect(&rc, &pRect->Get_Rect(), &Src->Get_CollisionRect()))
			{
				if (pRect->Get_Type() == TILE::THORN)
				{
					Src->Set_Hp(1000);
					return;
				}
				float fY = 0.f;
				if (CLineMgr::Get_Instance()->Collision_Ceiling(Src->Get_Info().fX, Src->Get_CollisionRect().top, &fY))
					continue;
				if (pRect->Get_Type() != TILE::LADDER)
				{
					float fX = (float)(rc.right - rc.left);
					float fY = (float)(rc.bottom - rc.top);
					if (rc.right == Src->Get_CollisionRect().right)
						Src->Set_PosX(-fX);
					else if (rc.left == Src->Get_CollisionRect().left)
						Src->Set_PosX(fX);

					Src->Set_Dead();
				}
				return;
			}
		}
	}

}

bool CCollisionMgr::Check_Sphere(CObj* _Dst, CObj* _Src)
{
	float fRad = (float)((_Dst->Get_Info().iCX + _Src->Get_Info().iCY) >> 1);

	float fX = _Dst->Get_Info().fX - _Src->Get_Info().fX;
	float fY = _Dst->Get_Info().fY - _Src->Get_Info().fY;
	float fDis = sqrtf(fX * fX + fY * fY);

	return fRad > fDis;
}

bool CCollisionMgr::Check_Rect(CObj* _Dst, CObj* _Src, float* _x, float* _y)
{
	float fX = abs(_Dst->Get_Info().fX - _Src->Get_Info().fX);
	float fY = abs(_Dst->Get_Info().fY - _Src->Get_Info().fY);

	float fCX = (float)((_Dst->Get_Info().iCollisionCX + _Src->Get_Info().iCollisionCX) >> 1);
	float fCY = (float)((_Dst->Get_Info().iCollisionCY + _Src->Get_Info().iCollisionCY) >> 1);

	if (fCX > fX && fCY > fY)
	{
		*_x = fCX - fX;
		*_y = fCY - fY;
		return true;
	}
	return false;
}
bool CCollisionMgr::Check_Rect(CMyRect* _Dst, CObj* _Src, float* _x, float* _y)
{
	float rtX = _Dst->Get_Rect().left + ((_Dst->Get_Rect().right - _Dst->Get_Rect().left) >> 1);
	float rtY = _Dst->Get_Rect().top + ((_Dst->Get_Rect().bottom - _Dst->Get_Rect().top) >> 1);
	int rtCX = (_Dst->Get_Rect().right - _Dst->Get_Rect().left) >> 1;
	int	rtCY = (_Dst->Get_Rect().bottom - _Dst->Get_Rect().top) >> 1;
	float fX = abs(rtX - _Src->Get_Info().fX);
	float fY = abs(rtY - _Src->Get_Info().fY);

	float fCX = (float)((rtCX + (_Src->Get_Info().iCollisionCX) >> 1));
	float fCY = (float)((rtCY + (_Src->Get_Info().iCollisionCY) >> 1));

	if (fCX > fX && fCY > fY)
	{
		*_x = fCX - fX;
		*_y = fCY - fY;
		return true;
	}
	return false;
}
