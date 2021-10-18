#include "stdafx.h"
#include "Program.h"

Program::Program()
{
	vertex_count = 4;

	//vertexData
	{
		// �ð� �������� ���� ���;� ��, �� �ð� ������ ��� �޸����� �ν�, ���������� ���� ��> 
		// Back Face Culling �� ���̽� �ø�

		// uv ��ǥ

		// 0 0                    1 0
		// 0 1                    1 1 

		// ���¸� ���� 0 ~ 1 ��ֶ����� ��ǥ��

		vertices = new VertexTexture[6];
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0); // ���� 
		vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);
		vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0); // �»�
		vertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);

		vertices[3].position = D3DXVECTOR3(0.5f, 0.5f, 0); // ���
		vertices[3].uv = D3DXVECTOR2(1.0f, 0.0f);
		vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0); // ����
		vertices[2].uv = D3DXVECTOR2(1.0f, 1.0f);

		// �÷� ���ؽ����� �����Ͽ� ����
		// ũ�� ����
		// ȸ�� �����ϰ� �����Ǿ�

		// �̵� ���� ���� ������ ƨ���, ȸ����Ű��, �� �ٲٱ�

		//vertices[4].position = D3DXVECTOR3(0.5f, -0.5f, 0); // ���� 
		//vertices[4].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		//vertices[5].position = D3DXVECTOR3(-0.5f, 0.5f, 0); // �»�
		//vertices[5].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// ����� -1 ~ 1�� ����ȭ�� NDC ������, ���ϴ� ���� �׸��� ���ؼ� �ٸ� ����� �ʿ�

		// NDC Normalized Device coordinate
		// ���� ��ǥ���� ��

	}

	// vectexBuffer
	{
		D3D11_BUFFER_DESC desc; // ���� ��ũ���� ����
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_IMMUTABLE;

		// default : GPU������ �����͸� �а� ��
		// Immutable GPU�� �����͸� �б⸸ ��, ������ ��ȭ ����, writable�� ���� �ӵ��� ����
		// Dynamic : GPU���� �б� ��� . map�� ���� CPU���� ���� ���, ���� ����
		// Staging : gpu���� cpu �޸𸮿� ���ҽ� ���縦 ���, ���� ����

		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(VertexTexture) * vertex_count; // �� ������ ����Ʈ ��, ���ؽ� �÷� 3������ 6���� ������

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

		subData.pSysMem = vertices; // ���� �������� �ý��� �޸𸮿� �������� vertices ����

		HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&desc, &subData, &vertexBuffer);
		// �ʱ� �����͸� �־������ Immutable �̱� ������
		assert(SUCCEEDED(hr));

	}

	{   //vertexShader
		HRESULT hr = D3DCompileFromFile // MS���� �����ϴ� �����Ϸ� ����
		(
			L"./Texture.hlsl", // ���� �̸��� �־�� ��
			nullptr, // ������ ��ũ�� ����
			nullptr,
			"VS", // ���ʷ� ����Ǵ� �Լ���
			"vs_5_0", // vertex ���̴� 5
			0,
			0,
			&vsBlob, // ����� ������ binary �����͸� ��� ����ü
			nullptr
		);
		assert(SUCCEEDED(hr)); // ���⼭ �ɸ�

		hr = Graphics::Get()->GetDevice()->CreateVertexShader
		(
			vsBlob->GetBufferPointer(), // ������ ù �ּ�
			vsBlob->GetBufferSize(), // ���� ũ��
			nullptr,
			&vertexShader // ���ؽ� ���̴� ����
		);

		assert(SUCCEEDED(hr));
	}

	{	// indexData
		indices = new UINT[6]{ 0,1,2,2,1,3 };
	}

	{ // indexBuffer
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D10_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(UINT) * 6;

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
		subData.pSysMem = indices;

		HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&desc, &subData, &indexBuffer);
		assert(SUCCEEDED(hr));
	}

	//input layout
	{
		D3D11_INPUT_ELEMENT_DESC layoutDesc[]
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(D3DXVECTOR3), D3D11_INPUT_PER_VERTEX_DATA}
			// semantic, position�� 0�� ������Ѷ�
			// Float 32 ¥�� 4��
			// ���� ������ 1�� �� ������ �� �ϳ��� INPUT_PER_VERTEXT_DATA
			// �ܿ��� INTANCING�� �ִµ� 3D���� ���
			// �Ȱ��� ���� ���� �ֵ��� ��ο����� �ѹ��� ���ļ� ����, ��ο����� �ڿ� �Ҹ� ���� ����
			// 0 ���� sizeof, ������ ũ�⸸ŭ ����ְ� �÷��� �����Ϸ��� �ǵ�
		};

		HRESULT hr = Graphics::Get()->GetDevice()->CreateInputLayout
		(
			layoutDesc,
			2, // input size 
			vsBlob->GetBufferPointer(),
			vsBlob->GetBufferSize(),
			&inputLayout
		);
		assert(SUCCEEDED(hr));
	}

	{ // pixel shader
		HRESULT hr = D3DCompileFromFile
		(
			L"./Texture.hlsl",
			nullptr,
			nullptr,
			"PS",
			"ps_5_0",
			0,
			0,
			&psBlob,
			nullptr
		);
		assert(SUCCEEDED(hr));

		hr = Graphics::Get()->GetDevice()->CreatePixelShader
		(
			psBlob->GetBufferPointer(),
			psBlob->GetBufferSize(),
			nullptr,
			&pixelShader
		);
		assert(SUCCEEDED(hr));
	}

	{ // create world view projection
		// ���� ��ǥ�踦 ��Ÿ���� ���, ���� �� ���� ���� ũ��, ȸ��, �̵��� ��ȯ�� ��
		// ī�޶��� ���� ������ ��� ���

		// ������ ���� ��ȯ�� �ؾ� ���� > ���� ��ǥ��� ����

		// 1. ���ÿ��� ��> ����� �������� 1 x 1 ���簢���� ��

		// �����, ����, �ٶ󺸴� ����, ���� ����

		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&projection);

		D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); // LH left hand ��ǥ��

		D3DXMatrixOrthoLH(&projection, WinMaxWidth, WinMaxHeight, 0, 1); // ���� ����

		// zn - z�� near, zf - z�� far ����� ���ʿ�

		cout << "World Matrix" << endl;
		cout << world._11 << " " << world._12 << " " << world._13 << " " << world._14 << endl;
		cout << world._21 << " " << world._22 << " " << world._23 << " " << world._24 << endl;
		cout << world._31 << " " << world._32 << " " << world._33 << " " << world._34 << endl;
		cout << world._41 << " " << world._42 << " " << world._43 << " " << world._44 << endl;

		cout << "View Matrix" << endl;
		cout << view._11 << " " << view._12 << " " << view._13 << " " << view._14 << endl;
		cout << view._21 << " " << view._22 << " " << view._23 << " " << view._24 << endl;
		cout << view._31 << " " << view._32 << " " << view._33 << " " << view._34 << endl;
		cout << view._41 << " " << view._42 << " " << view._43 << " " << view._44 << endl;

		cout << "projection Matrix" << endl;
		cout << projection._11 << " " << projection._12 << " " << projection._13 << " " << projection._14 << endl;
		cout << projection._21 << " " << projection._22 << " " << projection._23 << " " << projection._24 << endl;
		cout << projection._31 << " " << projection._32 << " " << projection._33 << " " << projection._34 << endl;
		cout << projection._41 << " " << projection._42 << " " << projection._43 << " " << projection._44 << endl;

		// 2 / 1280 �� ���� ���´� �ֿܼ���
		// 2 / 720

		/*world._11 = 50 * cos(30);
		world._12 = 50 * sin(30);
		world._21 = 100 * -1 * sin(30);
		world._22 = 100 * cos(30);
		world._41 = 100;
		world._42 = 100;*/

		D3DXMATRIX S, R, T;
		D3DXMatrixIdentity(&S);
		D3DXMatrixIdentity(&R);
		D3DXMatrixIdentity(&T);

		D3DXMatrixScaling(&S, 500.0f, 500.0f, 1);
		D3DXMatrixRotationZ(&R, static_cast<float>(D3DXToRadian(0.0f)));
		D3DXMatrixTranslation(&T, 100, 100, 0);

		world = S * R * T;

		// SRT ��> Scale up ��> Rotation ��> Translation

		// cos sin
		// -sin cos
	}

	{ // constantBuffer ��� ����
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU���� ���� ���, �ַ� �� �����Ӹ��� CPU���� ������ ���� �� �����
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.ByteWidth = sizeof(TransformData);
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&desc, nullptr, &gpuBuffer);
		assert(SUCCEEDED(hr));

	}

	{ // create reasterizer state
		D3D11_RASTERIZER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

		desc.FillMode = D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_BACK; // �޸� ����
		desc.FrontCounterClockwise = false; // �ð� ������ �ո��� �ƴ�, �ݽð谡 ����

		HRESULT hr = Graphics::Get()->GetDevice()->CreateRasterizerState(&desc, &RS);
		assert(SUCCEEDED(hr));
	}

	// �ڿ� ���� ����?

	// 1. rtv : render target view // ������ ������ ��
	// 2. srv : shader resource view // ���̴����� �� �ڿ� ��
	// 3. dsv : depth stencil view // ���� ���ټ� ��
	// 4. uav : unordered acces view

	{ // SRV
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			Graphics::Get()->GetDevice(),
			L"./Tree.png", // FromFile�� A ���̸� ���ڿ� L ������
			nullptr,
			nullptr,
			&srv,
			nullptr
		);

		assert(SUCCEEDED(hr));
	}

	{ // create sampler state
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));

		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; // �Ѿ�� ������ ���� ó�� ���
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; // �Ѿ �κ��� border �÷��� ĥ�϶�
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

		desc.BorderColor[0] = 0.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 0.0f;
		desc.BorderColor[3] = 1.0f;

		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; // ũ�� �Ű澲������

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // Ȯ�� ��Ұ��Ͼ �� � ���·� �� ������
		// POINT�� ��� �� UV ��ǥ�� �ΰ��� �ȼ��� ������ ��, �� ���� ���
		// LINEAR�� ��հ����� ����

		desc.MaxAnisotropy = 16; // ���� ���⿡ ���� �޶����� ���� ��, 2D���� �ʿ� X

		desc.MinLOD = numeric_limits<float>::min();
		desc.MaxLOD = numeric_limits<float>::max();

		desc.MipLODBias = 0.0f;

		HRESULT hr = Graphics::Get()->GetDevice()->CreateSamplerState(&desc, &samplerState);
		assert(SUCCEEDED(hr));
 	}

	{ // create blend state
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));
		
		desc.AlphaToCoverageEnable = false;

		desc.IndependentBlendEnable = false; // ������ ���� Ÿ��(�ִ� 8��)���� ������ ���� ������ ���� ���ΰ�
		// false�� �ϳ��� �� �Ȱ��� ������
	
		desc.RenderTarget[0].BlendEnable = true; // ��� 0�� ���� �ɼ� �� ����
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // ���İ� 0�� �Ϳ� ���� 1�� ä�� ��> �׷��� ���� ���� ��
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 1 - SrcAlpha �ϴ� �� src�� 1�̸� ����� 0, src�� 0�̸� ����� 1
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // ��� ���� �� src, dest �� ���ϴ� ���

		// (Src(�޾ƿ� png �ؽ���) * srcBlend) BlendOP (Dest * destBlend)
		// ���� ���ϱ�� �ƴϰ�...

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // Ư�� ä�θ� ���
		
		HRESULT hr = Graphics::Get()->GetDevice()->CreateBlendState(&desc, &blenderState);
	}


}

