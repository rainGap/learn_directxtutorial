#include <windows.h>
#include <windowsx.h>
#include <stdio.h>

/*
	https://docs.microsoft.com/en-us/windows/desktop/api/winuser/nf-winuser-peekmessagea
	PM_REMOVE : PeekMessage处理后，消息从队列里除掉。
	GetMessage是阻塞的，PeekMessage是非阻塞的
	如果消息队列为空,GetMessage函数将等待一个消息的到来以后才返回
	与之相对,PeekMessage如果取不到消息会立刻返回0,如果检索到消息则返回非0,不会阻塞到后面的游戏逻辑.
	
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

	//定义使用哪个方法来处理窗口消息
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

	while (TRUE) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// translate keystroke messages into the right format
			TranslateMessage(&msg);
			// send the message to the WindowProc function
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT)
		{
			OutputDebugString("quit\n");
			break;
		}
			

		// Run the game logic code here.
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