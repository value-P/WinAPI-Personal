#include "stdafx.h"
#include "Boss.h"
#include "BmpMgr.h"
#include "ScrollMgr.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "CollsionMgr.h"
#include "TileMgr.h"
#include "Skill.h"
#include "StonePillar.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "Effect.h"
#include "FloatRock.h"
#include "EarthCrack.h"
#include "UIMgr.h"
#include "Wave.h"
#include "BossHpBar.h"

CBoss::CBoss()
    :m_fJumpSpeed(0.f), 
    m_eCurState_Anim(BOSS_IDLE),
    m_ePrevState_Anim(BOSS_IDLE),
    m_iHitStack(0), m_fDetectRangle(0.f), 
    m_eCurState(M_IDLE), 
    m_eCurPattern(B_P_NONE),
    m_iPillarWaveStack(0),
    m_fTime(0.f),
    m_fPatternTime(0.f),
    m_iWave(0),
    m_bOnWave(true),
    m_fDeadTime(0.f)
{
}

CBoss::~CBoss()
{
    Release();
}

void CBoss::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/BOSS_ATTACK.bmp", L"BOSS_ATTACK");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/BOSS_LEFT.bmp", L"BOSS_LEFT");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Boss/BOSS_RIGHT.bmp", L"BOSS_RIGHT");

    Set_Hp(2000.f);

    m_eRenderID = GAMEOBJECT;
    m_eObjID = OBJ_MONSTER;

    // �ִϸ��̼� ���� �ʱ�ȭ
    m_tFrame.dwTime = GetTickCount();
    m_pFrameKey = L"BOSS_RIGHT";
    m_tFrame.SetInfo(0, 0, (int)IDLE, 200);
    // ���� ���Ȱ��� �ʱ�ȭ
    m_fAttack = 15.f;
    m_fSpeed = 8.f;
    m_fJumpSpeed = 20.f;
    m_fDetectRangle = 200.f;
    // ���� ������ ���� �ʱ�ȭ
    m_tInfo.fCX = 110.f;
    m_tInfo.fCY = 150.f;

    m_vTileColOffset = Vector2D(0.f, -32.f);
    m_tTileColInfo.fCX = 32.f;
    m_tTileColInfo.fCY = 32.f;

    // �ݶ��̴� ũ��
    m_tColInfo.fCX = 100.f;
    m_tColInfo.fCY = 130.f;

}

int CBoss::Update()
{
    if (m_bDead) return OBJ_DEAD;

    // Ÿ�� ã�������� �ƹ��͵� �������� �ʵ���
    if (!m_pTarget)
    {
        Detect_Target();
        __super::Update_Rect();
        __super::Update_TileColRect();
        return OBJ_NOEVENT;
    }             

    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    switch (m_eCurState)
    {
    case M_IDLE:
        m_eCurState_Anim = BOSS_IDLE;
        m_fPatternTime += deltaTime;
        if (m_fPatternTime > 0.5f)
        {
            m_fPatternTime = 0.f;
            m_eCurState = M_SKILL;
        }
        break;
    case M_SKILL:
        Pattern();
        break;
    case M_HIT:
        if (m_iHitStack > 20)
        {
            m_iHitStack = 0;
            m_eCurState = M_SKILL;
            Pattern();
        }
        else
            m_eCurState_Anim = BOSS_HIT;
        break;
    case M_DEAD:
        m_eCurState_Anim = BOSS_DEAD;

        m_fDeadTime += deltaTime;
        if (m_fDeadTime > 4.f)
            Set_Dead();

        break;
    }

    __super::Update_Rect();
    __super::Update_TileColRect();
    return OBJ_NOEVENT;
}

void CBoss::Late_Update()
{
    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    Anim_Update();

    // ���º�ȭ�� �����Ͽ� ��ȭ�� ���·� ��������ִ� �Լ�
    SetFrameByState();

    if (m_fCurHp <= 0.f) m_eCurState = M_DEAD;

    // �ִϸ��̼� �������� �ѹ������� ���� ANIM_END��ȯ
    // �� �ܿ��� �ش� ������ ��ȣ ��ȯ
    int animEnd = __super::Move_Frame();

    if (animEnd == ANIM_END)
    {
        if (m_eCurState_Anim == BOSS_HIT)
            m_eCurState = M_IDLE;
        else if (m_eCurState_Anim == BOSS_DEAD)
        {
            if (!m_tFrame.stopFrame)
            {
                m_tFrame.Stop_Frame();
                m_tFrame.iFrameStart = m_tFrame.iFrameEnd;
            }
        }
    }
}

