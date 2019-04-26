#include <iostream>
//#include <Common.h>
#include <FBXUtility.h>
//#include <AbstractMesh.h>
#include <Mesh.h>


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
	FBXUtility::Destroy();
	FBXUtility::Destroy();

	delete mesh;
	system("pause");
	return 0;
}