#pragma once

//API - Application Programming Interface
// 응용프로그램을 프로그래밍하기위한 인터페이스의 집합

//WinAPI - 윈도우 API

//DirectX
// GPU Rendering
// 게임그래픽에서 vertex, 정점들을 사용하여 면을 그리고 그러한 면이 모여서 오브젝트가 그려짐
// 매번 지우고 다시 그리고하는 작업이 반복되므로 정점이 많아질수록 연산이 더욱 많아지고 더욱 고성능의 GPU가 필요해짐
// Rendering - 정점들을 그리는 작업

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
	//I -> ComInterface - COM Interface(component object model interface), 사용자가 직접 할당, 삭제가 불가능하므로 지정한 함수를 호출해야함 - ex) 삭제시 Release()
	ID3D11Device* device = nullptr; // Device - 자원생산역할
	ID3D11DeviceContext* deviceContext = nullptr; // 렌더링 파이프라인을 통해 렌더링을 진행
	IDXGISwapChain* swapChain = nullptr; // 더블버퍼링에 사용, 버퍼의 포인터를 계속 바꿈으로써 더블버퍼링을 적용 
	ID3D11RenderTargetView* rtv = nullptr; // 그림을 그리는 영역, 도화지 역할
	D3D11_VIEWPORT viewport = { 0 }; // 보여질 화면 영역
	D3DXCOLOR clearColor = 0xff555566; // 정규화된 색깔
};

//렌더링 파이프라인 과정
// IA - VS - RS - PS - OM

//정규화 -1 ~ 1 , unsigned정규화 0 ~ 1