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

	/////////////////////////////////////////////////
	//	�t�@�C���ǂݍ���
	/////////////////////////////////////////////////
	
	//	�ǂݍ���.fbx�t�@�C���̃p�X���w�肵�܂��B
	std::string fbxPath = "";
	fbxPath = "Spider_3.fbx";

	//	�����o���t�@�C���̖��O���w�肵�܂��B
	std::string yfmPath = "";
	yfmPath = "spider";

	/////////////////////////////////////////////////
	//	�R���o�[�g
	/////////////////////////////////////////////////

	//	������
	Converter::FBXConverter::Setup();

	//	���s
	FBXConverter::Execute(FBXConverter::OutputType::SKIN, fbxPath, yfmPath);

	//	�j��
	FBXConverter::Teardown();

	/////////////////////////////////////////////////
	//	����������
	/////////////////////////////////////////////////
	system("pause");
	return 0;
}