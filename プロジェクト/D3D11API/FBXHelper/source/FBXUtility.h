/*
	@file	FBXUtility.h
	@date	2019/04/19
	@author	�ԏ� �G�P
	@brief	FBX_SDK��Manager,Scene�������w���p�[�N���X
	@detail	�V���O���g��
*/
#pragma once
//#include <fbxsdk/core/fbxmanager.h>
#include <fbxsdk.h>
#include <memory>

class FBXUtility
{
public:
	~FBXUtility() {};
	FBXUtility() {};

	static void Setup();
private:
	//~FBXUtility() = delete;
	//FBXUtility() = delete;
	
	//static std::shared_ptr<fbxsdk::FbxManager*>	m_pManager;
	//static std::shared_ptr<FbxScene*>	m_pScene;

	static fbxsdk::FbxManager*m_p;

	//static std::shared_ptr<fbxim>
};

class test
{
public :
	void a() {};
	static void b();
};