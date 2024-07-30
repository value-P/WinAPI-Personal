#pragma once
#include "Skill.h"
class CFlame :
    public CSkill
{
public:
    CFlame();
    virtual ~CFlame();

public:
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;

    void Start_Shoot() { m_bOnShoot = true; }
    // CSkill��(��) ���� ��ӵ�
    SKILL_TYPE Get_SkillType() override;

private:
    void    UpdateFrame(float _angle);
    bool    m_bOnShoot;

};

