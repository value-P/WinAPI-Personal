#include "stdafx.h"
#include "Player.h"
#include "BmpMgr.h"
#include "CollsionMgr.h"
#include "KeyMgr.h"
#include "ScrollMgr.h"
#include "TileMgr.h"
#include "SkillMgr.h"
#include "Effect.h"
#include "ObjMgr.h"
#include "SoundMgr.h"
#include "TimeMgr.h"

CPlayer::CPlayer()
	:m_eCurState(IDLE), m_ePrevState(IDLE), m_fDashSpeed(0.f), m_iAttackStack(0), m_dwRunTime(0)
	, m_bSpeedUp(false), m_bOnTrap(false), m_pInven(nullptr), m_bHitted(false), m_fHittedTime(0.f)
{
	ZeroMemory(&m_vPrevPos, sizeof(Vector2D));
}

CPlayer::~CPlayer()
{
	Release();
}

void CPlayer::Initialize()
{
	m_eRenderID = GAMEOBJECT;
	m_eObjID = OBJ_PLAYER;

	// ������ ������
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/BACK_COMPLETE.bmp", L"BACK");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/FRONT_COMPLETE.bmp", L"FRONT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/LEFT_COMPLETE.bmp", L"LEFT");
	CBmpMgr::Get_Instance()->Insert_Bmp(L"../Image/Player/RIGHT_COMPLETE.bmp", L"RIGHT");

	// �ִϸ��̼� ���� �ʱ�ȭ
	m_tFrame.dwTime = GetTickCount();
	m_pFrameKey = L"FRONT";
	m_wsShadowKey = L"NORMAL_SHADOW";
	m_fShdowOffset = 32.f;

	m_tFrame.SetInfo(0, 0, (int)IDLE, 200);
	// �÷��̾� ���Ȱ��� �ʱ�ȭ
	m_fSpeed = 300.f;
	m_fDashSpeed = 600.f;
	// �÷��̾� ������ ���� �ʱ�ȭ
	m_tInfo.fCX = 90.f;
	m_tInfo.fCY = 91.f;
	m_vLookDir = { 0,-1 };

	// �ݶ��̴� ũ��
	m_vColOffset = Vector2D::Zero();
	m_tColInfo.fCX = 25.f;
	m_tColInfo.fCY = 78.f;

	// Ÿ�� �ݶ��̴� ũ�� & offset
	m_vTileColOffset = Vector2D(0.f, -23.f);
	m_tTileColInfo.fCX = 32.f;
	m_tTileColInfo.fCY = 32.f;

	// �⺻ ��ų ����
	m_eSkill[Mouse_LB] = SKILL_NORMAL;
	m_eSkill[Key_Space] = SKILL_DASH;
	m_eSkill[Mouse_RB] = SKILL_NONE;
	m_eSkill[Key_Q] = SKILL_ICE_ORBIT;
	m_eSkill[Key_E] = SKILL_EARTH;
	m_eSkill[Key_R] = SKILL_VOIDSTORM;
}

