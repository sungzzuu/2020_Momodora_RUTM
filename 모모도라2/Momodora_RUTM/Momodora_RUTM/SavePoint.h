#pragma once
#ifndef __SAVEPOINT_H__
#define __SAVEPOINT_H__

#include "Obj.h"
class CSavePoint :
	public CObj
{
public:
	CSavePoint();
	virtual ~CSavePoint();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__SAVEPOINT_H__
