//
//	スケルタルアニメーション用のシェーダー
//

// 定数バッファ(CPU側からの値受け取り場)
cbuffer global {
	matrix gW;  // 変換行列
	matrix gV;  // 変換行列
	matrix gP;  // 変換行列
	float4 gC;	// 色
};

// 頂点シェーダ
float4 VS(float4 Pos : POSITION) : SV_POSITION{
	Pos = mul(Pos,gW);
	Pos = mul(Pos, gV);
	Pos = mul(Pos, gP);
	return Pos;
}

// ピクセルシェーダ
float4 PS(float4 Pos : SV_POSITION) : SV_Target{
	//return float4(0.5, 0.5, 1.0, 1.0);
	return float4(1.0, 0.0, 0.0, 1.0);
}