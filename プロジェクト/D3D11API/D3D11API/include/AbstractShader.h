/*
	@file	AbstractShader.h
	@date	2019/04/05
	@author	�ԏ� �G�P
	@brief	�V�F�[�_�[�̒��ۃN���X
	@note	�`��N���X�݌v�������ɍۂ��A�X�V���Ă���
*/
#pragma once
#include <d3d11.h>
#include <memory>
#include <string>

/*! Direct3D11�֘A */
namespace D3D11 {

	/*! �`��֘A�̖��O��ԂɊ܂߂� */
	namespace Graphic {

		/*!
			@brief	�V�F�[�_�[�̒��ۃN���X
		*/
		class AbstractShader abstract {
		public:
			AbstractShader(){}
			virtual HRESULT Setup();
			virtual HRESULT DynamicSetup();//���I

			inline std::shared_ptr<ID3D11InputLayout>GetInputLayout()	const { return m_pVertexLayout; }
			inline std::shared_ptr<ID3D11VertexShader>GetVertexShader()	const { return m_pVertexShader; }
			inline std::shared_ptr<ID3D11PixelShader>GetPixelShader()	const { return m_pPixelShader; };
			inline const std::shared_ptr<ID3D11Buffer>GetConstantBuffer()		const { return m_pConstantBuffer; }
		protected:
			static HRESULT PreCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob**ppBlob);
			static HRESULT DynamicCompile(std::string path, std::string funcName, std::string profileName, ID3DBlob**ppBlob);

			/*!
				@var	m_pVertexLayout
				@brief	���_���C�A�E�g
			*/
			std::shared_ptr<ID3D11InputLayout>	m_pVertexLayout;
			std::shared_ptr<ID3D11VertexShader>	m_pVertexShader;	/*< ���_�V�F�[�_�[ */
			std::shared_ptr<ID3D11PixelShader>	m_pPixelShader;		/*< �s�N�Z���V�F�[�_�[ */
			std::shared_ptr<ID3D11Buffer>		m_pConstantBuffer;	/*< �R���X�^���g�o�b�t�@�[ */

			static HRESULT CreateVertexShader(ID3DBlob*pBlob, ID3D11VertexShader*pVertexShader);
			static HRESULT CreatePixelShader(ID3DBlob*pBlob, ID3D11PixelShader*pPixelShader);
		};
	}
}