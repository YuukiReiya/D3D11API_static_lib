//グローバル
cbuffer global
{
	matrix g_World;			/*!< ワールド行列 */
	matrix g_View;			/*!< ビュー行列 */
	matrix g_Proj;			/*!< プロジェクション行列 */
	float4 g_Color;
};
//構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float4 Col : COLOR0;
};

//
//
//バーテックスシェーダー
PS_INPUT VS( float4 Pos : POSITION,float Col : COLOR0) 
{
	/*! 宣言 */
	PS_INPUT Out;

	/*! 座標計算 */
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
//ピクセルシェーダー
float4 PS( PS_INPUT Input ) : SV_Target
{
	float4 cr= { 1,0,0,1};
	return Input.Col;
}