/*
	@file	AbstractVertex.h
	@date	2019/04/25
	@author	番場 宥輝
	@brief	頂点の抽象クラス
	@detail	最低限必要な頂点情報を保持している
*/
#pragma once
namespace Abstract{
	 class AbstractVertex abstract
	{
	public:

		/*!
			@brief	コンストラクタ
		*/
		AbstractVertex() :x(0), y(0), z(0), w(0), normal(Normal()), uv(UV()) {}

		/*!
			@brief	デストラクタ
		*/
		virtual ~AbstractVertex() { normal = Normal(), uv = UV(); };

		/*!
			@var	x,y,z,w
			@brief	3次元空間の座標に該当
			@detail	'w'は行列に合わせて宣言。
					基本的に "w = 1"
		*/
		double x, y, z, w;

		/*!
			@struct	Normal
			@brief	法線情報を含む構造体
		*/
		struct Normal
		{
			/*!
				@var	x,y,z
				@brief	法線ベクトル
			*/
			double x, y, z;

			/*!
				@brief	コンストラクタ
			*/
			Normal():x(0),y(0),z(0){}
		}normal;

		/*!
			@struct	UV
			@brief	UV情報を含む構造体
		*/
		struct UV
		{
			/*!
				@var	x,y
				@brief	UV座標
			*/
			float x, y;

			/*!
				@brief	コンストラクタ
			*/
			UV() :x(0), y(0) {}
		}uv;
	};
}
