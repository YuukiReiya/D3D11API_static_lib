/*
	@file	IOMesh.cpp
	@date	2019/05/30
	@author	�ԏ� �G�P
	@brief	���b�V���f�[�^�̂̓��o�͊֌W
	@detail	�V���O���g��
*/
#include "Common.h"
#include "IOMesh.h"
#include "WinConsoleExpansion.h"
#include <filesystem>
/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

/*!
	@brief	using�f�B���N�e�B�u
	@using	ConsoleExpansion
*/
using namespace ConsoleExpansion;

/*!
	@brief	�G�C���A�X�錾
	@using	ConsoleExpansion::WinConsoleExpansion
	@var	wic
*/
using wic = ConsoleExpansion::WinConsoleExpansion;

/*!
	@fn			Delete
	@brief		yfm�t�@�C���폜
	@param[in]	�t�@�C���̃f�B���N�g��
	@param[in]	�t�@�C����(���g���q����)
*/
void Utility::IOMesh::Delete(std::string directoryPath, std::string fileName)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	ifstream ifs;

	//	�t�@�C�����J���Ă݂�
	ifs.open(path);

	//	�J���̂Ɏ��s
	if (ifs.fail()) {
		cout << "\""<<path<<"\" is open failed!" << endl;
		ifs.close();
		return; 
	}

	//	�t�@�C����Y�ꂸ�ɕ��Ă���
	ifs.close();

	//	�Y���t�@�C���폜
	remove(path.c_str());
	cout << "\"" << path << "\" is deleted!" << endl;
}

/*!
	@fn			Output
	@brief		�Ǝ��`���̏o��
	@param[in]	�t�@�C���̃f�B���N�g��
	@param[in]	�t�@�C����(���g���q����)
	@param[in]	���b�V�����
*/
void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, Utility::Mesh mesh)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	
	try
	{
		if (!filesystem::exists(directoryPath)) {
			wic::SetColor(Red);
			cout << "Directory not found" << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directoryPath))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
	}
	catch (exception& error)
	{
		wic::SetColor(Red);
		cout << error.what() << endl;
		cout << "This program has failed." << endl;
		cout << "this program exit here!" << endl;
		wic::SetColor(White);
		system("pause");
		exit(NULL);
	}
	
	ofstream ofs;
	ofs.open(path, ios::out);

	//	��������
	const string c_Space = " ";

	//	���_�C���f�b�N�X
	//ofs << "//i" << endl;
	for (auto it : mesh.vertexIndices)
	{
		ofs << it << c_Space;
	}
	ofs << endl;

	//	uv
	//ofs << "//uv" << endl;
	for (auto it : mesh.uv)
	{
		for (auto item : it.second)
			ofs << "{" << item.uv.x << c_Space << item.uv.y << "}";// << endl;
	}
	ofs << endl;

	//���_
	for (auto it : mesh.vertices)
	{
		ofs << "{" << it.x << c_Space << it.y << c_Space << it.z << "}";// << endl;
	}
}

void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, FBX::Utility::SkeltonMesh mesh)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();

	try
	{
		if (!filesystem::exists(directoryPath)) {
			wic::SetColor(Red);
			cout << "Directory not found" << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directoryPath))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
	}
	catch (exception& error)
	{
		wic::SetColor(Red);
		cout << error.what() << endl;
		cout << "This program has failed." << endl;
		cout << "this program exit here!" << endl;
		wic::SetColor(White);
		system("pause");
		exit(NULL);
	}

	ofstream ofs;
	ofs.open(path, ios::out);

	//	��������
	const string c_Space = " ";

	//	1.���_�C���f�b�N�X
	for (auto it : mesh.indices)
	{
		ofs << it << c_Space;
	}
	ofs << endl;

	//	2.UV
	for (auto it : mesh.vertices)
	{
		ofs << "{" << it.uv.x << c_Space << it.uv.y << "}";
	}
	ofs << endl;

	//	3.���_
	for (auto it : mesh.vertices) 
	{
		ofs << "{" << it.position.x << c_Space << it.position.y << c_Space << it.position.z << "}";
	}
	ofs << endl;

	//	4."1���_"���֘A����{�[���ԍ��̍ő吔(��Ԋ֘A�{�[�������������_�̃{�[����)
	ofs << mesh.maxBonesElementsCount << endl;

	//	5.�d��
	for (auto v : mesh.vertices)
	{
		ofs << "{" << c_Space;
		for (auto it : v.weight) {
			ofs << it.first << ":" << it.second << c_Space;
		}
		ofs << "}";

		//	�����o��
		//	{ x y z }{...�ɂȂ�͂�
	}
	ofs << endl;

	//	6.�����p��
	for (auto it : mesh.initialMatrix)
	{
		ofs << "{"
			<< it.elemnts[0].x << c_Space << it.elemnts[0].y << c_Space << it.elemnts[0].z << c_Space << it.elemnts[0].w << c_Space
			<< it.elemnts[1].x << c_Space << it.elemnts[1].y << c_Space << it.elemnts[1].z << c_Space << it.elemnts[1].w << c_Space
			<< it.elemnts[2].x << c_Space << it.elemnts[2].y << c_Space << it.elemnts[2].z << c_Space << it.elemnts[2].w << c_Space
			<< it.elemnts[3].x << c_Space << it.elemnts[3].y << c_Space << it.elemnts[3].z << c_Space << it.elemnts[3].w << c_Space
			<< "}";
	}
	ofs << endl;

	//	7.�t���[����
	ofs << mesh.frameMatrix.size() << endl;//	�t���[��

	//	8.�t���[�����p��
	for (size_t frame = 0; frame < mesh.frameMatrix.size(); ++frame)
	{
		for (auto it:mesh.frameMatrix[frame]) {

			ofs << "{"
				<< it.elemnts[0].x << c_Space << it.elemnts[0].y << c_Space << it.elemnts[0].z << c_Space << it.elemnts[0].w << c_Space
				<< it.elemnts[1].x << c_Space << it.elemnts[1].y << c_Space << it.elemnts[1].z << c_Space << it.elemnts[1].w << c_Space
				<< it.elemnts[2].x << c_Space << it.elemnts[2].y << c_Space << it.elemnts[2].z << c_Space << it.elemnts[2].w << c_Space
				<< it.elemnts[3].x << c_Space << it.elemnts[3].y << c_Space << it.elemnts[3].z << c_Space << it.elemnts[3].w << "}";
		}
	}
	ofs << endl;

}