int CPlayer::Update()
{
	KeyInput();

	m_vDir = m_vDir.Normalize();

	if (m_bHitted)
	{
		m_fHittedTime += CTimeMgr::Get_Instance()->Get_fDT();
	}

	// �޸����� ���ӵǴ� �ð� üũ
	if (m_eCurState == RUN && !m_bSpeedUp)
	{
		if (m_dwRunTime + WALK_TO_RUNTIME < GetTickCount())
		{
			m_fSpeed = RUNSPEED;
			m_bSpeedUp = true;
		}
	}
	
	// �����߿� �̵� �Ұ�
	if (m_eCurState != ATTACK_1 && m_eCurState != ATTACK_2 && m_eCurState != ONHIT)
	{
		float tempfX = 0.f;
		float tempfY = 0.f;
		float deltaTime = GET_SINGLE(CTimeMgr)->Get_fDT();

		//�̵��� �뽬 �̵����� ���̸� �α� ���Ͽ�
		if (m_eCurState != DASH)
		{
			tempfX = m_tInfo.fX + m_vDir.x * m_fSpeed * deltaTime;
			tempfY = m_tInfo.fY - m_vDir.y * m_fSpeed * deltaTime;

			TILE_ID returnID = TILE_NONE;
			bool collisioned = CheckTile(tempfX, tempfY, returnID);
			// �̵������� �浹�ȴٸ� �̵����� �ʵ��� , �̵��� ��� ��ȯ ���̵� Ʈ���̿��� ������ ���� ó��X
			if (!collisioned)
			{
				m_tInfo.fX = tempfX;
				m_tInfo.fY = tempfY;

				m_vPrevPos = Vector2D(m_tInfo.fX, m_tInfo.fY);
			} 
		}
		else
		{
			tempfX = m_tInfo.fX + m_vLookDir.x * m_fDashSpeed* deltaTime;
			tempfY = m_tInfo.fY - m_vLookDir.y * m_fDashSpeed* deltaTime;

			TILE_ID returnID = TILE_NONE;
			bool collisioned = CheckTile(tempfX, tempfY, returnID);
			// �뽬�� Ʈ���� �Ѿ �� �ֱ� ������ �����浹�ϵ���
			if (!collisioned || collisioned && returnID != TILE_WALL)
			{
				m_tInfo.fX = tempfX;
				m_tInfo.fY = tempfY;
			}
		}
	}

	CScrollMgr::Get_Instance()->Set_ScrollPos(m_tInfo.fX - WINCX * 0.5f, m_tInfo.fY - WINCY * 0.5f);

	__super::Update_Rect();
	__super::Update_TileColRect();
	return OBJ_NOEVENT;
}

void CPlayer::Late_Update()
{
	// �̵��� ���ߴ��� �ٶ󺸴� ������ �����ؾ��ϱ� ������ lookPos�� �̵��� �����ϰ� �������ش�
	Update_LookDir();
	// ��� ���ǿ� ���� �ִϸ��̼� ���¼���(�̵�,idle)
	Anim_Update();
	// ���º�ȭ�� �����Ͽ� ��ȭ�� ���·� ��������ִ� �Լ�
	SetFrameByState();
	// ������ ���¿��� �ִϸ��̼� �������� ������ endFrame�� �����ϸ� 1��ȯ 
	int animEnd = __super::Move_Frame();

	// �ִϸ��̼��� ���� Ư�� ������ ������ ���µ� ����ó��
	if (animEnd == ANIM_END)
	{
		switch (m_eCurState)
		{
		case DASH:
		{
			m_eCurState = IDLE;
			m_wsShadowKey = L"NORMAL_SHADOW";
			CheckFallTrap();
		}
			break;
		case ATTACK_1:
			m_eCurState = ATTACK_2;
			break;
		case ATTACK_2:
			m_eCurState = IDLE;
			m_wsShadowKey = L"NORMAL_SHADOW";
			break;
		case ONHIT:
			m_eCurState = IDLE;
			m_wsShadowKey = L"NORMAL_SHADOW";
			break;
		case DEAD:
			if (m_bOnTrap)
			{
				m_eCurState = IDLE;
				m_bOnTrap = false;
				m_tInfo.fX = m_vPrevPos.x;
				m_tInfo.fY = m_vPrevPos.y;
				m_fCurHp -= 5.f;
				float newScrollX = (m_tInfo.fX - WINCX * 0.5f) ;
				float newScrollY = (m_tInfo.fY - WINCY * 0.5f) ;
				CScrollMgr::Get_Instance()->Set_ScrollPos(newScrollX, newScrollY);
			}
			break;
		}
	}
	else
	{
		switch (m_eCurState)
		{
		case DASH:
			if (animEnd == 1)
			{
				CSoundMgr::Get_Instance()->Stop_Sound(SOUND_EFFECT_1);
				CSoundMgr::Get_Instance()->Play_Sound(L"DASH_1.mp3", SOUND_EFFECT_1, 1.f);
			}
			break;
		case ONHIT:
			if (animEnd == 0)
			{
				CSoundMgr::Get_Instance()->Play_Sound(L"PLAYER_HITED_1.mp3", SOUND_EFFECT_2, 1.f);
			}
			break;
		}
	}
}

