#include <iostream>
#include <FBXUtility.h>
#include <Mesh.h>
#include <IOMesh.h>

using namespace FBX;
using namespace Utility;

int main()
{
	//	���������[�N�ӏ�����
#if defined DEBUG||_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG||_DEBUG

	FBX::FBXUtility::Setup();
	FBXUtility::Setup();
	auto ret = FBXUtility::Load("ao_twinte_chan.fbx");
	ret = FBXUtility::Load("test.fbx");

	if (ret) {
		printf("����");
	}
	else {
		printf("���s");
	}

	Mesh*mesh = new Mesh;
	mesh->Setup();
	ret=FBXUtility::Load("ao_twinte_chan.fbx", mesh);

	if (ret) {
		printf("����2");
	}
	else {
		printf("���s2");
	}

	IOMesh::Output("../", "test", mesh);

	FBXUtility::Destroy();
	delete mesh;
	system("pause");
	return 0;
}