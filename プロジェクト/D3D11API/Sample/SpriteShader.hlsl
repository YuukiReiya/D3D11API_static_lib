//グローバル

Texture2D g_Texture : register(t0);
SamplerState g_Sampler : register(s0);

cbuffer global
{
	matrix g_World;			/*!< ワールド行列 */
	matrix g_View;			/*!< ビュー行列 */
	matrix g_Proj;			/*!< プロジェクション行列 */
	float2 g_DivNum;		/*!< テクスチャの分割数 */
	float2 g_useIndexUV;	/*!< 分割したテクスチャの描画インデックス */
	float4 g_Color;			/*!< カラー */
};
//構造体
struct PS_INPUT
{
	float4 Pos : SV_POSITION;
	float2 UV : TEXCOORD;
};

//
//
//バーテックスシェーダー
PS_INPUT VS( float4 Pos : POSITION ,float2 UV : TEXCOORD) 
{
	/*! 宣言 */
	PS_INPUT Out;

	/*! 座標計算 */
	{
		//matrix wvp = mul(g_World, (mul(g_View, g_Proj)));	/*!< 変換 */
		//matrix m = transpose(wvp);						/*!< 転置行列 */

		//Out.Pos = mul(Pos, m);

		//================================================
		float4 pos = mul(Pos, g_World);
		pos = mul(pos, g_View);
		Out.Pos = mul(pos, g_Proj);

	}

	/*! UV計算 */
	{
		float x, y;
		x = UV.x * (1.0f / g_DivNum.x) + (g_useIndexUV.x / g_DivNum.x);
		y = UV.y * (1.0f / g_DivNum.y) + (g_useIndexUV.y / g_DivNum.y);

		Out.UV.x = x;
		Out.UV.y = y;

		//切り抜き
		//Out.UV = UV * float2(0.5f, 0.5f) + float2(0.2f, 0.2f);
	}

	return Out;
}
//
//
//ピクセルシェーダー
float4 PS( PS_INPUT Input ) : SV_Target
{
	float4 color = g_Texture.Sample(g_Sampler, Input.UV);

	/*! カラー計算 */
	{
		color *= g_Color;
	}

	/*! α値計算 */
	{
		/*! α値0以下のピクセルは描画しない→アルファテスト */
		if (color.a <= 0.0f)discard;
	}

	return (g_Color * g_Texture.Sample(g_Sampler, Input.UV));
}