// DefaultWindow.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "DefaultWindow.h"
#include "MainGame.h"

#define MAX_LOADSTRING 100

// ���� ����:
HWND	g_hWnd;
HINSTANCE hInst;                                // ���� �ν��Ͻ��Դϴ�.
WCHAR szTitle[MAX_LOADSTRING];                  // ���� ǥ���� �ؽ�Ʈ�Դϴ�.
WCHAR szWindowClass[MAX_LOADSTRING];            // �⺻ â Ŭ���� �̸��Դϴ�.

// �� �ڵ� ��⿡ ��� �ִ� �Լ��� ������ �����Դϴ�.
ATOM                MyRegisterClass(HINSTANCE hInstance);		// ���� â�� ����� ���� �ɼ��� �����ϰ� â ��Ÿ���� �����ϴ� �Լ�
BOOL                InitInstance(HINSTANCE, int);				// â�� �����ϰ� �ʱ�ȭ �۾��� �����ϴ� �Լ�
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,			// �޸𸮿� �Ҵ�Ǵ� ��ü, �� ��ü ����
                     _In_opt_ HINSTANCE hPrevInstance,	// ���� ����Ǿ��� �ν��Ͻ� �ڵ� ��, ���� ��� NULL
                     _In_ LPWSTR    lpCmdLine,			// Ŀ�ǵ� ��ɾ�
                     _In_ int       nCmdShow)			// ������ â�� ��Ÿ�� �ɼ�, ���� (��� / �ּ�ȭ ��� / �ִ�ȭ ��� ��)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ���⿡ �ڵ带 �Է��մϴ�.

    // ���� ���ڿ��� �ʱ�ȭ�մϴ�.
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEFAULTWINDOW, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // ���� ���α׷� �ʱ�ȭ�� �����մϴ�.
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEFAULTWINDOW));

    MSG msg;
	msg.message = WM_NULL;

	CMainGame		MainGame;
	MainGame.Initialize();

	// �ü���� ���۵� �ķ� �帥 �ð��� ���� ���·� ��ȯ�ϴ� �Լ�
	DWORD	dwTime = GetTickCount();	// 1 / 1000���� ���� ���� ��ȯ
	// 200

	while (true)
	{
		// PM_REMOVE   : �޼��� ť���� �о�Ȱ� ���ÿ� �޼��� ť���� �޼����� ����
		// PM_NOREMOVE : �޼��� ť�� �޼����� �ִ����� ���� ���� �Ǵܸ� �ϰ� �޼��� ť���� �޼����� ���������� ����

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;

			// �޴� ����� ����Ű�� ����� �۵��ϵ��� �˻��ϴ� �Լ�
			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
			{
				// Ű���� �޼����� �����Ͽ� ���α׷����� ���� ��� �� �� �ֵ��� ���ִ� �Լ�
				// Getmessage�� �о�� Ű���� �޼����� �����ϴ� ����
				TranslateMessage(&msg);

				// �ý��� �޼��� ť���� ���� �޼����� ���α׷� �޼��� ó�� �Լ�(WndProc)�� �����ϴ� �Լ�
				DispatchMessage(&msg);
			}
		}
		else
		{
			if (dwTime + 10 < GetTickCount())
			{
				MainGame.Update();
				MainGame.Late_Update();
				MainGame.Render();

				dwTime = GetTickCount();
			}
            //MainGame.Update();
            //MainGame.Late_Update();
            //MainGame.Render();


		}		
	}

    return (int) msg.wParam;
}	 



//
//  �Լ�: MyRegisterClass()
//
//  ����: â Ŭ������ ����մϴ�.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;	// ������ â ���� �� �ʿ��� ������ �����ϴ� ����ü

    wcex.cbSize = sizeof(WNDCLASSEX);	// �ڱ� �ڽ��� ����� �����س���

    wcex.style          = CS_HREDRAW | CS_VREDRAW;	
    wcex.lpfnWndProc    = WndProc;		// �޼��� ó�� �Լ�, �����ص� �Լ��� ȣ��Ǿ� �޼����� ó���Ǵ� ���
    wcex.cbClsExtra     = 0;			// �����찡 Ư���� �������� ����ϴ� ������ ����
    wcex.cbWndExtra     = 0;			// ������ ���� �����̸� ������� �ʴ� ��� 0���� ����
    wcex.hInstance      = hInstance;	// �� ������ Ŭ������ ����ϴ� ���α׷��� ��ȣ ����(���� �Լ��� ���޵� hInstance���� �״�� ����)		
    
	wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEFAULTWINDOW));
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_DEFAULTWINDOW);
    
	wcex.lpszClassName  = szWindowClass;	// ���� ������ �̸��� ����(�������̸� ������Ʈ �̸��� ��ġ��Ŵ)
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));	// ������ â ��ܿ� �ִ� ���� ������ ����� ����

    return RegisterClassExW(&wcex);
}