Program::~Program()
{
	SAFE_RELEASE(samplerState);

	SAFE_RELEASE(srv);
	SAFE_RELEASE(RS);
	SAFE_RELEASE(gpuBuffer);
	SAFE_RELEASE(pixelShader);
	SAFE_RELEASE(psBlob);

	SAFE_RELEASE(inputLayout);
	
	SAFE_RELEASE(vertexShader);
	SAFE_RELEASE(vsBlob);

	SAFE_RELEASE(indexBuffer);
	SAFE_DELETE_ARRAY(indices);

	SAFE_RELEASE(vertexBuffer);
	SAFE_DELETE_ARRAY(vertices);
}

void Program::Update()
{
	// GPU�� �� �켱 ����� ����ϳ�, DX�� �� �켱 ����� ����Ѵ�.
	// ��ġ��ķ� ������ ��

	D3DXMatrixTranspose(&cpuBuffer.world, &world);
	D3DXMatrixTranspose(&cpuBuffer.view, &view);
	D3DXMatrixTranspose(&cpuBuffer.projection, &projection);

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	Graphics::Get()->GetDC()->Map
	(
		gpuBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD, // ���� ���� ���� �����
		0,
		&mappedSubResource
	);

	memcpy(mappedSubResource.pData, &cpuBuffer, sizeof(TransformData));
	Graphics::Get()->GetDC()->Unmap(gpuBuffer, 0);

}

