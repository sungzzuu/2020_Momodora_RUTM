#pragma once
#ifndef __GOLD_H__
#define __GOLD_H__

#include "Obj.h"
class CGold :
	public CObj
{
public:
	CGold();
	virtual ~CGold();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Jumping();
	void Frame_Move();
private:
	float			m_fJumpPower;
	float			m_fJumpAccel;
	bool			m_bJump;
};

#endif // !__GOLD_H__
