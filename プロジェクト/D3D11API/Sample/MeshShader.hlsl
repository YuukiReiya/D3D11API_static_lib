//グローバル

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

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
	//float2 UV : TEXCOORD;
};

//
//
//バーテックスシェーダー
PS_INPUT VS( float4 Pos : POSITION) 
{
	/*! 宣言 */
	PS_INPUT Out;

	/*! 座標計算 */
	{
		matrix wvp = mul(g_World, (mul(g_View, g_Proj)));	/*!< 変換 */
		matrix m = transpose(wvp);						/*!< 転置行列 */

		Out.Pos = mul(Pos, m);
	}

	return Out;
}
//
//
//ピクセルシェーダー
float4 PS( PS_INPUT Input ) : SV_Target
{
	//float4 color = g_Texture.Sample(g_Sampler, Input.UV);

	float2 tmp = {1,1};
float4 color = {1,0,0,1};//g_Texture.Sample(g_Sampler, tmp);

	/*! カラー計算 */
	{
		color *= g_Color;
	}

	/*! α値計算 */
	{
		/*! α値0以下のピクセルは描画しない→アルファテスト */
		if (color.a <= 0.0f)discard;
	}

	return (g_Color * g_Texture.Sample(g_Sampler, tmp));
}