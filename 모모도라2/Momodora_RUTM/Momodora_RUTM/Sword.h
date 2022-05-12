#pragma once
#ifndef __SWORD_H__
#define __SWORD_H__

#include "Obj.h"
class CSword :
	public CObj
{
public:
	CSword();
	virtual ~CSword();
	enum STATE
	{
		WALL, LIVE
	};
	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	STATE		m_eCurState;
};

#endif // !__SWORD_H__
