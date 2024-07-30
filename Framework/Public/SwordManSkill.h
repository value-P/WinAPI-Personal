#pragma once
#include "Skill.h"

class SwordManSkill :
    public CSkill
{
public:
    SwordManSkill();
    virtual ~SwordManSkill();

public:
    // CSkill��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;
    // CSkill��(��) ���� ��ӵ�
    SKILL_TYPE Get_SkillType() override;

protected:
    virtual void    InitFrameOption() override;

};

