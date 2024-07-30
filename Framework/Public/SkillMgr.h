#pragma once
#include "Skill.h"

class CSkillMgr
{
private:
	CSkillMgr();
	~CSkillMgr();

public:
	static CSkillMgr* Get_Instance()
	{
		if (!m_pInstance)
			m_pInstance = new CSkillMgr;

		return m_pInstance;
	}
	static void	Destroy_Instance()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

public:
	void	Initialize();
	void	Update();
	void	Render(HDC hDC);
	void	Release();

public:
	bool	Skill(PlayerSkill _skillType, CObj* _Owner);
	bool	Skill(PlayerSkill _skillType, CObj* _Owner, float _angle);

	float	Get_CoolTimeRate(PlayerSkill _skillType);

private:
	void	CoolUpdate();
	void	CoolMaxUpdate();

private:
	static CSkillMgr* m_pInstance;

private:
	// �� ��ų���� ��Ÿ���� ������ ���� ����
	float	m_fSkillCoolDown[SKILL_END] = { 0.f, };
	float	m_fSkiilCoolMax[SKILL_END] = { 0.f, };
	// �� ��ų�� �޺� �ִ�ġ ���� ����
	int		m_iMaxComboArr[SKILL_END] = { 0, };
	// Ư�� ������ ��ų(�ҿ�)�� ���� ����
	int		m_iSkillStack[SKILL_END] = { 0, };

private:
	//�ƽſ�
	float	m_fCutSceneTime;
	bool	m_bAwakened;
	bool	m_bIce;
};

