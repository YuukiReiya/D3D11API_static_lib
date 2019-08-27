#include <iostream>
#include <FBXConverter.h>
#include <Mesh.h>
#include <IOMesh.h>
#include <WinConsoleExpansion.h>
#include <string>
using namespace Utility;
using namespace Converter;
using namespace std;
using namespace ConsoleExpansion;

using wic = ConsoleExpansion::WinConsoleExpansion;

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
	string fbxPath = "";
	do
	{
		wic::SetColor(Cyan);
		cout << "FBX�t�@�C���̃p�X���w�肵�Ă��������B" << endl;
		wic::SetColor(White);
		getline(cin, fbxPath);
		wic::SetColor(Cyan);

		//	FBX�t�@�C�����w�肳��Ă��邩�m�F
		try
		{
			auto delimiterOffset = fbxPath.rfind(".");
			if (delimiterOffset == string::npos) { throw -1; }
			auto extension = fbxPath.substr(delimiterOffset);
			if (extension != ".fbx") { throw -1; }
		}
		//	FBX�t�@�C�����w�肳��Ă��Ȃ��̂ŁA�ē��͂𑣂��B
		catch (...)
		{
			wic::SetColor(Red);
			cout << ".fbx�t�@�C�����w�肳��Ă��܂���!" << endl
				<< "�ē��͂��Ă��������B" << endl;
			continue;
		}

		//	���͂̊m��m�F
		string ret;
		cout << "\"" << fbxPath << "\"" << "��ǂݍ��݂܂��B" << endl
			<< "��낵���ł���?<y/n>" << endl;
		getline(cin, ret);

		//	�����o���t�@�C���̎w��Ɉڂ�
		if (ret == "y") 
		{ 
			break;
		}
		//	�A�v���P�[�V�����I��
		else if(ret == "n")
		{
			wic::SetColor(Purple);
			cout << "�R���o�[�^�[���I�����܂��B" << endl;
			wic::SetColor(White);
			system("pause");
			return -1;
		}

		//	�R���\�[�����Y��ɂ���
		system("cls");
	} while (true);

	//	�����o���t�@�C���̖��O���w�肵�܂��B
	std::string yfmPath = "";
	do
	{
		wic::SetColor(Cyan);
		cout << "�����o����̃p�X�y�уt�H���_�����w�肵�Ă��������B" << endl;
		wic::SetColor(White);
		getline(cin, yfmPath);
		wic::SetColor(Cyan);

		string ret;
		cout << "\"" << yfmPath << "\"" << "�ɏ����o���܂��B" << endl
			<< "��낵���ł���?<y/n>" << endl;
		getline(cin, ret);

		//	���͂̊m��m�F
		if (ret == "y")
		{
			break;
		}
		//	�A�v���P�[�V�����̏I��
		else if (ret == "n")
		{
			wic::SetColor(Purple);
			cout << "�R���o�[�^�[���I�����܂��B" << endl;
			wic::SetColor(White);
			system("pause");
			return -1;
		}

		//	�R���\�[�����N���A
		system("cls");
	} while (true);


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