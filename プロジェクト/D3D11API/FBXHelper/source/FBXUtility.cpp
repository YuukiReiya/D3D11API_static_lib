#include "Common.h"
#include "FBXUtility.h"
#include <fbxsdk.h>
#include <iostream>

using namespace std;
using namespace FBX;

shared_ptr<fbxsdk::FbxManager*>		FBXUtility::m_pManager		= nullptr;
shared_ptr<fbxsdk::FbxIOSettings*>	FBXUtility::m_pIOsetting	= nullptr;
shared_ptr<fbxsdk::FbxScene*>		FBXUtility::m_pScene		= nullptr;
shared_ptr<fbxsdk::FbxImporter*>	FBXUtility::m_pImporter		= nullptr;


void FBXUtility::Setup()
{
	m_pManager		= make_shared<fbxsdk::FbxManager*>();
	m_pScene		= make_shared<fbxsdk::FbxScene*>();
	m_pIOsetting	= make_shared<fbxsdk::FbxIOSettings*>();
	m_pImporter		= make_shared<fbxsdk::FbxImporter*>();

	*m_pManager		= fbxsdk::FbxManager::Create();
	*m_pIOsetting	= fbxsdk::FbxIOSettings::Create(*m_pManager.get(), IOSROOT);
	(*m_pManager)->SetIOSettings(*m_pIOsetting);
	*m_pScene		= fbxsdk::FbxScene::Create(*m_pManager.get(), NULL);
	*m_pImporter	= fbxsdk::FbxImporter::Create(*m_pManager.get(), NULL);
}

bool FBX::FBXUtility::Load(std::string path)
{
	try
	{
		if(!(*m_pImporter.get())->Initialize(
			path.c_str(),
			-1,
			(*m_pManager.get())->GetIOSettings()
		)) {
			throw path;
		}
	}
	catch (string&str)
	{
		#pragma region —áŠOˆ—
#if defined DEBUG||_DEBUG
		std::cout << "Could not read \"" << str << "\"" << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
		#pragma endregion
		return false;
	}

	try
	{
		if (!(*m_pImporter.get())->Import(
			*m_pScene.get()
		)) {
			throw path;
		}
	}
	catch (string&str)
	{
#pragma region —áŠOˆ—
#if defined DEBUG||_DEBUG
		std::cout << "Could not output \"" << str << "\" to the scene" << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
#pragma endregion
		return false;
	}

	(*m_pImporter.get())->Destroy();
	return true;
}

bool FBX::FBXUtility::Triangulate()
{
	fbxsdk::FbxGeometryConverter converter(*m_pManager.get());

	try
	{

		if (!converter.Triangulate(*m_pScene.get(), true)) {
			throw - 1;
		}
	}
	catch (int&)
	{
#pragma region —áŠOˆ—
#if defined DEBUG||_DEBUG
		std::cout << "Failed to triangulate this scene." << endl;
		std::system("pause");
		std::exit(EXIT_FAILURE);
#endif
#pragma endregion
		return false;
	}

	return true;
}
