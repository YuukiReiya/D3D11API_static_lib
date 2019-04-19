//�O���[�o��

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
	matrix g_World;			/*!< ���[���h�s�� */
	matrix g_View;			/*!< �r���[�s�� */
	matrix g_Proj;			/*!< �v���W�F�N�V�����s�� */
	float2 g_DivNum;		/*!< �e�N�X�`���̕����� */
	float2 g_useIndexUV;	/*!< ���������e�N�X�`���̕`��C���f�b�N�X */
	float4 g_Color;			/*!< �J���[ */
};
//�\����
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS( float4 Pos : POSITION ,float2 UV : TEXCOORD) 
{
	/*! �錾 */
	PS_INPUT Out;

	/*! ���W�v�Z */
	{
		//matrix wvp = mul(g_World, (mul(g_View, g_Proj)));	/*!< �ϊ� */
		//matrix m = transpose(wvp);						/*!< �]�u�s�� */

		//Out.Pos = mul(Pos, m);

		//================================================
		float4 pos = mul(Pos, g_World);
		pos = mul(pos, g_View);
		Out.Pos = mul(pos, g_Proj);

	}

	/*! UV�v�Z */
	{
		float x, y;
		x = UV.x * (1.0f / g_DivNum.x) + (g_useIndexUV.x / g_DivNum.x);
		y = UV.y * (1.0f / g_DivNum.y) + (g_useIndexUV.y / g_DivNum.y);

		Out.UV.x = x;
		Out.UV.y = y;

		//�؂蔲��
		//Out.UV = UV * float2(0.5f, 0.5f) + float2(0.2f, 0.2f);
	}

	return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS( PS_INPUT Input ) : SV_Target
{
	float4 color = g_Texture.Sample(g_Sampler, Input.UV);

	/*! �J���[�v�Z */
	{
		color *= g_Color;
	}

	/*! ���l�v�Z */
	{
		/*! ���l0�ȉ��̃s�N�Z���͕`�悵�Ȃ����A���t�@�e�X�g */
		if (color.a <= 0.0f)discard;
	}

	return (g_Color * g_Texture.Sample(g_Sampler, Input.UV));
}