#include <iostream>
#include <FBXUtility.h>
#include <Mesh.h>
#include <IOMesh.h>

using namespace FBX;
using namespace Utility;

int main()
{
	//	メモリリーク箇所特定
#if defined DEBUG||_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG||_DEBUG

	FBX::FBXUtility::Setup();
	Mesh*mesh = new Mesh;
	mesh->Setup();
	auto ret = FBXUtility::Load("ao_twinte_chan.fbx", mesh);

	if (ret) {
		printf("成功");
	}
	else {
		printf("失敗");
	}

	//IOMesh::Output("../", "uvTest", mesh);
	//IOMesh::Output("", "fbx", mesh);
	FBXUtility::Destroy();
	delete mesh;
	system("pause");
	return 0;
}