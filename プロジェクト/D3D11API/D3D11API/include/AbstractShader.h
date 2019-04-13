/*
	@file	AbstractShader.h
	@date	2019/04/05
	@author	番場 宥輝
	@brief	シェーダーの抽象クラス
	@note	描画クラス設計見直しに際し、更新していく
*/
#pragma once
#include <d3d11.h>
#include <memory>
#include <string>

/*! Direct3D11関連 */
namespace D3D11 {

	/*! 描画関連の名前空間に含める */
	namespace Graphic {

		/*!
			@brief	シェーダーの抽象クラス
		*/
		class AbstractShader abstract {
		public:
			AbstractShader(){}
			virtual HRESULT Setup();
			virtual HRESULT DynamicSetup();//動的

			inline std::shared_ptr<ID3D11InputLayout>GetInputLayout()	const { return m_pVertexLayout; }
			inline std::shared_ptr<ID3D11VertexShader>GetVertexShader()	const { return m_pVertexShader; }
			inline std::shared_ptr<ID3D11PixelShader>GetPixelShader()	const { return m_pPixelShader; };
			inline const std::shared_ptr<ID3D11Buffer>GetConstantBuffer()		const { return m_pConstantBuffer; }
		protected:
			static HRESULT PreCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob**ppBlob);
			static HRESULT DynamicCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob**ppBlob);

			/*!
				@var	m_pVertexLayout
				@brief	頂点レイアウト
			*/
			std::shared_ptr<ID3D11InputLayout>	m_pVertexLayout;
			std::shared_ptr<ID3D11VertexShader>	m_pVertexShader;	/*< 頂点シェーダー */
			std::shared_ptr<ID3D11PixelShader>	m_pPixelShader;		/*< ピクセルシェーダー */
			std::shared_ptr<ID3D11Buffer>		m_pConstantBuffer;	/*< コンスタントバッファー */

			static HRESULT CreateVertexShader(ID3DBlob*pBlob, ID3D11VertexShader*pVertexShader);
			static HRESULT CreatePixelShader(ID3DBlob*pBlob, ID3D11PixelShader*pPixelShader);
		};
	}
}