void CPlayer::Render(HDC hDC)
{
	float	fScrollX = CScrollMgr::Get_Instance()->Get_ScrollX();
	float	fScrollY = CScrollMgr::Get_Instance()->Get_ScrollY();

	HDC hShadowDC = CBmpMgr::Get_Instance()->Find_Img(m_wsShadowKey);

	GdiTransparentBlt(hDC,
		m_tShadowRect.left + (long)fScrollX,
		m_tShadowRect.top + (long)fScrollY,
		(int)m_tShadowInfo.fCX,
		(int)m_tShadowInfo.fCY,
		hShadowDC,
		0,0,
		(int)m_tShadowInfo.fCX,
		(int)m_tShadowInfo.fCY,
		RGB(255, 0, 255));

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

void CPlayer::Release()
{
}

void CPlayer::Debug_Render(HDC hDC)
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

void CPlayer::Anim_Update()
{
	if (m_vDir.Magnitude() > 0 && 
		m_eCurState != DASH &&
		m_eCurState != DEAD &&
		m_eCurState != ONHIT)
		m_eCurState = RUN;
	else if (m_vDir.Magnitude() == 0 &&
		m_eCurState != DASH &&
		m_eCurState != ATTACK_1 &&
		m_eCurState != ATTACK_2 &&
		m_eCurState != DEAD &&
		m_eCurState != ONHIT)
	{
		m_eCurState = IDLE;
		m_wsShadowKey = L"NORMAL_SHADOW";
	}
}

void CPlayer::Update_LookDir()
{
	if (m_eCurState == DASH || m_eCurState == ATTACK_1 || m_eCurState == ATTACK_2 || m_eCurState == DEAD) return;

	if (m_vDir.x > 0)
	{
		m_pFrameKey = L"RIGHT";
		m_wsShadowKey = L"MIDDLE_SHADOW";
	}
	else if (m_vDir.x < 0)
	{
		m_pFrameKey = L"LEFT";
		m_wsShadowKey = L"MIDDLE_SHADOW";
	}
	else if (m_vDir.y > 0)
	{
		m_pFrameKey = L"BACK";
		m_wsShadowKey = L"NORMAL_SHADOW";
	}
	else if (m_vDir.y < 0)
	{
		m_pFrameKey = L"FRONT";
		m_wsShadowKey = L"NORMAL_SHADOW";
	}

	// �ٶ󺸴� ���Ⱚ ����
	if (m_vDir.Magnitude() > 0)
		m_vLookDir = m_vDir.Normalize();
}

void CPlayer::KeyInput()
{
	if (CKeyMgr::Get_Instance()->Key_Up('I'))
		m_pInven->Toggle();

	// �κ��丮 �����ִ� �߿��� �Է� ���� �ʵ���
	if (m_pInven->IsToggled()) return;

	// ��ų ���
	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_LBUTTON) && m_eCurState != RUN)
		Play_Skill(Mouse_LB);

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_RBUTTON))
		Play_Skill(Mouse_RB);

	if (CKeyMgr::Get_Instance()->Key_Pressing('Q'))
		Play_Skill(Key_Q);

	if (CKeyMgr::Get_Instance()->Key_Pressing('E'))
		Play_Skill(Key_E);

	if (CKeyMgr::Get_Instance()->Key_Pressing('R'))
		Play_Skill(Key_R);


	if (m_eCurState == ATTACK_1 || m_eCurState == ATTACK_2 || m_eCurState == DASH || m_eCurState == DEAD || m_eCurState == ONHIT)
		return;

	if (CKeyMgr::Get_Instance()->Key_Pressing(VK_SPACE))
	{
			m_eCurState = DASH;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('D'))
	{
		m_vDir.x = 1.f;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing('A'))
	{
		m_vDir.x = -1.f;
	}

	if (CKeyMgr::Get_Instance()->Key_Pressing('W'))
	{
		m_vDir.y = 1.f;
	}
	else if (CKeyMgr::Get_Instance()->Key_Pressing('S'))
	{
		m_vDir.y = -1.f;
	}

	if (CKeyMgr::Get_Instance()->Key_Up('D') || CKeyMgr::Get_Instance()->Key_Up('A'))
 		m_vDir.x = 0.f;
	if (CKeyMgr::Get_Instance()->Key_Up('W') || CKeyMgr::Get_Instance()->Key_Up('S'))
		m_vDir.y = 0.f;

}

