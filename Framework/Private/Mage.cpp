#include "stdafx.h"
#include "Mage.h"
#include "BmpMgr.h"
#include "ObjMgr.h"
#include "TimeMgr.h"
#include "SoundMgr.h"
#include "DamageEffect.h"
#include "ScrollMgr.h"
#include "Flame.h"
CMage::CMage()
    :m_eCurState_Anim(MG_IDLE), m_ePrevState_Anim(MG_IDLE), m_eCurState(M_IDLE), m_fTime(0.f), m_pFlame(nullptr)
{
}

CMage::~CMage()
{
	Release();
}

void CMage::Initialize()
{
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/WIZARD_LEFT.bmp", L"WIZARD_LEFT");
    CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Monster/Normal/WIZARD_RIGHT.bmp", L"WIZARD_RIGHT");

    Set_Hp(200.f);

    m_eRenderID = GAMEOBJECT;
    m_eObjID = OBJ_MONSTER;

    // �ִϸ��̼� ���� �ʱ�ȭ
    m_tFrame.dwTime = GetTickCount();
    m_pFrameKey = L"WIZARD_RIGHT";
    m_tFrame.SetInfo(0, 0, (int)MG_IDLE, 700);
    // ���� ���Ȱ��� �ʱ�ȭ
    m_fAttack = 5.f;
    m_fSpeed = 150.f;
    // ���� ������ ���� �ʱ�ȭ
    m_tInfo.fCX = 85.f;
    m_tInfo.fCY = 115.f;

    m_vTileColOffset = Vector2D(0.f, -40.f);
    m_tTileColInfo.fCX = 32.f;
    m_tTileColInfo.fCY = 32.f;

    // �ݶ��̴� ũ��
    m_tColInfo.fCX = 35.f;
    m_tColInfo.fCY = 90.f;

    m_pTarget = GET_SINGLE(CObjMgr)->Get_Player();
}

int CMage::Update()
{
    if (m_bDead)
    {
        __super::Drop_Money();
        return OBJ_DEAD;
    }
    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    Vector2D targetDir = m_pTarget->Get_Pos() - Get_Pos();
    m_vDir = (targetDir).Normalize();

    if (m_fCurHp <= 0.f)
        m_eCurState = M_DEAD;

    if (m_eCurState != M_HIT && m_eCurState != M_ATTACK && m_eCurState != M_DEAD)
    {
        // �÷��̾���� �Ÿ��� 32 �̳��϶� ����
        if (targetDir.Magnitude() < 300.f)
            m_eCurState = M_ATTACK;
        // �� �ܿ��� ����
        else
            m_eCurState = M_MOVE;
    }

    switch (m_eCurState)
    {
    case M_IDLE:
        m_eCurState_Anim = MG_IDLE;
        break;
    case M_MOVE:
    {
        m_tInfo.fX += m_fSpeed * deltaTime * m_vDir.x;
        m_tInfo.fY += m_fSpeed * deltaTime * m_vDir.y;
        m_eCurState_Anim = MG_RUN;
    }
    break;
    case M_ATTACK:
        if (m_pFlame == nullptr)
        {
            m_pFlame = new CFlame;
            m_pFlame->Set_Owner(this);
            m_pFlame->Set_Target(GET_SINGLE(CObjMgr)->Get_Player());
            m_pFlame->Initialize();
            GET_SINGLE(CObjMgr)->Add_Object(OBJ_SKILL, m_pFlame);
        }
        m_eCurState_Anim = MG_ATTACK;

        break;
    case M_HIT:
        m_eCurState_Anim = MG_HIT;

        m_fTime += deltaTime;
        if (m_fTime > 0.5f)
        {
            CSoundMgr::Get_Instance()->Play_Sound(L"ENEMY_HITED_1.mp3", SOUND_MONSTER_HIT, 1.f);
            FRAME frameInfo;
            frameInfo.SetInfo(0, 4, 0, 100);
            CObj* pEffect = CAbstractFactory<CEffect>::Create_Obj(INFO(m_tInfo.fX, m_tInfo.fY, 115.f, 115.f), L"HITEFFECT", frameInfo);
            CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
        }
        break;
    case M_DEAD:
        m_eCurState_Anim = MG_DEAD;
        break;
    }

    __super::Update_Rect();
    __super::Update_TileColRect();
    return OBJ_NOEVENT;

}

void CMage::Late_Update()
{
    float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

    Anim_Update();

    // ���º�ȭ�� �����Ͽ� ��ȭ�� ���·� ��������ִ� �Լ�
    SetFrameByState();

    // �ִϸ��̼� �������� �ѹ������� ���� ANIM_END��ȯ
    // �� �ܿ��� �ش� ������ ��ȣ ��ȯ
    int animEnd = __super::Move_Frame();

    if (animEnd == ANIM_END)
    {
        switch (m_eCurState_Anim)
        {
        case MG_IDLE:
            break;
        case MG_RUN:
            break;
        case MG_ATTACK:
            Shot();
            m_eCurState = M_IDLE;
            break;
        case MG_HIT:
            m_eCurState = M_IDLE;
            GET_SINGLE(CSoundMgr)->Stop_Sound(SOUND_MONSTER_HIT);
            break;
        case MG_DEAD:
            Set_Dead();
            break;
        }
    }
}

void CMage::Render(HDC hDC)
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

void CMage::Release()
{
}

void CMage::Debug_Render(HDC hDC)
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

void CMage::Anim_Update()
{
}

void CMage::Get_Damage(float _dmg, CObj* _pSrc)
{
    if (m_eCurState == M_DEAD) return;

    Vector2D nuckBackDir = (Get_Pos() - _pSrc->Get_Pos()).Normalize();
    m_tInfo.fX -= nuckBackDir.x * 1.5f;
    m_tInfo.fY -= nuckBackDir.y * 1.5f;

    m_fCurHp -= _dmg;

    m_eCurState = M_HIT;
}

void CMage::Shot()
{
    m_pFlame->Start_Shoot();
    m_pFlame = nullptr;
}

void CMage::SetFrameByState()
{
    if (m_vDir.x > 0) m_pFrameKey = L"WIZARD_RIGHT";
    else m_pFrameKey = L"WIZARD_LEFT";

    // ���� ��ȭ ���ٸ� ������
    if (m_eCurState_Anim == m_ePrevState_Anim)
        return;

    // ���� ���� ����
    m_ePrevState_Anim = m_eCurState_Anim;

    switch (m_eCurState_Anim)
    {
    case MG_IDLE:
        m_tFrame.SetInfo(0, 0, (int)MG_IDLE, 200);
        break;
    case MG_RUN:
        m_tFrame.SetInfo(0, 3, (int)MG_RUN, 200);
        break;
    case MG_ATTACK:
        m_tFrame.SetInfo(0, 3, (int)MG_ATTACK, 500);
        break;
    case MG_HIT:
        m_tFrame.SetInfo(0, 1, (int)MG_HIT, 200);
        break;
    case MG_DEAD:
        m_tFrame.SetInfo(0, 3, (int)MG_DEAD, 200);
        break;
    }
}

void CMage::SummonBall()
{
}
