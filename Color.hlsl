struct VertexInput // 정점 1개가 가지고있는 정보
{
	float4 position : POSITION0;
	float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float4 color : COLOR0;
};

cbuffer TransformBuffer : register(b0) // 슬롯 등록, 순서를 정해줌 b0 ~ b13까지 있음
{
    matrix _world;
    matrix _view;
    matrix _proj;
}

PixelInput VS(VertexInput input) // 공간 변환은 여기서
{
	PixelInput output;
	// 위치 벡터 * 원하는 스페이스 행렬 ㅡ> 원하는 스페이스에서의 벡터
    
    output.position = mul(input.position, _world); 
    output.position = mul(output.position, _view);
    output.position = mul(output.position, _proj);
    
    output.color = input.color;
	
	return output;
}

float4 PS(PixelInput input) : SV_Target
{
	return input.color;
}