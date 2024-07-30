#pragma once
#include "Scene.h"

enum MENU_ID {READY,SINGLE,OPTION,QUIT,MENU_END};

class CLogo :
    public CScene
{
public:
    CLogo();
    virtual ~CLogo();

public:
    // CScene��(��) ���� ��ӵ�
    virtual void Initialize() override;
    virtual int Update() override;
    virtual void Late_Update() override;
    virtual void Render(HDC hDC) override;
    virtual void Release() override;

private:
    void  KeyInput();

private:
    MENU_ID     m_eMenuType; // �޴� Ÿ��
    // 0 : �⺻â / 1 : �̱��÷��� / 2:�ɼ�(����) / 3:������ 
};

