#include "Common.h"
#include "FBXUtility.h"
#include <iostream>

using namespace std;
using namespace fbxsdk;

FbxManager* FBXUtility::m_p = nullptr;


void FBXUtility::Setup()
{
	cout << "test";
	//m_p = FbxManager::Create();
}
