#pragma once
#ifndef __PLAYERUI_H__
#define __PLAYERUI_H__

#include "Obj.h"
class CPlayerUI :
	public CObj
{
public:
	CPlayerUI();
	virtual ~CPlayerUI();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	bool		m_bFirst;
};

#endif // !__PLAYERUI_H__
