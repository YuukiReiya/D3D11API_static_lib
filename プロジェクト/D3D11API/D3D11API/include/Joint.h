/*
	@file	Joint.h
	@date	2019/08/14
	@author	番場 宥輝
	@brief	関節の構造体定義
*/
#pragma once
#include <stdint.h>

namespace D3D11
{
	namespace Graphic {

		/*!
			@struct	Joint
			@brief	関節
		*/
		struct Joint
		{
			/*!
				@var	index
				@brief	インデックス
				@detail	ジョイントのインデックス
			*/
			uint32_t index;

			/*!
				@var	weight
				@brief	重み
			*/
			float weight;

			/*!
				@operator ==
				@brief	等演算子
				@detail	左辺 == 右辺 でメンバの完全一致かどうか返す。
				@return	true:メンバが完全一致 false:メンバが一つでも違う
				@note	STLのアルゴリズム用に定義
			*/
			bool operator==(Joint self)
			{
				return this->index == self.index&&this->weight == self.weight;
			}
		};
	}
}