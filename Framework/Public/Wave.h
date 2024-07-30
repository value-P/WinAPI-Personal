#pragma once
#include "Skill.h"
class CWave :
    public CSkill
{
public:
    CWave();
    virtual ~CWave();

public:
    // CSkill��(��) ���� ��ӵ�
    void    Initialize() override;
    int     Update() override;
    void    Late_Update() override;
    void    Render(HDC hDC) override;
    void    Release() override;
    void    Debug_Render(HDC hDC) override;
    void    Anim_Update() override;
    
    // CSkill��(��) ���� ��ӵ�
    SKILL_TYPE Get_SkillType() override;

private:
    bool    m_bAttacked;
    bool    m_bStartAnim;



};

