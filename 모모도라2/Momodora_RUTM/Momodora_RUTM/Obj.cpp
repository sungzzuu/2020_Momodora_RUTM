#include "stdafx.h"
#include "Obj.h"


CObj::CObj()
	: m_bDead(false), m_fSpeed(0.f), m_fAngle(0.f), m_fDis(0.f), m_pTarget(nullptr)
	, m_pFrameKey(L""), m_bLineOn(false), m_bAttackCol(false),m_dwCollision(false)
	, m_iObjOption(0), m_bChannelChange(false)
{
	ZeroMemory(&m_tInfo, sizeof(m_tInfo));
	ZeroMemory(&m_tRect, sizeof(m_tRect));
}

CObj::~CObj()
{
}

void CObj::Update_Rect()
{
	m_tRect.left = (LONG)(m_tInfo.fX - (m_tInfo.iCX >> 1));
	m_tRect.top = (LONG)(m_tInfo.fY - (m_tInfo.iCY >> 1));
	m_tRect.right = (LONG)(m_tInfo.fX + (m_tInfo.iCX >> 1));
	m_tRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCY >> 1));

	m_tCollisionRect.left = (LONG)(m_tInfo.fX - (m_tInfo.iCollisionCX >> 1));
	m_tCollisionRect.top = (LONG)(m_tInfo.fY - (m_tInfo.iCollisionCY >> 1));
	m_tCollisionRect.right = (LONG)(m_tInfo.fX + (m_tInfo.iCollisionCX >> 1));
	m_tCollisionRect.bottom = (LONG)(m_tInfo.fY + (m_tInfo.iCY >> 1));
}

