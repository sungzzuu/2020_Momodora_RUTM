#pragma once
#ifndef __LEAF_H__
#define _LEAF_H__


#include "Obj.h"
class CLeaf :
	public CObj
{
public:
	CLeaf();
	virtual ~CLeaf();

public:
	enum STATE { ATTACK1, ATTACK2, ATTACK3, END };

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;
public:
	bool Get_LEAF_STATE() { if (END != m_eCurState) return true; else return false; }
	int	 Get_Leaf();
public:
	void Frame_Move();
public:
	void Set_FrameKey(TCHAR* _FrameKey) { m_pFrameKey = _FrameKey; }
public:
	void Set_State(int _state);
public:
	void Scene_Change();
private:
	STATE			m_eCurState;
	STATE			m_ePreState;


};

#endif // !__LEAF_H__
