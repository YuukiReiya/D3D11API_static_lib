#pragma once
#include <fbxsdk.h>
#include <memory>

class FBXLoader
{
public:
	~FBXLoader();

private:
	FBXLoader();

	static std::shared_ptr<FbxImporter>m_pImporter;
};