void CBoss::Render(HDC hDC)
{
    float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HDC hMemDC = CBmpMgr::Get_Instance()->Find_Img(m_pFrameKey);

    GdiTransparentBlt(hDC,
        m_tRect.left + (long)fScrollX,
        m_tRect.top + (long)fScrollY,
        (int)m_tInfo.fCX,
        (int)m_tInfo.fCY,
        hMemDC,
        m_tFrame.iFrameStart * (int)m_tInfo.fCX,
        m_tFrame.iMotion * (int)m_tInfo.fCY,
        (int)m_tInfo.fCX,
        (int)m_tInfo.fCY,
        RGB(255, 0, 255));
}

void CBoss::Release()
{
}

void CBoss::Debug_Render(HDC hDC)
{
    float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
    float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

    HPEN hNewPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hDC, hNewPen);

    // �Ϲ� �ݶ��̴�
    MoveToEx(hDC, m_tColRect.left + fScrollX, m_tColRect.top + fScrollY, nullptr);
    LineTo(hDC, m_tColRect.right + fScrollX, m_tColRect.top + fScrollY);
    LineTo(hDC, m_tColRect.right + fScrollX, m_tColRect.bottom + fScrollY);
    LineTo(hDC, m_tColRect.left + fScrollX, m_tColRect.bottom + fScrollY);
    LineTo(hDC, m_tColRect.left + fScrollX, m_tColRect.top + fScrollY);

    SelectObject(hDC, hOldPen); // ������ ����ϴ���(Newpen)�� ������
    DeleteObject(hNewPen);

    // �� �缳��
    hNewPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
    hOldPen = (HPEN)SelectObject(hDC, hNewPen);
    // Ÿ�� �ݶ��̴�
    MoveToEx(hDC, m_tTileColRect.left + fScrollX, m_tTileColRect.top + fScrollY, nullptr);
    LineTo(hDC, m_tTileColRect.right + fScrollX, m_tTileColRect.top + fScrollY);
    LineTo(hDC, m_tTileColRect.right + fScrollX, m_tTileColRect.bottom + fScrollY);
    LineTo(hDC, m_tTileColRect.left + fScrollX, m_tTileColRect.bottom + fScrollY);
    LineTo(hDC, m_tTileColRect.left + fScrollX, m_tTileColRect.top + fScrollY);


    SelectObject(hDC, hOldPen); // ������ ����ϴ���(Newpen)�� ������
    DeleteObject(hNewPen);
}

void CBoss::Anim_Update()
{
}

void CBoss::Get_Damage(float _dmg, CObj* _pSrc)
{
    m_fCurHp -= _dmg;

    // �����߿��� ���� �ɸ��� �ʵ���
    if (m_eCurState == M_SKILL)
        return;
    
    m_iHitStack++;

    // �ǰ� ����Ʈ ����
    Vector2D hitDir = _pSrc->Get_Pos() - Get_Pos();
    float	 Angle = hitDir.ToAngle_Rad();

    FRAME frameInfo;

    if (m_fAngle >= 7 * PI / 4 || m_fAngle < PI / 4)
    {
        m_pFrameKey = L"RIGHT";
        frameInfo.SetInfo(0, 4, 0, 300);
    }
    else if (m_fAngle >= PI / 4 && m_fAngle < 3 * PI / 4)
    {
        m_pFrameKey = L"BACK";
        frameInfo.SetInfo(0, 4, 1, 300);
    }
    else if (m_fAngle >= 3 * PI / 4 && m_fAngle < 5 * PI / 4)
    {
        m_pFrameKey = L"LEFT";
        frameInfo.SetInfo(0, 4, 2, 300);
    }
    else if (m_fAngle >= 5 * PI / 4 && m_fAngle < 7 * PI / 4)
    {
        m_pFrameKey = L"FRONT";
        frameInfo.SetInfo(0, 4, 3, 300);
    }

    CSoundMgr::Get_Instance()->Play_Sound(L"BOSS_WIND_HIT_1.mp3", SOUND_MONSTER_HIT, 1.f);
    CObj* pEffect = CAbstractFactory<CEffect>::Create_Obj(INFO(m_tInfo.fX, m_tInfo.fY, 115.f, 115.f), L"HITEFFECT", frameInfo);
    CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);


    if (_pSrc->Get_Pos().x > m_tInfo.fX)
        m_pFrameKey = L"BOSS_RIGHT";
    else
        m_pFrameKey = L"BOSS_LEFT";

    // ��Ʈ������ 20�̻��� �Ǹ� ��ų�� ������ �ߵ���Ű�� ���� ���� �ʱ�ȭ��Ű��

    if(m_eCurState == M_IDLE)
        m_eCurState = M_HIT;
}

