struct VertexInput // 정점 1개가 가지고있는 정보
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
    float2 uv : TEXCOORD0;
};

cbuffer TransformBuffer : register(b0) // 슬롯 등록, 순서를 정해줌 
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
    
    output.uv = input.uv;
	
	return output;
}

Texture2D sourceTex : register(t0);

SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
    float4 color = sourceTex.Sample(samp, input.uv); // 픽셀에서의 색 <- uv 비율 좌표를 샘플링하여 대입, samp는 기본값
    
    //clip(color.a - 0.9); // 받은 데이터가 0보다 작으면 폐기하는 함수
    
    //if(color.a < 0.01f) // 인풋의 알파가 0보다 작으면 폐기
    //{
    //    discard;
    //}
    
    return color;
}