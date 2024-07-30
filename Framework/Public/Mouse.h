#pragma once
#include "UI.h"
class CMouse :
	public CUI
{
public:
	CMouse();
	virtual ~CMouse();
public:
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual int Update() override;
	virtual void Late_Update() override;
	virtual void Render(HDC hDC) override;
	virtual void Release() override;

public:
	virtual void Debug_Render(HDC hDC) override;
	virtual void Anim_Update() override;
};