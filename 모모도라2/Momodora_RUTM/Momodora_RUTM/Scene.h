#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

class CScene
{
public:
	CScene();
	virtual ~CScene();

public:
	virtual void Initialize()=0;
	virtual void Update() = 0;
	virtual void Late_Update() = 0;
	virtual void Render(HDC _DC) = 0;
	virtual void Release() = 0;

public:
	void Set_SlotNum(int _SlotNum) { m_iSlotNum = _SlotNum; }
	int	Get_SlotNum() { return m_iSlotNum; }
protected:
	int				m_iSlotNum;

};

#endif // !__SCENE_H__
