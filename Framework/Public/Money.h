#pragma once
#include "Obj.h"
class CMoney :
    public CObj
{
public:
    CMoney();
    virtual ~CMoney();

public:
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;
};

