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
	@var	c_Comma
	@brief	�R���}
*/
static constexpr string_view c_Comma = ",";

/*!
	@var	c_Space
	@brief	�X�y�[�X
*/
static constexpr string_view c_Space = " ";

/*!
	@var	c_Colon
	@brief	�R����
*/
static constexpr string_view c_Colon = ":";

/*!
	@var	c_Slash
	@brief	�X���b�V��
*/
static constexpr string_view c_Slash = "/";

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
	@fn			OutputMesh
	@brief		�Ǝ��`���̃X�L�����b�V�����o��
	@param[in]	�o�̓t�@�C���p�X
	@param[in]	���_�C���f�b�N�X
	@param[in]	���_���
*/
void Utility::IOMesh::OutputMesh(std::string filePath, std::vector<uint32_t> indices, std::vector<D3D11::Graphic::SkinnedVertex> vertices)
{
	string fileName = filePath;
	string buf = "";

	//	�f�B���N�g������
	while (true)
	{
		auto slashOffset = fileName.find(c_Slash);
		if (slashOffset == string::npos) { break; }
		string directryName = buf + fileName.substr(0, slashOffset);
		if (!filesystem::exists(directryName)) {
			wic::SetColor(Red);
			cout << "Directory not found." << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directryName))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
		buf = directryName + c_Slash.data();
		fileName = fileName.substr(slashOffset + 1);
	}

	ofstream ofs;
	ofs.open(filePath + c_Delimiter.data() + c_Extension.data(), ios::out);

	//	���_�C���f�b�N�X
	for (auto it : indices) { ofs << it << c_Comma; }
	ofs << endl;

	//	UV
	for (auto it : vertices) { ofs << it.uv.x << c_Comma << it.uv.y << c_Space; }
	ofs << endl;

	//	���_���W
	for (auto it : vertices) { ofs << it.position.x << c_Comma << it.position.y << c_Comma << it.position.z << c_Space; }
	ofs << endl;

	//	�d��
	for (auto v : vertices) {
		for (auto joint : v.joints)
		{
			ofs << joint.index << c_Colon << joint.weight << c_Comma;
		}
		ofs << endl;
	}
}

/*!
	@fn			OutputAnimation
	@brief		�Ǝ��`���̃A�j���[�V�������o��
	@param[in]	�o�̓t�@�C���p�X
	@param[in]	�A�j���[�V�����N���b�v
*/
void Utility::IOMesh::OutputAnimation(std::string filePath, API::AnimationClip clips)
{
	string fileName = filePath;
	string buf = "";
	//	�f�B���N�g������
	while (true)
	{
		auto slashOffset = fileName.find(c_Slash);
		if (slashOffset == string::npos) { break; }
		string directryName = buf + fileName.substr(0, slashOffset);
		if (!filesystem::exists(directryName)) {
			wic::SetColor(Red);
			cout << "Directory not found." << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directryName))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
		buf = directryName + c_Slash.data();
		fileName = fileName.substr(slashOffset + 1);
	}

	ofstream ofs;
	ofs.open(filePath + c_Delimiter.data() + c_AnimExtension.data(), ios::out);

	for (auto joint : clips.matrixPalette)
	{
		for (auto matrix : joint)
		{
			ofs <<
				matrix.r[0].m128_f32[0] << c_Comma << matrix.r[0].m128_f32[1] << c_Comma << matrix.r[0].m128_f32[2] << c_Comma << matrix.r[0].m128_f32[3] << c_Comma <<
				matrix.r[1].m128_f32[0] << c_Comma << matrix.r[1].m128_f32[1] << c_Comma << matrix.r[1].m128_f32[2] << c_Comma << matrix.r[1].m128_f32[3] << c_Comma <<
				matrix.r[2].m128_f32[0] << c_Comma << matrix.r[2].m128_f32[1] << c_Comma << matrix.r[2].m128_f32[2] << c_Comma << matrix.r[2].m128_f32[3] << c_Comma <<
				matrix.r[3].m128_f32[0] << c_Comma << matrix.r[3].m128_f32[1] << c_Comma << matrix.r[3].m128_f32[2] << c_Comma << matrix.r[3].m128_f32[3] << c_Space;
		}
		ofs << endl;
	}
}

/*!
	@fn			OutputTextureName
	@brief		�g�p����Ă���e�N�X�`������.ini�t�@�C���Ƃ��ď����o��
	@param[in]	�o�̓t�@�C���p�X
	@param[in]	�g�p����Ă���e�N�X�`���̉ϒ��z��
*/
void Utility::IOMesh::OutputTextureName(std::string filePath, std::vector<std::string> textureNames)
{
	string fileName = filePath;
	string buf = "";
	//	�f�B���N�g������
	while (true)
	{
		auto slashOffset = fileName.find(c_Slash);
		if (slashOffset == string::npos) { break; }
		string directryName = buf + fileName.substr(0, slashOffset);
		if (!filesystem::exists(directryName)) {
			wic::SetColor(Red);
			cout << "Directory not found." << endl;
			wic::SetColor(White);
			if (!filesystem::create_directory(directryName))throw runtime_error("Failed to create directory");
			cout << "Created a directory!" << endl;
		}
		buf = directryName + c_Slash.data();
		fileName = fileName.substr(slashOffset + 1);
	}

	ofstream ofs;
	ofs.open(filePath + c_Delimiter.data() + c_IniExtension.data(), ios::out);

	ofs << "���b�V���Ɏg�p����Ă���e�N�X�`����" << endl;
	for (auto it : textureNames)
	{
		ofs << it << endl;
	}
}