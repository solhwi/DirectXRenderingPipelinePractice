struct VertexInput // ���� 1���� �������ִ� ����
{
	float4 position : POSITION0;
	float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float4 color : COLOR0;
};

cbuffer TransformBuffer : register(b0) // ���� ���, ������ ������ b0 ~ b13���� ����
{
    matrix _world;
    matrix _view;
    matrix _proj;
}

PixelInput VS(VertexInput input) // ���� ��ȯ�� ���⼭
{
	PixelInput output;
	// ��ġ ���� * ���ϴ� �����̽� ��� ��> ���ϴ� �����̽������� ����
    
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