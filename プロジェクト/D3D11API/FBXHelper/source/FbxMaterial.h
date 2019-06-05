#pragma once
#include "OriginalFormatMath.h"
//http://marupeke296.com/FBX_No7_TextureMaterial.html


class Material
{
public:
	Material();
	~Material();

	//	Lambert
	using Ambient = Math::FLOAT4;
	using Diffuse = Math::FLOAT4;
	using Emissive = Math::FLOAT4;
	using BumpMap = Math::FLOAT4;
	using Transparent = float;

	//	Phong
	//	@note	nullptr”»’è‚Å‚«‚é‚æ‚¤‚ÉptrŒ^
	using Specular =Math::FLOAT4* ;
	using Shininess = float*;
	using Reflection = float*;

	//	member
	Ambient ambient;
	Diffuse diffuse;
	Emissive emissive;
	BumpMap bumpMap;
	Transparent transparent;
	Specular specular = nullptr;
	Shininess shiniess = nullptr;
	Reflection reflection = nullptr;
private:

};

Material::Material()
{
}

Material::~Material()
{
}