/*!
	@file	TransitionTestScene.cpp
	@date	2019/03/29
	@author	�ԏ� �G�P
	@brief	�V�[���J�ڂ̃e�X�g�p�V�[��
*/
#include "TransitionTestScene.h"
#include <iostream>

using namespace std;

/*!
	@brief	�R���X�g���N�^
*/
TransitionTestScene::TransitionTestScene()
{
}

/*!
	@fn		Initialize
	@brief	����������
*/
void TransitionTestScene::Initialize()
{
	cout << "test init" << endl;
}

/*!
	@fn		Finalize
	@brief	�j������
*/
void TransitionTestScene::Finalize()
{
	cout << "test destroy" << endl;
}

/*!
	@fn		Update
	@brief	�X�V����
*/
bool s = false;
void TransitionTestScene::Update()
{
	if (!s) {
		s = true;
		cout << "test update" << endl;
	}
}

/*!
	@fn		Render
	@brief	�`�揈��
*/
void TransitionTestScene::Render()
{

}