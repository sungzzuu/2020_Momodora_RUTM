#pragma once

#ifndef __ARROW_H__
#define __ARROW_H__

#include "Obj.h"
class CArrow :
	public CObj
{
public:
	CArrow();
	virtual ~CArrow();
	enum STATE
	{
		LIVE, WALL
	};
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

	STATE	 m_eCurState;
};

#endif // !__ARROW_H__
