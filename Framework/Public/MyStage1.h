#pragma once
#include "Scene.h"

class CObj;

class CMyStage1 :
    public CScene
{
public:
    CMyStage1();
    virtual ~CMyStage1();

public:
    // CScene��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void    KeyInput();

private:
    CObj*   m_pPlayer;

    int     m_iRenderMode;
};

