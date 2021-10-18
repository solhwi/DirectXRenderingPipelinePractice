#include "stdafx.h"
#include "Graphics.h"

void Graphics::Init()
{
	DXGI_SWAP_CHAIN_DESC desc;
	ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));

	desc.BufferDesc.Width = 0;
	desc.BufferDesc.Height = 0;
	desc.BufferDesc.RefreshRate.Numerator = 60; // RefreshRate - �ֻ���, Numerator - ����
	desc.BufferDesc.RefreshRate.Denominator = 1; // Denominator - �и� -> ȭ���ֻ��� 60
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 4���� ä��, unsigned normalized
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // ��ĵ���θ��, Ư����������
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED; //�����ϸ����
	
	desc.BufferCount = 1; // ������� ����
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // rtv �뵵�� ���
	desc.SampleDesc.Count = 1; //2D������ �ʿ䰡 ���°͵��̹Ƿ� 1, 0���� ����
	desc.SampleDesc.Quality = 0;

	desc.OutputWindow = handle;
	desc.Windowed = true; // â��带 ����Ұ��� ����
	desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // ������ ��ü�� �߻��� ȿ��, DISCARD - ����Ʈ�� ������� ����

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
	//�Լ� ��ȯ���� HRESULT -> ����� �Լ������� �Ǿ����� Ȯ���� �Ҽ� ����
	HRESULT hr = swapChain->ResizeBuffers
	(
		0, // ����ī��Ʈ�� �ٲ��ʿ䰡 �����Ƿ� 0
		WinMaxWidth,
		WinMaxHeight,
		DXGI_FORMAT_UNKNOWN, // ���˺����� ����
		0
	);
	assert(SUCCEEDED(hr));

	ID3D11Texture2D* backBuffer = nullptr;

	hr = swapChain->GetBuffer
	(
		0, // ù��° ��ġ
		__uuidof(ID3D11Texture2D), // Ư���ڿ��� ����ID�� �������� �۾�
		(void**)&backBuffer // ��𿡴ٰ� �����ð��� ����
	);
	assert(SUCCEEDED(hr));

	hr = device->CreateRenderTargetView
	(
		backBuffer, // ����� �ڿ�
		nullptr,
		&rtv
	);
	assert(SUCCEEDED(hr));

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(WinMaxWidth);
	viewport.Height = static_cast<float>(WinMaxHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;// 2D������ ���̰� �ʿ�����Ƿ� �⺻������ ����

	SAFE_RELEASE(backBuffer);
}

void Graphics::Begin()
{
	deviceContext->RSSetViewports(1, &viewport); // RS���� viewport ����
	deviceContext->OMSetRenderTargets(1, &rtv, nullptr); // OM���� render target ����
	deviceContext->ClearRenderTargetView(rtv, clearColor); //InvalidateRectó�� ȭ���� ����� ����
}

void Graphics::End()
{
	HRESULT hr = swapChain->Present(1, 0); // ����ۿ� �������� ��ģ���� �����͸� �����Ͽ� ����Ʈ���ۿ� ��ü�Ͽ� �����ִ� �۾�
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
