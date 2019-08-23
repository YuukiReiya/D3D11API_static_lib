#include <iostream>
#include <FBXConverter.h>
#include <Mesh.h>
#include <IOMesh.h>
#include <WinConsoleExpansion.h>
using namespace Utility;
using namespace Converter;

int main()
{
	//	���������[�N�ӏ�����
#if defined DEBUG||_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG||_DEBUG

	//	�ǂݍ��݃t�@�C��
	//std::string fbxPath = "SD_unitychan_humanoid.fbx";
	//std::string fbxPath = "ao_twinte_chan.fbx"; 
	//std::string fbxPath = "Kouhai_chan.fbx";
	//std::string fbxPath = "JNT_00100_anathema_H.fbx";
	//std::string fbxPath = "Pikachu.fbx";
	std::string fbxPath = "humanoid.fbx";


	//	�����o���t�@�C��
	//std::string yfmPath = "sd-unity";
	//std::string yfmPath = "Animation/humanoid";
	std::string yfmPath = "newSample.yfm";
	//std::string yfmPath = "twin";
	//std::string yfmPath = "test00";

	//	�R���o�[�g
	Converter::FBXConverter::Setup();
	//FBXConverter::Execute(fbxPath, yfmPath);

	FBXConverter::Execute(FBXConverter::OutputType::SKIN, fbxPath, yfmPath);


	FBXConverter::Teardown();
	system("pause");
	return 0;
}