void CPlayer::SetFrameByState()
{
	// ���� ��ȭ ���ٸ� ������
	if (m_eCurState == m_ePrevState)
		return;

	if (m_ePrevState == RUN)
	{
		m_fSpeed = 300.f;
		m_bSpeedUp = false;
	}

	// ���� ���� ����
	m_ePrevState = m_eCurState;

	// ���¿� ���� �ִϸ��̼� ������ ����
	switch (m_eCurState)
	{
	case IDLE:
		m_tFrame.SetInfo(0, 0, (int)IDLE, 200);
		break;
	case RUN:
		m_tFrame.SetInfo(0, 9, (int)RUN, 80);
		m_dwRunTime = GetTickCount(); //üũ����
		break;
	case DASH:
		m_tFrame.SetInfo(0, 7, (int)DASH, 40);
		break;
	case ATTACK_1:
		m_tFrame.SetInfo(0, 7, (int)ATTACK_1, 15);
		break;
	case ATTACK_2:
		m_tFrame.SetInfo(0, 7, (int)ATTACK_2, 15);
		break;
	case ONHIT:
		m_tFrame.SetInfo(0, 1, (int)ONHIT, 400);
		break;
	// ��� ����� front�ۿ� ����
	case DEAD:
		m_tFrame.SetInfo(0, 6, (int)DEAD, 80);
		break;
	}
}

void CPlayer::Play_Skill(PlayerSkillBtn _btn)
{
	POINT pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	switch (m_eSkill[_btn])
	{
	case SKILL_NORMAL:
		Skill_OnMouse(pt, SKILL_NORMAL);
		break;
	case SKILL_DASH:
		break;
	case SKILL_SHARK:
		Skill_NoMouse(SKILL_SHARK);
		break;
	case SKILL_EARTH:
		Skill_NoMouse(SKILL_EARTH);
		break;
	case SKILL_VOIDSTORM:
		Skill_OnMouse(pt, SKILL_VOIDSTORM);
		break;
	case SKILL_ICE_ORBIT:
		Skill_NoMouse(SKILL_ICE_ORBIT);
		break;
	default:
		break;
	}
}

void CPlayer::Skill_OnMouse(POINT _mousePt, PlayerSkill _skill)
{
	Vector2D vMousePos((float)_mousePt.x, (float)_mousePt.y);
	Vector2D vCamCenterPos((float)(WINCX >> 1), (float)(WINCY >> 1));

	m_vLookDir = (vMousePos - vCamCenterPos).Normalize();
	float angle = m_vLookDir.ToAngle_Rad();

	// ���� ���⿡ ���� ������ȯ
	if (angle >= 7 * PI / 4 || angle < PI / 4)
	{
		m_pFrameKey = L"RIGHT";
		m_wsShadowKey = L"MIDDLE_SHADOW";
	}
	else if (angle >= PI/4 && angle < 3*PI/4)
	{
		m_pFrameKey = L"BACK";
		m_wsShadowKey = L"BIG_SHADOW_VER";
	}
	else if (angle >= 3 * PI / 4 && angle < 5*PI/4)
	{
		m_pFrameKey = L"LEFT";
		m_wsShadowKey = L"MIDDLE_SHADOW";
	}
	else if (angle >= 5 * PI / 4 && angle < 7 * PI / 4)
	{
		m_pFrameKey = L"FRONT";
		m_wsShadowKey = L"BIG_SHADOW_VER";
	}

	bool success = CSkillMgr::Get_Instance()->Skill(_skill, this, angle);
	
	if(success)
		m_eCurState = ATTACK_1;
}

void CPlayer::Skill_NoMouse(PlayerSkill _skill)
{
	CSkillMgr::Get_Instance()->Skill(_skill, this);
}

void CPlayer::CheckFallTrap()
{
	int idxX = m_tTileColInfo.fX / TILECX;
	int idxY = m_tTileColInfo.fY / TILECY;

	CObj* Tile = CTileMgr::Get_Instance()->Get_Tile(idxX, idxY);
	TILE_ID type = static_cast<CTile*>(Tile)->Get_TileType();

	if (type == TILE_TRAP)
	{
		m_bOnTrap = true;
		m_pFrameKey = L"FRONT";
		m_eCurState = DEAD;
	}
}

