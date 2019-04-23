#include <iostream>
#include <Common.h>
#include <FBXUtility.h>

int main()
{
	auto p=fbxsdk::FbxManager::Create();
	FBXUtility::Setup();
	return 0;
}