void Program::Render()
{
	UINT stride = sizeof(VertexTexture); // ����
	UINT offset = 0;

	// IA
	Graphics::Get()->GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	Graphics::Get()->GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0); // ���� �� 1����
	Graphics::Get()->GetDC()->IASetInputLayout(inputLayout);
	Graphics::Get()->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // ������ � ������� ���� ��?

	// POINTLIST ��
	// LINELIST ��
	// TRIANGLELIST �ﰢ�� ��> 5�� �������

	// cpu buffer
	Graphics::Get()->GetDC()->VSSetConstantBuffers(0, 1, &gpuBuffer);

	// VS
	Graphics::Get()->GetDC()->VSSetShader(vertexShader, nullptr, 0);

	// RS
	Graphics::Get()->GetDC()->RSSetState(RS);
	
	// PS
	Graphics::Get()->GetDC()->PSSetShader(pixelShader, nullptr, 0);
	Graphics::Get()->GetDC()->PSSetShaderResources(0, 1, &srv);
	Graphics::Get()->GetDC()->PSSetSamplers(0, 1, &samplerState);
	

	// OM
	Graphics::Get()->GetDC()->OMSetBlendState(blenderState, nullptr, 0xFFFFFFFF);


	Graphics::Get()->GetDC()->DrawIndexed(6, 0, 0);
}
