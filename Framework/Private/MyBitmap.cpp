#include "stdafx.h"
#include "MyBitmap.h"

CMyBitmap::CMyBitmap()
{
}

CMyBitmap::~CMyBitmap()
{
	Release();
}

void CMyBitmap::Load_Bmp(wstring pFilePath)
{
	HDC	hDC = GetDC(g_hWnd);

	m_hMemDC = CreateCompatibleDC(hDC);

	ReleaseDC(g_hWnd,hDC);

	m_hBitmap = (HBITMAP)LoadImage(NULL,
									pFilePath.c_str(),
									IMAGE_BITMAP,
									0, 0,
									LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_hOldBmp = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
}


void CMyBitmap::Release()
{
	SelectObject(m_hMemDC, m_hOldBmp);
	DeleteObject(m_hBitmap);
	DeleteDC(m_hMemDC);
}
