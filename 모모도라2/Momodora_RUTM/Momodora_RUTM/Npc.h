#pragma once
#ifndef __NPC_H__
#define __NPC_H__

#include "Obj.h"
class CNpc :
	public CObj
{
public:
	CNpc();
	virtual ~CNpc();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
};

#endif // !__NPC_H__
