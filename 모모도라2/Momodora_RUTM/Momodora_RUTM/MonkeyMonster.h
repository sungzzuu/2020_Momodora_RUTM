#pragma once
#ifndef __MONKETMONSTER_H__
#define __MONKETMONSTER_H__

#include "Obj.h"
class CMonkeyMonster :
	public CObj
{
public:
	CMonkeyMonster();
	virtual ~CMonkeyMonster();

public:
	enum STATE
	{
		MOVE, HIT, ATTACK,EFFECTON,EFFECTOFF

	};

public:
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Scene_Change();
	void Frame_Move();
private:
	DWORD		m_dwAttack;
	STATE		m_eCurState;
	STATE		m_ePreState;
	bool		m_bRight;
	int			m_iAttackRandom;
	int			m_iLeafNum;
	int			m_iAttackTimeRan;
	MONSTER::TYPE	m_eType;

	STATE		m_EffectState;
	FRAME		m_tEffect;
};

#endif // !__MONKETMONSTER_H__
