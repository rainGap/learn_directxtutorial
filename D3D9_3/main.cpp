// include the basic windows header files and the Direct3D header file
#include <windows.h>
#include <windowsx.h>
#include <d3d9.h>
#include "utils.h"
#include "main.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/*
http://www.directxtutorial.com/Lesson.aspx?lessonid=9-4-2
full screen
*/
#pragma comment (lib, "d3d9.lib")

LPDIRECT3D9 d3d;
LPDIRECT3DDEVICE9 d3ddev;
LPDIRECT3DVERTEXBUFFER9 v_buffer;

// 声明函数
void initD3D(HWND hWnd);
void render_frame(void);
void cleanD3D(void);
void init_graphics(void);

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE  hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	odprintf("WinMain start!\n");
	HWND hWnd;
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	int haha = 1;
	haha = getNum();
	//What these two do is tell Windows to redraw the window if it is moved vertically or horizontally.
	wc.style = CS_HREDRAW | CS_VREDRAW;

	//定义使用哪个方法来处理窗口消息
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	//注释掉下一句,可以避免全屏的时候闪一下. his leaves the background color untouched, which means it won't be visible as window for a second or two before the game starts (important to making your game look professional).
	//wc.hbrBackground = (HBRUSH)COLOR_WINDOW;

	wc.lpszClassName = "WindowClass1";
	RegisterClassEx(&wc);
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",
		"Our First Windowed Program",
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);
	ShowWindow(hWnd, nCmdShow);
	initD3D(hWnd);
	init_graphics();

	MSG msg;

	BOOL flag = FALSE;
	while (TRUE) {
		BOOL ret;
		while (ret = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// translate keystroke messages into the right format
			TranslateMessage(&msg);
			// send the message to the WindowProc function
			DispatchMessage(&msg);

			//加了D3D相关方法后,发现这里的消息队列在WM_QUIT后,
			//会紧跟着另外一个message,
			//所以导致while循环外的break判断失效
			//所以决定在这里就break掉以免读到WM_QUIT后面的那个message
			if (msg.message == WM_QUIT)
				flag = TRUE;
			//具体明天查一下那个Message是啥
		}
		if (flag)
		{
			//这里打印出来的是WM_TIMER
			//为什么我的消息队列里全是WM_TIMER
			//https://blogs.msdn.microsoft.com/oldnewthing/20160624-00/?p=93745
			odprintf("after wm_quit:%X\n", msg.message);
			flag = FALSE;
			break;
		}



		if (msg.message == WM_QUIT)
			break;
		render_frame();
	}
	cleanD3D();
	return msg.wParam;
}
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		//MessageBox(NULL, "Quit", "Just Quit", MB_OK | MB_ICONEXCLAMATION);
		PostQuitMessage(0);
		return 0;
	}
	break;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		{
			odprintf("VK_ESCAPE\n");
			PostQuitMessage(0);
			return 0;
		}

		}
		break;

	}

	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void initD3D(HWND hWnd)
{
	// create the Direct3D interface
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	// create a struct to hold various device information
	D3DPRESENT_PARAMETERS d3dpp;

	// clear out the struct for use
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	// program windowed, not fullscreen
	d3dpp.Windowed = TRUE;

	// discard old frames
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	// set the window to be used by Direct3D
	d3dpp.hDeviceWindow = hWnd;

	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;// set the back buffer format to 32-bit, 8 means 8bits
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;

	// create a device class using this information and information from the d3dpp stuct
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);
}

void init_graphics(void)
{
	// create three vertices using the CUSTOMVERTEX struct built earlier
	CUSTOMVERTEX vertices[] =
	{
		{ 320.0f, 50.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(122, 122, 122), },
		{ 520.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(0, 0, 0), },
		{ 120.0f, 400.0f, 0.5f, 1.0f, D3DCOLOR_XRGB(255,255, 255), },
	};

	// create the vertex and store the pointer into v_buffer, which is created globally
	d3ddev->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0,
		CUSTOMFVF,
		D3DPOOL_MANAGED,
		&v_buffer,
		NULL);

	VOID* pVoid;    // the void pointer

	v_buffer->Lock(0, 0, (void**)&pVoid, 0);    // lock the vertex buffer
	memcpy(pVoid, vertices, sizeof(vertices));    // copy the vertices to the locked buffer
	v_buffer->Unlock();    // unlock the vertex buffer
}

void render_frame(void)
{
	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0F, 0);

	// begins the 3D scene
	d3ddev->BeginScene();

	// do 3D rendering on the back buffer here
	d3ddev->SetFVF(CUSTOMFVF);
	d3ddev->SetStreamSource(0, v_buffer, 0, sizeof(CUSTOMVERTEX));
	d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	d3ddev->EndScene();

	// displays the created frame
	d3ddev->Present(NULL, NULL, NULL, NULL);

}

void cleanD3D(void)
{
	v_buffer->Release();
	d3ddev->Release();
	d3d->Release();
}




