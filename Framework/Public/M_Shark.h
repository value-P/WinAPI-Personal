#pragma once
#include "Skill.h"
class CM_Shark :
    public CSkill
{
public:
    CM_Shark();
    virtual ~CM_Shark();

public:
    // CSkill��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;

    // CSkill��(��) ���� ��ӵ�
    SKILL_TYPE Get_SkillType() override;
private:
    float   m_fTime;       // �ð� �帧 üũ

};

