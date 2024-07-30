#pragma once
#include "Obj.h"

class CSkill : public CObj
{
public:
	CSkill();
	virtual ~CSkill();

public:
	// CObj��(��) ���� ��ӵ�
	virtual void Initialize()		PURE;
	virtual int Update()			PURE;
	virtual void Late_Update()		PURE;
	virtual void Render(HDC hDC)	PURE;
	virtual void Release()			PURE;
	virtual void Debug_Render(HDC hDC) PURE;
	virtual void Anim_Update() PURE;

	virtual SKILL_TYPE Get_SkillType() PURE;


public:
	virtual void InitFrameOption() {}
	void		Set_Owner(CObj* _pOwner) { m_pOwner = _pOwner; }
	float		Get_CoolTime() { return m_fCoolTime; }
	void		Set_Combo(int _curCombo) { m_iCurCombo = _curCombo; }
	void		Set_Dir(Vector2D _dir) { m_vDir = _dir; }
	void		Set_StartPos(Vector2D _startPos) { m_vStartPos = _startPos; }
	void		Set_Duration(float _duration) { m_fDuration = _duration; }

protected:
	CObj*		m_pOwner;

	int			m_iCurCombo;  // �ش� ��ų�� ���޺�¥������

	float		m_fCoolTime;
	PlayerSkill	m_eSkillType;
	float		m_fOffset;		// ������ �󸶸�ŭ �������� ��������
	float		m_fDuration;	// �����ð�

	bool		m_bSkillEnded; // �������ӿ��� ������ �ִϸ��̼��� ����Ǿ��°�

	list<CObj*> m_TargetList;

	Vector2D	m_vStartPos; // ���� ��ġ�� �ʿ��Ҷ� ����� ����
};

