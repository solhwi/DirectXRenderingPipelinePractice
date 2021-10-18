#pragma once

//API - Application Programming Interface
// �������α׷��� ���α׷����ϱ����� �������̽��� ����

//WinAPI - ������ API

//DirectX
// GPU Rendering
// ���ӱ׷��ȿ��� vertex, �������� ����Ͽ� ���� �׸��� �׷��� ���� �𿩼� ������Ʈ�� �׷���
// �Ź� ����� �ٽ� �׸����ϴ� �۾��� �ݺ��ǹǷ� ������ ���������� ������ ���� �������� ���� ������ GPU�� �ʿ�����
// Rendering - �������� �׸��� �۾�

class Graphics : public Singleton<Graphics>
{
public:
	friend class Singleton<Graphics>;

	void Init();
	void CreateBackBuffer();

	ID3D11Device* GetDevice() { return device; }
	ID3D11DeviceContext* GetDC() { return deviceContext; }

	void Begin();
	void End();

private:
	Graphics();
	~Graphics();

private:
	//I -> ComInterface - COM Interface(component object model interface), ����ڰ� ���� �Ҵ�, ������ �Ұ����ϹǷ� ������ �Լ��� ȣ���ؾ��� - ex) ������ Release()
	ID3D11Device* device = nullptr; // Device - �ڿ����꿪��
	ID3D11DeviceContext* deviceContext = nullptr; // ������ ������������ ���� �������� ����
	IDXGISwapChain* swapChain = nullptr; // ������۸��� ���, ������ �����͸� ��� �ٲ����ν� ������۸��� ���� 
	ID3D11RenderTargetView* rtv = nullptr; // �׸��� �׸��� ����, ��ȭ�� ����
	D3D11_VIEWPORT viewport = { 0 }; // ������ ȭ�� ����
	D3DXCOLOR clearColor = 0xff555566; // ����ȭ�� ����
};

//������ ���������� ����
// IA - VS - RS - PS - OM

//����ȭ -1 ~ 1 , unsigned����ȭ 0 ~ 1