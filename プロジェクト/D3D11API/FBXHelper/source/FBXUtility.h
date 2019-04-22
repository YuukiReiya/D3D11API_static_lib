/*
	@file	FBXUtility.h
	@date	2019/04/19
	@author	番場 宥輝
	@brief	FBX_SDKのManager,Sceneを扱うヘルパークラス
	@detail	シングルトン
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