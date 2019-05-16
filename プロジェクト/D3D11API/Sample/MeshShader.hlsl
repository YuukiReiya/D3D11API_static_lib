//�O���[�o��
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
	float4 Col : COLOR0;
};

//
//
//�o�[�e�b�N�X�V�F�[�_�[
PS_INPUT VS( float4 Pos : POSITION,float Col : COLOR0) 
{
	/*! �錾 */
	PS_INPUT Out;

	/*! ���W�v�Z */
	{
		Pos = mul(Pos, g_World);
		Pos = mul(Pos, g_View);
		Out.Pos = mul(Pos, g_Proj);
	}

	float4 tmp = { 0, 0, 0, 0 };
	Out.Pos = tmp;
	Out.Col = Col;
	return Out;
}
//
//
//�s�N�Z���V�F�[�_�[
float4 PS( PS_INPUT Input ) : SV_Target
{
	float4 cr= { 1,0,0,1};
	return Input.Col;
}