bool CBoss::CheckTile(float fX, float fY)
{
    list<CObj*> tempTileList;

    float tempX = fX;
    float tempY = fY;

    INFO tempTileColInfo{ tempX,tempY,m_tTileColInfo.fCX,m_tTileColInfo.fCY };

    int startX =    (tempX - 16.f) / TILECX;
    int startY =    (tempY - 16.f) / TILECY;
    int endX =      (tempX + 16.f) / TILECX;
    int endY =      (tempY + 16.f) / TILECY;

    CTileMgr::Get_Instance()->Get_Range_TileList(startX, startY, endX, endY, &tempTileList);

    for (auto& iter : tempTileList)
    {
        TILE_ID eType = dynamic_cast<CTile*>(iter)->Get_TileType();

        float fX, fY;

        if (eType == TILE_WALL)
        {
            if (CCollsionMgr::Check_Rect(iter->Get_Info(), tempTileColInfo, &fX, &fY))
            {
                return true;
            }
        }
    }

    return false;

}

void CBoss::Pattern()
{
    if (m_eCurPattern == B_P_NONE)
    {
        int randNum = rand() % (BOSS_P_END);
        m_eCurPattern = (BOSS_PATTERN)randNum;
        //m_eCurPattern = BOSS_3;
    }
    else
    {
        switch (m_eCurPattern)
        {
        case BOSS_1:
        {
            int Result = Pattern_first();
            if (Result == PATTERN_END)
            {
                m_eCurState = M_IDLE;
                m_eCurPattern = B_P_NONE;
            }
        }
            break;
        case BOSS_2:
        {
            int Result = Pattern_second();
            if (Result == PATTERN_END)
            {
                m_pFrameKey = L"BOSS_LEFT";
                m_eCurState = M_IDLE;
                m_eCurState_Anim = BOSS_IDLE;
                m_eCurPattern = B_P_NONE;
            }
        }
            break;
        case BOSS_3:
            int Result = Pattern_third();
            if (Result == PATTERN_END)
            {
                m_eCurState = M_IDLE;
                m_eCurPattern = B_P_NONE;
            }
            break;
        }
    }
}

int CBoss::Pattern_first()
{
    // �ִϸ��̼� ����
    if (m_eCurState_Anim != BOSS_SKILL2)
    {
        m_eCurState_Anim = BOSS_SKILL2;
    }
    else
    {
        // �ִ� �������� �ȸ����� �ִٸ�
        if (!m_tFrame.stopFrame)
        {
            // ������ �����ӿ� ����
            int frameCur = m_tFrame.Get_CurFrame();
            if (frameCur >= m_tFrame.iFrameEnd)
            {
                m_tFrame.Stop_Frame();
            }
        }
        // ���� ����
        else
        {
            Vector2D targetDir = (m_pTarget->Get_TileColPos() - this->Get_Pos()).Normalize();

            if (m_iPillarWaveStack < 2)
            {
                float Angle1 = targetDir.ToAngle_Rad() + PI/4;
                float Angle2 = targetDir.ToAngle_Rad() - PI/4;

                // ĳ���� ���Ⱒ + 45�� -45����ŭ �������
                Vector2D TargetPos1(m_tInfo.fX + 10.f * cosf(Angle1),m_tInfo.fY + 10.f *sinf(Angle1));
                Vector2D TargetPos2(m_tInfo.fX + 10.f * cosf(Angle2), m_tInfo.fY + 10.f * sinf(Angle2));

                Vector2D TargetDir1 = (TargetPos1 - this->Get_Pos()).Normalize();
                Vector2D TargetDir2 = (TargetPos2 - this->Get_Pos()).Normalize();

                // ����� 2�� ����(10�� ����)
                CObj* pPillar1 = CAbstractFactory<CStonePillar>::Create_Obj(TargetDir1, this, 6.f);
                CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pPillar1);
                CObj* pPillar2 = CAbstractFactory<CStonePillar>::Create_Obj(TargetDir2,this, 6.f);
                CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pPillar2);

                m_iPillarWaveStack += 2;
            }
            else
            {
                // ���� Ƚ�� �̻� ����� ���������� ������
                // ����ȭ Ȥ�� �Ϲ� ���� Ż�� ����
                if ((Get_HpRate() < 0.3f && m_iPillarWaveStack >= 7 ) ||
                    (Get_HpRate() >= 0.3f && m_iPillarWaveStack >= 5))
                {
                        // ������ �ʱ�ȭ
                        m_fTime = 0.f;
                        m_iPillarWaveStack = 0;
                        // ���� �ʱ�ȭ
                        m_eCurState = M_IDLE;
                        // �ִϸ��̼� ������ �ٽõ��ư����� ����
                        m_tFrame.Resume_Frame();
                        return PATTERN_END;
                }

                // �÷��̾� �������� ����� ����(1�ʵ�����)
                m_fTime += CTimeMgr::Get_Instance()->Get_fDT();

                if (m_fTime > 1.f)
                {
                    m_fTime = 0.f;
                    CObj* pPillar = CAbstractFactory<CStonePillar>::Create_Obj(targetDir*Vector2D(1,-1), this, 2.f);
                    CObjMgr::Get_Instance()->Add_Object(OBJ_SKILL, pPillar);

                    m_iPillarWaveStack++;
                }

            }
        }
    }

    return PATTERN_ING;
}

