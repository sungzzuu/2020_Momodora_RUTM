#pragma once
#ifndef __BIGPLANT_H__
#define __BIGPLANT_H__

#include "Obj.h"
class CBigPlant :
	public CObj
{
public:
	CBigPlant();
	virtual ~CBigPlant();
public:
	enum STATE
	{
		WALK, ATTACK
	};
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
};

#endif // !__BIGPLANT_H__
