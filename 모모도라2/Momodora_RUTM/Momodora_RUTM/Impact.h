#pragma once
#ifndef __IMPACT_H_
#define __IMPACT_H_

#include "Obj.h"
class CImpact :
	public CObj
{
public:
	CImpact();
	virtual ~CImpact();

private:
	void Frame_Move();
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__IMPACT_H_
