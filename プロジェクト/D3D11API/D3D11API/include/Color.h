/*
	@file	Color.h
	@date	2018/11/26
	@author	�ԏ� �G�P
	@brief	�J���[�N���X
*/
#pragma once
#include <DirectXMath.h>

/*!
	@brief	�J���[�N���X
	@detail	�����o��( 0.0f �` 1.0f )�܂ł͈̔͂Ɏ��߂�
*/
class Color
{
public:
	/*!
		@brief	��R���X�g���N�^
	*/
	Color();

	/*!
		@brief		�����t���R���X�g���N�^
		@param[in]	�ꎞ�ϐ�
	*/
	Color(Color&&color);

	/*!
		@brief		�����t���R���X�g���N�^
		@param[in]	R( 0.0f �` 1.0f )
		@param[in]	G( 0.0f �` 1.0f )
		@param[in]	B( 0.0f �` 1.0f )
		@param[in]	A( 0.0f �` 1.0f )
	*/
	Color(float r, float g, float b, float a = 1.0f);

	/*!
		@brief		�����t���R���X�g���N�^
		@param[in]	FLOAT4�^����J���[�^�֕ϊ�
	*/
	Color(DirectX::XMFLOAT4 color);

	/*!
		@brief	�f�X�g���N�^
		@detail	��f�X�g���N�^
	*/
	~Color();

	/*!
		@def	�ϐ��̕ʖ���`�}�N��
		@detail	�Q�l:https://qiita.com/tyanmahou/items/7fbc5c0638ef9fd4c467
	*/
	#pragma region def
#define PropertyAlias(base,name,...)\
__declspec(property(get = Get_##base, put = Set_##base)) __VA_ARGS__ name;\
__VA_ARGS__ & Get_##base(){return base;}\
void Set_##base(const __VA_ARGS__& value){base = value;}
	#pragma endregion

	/*!
		@fn		�J���[�̃Q�b�^�[
		@brief	�J���[��XMFLOAT3�^�Ŏ擾
		@detail	�J���[��RGB��������XMFLOAT3�^���擾����
	*/
	DirectX::XMFLOAT3 GetRGB()const;

	/*!
		@fn		�J���[�̃Q�b�^�[
		@brief	�J���[��XMFLOAT4�^�Ŏ擾
		@detail	�J���[��RGBA��������XMFLOAT4�^���擾����
	*/
	DirectX::XMFLOAT4 GetRGBA()const;

	/*!
		@var	c_MinValue
		@brief	��肤��l�̍ŏ��l
	*/
	static constexpr float c_MinValue = 0.0f;

	/*!
		@var	c_MinValue
		@brief	��肤��l�̍ő�l
	*/
	static constexpr float c_MaxValue = 1.0f;

	/*!
		@var	�����o�ϐ�
		@brief	x = r , y = g , z = b ,w = a
	*/
	float x, y, z, w;

	/*!
		@brief	�ʖ���`(��)
		@detail	x��r�Ɗ֘A�t����
	*/
	PropertyAlias(x, r, float);

	/*!
		@brief	�ʖ���`(��)
		@detail	y��g�Ɗ֘A�t����
	*/
	PropertyAlias(y, g, float);

	/*!
		@brief	�ʖ���`(��)
		@detail	z��b�Ɗ֘A�t����
	*/
	PropertyAlias(z, b, float);

	/*!
		@brief	�ʖ���`(����)
		@detail	w��a�Ɗ֘A�t����
	*/
	PropertyAlias(w, a, float);

	/*!
		@brief	������Z�q
	*/
	Color &operator=(const Color &color) {
		this->x = color.x;
		this->y = color.y;
		this->z = color.z;
		this->w = color.w;
		return (*this);
	}
};