#pragma once
#include "UI.h"
class CMiniMap :
    public CUI
{
public:
    CMiniMap();
    virtual ~CMiniMap();

public:
    // CObj��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

private:
    wstring wsRealMapKey;
    wstring wsPlayerIconKey;

};

