#pragma once
#include "Scene.h"
class CMyEdit :
    public CScene
{
public:
    CMyEdit();
    virtual ~CMyEdit();

public:
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void    Key_Input();

private:
    float   m_fCameraSpeed;

    // ���� ���
    int     m_EditMode;
    // Ÿ�ϸ�� -> ����Ÿ��,����Ÿ�� ���
    bool    m_RangeTileMode;

    // Ÿ�ϸ����
    bool    m_bClicked;
    POINT   m_tStartPt;
    POINT   m_tEndPt;

};

