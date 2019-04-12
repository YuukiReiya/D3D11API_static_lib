#include "stdafx.h"
#include "RenderQueue.h"
#include "MemoryLeaks.h"
#include "MyGame.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	API
*/
using namespace API;

/*!
	@fn		Render
	@brief	�`��I�u�W�F�N�g�̈ꊇ�`��
	@detail	�ς܂ꂽ�`��I�u�W�F�N�g��Draw()�֐����ꊇ�Ăяo��
*/
void D3D11::Graphic::RenderQueue::Render()
{
	//	�D��x:��
	Loop(m_pLowRenders);

	//	�D��x:��
	Loop(m_pHighRenders);

	//	�s�����`��I�u�W�F�N�g�̕`��
	Loop(m_pOpacityRenders);
}

/*!
	@fn			Add
	@brief		�`��R���e�i�ւ̒ǉ�
	@param[in]	�`��I�u�W�F�N�g
	@param[in]	�`��̗D��x
*/
void D3D11::Graphic::RenderQueue::Add(API::AbstractRender* render, Priority priority)
{
	switch (priority)
	{
		//	�s����
		case D3D11::Graphic::RenderQueue::Opacity:
			m_pOpacityRenders.push_back(render);
		break;

		//	�D��x:��
		case D3D11::Graphic::RenderQueue::High:
			m_pHighRenders.push_back(render);
		break;

		//	�D��x:��
		case D3D11::Graphic::RenderQueue::Low:
			m_pLowRenders.push_back(render);
		break;

		//	��O
		default:
			std::string error = "RenderQueue�̃R���e�i�ɒǉ����邱�Ƃ��o���܂���ł����B";
			ErrorLog(error);
		break;
	}
}

/*!
	@fn		Clear
	@brief	�`��R���e�i�̃N���A
*/
void D3D11::Graphic::RenderQueue::Clear()
{
	m_pOpacityRenders.clear();
	m_pLowRenders.clear();
	m_pHighRenders.clear();
}

/*!
	@fn			Loop
	@brief		�`�惋�[�v
	@param[in]	�`��I�u�W�F�N�g�̃R���e�i
*/
void D3D11::Graphic::RenderQueue::Loop(std::vector<API::AbstractRender*>& renders)
{
	//	�R���e�i�Q�Ɨp�̃C�e���[�^
	auto it = renders.rbegin();

	//	���[�v
	while (true)
	{
		//	�R���e�i�͋�H
		if (renders.empty())		{ break; }

		//	�Ō�̗v�f�H
		if (renders.rend() == it)	{ break; }

		//	�v�f�Q��
		const auto render = *it;

		//	����
		render->Draw();

		//	�C�e���[�^�X�V
		it++;
	}
}
