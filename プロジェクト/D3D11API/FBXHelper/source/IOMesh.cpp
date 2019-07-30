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

void Utility::IOMesh::Output(std::string fileName, SkeletonMesh mesh,AnimationClip clip)
{
	string path = fileName + c_Delimiter.data() + c_Extension.data();

	ofstream ofs;
	ofs.open(path, ios::out);

	//	��������
	const string c_Space = " ";

	//	���_�C���f�b�N�X
	for (auto it : mesh.indices)
	{
		//ofs << it << c_Space;
		ofs << it << ",";
	}
	ofs << endl;

	//	uv
	for (auto it : mesh.vertices)
	{
	//		ofs << "{" << it.uv.x << c_Space << it.uv.y << "}";// << endl;
	}
	ofs << endl;

	//���_
	for (auto it : mesh.vertices)
	{
		//ofs << "{" << it.position.x << c_Space << it.position.y << c_Space << it.position.z << "}";// << endl;
		ofs << "{" << it.position.x << "," << it.position.y << "," << it.position.z << "},";// << endl;
	}
	ofs << endl;

	//	�d��
	//for (auto it : mesh.vertices) 
	//{
	//	ofs << "{ "
	//		<< static_cast<int>(it.joijntIndex[0]) << ":" << it.jointWeights[0] << c_Space 
	//		<< static_cast<int>(it.joijntIndex[1]) << ":" << it.jointWeights[1] << c_Space
	//		<< static_cast<int>(it.joijntIndex[2]) << ":" << it.jointWeights[2] << c_Space
	//		<< "}";
	//}
	for (auto v : mesh.vertices)
	{
		//ofs << "{" << c_Space;
		//for (size_t i = 0; i < v.jointsIndex.size(); i++)
		//{
		//	ofs << static_cast<int>(v.jointsIndex[i]) << ":" << v.jointsWeight[i] << c_Space;
		//}
		//ofs << "}";
		

		//	�����o��
		//	{ x y z }{...�ɂȂ�͂�
	}
	ofs << endl;

	//	�����p��(�t�s��)
	//for (size_t i = 0; i < mesh.skeleton.jointCount; i++)
	for (size_t i = 0; i < mesh.skeleton.joints.size(); i++)
	{
		auto& m = mesh.skeleton.joints[i].invBindPose;
		ofs << "{" <<
			m._11 << c_Space << m._12 << c_Space << m._13 << c_Space <<
			m._21 << c_Space << m._22 << c_Space << m._23 << c_Space <<
			m._31 << c_Space << m._32 << c_Space << m._33 << c_Space <<
			m._41 << c_Space << m._42 << c_Space << m._43 << c_Space <<
			"}";
	}

	//	�A�j���t���[��
	ofs << endl;
	ofs << clip.frameCount << endl;

	//	�t���[���s��
	//��1�A�j��

	for (size_t i = 0; i < clip.bonesMatrix.size(); i++)
	{
		//auto frameCount = clip.frameCount;
		//for (size_t j = 0; j < frameCount; j++)

		{
			//auto it = clip.bonesMatrix[i][j];
			//
			//ofs << "{"
			//	<< it.r[0].m128_f32[0] << c_Space << it.r[0].m128_f32[1] << c_Space << it.r[0].m128_f32[2] << c_Space << it.r[0].m128_f32[3] << c_Space
			//	<< it.r[1].m128_f32[0] << c_Space << it.r[1].m128_f32[1] << c_Space << it.r[1].m128_f32[2] << c_Space << it.r[1].m128_f32[3] << c_Space
			//	<< it.r[2].m128_f32[0] << c_Space << it.r[2].m128_f32[1] << c_Space << it.r[2].m128_f32[2] << c_Space << it.r[2].m128_f32[3] << c_Space
			//	<< it.r[3].m128_f32[0] << c_Space << it.r[3].m128_f32[1] << c_Space << it.r[3].m128_f32[2] << c_Space << it.r[3].m128_f32[3] << 
			//	"}";
		}
	}
}