bool CPlayer::CheckTile(float fX, float fY, TILE_ID& _returnID)
{
	list<CObj*> tempTileList;

	float tempX = fX + m_vTileColOffset.x;
	float tempY = fY - m_vTileColOffset.y;

	INFO tempTileColInfo{ tempX,tempY,m_tTileColInfo.fCX,m_tTileColInfo.fCY };

	int startX = (tempX - tempTileColInfo.fCX * 0.5f) / TILECX;
	int startY = (tempY - tempTileColInfo.fCY * 0.5f) / TILECY;
	int endX = (tempX + tempTileColInfo.fCX * 0.5f) / TILECX;
	int endY = (tempY + tempTileColInfo.fCY * 0.5f) / TILECY;

	CTileMgr::Get_Instance()->Get_Range_TileList(startX, startY, endX, endY, &tempTileList);

	for (auto& iter : tempTileList)
	{
		TILE_ID eType = dynamic_cast<CTile*>(iter)->Get_TileType();
		
		float fX, fY;

		if (eType == TILE_WALL)
		{
			if (CCollsionMgr::Check_Rect(iter->Get_Info(), tempTileColInfo, &fX, &fY))
			{
				_returnID = TILE_WALL;
				return true;
			}
		}
		else if (eType == TILE_TRAP)
		{
			if (CCollsionMgr::Check_Rect(iter->Get_Info(), tempTileColInfo, &fX, &fY))
			{
				_returnID = TILE_TRAP;
				return true;
			}
		}
	}


	return false;
}

void CPlayer::Set_Skill(int _slotIdx, PlayerSkill _skill)
{
	if (_skill >= SKILL_END) return;

	m_eSkill[_slotIdx] = _skill;
}

void CPlayer::Get_Damage(float _dmg, CObj* _pSrc)
{
	m_fCurHp -= _dmg;

	Vector2D hitDir = _pSrc->Get_Pos() - Get_Pos();
	float	 Angle = hitDir.ToAngle_Rad();

	FRAME frameInfo;

	if (m_fAngle >= 7 * PI / 4 || m_fAngle < PI / 4)
	{
		m_pFrameKey = L"RIGHT";
		frameInfo.SetInfo(0, 4, 0, 100);
	}
	else if(m_fAngle >= PI / 4 && m_fAngle <  3 * PI / 4)
	{
		m_pFrameKey = L"BACK";
		frameInfo.SetInfo(0, 4, 1, 100);
	}
	else if(m_fAngle >= 3 * PI / 4 && m_fAngle <  5 * PI / 4)
	{
		m_pFrameKey = L"LEFT";
		frameInfo.SetInfo(0, 4, 2, 100);
	}
	else if(m_fAngle >= 5 * PI / 4 && m_fAngle <  7 * PI / 4)
	{
		m_pFrameKey = L"FRONT";
		frameInfo.SetInfo(0, 4, 3, 100);
	}


	if (m_fHittedTime > 1.f)
	{
		m_fHittedTime = 0.f;
		m_bHitted = false;
	}

	if (!m_bHitted)
	{
		CSoundMgr::Get_Instance()->Play_Sound(L"PLAYER_HITED_1.mp3",SOUND_EFFECT_2,1.f);
		CObj* pEffect = CAbstractFactory<CEffect>::Create_Obj(INFO(m_tInfo.fX, m_tInfo.fY, 115.f, 115.f), L"HITEFFECT", frameInfo);
		CObjMgr::Get_Instance()->Add_Object(OBJ_EFFECT, pEffect);
		m_eCurState = ONHIT;
		m_bHitted = true;
	}

}

void CPlayer::Change_SkillIdx(PlayerSkillBtn _Dst, PlayerSkillBtn _Src)
{
	PlayerSkill temp = m_eSkill[_Src];

	m_eSkill[_Src] = m_eSkill[_Dst];
	m_eSkill[_Dst] = temp;
}

void CPlayer::Add_NewSkill(PlayerSkill _eAddSkillType)
{
	for (int i = 0; i < Btn_End; ++i)
	{
		if (m_eSkill[i] == SKILL_NONE)
		{
			m_eSkill[i] = _eAddSkillType;
			return;
		}
	}
}
