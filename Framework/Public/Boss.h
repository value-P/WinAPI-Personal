#pragma once
#include "Obj.h"

class CFloatRock;

class CBoss :
    public CObj
{
public:
    CBoss();
    virtual ~CBoss();

public: 
    // CObj��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;
    virtual void Debug_Render(HDC hDC) override;
    virtual void Anim_Update() override;

public:
    virtual void Get_Damage(float _dmg, CObj* _pSrc) override;

    // �Է¹��� ��ǥ���� �ڽ� �ֺ��� Ÿ���� ��ȸ�Ͽ� �浹�� true��ȯ
    bool    CheckTile(float fX, float fY);

private:
    void    Pattern();
    int     Pattern_first();
    int     Pattern_second();
    int     Pattern_third();

private:
    void    SetFrameByState();
    void    Detect_Target();

    float   Get_TargetAngle();

private:
    // ���� ����
    float   m_fJumpSpeed;
    float   m_fDetectRangle; // �÷��̾ �����ϴ� ����
    
    // ���� �ִϸ��̼� ���� 
    //1.�⺻ 2.��ų1 3.��ų2(����1,2����) 4.�������� 5.�ǰ� 6.���
    BOSS_ANIM_STATE  m_eCurState_Anim;
    BOSS_ANIM_STATE  m_ePrevState_Anim;
    MONSTER_STATE    m_eCurState;
    BOSS_PATTERN     m_eCurPattern;

    // ���� �������� Ÿ����ġ
    Vector2D    m_vJumpStartPos;

    // Ÿ���� ���ӵǸ� ������ ������ ����ϵ��� ��Ʈ���� ����
    int         m_iHitStack;

    // ����� ���̺� ����ߴ°� Ȯ�ο�
    int         m_iPillarWaveStack;

    float       m_fTime;
    float       m_fPatternTime;

    int         m_iWave;
    bool        m_bOnWave;

    list<CFloatRock*> m_floatRockList;

    float       m_fDeadTime;
};