int CBoss::Pattern_second()
{
    if (!m_tFrame.stopFrame)
    {
        if (m_pFrameKey == L"BOSS_ATTACK")
        {
            // ������ �����ӿ� ����
            int frameCur = m_tFrame.Get_CurFrame();
            if (frameCur >= m_tFrame.iFrameEnd)
            {
                m_tFrame.Stop_Frame();

                // �� �� �������� ����
                if (m_floatRockList.empty())
                {
                    m_tFrame.Resume_Frame();
                    return PATTERN_END;
                }
                else
                {
                    GET_SINGLE(CSoundMgr)->Play_Sound_NoInterval(L"ROCK_THROW_START.mp3", SOUND_EFFECT_2, 1.f);
                    // �� ������ ����Ʈ���� ����
                    auto iter = m_floatRockList.begin();
                    static_cast<CFloatRock*>(*iter)->Start_Throw();
                    m_floatRockList.erase(iter);
                }
            }
        }
        else
        {
            // �ִϸ��̼� ����
            if (m_eCurState_Anim != BOSS_SKILL2)
            {
                m_eCurState_Anim = BOSS_SKILL2;
            }
            else
            {
                // ������ �����ӿ� ����
                int frameCur = m_tFrame.Get_CurFrame();
                if (frameCur >= m_tFrame.iFrameEnd)
                {
                    m_tFrame.Stop_Frame();

                    // �� ���� ( ����ȭ�� _�� , �⺻ 3�� )
                    if (Get_HpRate() < 0.3f)
                    {
                        //TODO 
                        GET_SINGLE(CSoundMgr)->Play_Sound_NoInterval(L"ROCK_THROW_1.mp3", SOUND_EFFECT_1, 1.f);
                        for (int i = 0; i < 6; ++i)
                        {
                            float Angle = 60 * i;
                            if (Angle < 0) Angle = 360 + Angle;
                            CFloatRock* pRock = new CFloatRock;
                            pRock->Set_Angle(Angle);
                            pRock->Set_Owner(this);
                            pRock->Set_Attack(m_fAttack);
                            pRock->Initialize();
                            // ���� �����ϱ� ���� ����Ʈ�� �ֱ�
                            m_floatRockList.push_back(pRock);
                            // ������ ������Ʈ�� ObjMgr����
                            GET_SINGLE(CObjMgr)->Add_Object(OBJ_SKILL, pRock);
                        }
                    }
                    else
                    {
                        GET_SINGLE(CSoundMgr)->Play_Sound_NoInterval(L"ROCK_THROW_1.mp3", SOUND_EFFECT_1, 1.f);
                        for (int i = 0; i < 3; ++i)
                        {
                            float Angle = 120 * i - 30;
                            if (Angle < 0) Angle = 360 + Angle;
                            CFloatRock* pRock = new CFloatRock;
                            pRock->Set_Angle(Angle);
                            pRock->Set_Owner(this);
                            pRock->Set_Attack(m_fAttack);
                            pRock->Initialize();
                            // ���� �����ϱ� ���� ����Ʈ�� �ֱ�
                            m_floatRockList.push_back(pRock);
                            // ������ ������Ʈ�� ObjMgr����
                            GET_SINGLE(CObjMgr)->Add_Object(OBJ_SKILL, pRock);
                        }
                    }

                }
            }
        }
    }
    // ���� ����
    else
    {
        if (m_pFrameKey == L"BOSS_ATTACK")
        {
            m_fTime += GET_SINGLE(CTimeMgr)->Get_fDT();
            if (m_fTime > 0.4f)
            {
                m_fTime = 0.f;

                float targetAngle = Get_TargetAngle();

                // �÷��̾� ���⿡ ���� �ִϸ��̼� ����
                if (targetAngle < 45.f || targetAngle >= 315.f)
                    m_tFrame.SetInfo(0, 5, 2, 100);
                else if (targetAngle >= 45.f && targetAngle < 135.f)
                    m_tFrame.SetInfo(0, 5, 1, 100);
                else if (targetAngle >= 135.f && targetAngle < 225.f)
                    m_tFrame.SetInfo(0, 5, 0, 100);
                else if (targetAngle >= 225.f && targetAngle < 315.f)
                    m_tFrame.SetInfo(0, 5, 3, 100);

                // ������ �ٽ� ���
                m_tFrame.Resume_Frame();
            }

        }
        else if (m_eCurState_Anim == BOSS_SKILL2)
        {
            m_fTime += GET_SINGLE(CTimeMgr)->Get_fDT();
            if (m_fTime > 2.f)
            {
                m_fTime = 0.f;

                m_pFrameKey = L"BOSS_ATTACK";

            }
        }
    }

    return PATTERN_ING;
}

