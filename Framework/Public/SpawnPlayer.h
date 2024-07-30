#pragma once
#include "Effect.h"
class CSpawnPlayer :
    public CEffect
{
public:
    CSpawnPlayer();
    virtual ~CSpawnPlayer();

public:
    // CObj��(��) ���� ��ӵ�
    void Initialize() override;
    int Update() override;
    void Late_Update() override;
    void Render(HDC hDC) override;
    void Release() override;

};

