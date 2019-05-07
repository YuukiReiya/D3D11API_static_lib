//�O���[�o��

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
	matrix g_World;			/*!< ���[���h�s�� */
	matrix g_View;			/*!< �r���[�s�� */
	matrix g_Proj;			/*!< �v���W�F�N�V�����s�� */
	float4 g_Color;
};
//�\����
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	//float2 UV : TEXCOORD;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS( float4 Pos : POSITION) 
{
	/*! �錾 */
	PS_INPUT Out;

	/*! ���W�v�Z */
	{
		matrix wvp = mul(g_World, (mul(g_View, g_Proj)));	/*!< �ϊ� */
		matrix m = transpose(wvp);						/*!< �]�u�s�� */

		Out.Pos = mul(Pos, m);
	}

	return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS( PS_INPUT Input ) : SV_Target
{
	//float4 color = g_Texture.Sample(g_Sampler, Input.UV);

	float2 tmp = {1,1};
float4 color = {1,0,0,1};//g_Texture.Sample(g_Sampler, tmp);

	/*! �J���[�v�Z */
	{
		color *= g_Color;
	}

	/*! ���l�v�Z */
	{
		/*! ���l0�ȉ��̃s�N�Z���͕`�悵�Ȃ����A���t�@�e�X�g */
		if (color.a <= 0.0f)discard;
	}

	return (g_Color * g_Texture.Sample(g_Sampler, tmp));
}