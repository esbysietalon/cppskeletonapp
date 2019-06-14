// GraphicsBase.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "GraphicsBase.h"

#include <windows.h>    // include the basic windows header file
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")



IDXGISwapChain *swapchain;
ID3D11Device *dev;
ID3D11DeviceContext *devcon;
ID3D11RenderTargetView *backbuffer;

void InitD3D(HWND hWnd);
void CleanD3D(void);
void RenderFrame(void);


WCHAR* windowTitle = L"Hello World!";
WCHAR* windowCaption = L"oWo what's this";

int windowX = 300;
int windowY = 300;
int windowWidth = 800;
int windowHeight = 600;

RECT wr = { 0, 0, windowWidth, windowHeight };


LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nShowCmd)
{

	HWND hWnd;
	
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = L"GraphicsBase";

	RegisterClassEx(&wc);

	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		L"GraphicsBase",
		windowTitle,
		WS_OVERLAPPEDWINDOW,
		windowX,
		windowY,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nShowCmd);

	InitD3D(hWnd);

	MSG msg;

	while (TRUE) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		
		RenderFrame();

	}

	CleanD3D();

	return msg.wParam;
	/*// create a "Hello World" message box using MessageBox()
	MessageBox(NULL,
		windowTitle,
		windowCaption,
		MB_ICONEXCLAMATION | MB_OK);
		*/

	// return 0 to Windows
	//return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		} break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

void InitD3D(HWND hWnd) {
	DXGI_SWAP_CHAIN_DESC scd;

	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);
	
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	devcon->OMSetRenderTargets(1, &backbuffer, NULL);


	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = windowWidth;
	viewport.Height = windowHeight;

	devcon->RSSetViewports(1, &viewport);
}

void RenderFrame(void) {

	float clearColor[4] = { 0.0f, 0.2f, 0.4f, 1.0f };

	devcon->ClearRenderTargetView(backbuffer, clearColor);


	swapchain->Present(0, 0);
}

void CleanD3D() {
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}