#include"../MyLibrary/PostEffectShaderHeader.hlsli"

Texture2D<float4> tex:register(t0);
SamplerState smp:register(s0);
float4 PSmain(VSOutput input) : SV_TARGET
{
	float4 ret = float4(0,0,0,0);
	float num = 0.0025f;
	ret += tex.Sample(smp, float2(input.uv.x - num, input.uv.y + num));
	ret += tex.Sample(smp, float2(input.uv.x - num, input.uv.y ));
	ret += tex.Sample(smp, float2(input.uv.x - num, input.uv.y - num));
	
	ret += tex.Sample(smp, float2(input.uv.x , input.uv.y + num));
	ret += tex.Sample(smp, float2(input.uv.x , input.uv.y));
	ret += tex.Sample(smp, float2(input.uv.x , input.uv.y - num));

	ret += tex.Sample(smp, float2(input.uv.x + num, input.uv.y + num));
	ret += tex.Sample(smp, float2(input.uv.x + num, input.uv.y));
	ret += tex.Sample(smp, float2(input.uv.x + num, input.uv.y - num));

	ret /= 9;
	return ret;
}