/*
	@file	MeshReadHelper.h
	@date	2019/05/05
	@author	番場 宥輝
	@brief	外部ファイルのメッシュ読み込み
	@detail	シングルトン
*/
#pragma once
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <fstream>
#include "MeshVertex.h"
#include "FrameAnimationMesh.h"

namespace D3D11 {
	namespace Helper {
		class MeshReadHelper
		{
		public:
			/*!
				@struct	ReadBuffer
				@brief	読み込んだバッファ
			*/
			struct ReadBuffer
			{
				/*!
					@var	vertices
					@brief	頂点
					@TODO	外部ファイルにはdouble型で書き込まれているため、float型にキャストする
				*/
				std::vector<Graphic::MeshVertex>vertices;

				/*!
					@var	indices
					@brief	頂点インデックス
				*/
				std::vector<uint32_t>indices;
			};

			/*!
				@fn			Read
				@brief		外部ファイルの読み込み
				@param[in]	読み込みパス
				@return		成功:情報が格納された構造体
							失敗:空の構造体
			*/
			static ReadBuffer Read(std::string path);

			/*!
				@fn		ReadFrameAnim
				@brief	フレームアニメーションメッシュの外部ファイル読み込み
				@param[in]	読み込みパス
				@return		成功:情報が格納された構造体
							失敗:空の構造体
			*/
			static D3D11::Graphic::FrameAnimationMesh ReadFrameAnim(std::string path);
		private:

			/*!
				@brief	コンストラクタ
			*/
			MeshReadHelper() = delete;

			/*!
				@brief	デストラクタ
			*/
			~MeshReadHelper() = delete;

		};
	}
}