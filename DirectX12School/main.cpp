

#include "main.h"
#include "renderer.h"



const wchar_t* CLASS_NAME = L"DX12AppClass";
const wchar_t* WINDOW_NAME = L"DX12";


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


HWND g_Window;

HWND GetWindow()
{
	return g_Window;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	try
	{
		WNDCLASSEX wcex =
		{
			sizeof(WNDCLASSEX),
			CS_CLASSDC,
			WndProc,
			0,
			0,
			hInstance,
			NULL,
			LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)(COLOR_WINDOW + 1),
			NULL,
			CLASS_NAME,
			NULL
		};

		// �E�B���h�E�N���X�̓o�^
		RegisterClassEx(&wcex);

		// �E�B���h�E�̍쐬
		g_Window = CreateWindowEx(0,
			CLASS_NAME,
			WINDOW_NAME,
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),
			(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
			NULL,
			NULL,
			hInstance,
			NULL);


		// ����������(�E�B���h�E��쐬���Ă���s��)
		auto renderer = std::make_unique<CRenderer>();


		// �E�C���h�E�̕\��(�����������̌�ɍs��)
		ShowWindow(g_Window, nCmdShow);
		UpdateWindow(g_Window);



		//�t���[���J�E���g������
		DWORD dwExecLastTime;
		DWORD dwCurrentTime;
		timeBeginPeriod(1);
		dwExecLastTime = timeGetTime();
		dwCurrentTime = 0;


		// ���b�Z�[�W���[�v
		MSG msg;
		while (1)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				if (msg.message == WM_QUIT)
				{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
					break;
				}
				else
				{
					// ���b�Z�[�W�̖|��ƃf�B�X�p�b�`
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
			else
			{
				dwCurrentTime = timeGetTime();

				if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
				{
					dwExecLastTime = dwCurrentTime;

					// �X�V����
					//CManager::Update();
					renderer->Update();

					/// �`�揈��
					//CManager::Draw();
					renderer->Draw();
				}
			}
		}

		timeEndPeriod(1);				// ����\��߂�

		// �E�B���h�E�N���X�̓o�^����
		UnregisterClass(CLASS_NAME, wcex.hInstance);


		return (int)msg.wParam;
	}
	catch (DxException& e)
	{
		MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
		return 0;
	}
}


//=============================================================================
// �E�C���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

