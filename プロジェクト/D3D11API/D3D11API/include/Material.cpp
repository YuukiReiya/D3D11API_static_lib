/*
	@file	Material.h
	@date	2019/06/06
	@author	番場 宥輝
	@brief	マテリアル
*/
#include "stdafx.h"
#include "Material.h"

/*!
	@brief	usingディレクティブ
	@using	API
*/
using namespace API;

/*!
	@brief	コンストラクタ
*/
Material::Material()
{
	m_pTexture = std::make_shared<Texture>();
	m_pShared = std::make_shared<Material*>(this);
}

/*!
	@brief	デストラクタ
*/
Material::~Material()
{
	m_pTexture.reset();
}