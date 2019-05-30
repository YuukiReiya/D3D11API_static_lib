#include <iostream>
#include <FBXUtility.h>
#include <Mesh.h>
#include <IOMesh.h>
#include <WinConsoleExpansion.h>

using namespace FBX;
using namespace Utility;

int main()
{
	//	ÉÅÉÇÉäÉäÅ[ÉNâ”èäì¡íË
#if defined DEBUG||_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG||_DEBUG

	FBX::FBXUtility::Setup();
	Mesh*mesh = new Mesh;

	//std::string fbxPath = "SD_unitychan_humanoid.fbx";
	std::string fbxPath = "ao_twinte_chan.fbx"; 
	//std::string fbxPath = "Kouhai_chan.fbx";
	//std::string fbxPath = "JNT_00100_anathema_H.fbx";
	//std::string fbxPath = "Pikachu.fbx";

	ConsoleExpansion::WinConsoleExpansion::SetColor(ConsoleExpansion::Yellow);
	std::string yfmPath = "test8";

	IOMesh::Delete("", yfmPath);
	ConsoleExpansion::WinConsoleExpansion::SetColor(ConsoleExpansion::White);

	if (FBX::FBXUtility::Load(fbxPath, mesh))
	{
		IOMesh::Output("", yfmPath, *mesh);
	}

	
	//IOMesh::Output("../", "uvTest", mesh);
	//IOMesh::Output("", "fbx", mesh);
	FBXUtility::Teardown();
	delete mesh;
	system("pause");
	return 0;
}