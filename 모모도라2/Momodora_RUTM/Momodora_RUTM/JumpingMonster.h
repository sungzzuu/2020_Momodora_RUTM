#pragma once
#ifndef __JUMPING_H__
#define __JUMPING_H__

#include "Obj.h"
class CJumpingMonster :
	public CObj
{
public:
	CJumpingMonster();
	virtual ~CJumpingMonster();
public:
	enum STATE
	{
		JUMP, FALL, HURT, THROW
	};

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Jumping();
private:
	void Scene_Change();
	void Frame_Move();
private:

	STATE			m_eCurState;
	STATE			m_ePreState;
	float			m_fJumpPower;
	float			m_fJumpAccel;
	bool			m_bJump;

	DWORD			m_dwAttack;
	DWORD			m_dwTrunTime;

	MONSTER::TYPE	m_eType;

};

#endif // !__JUMPING_H__