int CBoss::Pattern_third()
{
    // �ִϸ��̼� ����
    if (m_eCurState_Anim != BOSS_JUMPATK)
    {
        m_eCurState_Anim = BOSS_JUMPATK;
        m_vJumpStartPos = Get_Pos();
    }
    else
    {
        float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

        // �ִ� �������� �ȸ����� �ִٸ�
        if (!m_tFrame.stopFrame)
        {
            // ������ �����ӿ� ����
            int frameCur = m_tFrame.Get_CurFrame();
            switch (frameCur)
            {
            case 1:
                m_tFrame.Stop_Frame();
                break;
            case 2:
                m_tFrame.Stop_Frame();
                break;
            case 6:
                m_tFrame.Stop_Frame();
                break;
            case 7:
                return PATTERN_END;
            }
        }
        // ���� ����
        else
        {
            int frameCur = m_tFrame.Get_CurFrame();
            Vector2D targetDir = (m_pTarget->Get_TileColPos() - Get_TileColPos()).Normalize();

            switch (frameCur)
            {
            case 1:
                m_tInfo.fY -= 300.f * deltaTime;

                if (m_tInfo.fY <= m_vJumpStartPos.y - 300)
                    m_tFrame.Resume_Frame();
                break;
            case 2:
                m_tInfo.fY += 400.f * deltaTime;

                m_tInfo.fX += 400.f * deltaTime * targetDir.x;

                if (m_tInfo.fY >= m_pTarget->Get_TileColPos().y)
                {
                    m_tInfo.fY = m_pTarget->Get_TileColPos().y;
                    m_tFrame.Resume_Frame();
                }
                break;
            case 6:
                m_fTime += deltaTime;
                if (m_fTime > 0.5f)
                {
                    m_fTime = 0.f;
                    ++m_iWave;
                    m_bOnWave = true;
                }
                // �������� , ���̺꺰�� ���� ���������� ������ �ı����� ����
                if (m_bOnWave)
                {
                    switch (m_iWave)
                    {
                    case 0:
                    {
                        GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_EFFECT_3);
                        GET_SINGLE(CSoundMgr)->Play_Sound(L"BOSS_JUMP.mp3", SOUND_EFFECT_3, 1.f);
                        for (int i = 0; i < 8; ++i)
                        {
                            float tempAngle = (360.f / 8.f) * i;
                            CWave* wave = new CWave;
                            wave->Set_Pos(m_tTileColInfo.fX + 60 * cosf(tempAngle * RAD2DEG),
                                m_tTileColInfo.fY - 60 * sinf(tempAngle * RAD2DEG) - 48.f);
                            wave->Set_Owner(this);
                            wave->Set_Target(m_pTarget);
                            wave->Initialize();
                            GET_SINGLE(CObjMgr)->Add_Object(OBJ_SKILL, wave);
                        }
                        m_bOnWave = false;
                    }
                    break;
                    case 1:
                    {
                        GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_EFFECT_3);
                        GET_SINGLE(CSoundMgr)->Play_Sound(L"BOSS_JUMP.mp3", SOUND_EFFECT_3, 1.f);
                        for (int i = 0; i < 12; ++i)
                        {
                            float tempAngle = (360.f / 12.f) * i;
                            CWave* wave = new CWave;
                            wave->Set_Pos(m_tTileColInfo.fX + 120 * cosf(tempAngle * RAD2DEG),
                                m_tTileColInfo.fY - 120 * sinf(tempAngle * RAD2DEG) - 48.f);
                            wave->Set_Owner(this);
                            wave->Set_Target(m_pTarget);
                            wave->Initialize();
                            GET_SINGLE(CObjMgr)->Add_Object(OBJ_SKILL, wave);
                        }
                        m_bOnWave = false;
                    }
                    break;
                    case 2:
                    {
                        GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_EFFECT_3);
                        GET_SINGLE(CSoundMgr)->Play_Sound(L"BOSS_JUMP.mp3", SOUND_EFFECT_3, 1.f);
                        for (int i = 0; i < 16; ++i)
                        {
                            float tempAngle = (360.f / 16.f) * i;
                            CWave* wave = new CWave;
                            wave->Set_Pos(m_tTileColInfo.fX + 180 * cosf(tempAngle * RAD2DEG),
                                m_tTileColInfo.fY - 180 * sinf(tempAngle * RAD2DEG) - 48.f);
                            wave->Set_Owner(this);
                            wave->Set_Target(m_pTarget);
                            wave->Initialize();
                            GET_SINGLE(CObjMgr)->Add_Object(OBJ_SKILL, wave);
                        }
                        m_bOnWave = false;
                    }
                    break;
                    default:
                    {
                        m_tFrame.Resume_Frame();
                        m_iWave = 0;
                        m_bOnWave = true;
                    }
                    }

                }
            }

        }
    }

    return PATTERN_ING;

}

