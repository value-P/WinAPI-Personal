#pragma once
#include "Skill.h"
class CStonePillar :
    public CSkill
{
public:
    CStonePillar();
    virtual ~CStonePillar();

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

private:
    list<CObj*> m_pPillarList;
    bool        m_bWallCollisioned; // ����� ������ ������ �׸� ������Ű�� ��յ��� �ִϸ��̼��� ����ϵ���
    float       m_fTime;
    Vector2D    m_vStartPos;

};

