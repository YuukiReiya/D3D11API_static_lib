//
//	�X�P���^���A�j���[�V�����p�̃V�F�[�_�[
//

// �萔�o�b�t�@(CPU������̒l�󂯎���)
cbuffer global {
	matrix gW;  // �ϊ��s��
	matrix gV;  // �ϊ��s��
	matrix gP;  // �ϊ��s��
	float4 gC;	// �F
};

// ���_�V�F�[�_
float4 VS(float4 Pos : POSITION) : SV_POSITION{
	Pos = mul(Pos,gW);
	Pos = mul(Pos, gV);
	Pos = mul(Pos, gP);
	return Pos;
}

// �s�N�Z���V�F�[�_
float4 PS(float4 Pos : SV_POSITION) : SV_Target{
	//return float4(0.5, 0.5, 1.0, 1.0);
	return float4(1.0, 0.0, 0.0, 1.0);
}