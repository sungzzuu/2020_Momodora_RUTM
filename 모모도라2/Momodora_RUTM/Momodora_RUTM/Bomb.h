#pragma once
#ifndef __BOMB_H__
#define __BOMB_H__

#include "Obj.h"
class CBomb :
	public CObj
{
public:
	CBomb();
	virtual ~CBomb();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;


private:
	bool	m_bFirst;

};

#endif // !__BOMB_H__
