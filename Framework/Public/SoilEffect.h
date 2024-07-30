#pragma once
#include "Effect.h"
class CSoilEffect :
    public CEffect
{
public:
    CSoilEffect();
    virtual ~CSoilEffect();

public:
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;

private:
    float   m_fTime;

};

