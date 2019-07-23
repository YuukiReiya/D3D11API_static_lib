/*
	@file	IOMesh.h
	@date	2019/05/30
	@author	番場 宥輝
	@brief	メッシュデータのの入出力関係
	@detail	シングルトン
*/
#pragma once
#include <fstream>
#include <string_view>
#include <iostream>
#include "Mesh.h"
#include <SkeletonMesh.h>
#include <AnimationClip.h>

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

		//static void Output(std::string directoryPath, std::string fileName, Mesh mesh);

		static void Output(std::string fileName,SkeletonMesh mesh,AnimationClip clip);
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
	};
}
