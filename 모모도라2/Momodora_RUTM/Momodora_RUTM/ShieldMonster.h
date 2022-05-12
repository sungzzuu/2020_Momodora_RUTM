#pragma once
#ifndef __SHIELDMONSTER_H__
#define __SHIELDMONSTER_H__

#include "Obj.h"
class CShieldMonster :
	public CObj
{
public:
	CShieldMonster();
	virtual ~CShieldMonster();

public:
	enum STATE
	{
		SHIELDILE, SHIELDHURT, SHIELDWALK, SHIELDTHROW

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

	STATE			m_eCurState;
	STATE			m_ePreState;

	DWORD			m_dwAttack;
	DWORD			m_dwTurnTime;

	MONSTER::TYPE	m_eType;

};

#endif // !__SHIELDMONSTER_H__
