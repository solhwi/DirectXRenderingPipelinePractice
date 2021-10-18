#include "stdafx.h"
#include "Program.h"

Program::Program()
{
	vertex_count = 4;

	//vertexData
	{
		// 시계 방향으로 점이 들어와야 함, 반 시계 방향인 경우 뒷면으로 인식, 렌더링하지 않음 ㅡ> 
		// Back Face Culling 백 페이스 컬링

		// uv 좌표

		// 0 0                    1 0
		// 0 1                    1 1 

		// 형태를 가진 0 ~ 1 노멀라이즈 좌표계

		vertices = new VertexTexture[6];
		vertices[0].position = D3DXVECTOR3(-0.5f, -0.5f, 0); // 좌하 
		vertices[0].uv = D3DXVECTOR2(0.0f, 1.0f);
		vertices[1].position = D3DXVECTOR3(-0.5f, 0.5f, 0); // 좌상
		vertices[1].uv = D3DXVECTOR2(0.0f, 0.0f);

		vertices[3].position = D3DXVECTOR3(0.5f, 0.5f, 0); // 우상
		vertices[3].uv = D3DXVECTOR2(1.0f, 0.0f);
		vertices[2].position = D3DXVECTOR3(0.5f, -0.5f, 0); // 우하
		vertices[2].uv = D3DXVECTOR2(1.0f, 1.0f);

		// 컬러 버텍스끼리 통일하여 랜덤
		// 크기 랜덤
		// 회전 랜덤하게 생성되어

		// 이동 랜덤 벽에 닿으면 튕기고, 회전시키고, 색 바꾸기

		//vertices[4].position = D3DXVECTOR3(0.5f, -0.5f, 0); // 우하 
		//vertices[4].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		//vertices[5].position = D3DXVECTOR3(-0.5f, 0.5f, 0); // 좌상
		//vertices[5].color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		// 현재는 -1 ~ 1로 정규화된 NDC 상태임, 원하는 곳에 그리기 위해선 다른 기법이 필요

		// NDC Normalized Device coordinate
		// 로컬 좌표계라고도 함

	}

	// vectexBuffer
	{
		D3D11_BUFFER_DESC desc; // 버퍼 디스크립션 제작
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_IMMUTABLE;

		// default : GPU에서만 데이터를 읽고 씀
		// Immutable GPU가 데이터를 읽기만 함, 앞으로 변화 없음, writable에 비해 속도가 빠름
		// Dynamic : GPU에서 읽기 허용 . map를 통해 CPU에서 쓰기 허용, 조금 느림
		// Staging : gpu에서 cpu 메모리에 리소스 복사를 허용, 많이 느림

		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(VertexTexture) * vertex_count; // 한 간격의 바이트 수, 버텍스 컬러 3개에서 6개로 수정함

		D3D11_SUBRESOURCE_DATA subData;
		ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));

		subData.pSysMem = vertices; // 서브 데이터의 시스템 메모리에 만들어놓은 vertices 삽입

		HRESULT hr = Graphics::Get()->GetDevice()->CreateBuffer(&desc, &subData, &vertexBuffer);
		// 초기 데이터를 넣어줘야함 Immutable 이기 떄문임
		assert(SUCCEEDED(hr));

	}

	{   //vertexShader
		HRESULT hr = D3DCompileFromFile // MS에서 권장하는 컴파일러 형태
		(
			L"./Texture.hlsl", // 파일 이름을 넣어야 함
			nullptr, // 디파인 매크로 없음
			nullptr,
			"VS", // 최초로 실행되는 함수명
			"vs_5_0", // vertex 쉐이더 5
			0,
			0,
			&vsBlob, // 기계어로 번역된 binary 데이터를 담는 구조체
			nullptr
		);
		assert(SUCCEEDED(hr)); // 여기서 걸림

		hr = Graphics::Get()->GetDevice()->CreateVertexShader
		(
			vsBlob->GetBufferPointer(), // 버퍼의 첫 주소
			vsBlob->GetBufferSize(), // 버퍼 크기
			nullptr,
			&vertexShader // 버텍스 쉐이더 삽입
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
			// semantic, position을 0과 연결시켜라
			// Float 32 짜리 4개
			// 정점 데이터 1개 당 포지션 값 하나다 INPUT_PER_VERTEXT_DATA
			// 외에도 INTANCING이 있는데 3D에서 배움
			// 똑같은 모델을 쓰는 애들은 드로우콜을 한번에 뭉쳐서 실행, 드로우콜이 자원 소모가 제일 심함
			// 0 다음 sizeof, 포지션 크기만큼 띄워주고 컬러를 세팅하려는 의도
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
		// 월드 좌표계를 나타내는 행렬, 월드 내 값에 따라 크기, 회전, 이동을 변환할 것
		// 카메라의 시점 정보를 담는 행렬

		// 세번에 걸쳐 변환을 해야 로컬 > 월드 좌표계로 변함

		// 1. 로컬에서 ㅡ> 월드로 땡겨지면 1 x 1 정사각형이 됨

		// 결과물, 시점, 바라보는 방향, 위쪽 방향

		D3DXMatrixIdentity(&world);
		D3DXMatrixIdentity(&view);
		D3DXMatrixIdentity(&projection);

		D3DXMatrixLookAtLH(&view, &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 0, 1), &D3DXVECTOR3(0, 1, 0)); // LH left hand 좌표계

		D3DXMatrixOrthoLH(&projection, WinMaxWidth, WinMaxHeight, 0, 1); // 직교 투영

		// zn - z축 near, zf - z축 far 현재는 무필요

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

		// 2 / 1280 한 값이 나온다 콘솔에서
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

		// SRT ㅡ> Scale up ㅡ> Rotation ㅡ> Translation

		// cos sin
		// -sin cos
	}

	{ // constantBuffer 상수 버퍼
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.Usage = D3D11_USAGE_DYNAMIC; // CPU에서 쓰기 허용, 주로 매 프레임마다 CPU에서 변경이 있을 때 사용함
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
		desc.CullMode = D3D11_CULL_BACK; // 뒷면 삭제
		desc.FrontCounterClockwise = false; // 시계 방향이 앞면이 아님, 반시계가 정면

		HRESULT hr = Graphics::Get()->GetDevice()->CreateRasterizerState(&desc, &RS);
		assert(SUCCEEDED(hr));
	}

	// 자원 뷰의 종류?

	// 1. rtv : render target view // 렌더의 목적지 뷰
	// 2. srv : shader resource view // 셰이더에서 쓸 자원 뷰
	// 3. dsv : depth stencil view // 깊이 스텐셜 뷰
	// 4. uav : unordered acces view

	{ // SRV
		HRESULT hr = D3DX11CreateShaderResourceViewFromFile
		(
			Graphics::Get()->GetDevice(),
			L"./Tree.png", // FromFile에 A 붙이면 문자열 L 떼야함
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

		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; // 넘어가는 영역에 대한 처리 방식
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE; // 넘어간 부분을 border 컬러로 칠하라
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;

		desc.BorderColor[0] = 0.0f;
		desc.BorderColor[1] = 1.0f;
		desc.BorderColor[2] = 0.0f;
		desc.BorderColor[3] = 1.0f;

		desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS; // 크게 신경쓰지말자

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // 확대 축소가일어날 때 어떤 형태로 할 것인지
		// POINT면 축소 시 UV 좌표에 두개의 픽셀이 존재할 때, 한 쪽을 폐기
		// LINEAR면 평균값으로 섞음

		desc.MaxAnisotropy = 16; // 놓인 방향에 따라 달라지는 성질 값, 2D에선 필요 X

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

		desc.IndependentBlendEnable = false; // 각각의 렌더 타겟(최대 8개)들이 독립된 블렌드 설정을 가질 것인가
		// false면 하나를 다 똑같이 나눠씀
	
		desc.RenderTarget[0].BlendEnable = true; // 모두 0번 블렌드 옵션 쓸 거임
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // 알파가 0인 것에 대해 1로 채움 ㅡ> 그래서 검은 색이 됨
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 1 - SrcAlpha 하는 거 src가 1이면 배경이 0, src가 0이면 배경이 1
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; // 어떻게 섞을 지 src, dest 를 더하는 방식

		// (Src(받아온 png 텍스쳐) * srcBlend) BlendOP (Dest * destBlend)
		// ㄹㅇ 곱하기는 아니고...

		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 특정 채널만 출력
		
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
	// GPU는 열 우선 행렬을 사용하나, DX는 행 우선 행렬을 사용한다.
	// 전치행렬로 연산할 것

	D3DXMatrixTranspose(&cpuBuffer.world, &world);
	D3DXMatrixTranspose(&cpuBuffer.view, &view);
	D3DXMatrixTranspose(&cpuBuffer.projection, &projection);

	D3D11_MAPPED_SUBRESOURCE mappedSubResource;
	Graphics::Get()->GetDC()->Map
	(
		gpuBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD, // 쓰고 나서 정보 폐기함
		0,
		&mappedSubResource
	);

	memcpy(mappedSubResource.pData, &cpuBuffer, sizeof(TransformData));
	Graphics::Get()->GetDC()->Unmap(gpuBuffer, 0);

}

void Program::Render()
{
	UINT stride = sizeof(VertexTexture); // 보폭
	UINT offset = 0;

	// IA
	Graphics::Get()->GetDC()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	Graphics::Get()->GetDC()->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0); // 정점 당 1개만
	Graphics::Get()->GetDC()->IASetInputLayout(inputLayout);
	Graphics::Get()->GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 정점을 어떤 방식으로 이을 것?

	// POINTLIST 점
	// LINELIST 선
	// TRIANGLELIST 삼각형 ㅡ> 5개 조사과제

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
