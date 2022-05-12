#pragma once
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Obj.h"
class CMyButton :
	public CObj
{
public:
	CMyButton();
	virtual ~CMyButton();

	// CObj을(를) 통해 상속됨
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC _DC) override;
	virtual void Release() override;

private:
	void Check_File();
private:
	int		m_iDrawID;
	bool	m_bFirst;
private:
	int		m_iCheckNum = 0;
};

#endif // !__BUTTON_H__
