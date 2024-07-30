#pragma once
#include "Obj.h"
class CUI :
    public CObj
{
public:
    CUI();
    virtual ~CUI();

public:
    // CObj��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

public:
    void    Set_Type(UI_ID _type) { m_eType = _type; }
    void    Set_Owner(CObj* _Obj) { m_pOwner = _Obj; }
    // �ۼ�Ƽ����ŭ(0~1) UIũ�⸦ �ٿ��ִ� �Լ�(right�� �ٿ���)
    void    UI_Slider(float _fPercentage);
    // �κ��丮���� UI�� ��� ������ֱ� ����
    void    Toggle();

    bool    IsToggled() { return m_bToggle; }

protected:
    CObj*   m_pOwner;
    UI_ID   m_eType;

    // �κ��丮 UI�� ����
    bool    m_bToggle;

    // �����̴� UI�� ����
    float   m_fSliderMultiflier;
};