//
//   �Լ�: InitInstance(HINSTANCE, int)
//
//   ����: �ν��Ͻ� �ڵ��� �����ϰ� �� â�� ����ϴ�.
//
//   ����:
//
//        �� �Լ��� ���� �ν��Ͻ� �ڵ��� ���� ������ �����ϰ�
//        �� ���α׷� â�� ���� ���� ǥ���մϴ�.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	RECT rc{ 0, 0, WINCX, WINCY };

   hInst = hInstance; // �ν��Ͻ� �ڵ��� ���� ������ �����մϴ�.

   // rc = rc + �⺻ â ��Ÿ�� + �޴� ���� ũ�� ���
   AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

   // ���� â�� �����ϴ� �Լ�
   HWND hWnd = CreateWindowW(szWindowClass,			// �ռ� ������ ������ Ŭ������ ������ Ŭ������ �̸��� �״�� �־���
						   szTitle,					// ������ â ����� ����� ���ڿ�
						   WS_OVERLAPPEDWINDOW,		// ������� �ϴ� ������ â ��Ÿ�� ����(�⺻ �� �����ϸ� ���� â ��Ÿ�� ��� ����)
						   CW_USEDEFAULT, 0,		// ������ â�� X,Y ��ǥ
						   rc.right - rc.left, 
						   rc.bottom - rc.top,	// ������ â�� ����, ���� ������
						   nullptr, // �θ� �����찡 �ִٸ� �θ� �������� �ڵ��� ����
						   nullptr, // �����쿡�� ����� �޴��� �ڵ��� ����
						   hInstance, // �����츦 ����� ��ü
						   nullptr); // �ü���� Ư���� �������� ����ϹǷ� �ǵ帱 �� ����

   
   if (!hWnd)
   {
      return FALSE;
   }

   g_hWnd = hWnd;

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

INT_PTR CALLBACK TileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK RangeTileCountProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK TileTypeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK MakeObjectProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
//
//  �Լ�: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ����:  �� â�� �޽����� ó���մϴ�.
//
//  WM_COMMAND  - ���� ���α׷� �޴��� ó���մϴ�.
//  WM_PAINT    - �� â�� �׸��ϴ�.
//  WM_DESTROY  - ���� �޽����� �Խ��ϰ� ��ȯ�մϴ�.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RECT	rcWnd{ 100, 100, 200, 200 };
	static list<RECT>	BulletList;


    switch (message)
    {
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // �޴� ������ ���� �м��մϴ�.
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case ID_MENU_TILE:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_TILECOUNT), hWnd, TileCountProc);
                if (iRet == IDOK)
                {

                }
            }
                break;
            case ID_MENU_TILES:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_RANGETILE), hWnd, RangeTileCountProc);
                if (iRet == IDOK)
                {

                }

            }
                break;
            case ID_TILE_OPTION:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_TILEOPTION), hWnd, TileTypeProc);
                if (iRet == IDOK)
                {

                }

            }
            break;
            case ID_STRUCT:
            {
                INT_PTR iRet = DialogBox(hInst, MAKEINTRESOURCE(IDD_OBJECT), hWnd, MakeObjectProc);
                if (iRet == IDOK)
                {

                }

            }
            break;

            case IDM_EXIT:
                DestroyWindow(g_hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: ���⿡ hdc�� ����ϴ� �׸��� �ڵ带 �߰��մϴ�.
			
            EndPaint(hWnd, &ps);
        }
        break;



    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// ���� ��ȭ ������ �޽��� ó�����Դϴ�.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;			
}

// �Լ� ȣ�� �Ծ� : �Լ��� ������ ����Ǹ� �Ҵ��ߴ� ���� �޸𸮸� �����ؾ� �ϴµ� �Լ��� ȣ���� ȣ���(caller)���� ������ ���ΰ�
// ȣ�� ���� ��ȣ����(callee)���� �� ���ΰ��� ���� ��� �Ǵ� �Ծ�

// __cdecl : c / c++ ����� �⺻ ȣ�� �Ծ�, ȣ����� ������ ����, ��ǥ���� �Լ��� printf�� ���ø��� ����.
// ���� �ڱ� �ڽſ��� � �Ű������� �󸶳� ������ �𸥴ٴ� ��(�������� �Լ�)������ ������ ������ ������ �� ����.

// __stdcall : WIN32 api�� �⺻ ȣ�� �Ծ�, ��ȣ���ڰ� ������ ����, api���� �����ϴ� ��κ��� �Լ����� '�������� �Լ�'�̴�. 
// �Ű� ������ �����Ǿ� �־� ��ȣ���ڰ� � ���� ������ ũ�Ⱑ ������ ���� �����ϱ� ������ ������ ������ ���� �� �� �ִ�.

// __fastcall : �Լ� ȣ���� ������ ó���ϱ� ���� �Ծ�, �Լ� �ڵ带 �������͸� ���� ����
// __thiscall : this �����͸� �Ű� ������ �Ѱ� �޾��� �� ����ϴ� ȣ�� �Ծ�

// �� �� ���� ȣ�� �Ծ� �� __cdecl�� ȣ����� ������ ����

// 1. �׸��� ����(�׸��� �׷��Ͷ�)
// 2. �����̽� �ٸ� ������ �� ���� ������ �Ѿ�(ellipse)�� �����Ͷ