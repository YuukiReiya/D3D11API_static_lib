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

void Utility::IOMesh::Write(std::vector<DirectX::XMMATRIX> mat)
{
	ofstream ofs;
	ofs.open("�t���[���������s��.md", ios::out);

	for (auto it : mat)
	{
		ofs << "{" <<
			it.r[0].m128_f32[0] << "," << it.r[0].m128_f32[1] << "," << it.r[0].m128_f32[2] << "," << it.r[0].m128_f32[3] << "," <<
			it.r[1].m128_f32[0] << "," << it.r[1].m128_f32[1] << "," << it.r[1].m128_f32[2] << "," << it.r[1].m128_f32[3] << "," <<
			it.r[2].m128_f32[0] << "," << it.r[2].m128_f32[1] << "," << it.r[2].m128_f32[2] << "," << it.r[2].m128_f32[3] << "," <<
			it.r[3].m128_f32[0] << "," << it.r[3].m128_f32[1] << "," << it.r[3].m128_f32[2] << "," << it.r[3].m128_f32[3] 
			<< "},";
	}
}

void Utility::IOMesh::WriteJ(std::vector<std::vector<Joint>> js)
{
	ofstream ofs;
	ofs.open("�W���C���gW&M.md", ios::out);
	for (size_t i = 0; i < js.size(); i++)
	{
		ofs << "{";
		for (size_t j = 0; j < js[i].size(); j++)
		{
			auto joint = js[i][j];
			ofs << "{" << joint.index << "," << joint.weight << "},";
		}
		ofs << "},";
	}
}

void Utility::IOMesh::WriteM(std::vector<std::vector<DirectX::XMMATRIX>> mats)
{
	ofstream ofs;
	ofs.open("�t���[�����X�L���s��.md", ios::out);
	for (auto fm : mats)
	{
		ofs << "{";
		for (auto it : fm)
		{
			ofs << "{" <<
				it.r[0].m128_f32[0] << "," << it.r[0].m128_f32[1] << "," << it.r[0].m128_f32[2] << "," << it.r[0].m128_f32[3] << "," <<
				it.r[1].m128_f32[0] << "," << it.r[1].m128_f32[1] << "," << it.r[1].m128_f32[2] << "," << it.r[1].m128_f32[3] << "," <<
				it.r[2].m128_f32[0] << "," << it.r[2].m128_f32[1] << "," << it.r[2].m128_f32[2] << "," << it.r[2].m128_f32[3] << "," <<
				it.r[3].m128_f32[0] << "," << it.r[3].m128_f32[1] << "," << it.r[3].m128_f32[2] << "," << it.r[3].m128_f32[3]
				<< "},";
		}
		ofs << "},";
	}
}
