#pragma once
#include "Obj.h"


class CPlayer :
    public CObj
{
public:
    CPlayer();
    virtual ~CPlayer();

public:
    // CObj��(��) ���� ��ӵ�
    virtual void    Initialize()    override;
    virtual int     Update()        override;
    virtual void    Late_Update()   override;
    virtual void    Render(HDC hDC) override;
    virtual void    Release()       override;

public:
    virtual void    Debug_Render(HDC hDC)   override;

protected:
    virtual void    Anim_Update()           override;
    
public:
    void            Set_Inven(CUI* _inven)                      { m_pInven = _inven; }
    PlayerSkill     Get_SkillTypeInKey(PlayerSkillBtn _btnType) { return m_eSkill[_btnType]; }
    void            Change_SkillIdx(PlayerSkillBtn _Dst, PlayerSkillBtn _Src);
    void            Add_NewSkill(PlayerSkill _eAddSkillType);

protected:
    void    Update_LookDir();

    void    KeyInput();
    void    SetFrameByState();
    
    void    Play_Skill(PlayerSkillBtn _btn);
    void    Skill_OnMouse(POINT _mousePt, PlayerSkill _skill);
    void    Skill_NoMouse(PlayerSkill _skill);

    void    CheckFallTrap();
    // �Է¹��� ��ǥ���� �ڽ� �ֺ��� Ÿ���� ��ȸ�Ͽ� �浹�� true��ȯ
    bool    CheckTile(float fX, float fY,TILE_ID& _returnID);

    // eSkill�� Ư�� �ε����� ���ϴ� ��ų�� �־��ִ� �Լ�
    void    Set_Skill(int _slotIdx, PlayerSkill _skill);

    virtual void	Get_Damage(float _dmg, CObj* _pSrc);


protected:
    bool        m_bHitted;      // �¾Ҵ���
    float       m_fHittedTime;  // ���� �� ���ʰ� ��������

    int         m_iAttackStack; // ���ݸ�ǽ� 3�������� ���� ����
    
    // �޸��� ����
    float       m_fDashSpeed;
    DWORD       m_dwRunTime;    // �̵��ð��� üũ�Ͽ� 2���̻� ���ٸ� �����ϵ���;
    bool        m_bSpeedUp;     // �̵��ӵ��� �����Ǿ�����

    // �뽬 ����
    Vector2D    m_vPrevPos;     // �뽬 ���������� ������ ���� ����
    bool        m_bOnTrap;      // Ʈ���� ���� DEAD���°� �Ǵ°��� �����Ϸ���

    // �ִϸ��̼� ����
    ANIM_STATE m_eCurState;
    ANIM_STATE m_ePrevState;

    // �κ� ����
    CUI*        m_pInven;

    PlayerSkill m_eSkill[Btn_End];

};

