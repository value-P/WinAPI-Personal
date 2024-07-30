#pragma once
#include "Obj.h"
class CEffect :
    public CObj
{
public:
    CEffect();
    virtual ~CEffect();
    
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

