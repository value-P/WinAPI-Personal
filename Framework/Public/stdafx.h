// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//
#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++���̺귯��
#include <string>
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <io.h>
#include <iostream>

// �ܺ� ���̺귯��
#include <ole2.h>
#include <gdiplus.h>
#include <vfw.h>
#include "fmod.h"
#include "fmod.hpp"
// ��Ŀ �߰�
#pragma comment(lib,"Gdiplus.lib")
#pragma comment(lib, "vfw32.lib")
#pragma comment(lib, "fmodex_vc.lib")

// ����� ���� ���̺귯��
#include "Define.h"
#include "Vector2D.h"
#include "AbstractFactory.h"

// ��ũ��
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;
using namespace Gdiplus;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
