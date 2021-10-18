#include "stdafx.h"
#include "Graphics.h"

void Graphics::Init()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.RefreshRate.Numerator = 60; // RefreshRate - 주사율, Numerator - 분자
	desc.BufferDesc.RefreshRate.Denominator = 1; // Denominator - 분모 -> 화면주사율 60
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 4개의 채널, unsigned normalized
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // 스캔라인모드, 특정하지않음
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //스케일링모드
	
	desc.BufferCount = 1; // 백버퍼의 갯수
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // rtv 용도로 사용
	desc.SampleDesc.Count = 1; //2D에서는 필요가 없는것들이므로 1, 0으로 세팅
	desc.SampleDesc.Quality = 0;

	desc.OutputWindow = handle;
	desc.Windowed = true; // 창모드를 사용할건지 설정
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // 버퍼의 교체가 발생시 효과, DISCARD - 이펙트를 사용하지 않음

	vector<D3D_FEATURE_LEVEL> featureLevel
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};

	HRESULT hr = D3D11CreateDeviceAndSwapChain
	(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		featureLevel.data(),
		featureLevel.size(),
		D3D11_SDK_VERSION,
		&desc,
		&swapChain,
		&device,
		nullptr,
		&deviceContext
		);
	assert(SUCCEEDED(hr));

	CreateBackBuffer();
}

void Graphics::CreateBackBuffer()
{
	//함수 반환값이 HRESULT -> 제대로 함수적용이 되었는지 확인을 할수 있음
	HRESULT hr = swapChain->ResizeBuffers
	(
		0, // 버퍼카운트를 바꿀필요가 없으므로 0
		WinMaxWidth,
		WinMaxHeight,
		DXGI_FORMAT_UNKNOWN, // 포맷변경을 안함
		0
	);
	assert(SUCCEEDED(hr));

	ID3D11Texture2D* backBuffer = nullptr;

	hr = swapChain->GetBuffer
	(
		0, // 첫번째 위치
		__uuidof(ID3D11Texture2D), // 특정자원의 고유ID를 가져오는 작업
		(void**)&backBuffer // 어디에다가 가져올건지 결정
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		backBuffer, // 사용할 자원
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(WinMaxWidth);
	viewport.Height = static_cast<float>(WinMaxHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;// 2D에서는 깊이가 필요없으므로 기본값으로 설정

	SAFE_RELEASE(backBuffer);
}

void Graphics::Begin()
{
	deviceContext->RSSetViewports(1, &viewport); // RS에서 viewport 세팅
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr); // OM에서 render target 세팅
	deviceContext->ClearRenderTargetView(rtv, clearColor); //InvalidateRect처럼 화면을 지우는 역할
}

void Graphics::End()
{
	HRESULT hr = swapChain->Present(1, 0); // 백버퍼에 렌더링을 마친것을 포인터를 변경하여 프론트버퍼와 교체하여 보여주는 작업
	assert(SUCCEEDED(hr));
}

Graphics::Graphics()
{
	Init();
}

Graphics::~Graphics()
{
	SAFE_RELEASE(rtv);
	SAFE_RELEASE(swapChain);
	SAFE_RELEASE(deviceContext);
	SAFE_RELEASE(device);
}
