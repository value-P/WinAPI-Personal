#pragma once
#include "Obj.h"

class MiddleBoss :
    public CObj
{
    public:
        MiddleBoss();
        virtual ~MiddleBoss();

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
        float   m_fDetectRangle; // �÷��̾ �����ϴ� ����

        // ���� �ִϸ��̼� ���� 
        //1.�⺻ 2.��ų1 3.��ų2(����1,2����) 4.�������� 5.�ǰ� 6.���
        BOSS_ANIM_STATE  m_eCurState_Anim;
        BOSS_ANIM_STATE  m_ePrevState_Anim;
        MONSTER_STATE    m_eCurState;
        BOSS_PATTERN     m_eCurPattern;

        float       m_fTime;
        float       m_fPatternTime;
        float       m_fPattern1Time;
        float       m_fPattern2Time;

        int iPattern_1_Cnt;
        int iPattern_2_Cnt;
        bool bPattern_3_Start;

        float       m_fDeadTime;
};


