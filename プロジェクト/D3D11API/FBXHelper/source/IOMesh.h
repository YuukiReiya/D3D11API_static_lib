/*
	@file	IOMesh.h
	@date	2019/05/30
	@author	番場 宥輝
	@brief	メッシュデータのの入出力関係
	@detail	シングルトン

	TODO:出力関数に調整必須(変数名、関数分けetc.)
*/
#pragma once
#include <fstream>
#include <string_view>
#include <iostream>
#include "Mesh.h"
#include <AnimationClip.h>
#include <SkinnedVertex.h>
#include <Joint.h>

namespace Utility{
	class IOMesh
	{
	public:
		/*!
			@fn			Delete
			@brief		yfmファイル削除
			@param[in]	ファイルのディレクトリ
			@param[in]	ファイル名(※拡張子除く)
		*/
		static void Delete(std::string directoryPath, std::string fileName);

		/*!
			@fn			Output
			@brief		独自形式の出力
			@param[in]	ファイルのディレクトリ
			@param[in]	ファイル名(※拡張子除く)
			@param[in]	メッシュ情報
			@TODO		出力形式の見直しと関数分けを行う...
		*/
		static void Output(std::string directoryPath, std::string fileName, Mesh mesh);

		/*!
			@fn			OutputSkinMesh
			@brief		独自形式のスキンメッシュを出力
			@param[in]	ファイルパス
			@param[in]	頂点インデックス
			@param[in]	頂点情報
			@param[in]	ジョイント
		*/
		//static void OutputSkinMesh(std::string filePath, std::vector<uint32_t>indices, std::vector<D3D11::Graphic::SkinnedVertex>vertices, std::vector<D3D11::Graphic::Joint>joints);

		/*!
			@fn			OutputSkinMesh
			@brief		独自形式のスキンメッシュを出力
			@param[in]	ファイルパス
			@param[in]	頂点インデックス
			@param[in]	頂点情報
		*/
		static void OutputSkinMesh(std::string filePath, std::vector<uint32_t>indices, std::vector<D3D11::Graphic::SkinnedVertex>vertices);

		static void OutputAnimation(std::string filePath, API::AnimationClip clips);
		static void Write(std::vector<DirectX::XMMATRIX>mat);
	private:
		/*!
			@brief	コンストラクタ削除
		*/
		IOMesh() = delete;

		/*!
			@brief	デストラクタ削除
		*/
		~IOMesh() = delete;

		/*!
			@var	c_Delimiter
			@brief	拡張子の区切り文字
		*/
		static constexpr std::string_view c_Delimiter = ".";

		/*!
			@brief	独自形式の拡張子
			@detail	y(uuki)f(ormat)m(esh)
		*/
		static constexpr std::string_view c_Extension = "yfm";

		/*!
			@brief	アニメーションデータの独自拡張子
			@detail	a(nimation)c(lip)
		*/
		static constexpr std::string_view c_AnimExtension = "ac";
	};
}
