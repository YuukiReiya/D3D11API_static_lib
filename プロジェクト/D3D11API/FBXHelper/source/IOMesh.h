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
			@fn			OutputMesh
			@brief		独自形式のスキンメッシュを出力
			@param[in]	出力ファイルパス
			@param[in]	頂点インデックス
			@param[in]	頂点情報
		*/
		static void OutputMesh(std::string filePath, std::vector<uint32_t>indices, std::vector<D3D11::Graphic::SkinnedVertex>vertices);

		/*!
			@fn			OutputAnimation
			@brief		独自形式のアニメーションを出力
			@param[in]	出力ファイルパス
			@param[in]	アニメーションクリップ
		*/
		static void OutputAnimation(std::string filePath, API::AnimationClip clips);

		/*!
			@fn			OutputTextureName
			@brief		使用されているテクスチャ名を.iniファイルとして書き出す
			@param[in]	出力ファイルパス
			@param[in]	使用されているテクスチャの可変長配列
		*/
		static void OutputTextureName(std::string filePath, std::vector<std::string> textureNames);
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

		/*!
			@brief	テクスチャのパス名が入る拡張子(.ini)
			@detail	別途読み込みできるように設定ファイル(.ini)として書き出す。
		*/
		static constexpr std::string_view c_IniExtension = "ini";
	};
}
