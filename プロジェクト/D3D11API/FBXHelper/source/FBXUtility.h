/*
	@file	FBXUtility.h
	@date	2019/04/19
	@author	�ԏ� �G�P
	@brief	FBX_SDK��Manager,Scene�������w���p�[�N���X
	@detail	�V���O���g��
*/
#pragma once
//#include <fbxsdk/core/fbxmanager.h>
//#include <fbxsdk>

namespace FBX {
	class FBXUtility
	{
	public:

		static void Setup();
		static bool Load(std::string path);
		static bool Triangulate();
	private:
		~FBXUtility() = delete;
		FBXUtility() = delete;

		static std::shared_ptr<fbxsdk::FbxManager*>		m_pManager;
		static std::shared_ptr<fbxsdk::FbxIOSettings*>	m_pIOsetting;
		static std::shared_ptr<fbxsdk::FbxScene*>		m_pScene;
		static std::shared_ptr<fbxsdk::FbxImporter*>	m_pImporter;
	};

}