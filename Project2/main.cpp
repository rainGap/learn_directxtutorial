#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

/*
	https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-getmessage
	GetMessage ����ֵ
	��0����	:�������˳��� WM_QUIT ֮���������Ϣ
	0			:�������� WM_QUIT ��Ϣ
	-1		:��������(����ָ�����,���ھ��������)
*/
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE  hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);

	//What these two do is tell Windows to redraw the window if it is moved vertically or horizontally.
	wc.style = CS_HREDRAW | CS_VREDRAW;

	//����ʹ���ĸ���������������Ϣ
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";
	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",
		"Our First Windowed Program",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		500,
		400,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hWnd, nCmdShow);
	MSG msg;
	OutputDebugString("Hello\n");
	BOOL bRet;
	while (bRet = GetMessage(&msg, NULL, 0, 0))
	{
		if (bRet == -1)
		{
			//handle the getMessage error.
		}
		else {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		

	}
	return msg.wParam;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			MessageBox(NULL, "Quit", "Just Quit", MB_OK | MB_ICONEXCLAMATION);
			PostQuitMessage(0);
			return 0;
		}break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}