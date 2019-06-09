/*
	@file	Material.h
	@date	2019/06/06
	@author	�ԏ� �G�P
	@brief	�}�e���A��
*/
#include "stdafx.h"
#include "Material.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@brief	�R���X�g���N�^
*/
Material::Material()
{
	m_pTexture = std::make_shared<Texture>();
	m_pShared = std::make_shared<Material*>(this);
}

/*!
	@brief	�f�X�g���N�^
*/
Material::~Material()
{
	m_pTexture.reset();
}