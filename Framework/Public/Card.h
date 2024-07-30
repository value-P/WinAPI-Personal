#pragma once
#include "Obj.h"
class CCard :
    public CObj
{
public:
    CCard();
    virtual ~CCard();

public:
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;

public:
    void    Set_SkillType(PlayerSkill _eSkillType) { m_eSkillType = _eSkillType; }

private:
    PlayerSkill     m_eSkillType;
};

