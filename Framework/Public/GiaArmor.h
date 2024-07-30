#pragma once
#include "Skill.h"
class CGiaArmor :
    public CSkill
{
public:
    CGiaArmor();
    virtual ~CGiaArmor();

public:
    // CSkill��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;
    void Debug_Render(HDC hDC) override;
    void Anim_Update() override;


private:
    void    UpdateFrame(float _angle);

private:
    bool    m_bEndFloat;   // ���߿� �� �㶧����
    float   m_fStartOffsetY;  // �ʱ� ��ġ���� �ش� ��ġ���� �ö󰡸� ���� �Ϸ�
    float   m_fTime;
    // CSkill��(��) ���� ��ӵ�
    SKILL_TYPE Get_SkillType() override;
    // �ð� �帧 üũ
};

