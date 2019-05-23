/*
	@file	FBXUtility.h
	@date	2019/04/19
	@author	�ԏ� �G�P
	@brief	FBX_SDK��Manager,Scene�������w���p�[�N���X
	@detail	�V���O���g��
*/
#pragma once
#include "AbstractMesh.h"

namespace FBX {
	class FBXUtility
	{
	public:

		/*!
			@fn		Setup
			@brief	������
			@detail	�����o�̃C���X�^���X���Ə�����
		*/
		static void Setup();

		/*!
			@fn			Load
			@brief		.fbx�̃��[�h
			@detail		��O�����L��(Release�r���h�Ŗ߂�l)
						��Debug�r���h�ł�exit�֐����g���������I��
			@param[in]	�ǂݍ��ރt�@�C���̃p�X
			@return		true:���� false:���s
		*/
		static bool Load(std::string path);

		/*!
			@fn			Load
			@brief		.fbx�̃��[�h
			@detail		��O�����L��(Release�r���h�Ŗ߂�l)
						��Debug�r���h�ł�exit�֐����g���������I��
			@param[in]	�ǂݍ��ރt�@�C���̃p�X
			@return		true:���� false:���s
		*/
		static bool Load(std::string path, std::string outputPath, Abstract::AbstractMesh*mesh);

		static void SetupVertex(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void SetupIndexBuffer(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void SetupNormal(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void SetupUV(fbxsdk::FbxMesh*fbxMesh, Abstract::AbstractMesh*mesh);

		static void Destroy();

		/*!
			@fn		Triangulate
			@brief	�V�[�����̃��f���̎O�p�|���S����
			@return	true:���� false:���s
		*/
		static bool Triangulate();

	private:
		/*!
			@brief	�R���X�g���N�^
		*/
		FBXUtility() = delete;

		/*!
			@brief	�f�X�g���N�^
		*/
		~FBXUtility() = delete;


		/*!
			@var	m_pManager
			@brief	FbxManager�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxManager*>		m_pManager;

		/*!
			@var	m_pIOsetting
			@brief	FbxIOSettings�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxIOSettings*>	m_pIOsetting;

		/*!
			@var	m_pScene
			@brief	FbxScene�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxScene*>		m_pScene;

		/*!
			@var	m_pImporter
			@brief	FbxImporter�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxImporter*>	m_pImporter;


		////
		static fbxsdk::FbxManager* pManager;
		static fbxsdk::FbxScene* pScene;
		static fbxsdk::FbxImporter* pImporter;
		static fbxsdk::FbxIOSettings*pSettings;

	};

}