void CBoss::SetFrameByState()
{
    if (m_pFrameKey == L"BOSS_ATTACK")
        return;

    // ���� ��ȭ ���ٸ� ������
    if (m_eCurState_Anim == m_ePrevState_Anim)
        return;

    // ���� ���� ����
    m_ePrevState_Anim = m_eCurState_Anim;

    switch (m_eCurState_Anim)
    {
    case BOSS_IDLE:
        m_tFrame.SetInfo(0, 0, (int)BOSS_IDLE, 200);
        break;
    case BOSS_SKILL1:
        m_tFrame.SetInfo(0, 8, (int)BOSS_SKILL1, 30);
        break;
    case BOSS_SKILL2:
        m_tFrame.SetInfo(0, 1, (int)BOSS_SKILL2, 1000);
        break;
    case BOSS_JUMPATK:
        m_tFrame.SetInfo(0, 7, (int)BOSS_JUMPATK, 10);
        break;
    case BOSS_HIT:
        m_tFrame.SetInfo(0, 1, (int)BOSS_HIT, 200);
        break;
    case BOSS_DEAD:
        m_tFrame.SetInfo(0, 0, (int)BOSS_DEAD, 1000);
        break;
    }
}

void CBoss::Detect_Target()
{
    // �̹� ������ ���� ������ ���� ����
    if (m_pTarget) return;

    m_pTarget = CObjMgr::Get_Instance()->Get_NearestTarget(this, OBJ_PLAYER, m_fDetectRangle);

    if (m_pTarget)
    {
        CBossHpBar* pUI = new CBossHpBar();
        pUI->Set_Owner(this);
        pUI->Set_FrameName(L"BOSS_NAMEBAR");
        pUI->Set_Pos(WINCX * 0.5f, 55.f);
        pUI->Initialize();
        GET_SINGLE(CUIMgr)->Add_UI(UI_NORMAL, pUI);

    }
}

float CBoss::Get_TargetAngle()
{
    Vector2D dir = (m_pTarget->Get_Pos() - Get_Pos()).Normalize();

    float tempAngle = dir.ToAngle_Rad();

    return tempAngle * RAD2DEG;
}
 