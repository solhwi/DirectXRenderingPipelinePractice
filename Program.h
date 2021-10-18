#pragma once

struct VertexColor
{
	D3DXVECTOR3 position;
	D3DXCOLOR color;
};

struct TransformData
{
	D3DXMATRIX world;
	// �� ���̿� ������ �е��ؾ���, GPU�� 16����Ʈ ������ �߷������Ƿ� ������ �е��ؾ���
	D3DXMATRIX view;
	D3DXMATRIX projection;
};

struct VertexTexture
{
	D3DXVECTOR3 position;
	D3DXVECTOR2 uv; // ��ǥ�� 0 ~ 1�� ������ ����ȭ��Ų ��..?
};

class Program
{
public:
	Program();
	~Program();

public:
	void Update();
	void Render();

	int vertex_count;
private:
	//VertexColor* vertices = nullptr;
	VertexTexture* vertices = nullptr;
	ID3D11Buffer* vertexBuffer = nullptr;

	ID3D11Buffer* indexBuffer = nullptr;
	UINT* indices = nullptr;

	ID3D11InputLayout* inputLayout = nullptr;


	ID3D11VertexShader* vertexShader = nullptr;
	ID3DBlob* vsBlob = nullptr;

	ID3D11PixelShader* pixelShader = nullptr;
	ID3DBlob* psBlob = nullptr;

	D3DXMATRIX world; // 4 by 4, ��ķ� ������ �ٷ� �� �⺻��
	D3DXMATRIX view;
	D3DXMATRIX projection;

	TransformData cpuBuffer;
	ID3D11Buffer* gpuBuffer;

	ID3D11RasterizerState* RS = nullptr;

	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11SamplerState* samplerState = nullptr;

	ID3D11BlendState* blenderState = nullptr;
};