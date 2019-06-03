#include "Common.h"
#include "FBXConverter.h"
#include "WinConsoleExpansion.h"
#include "IOMesh.h"
#include <fbxsdk.h>
#include <iostream>
#if defined DEBUG || defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define	new	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif
#include <fstream>
#include <algorithm>
#include <Windows.h>
/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

/*!
	@brief	using�f�B���N�e�B�u
	@using	Converter
*/
using namespace Converter;

/*!
	@brief	using�f�B���N�e�B�u
	@using	ConsoleExpansion
*/
using namespace ConsoleExpansion;

/*!
	@brief		�ʖ���`
	@typedef	wic
	@value		ConsoleExpansion::WinConsoleExpansion
*/
typedef ConsoleExpansion::WinConsoleExpansion wic;

/*!
	@var	m_pManager
	@brief	FbxManager�̃|�C���^
	@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
*/
shared_ptr<fbxsdk::FbxManager*>		FBXConverter::m_pManager	= nullptr;

/*!
	@var	m_pIOsetting
	@brief	FbxIOSettings�̃|�C���^
	@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
*/
shared_ptr<fbxsdk::FbxIOSettings*>	FBXConverter::m_pIOsetting	= nullptr;

/*!
	@var	m_pScene
	@brief	FbxScene�̃|�C���^
	@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
*/
shared_ptr<fbxsdk::FbxScene*>		FBXConverter::m_pScene		= nullptr;

/*!
	@var	m_pImporter
	@brief	FbxImporter�̃|�C���^
	@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
*/
shared_ptr<fbxsdk::FbxImporter*>	FBXConverter::m_pImporter	= nullptr;

/*!
	@brief	�R���X�g���N�^
*/
Converter::FBXConverter::FBXConverter()
{
	Setup();
}

/*!
	@brief	�f�X�g���N�^
*/
Converter::FBXConverter::~FBXConverter()
{
	Teardown();
}

/*!
	@fn		Setup
	@brief	������
	@detail	�����o�̃C���X�^���X���Ə�����
*/
void FBXConverter::Setup()
{
	//	�����o�j��
	Teardown();

	m_pManager		= make_shared<fbxsdk::FbxManager*>();
	m_pScene		= make_shared<fbxsdk::FbxScene*>();
	m_pIOsetting	= make_shared<fbxsdk::FbxIOSettings*>();
	m_pImporter		= make_shared<fbxsdk::FbxImporter*>();

	*m_pManager		= fbxsdk::FbxManager::Create();
	*m_pIOsetting	= fbxsdk::FbxIOSettings::Create(*m_pManager.get(), IOSROOT);
	(*m_pManager)->SetIOSettings(*m_pIOsetting);
	*m_pScene		= fbxsdk::FbxScene::Create(*m_pManager.get(), NULL);
	*m_pImporter	= fbxsdk::FbxImporter::Create(*m_pManager.get(), NULL);

}

/*!
	@fn		Teardown
	@brief	�j�������B
*/
void Converter::FBXConverter::Teardown()
{
	if (m_pImporter) {
		TeardownImporter();
	}
	if (m_pIOsetting) {
		(*m_pIOsetting.get())->Destroy();
		m_pIOsetting.reset();
	}
	if (m_pScene) {
		(*m_pScene.get())->Destroy();
		m_pScene.reset();
	}
	if (m_pManager) {
		(*m_pManager.get())->Destroy();
		m_pManager.reset();
	}
}

/*!
	@fn			Execute
	@brief		���s����
	@detail		FBX��ǂݍ��݊O���t�@�C���ɏ����o���B
	@param[in]	FBX�̃p�X
	@param[in]	�o�͐�̃t�@�C���p�X
*/
void Converter::FBXConverter::Execute(std::string fbxPath, std::string outName)
{
	//	FBXSDK�̃Z�b�g�A�b�v
	try
	{
		//	�C���|�[�^�[
		if (!SetupImporter(fbxPath)) { throw "Could not read \"" + fbxPath + "\""; }

		//	�V�[��
		if (!SetupScene(fbxPath)) { throw "Could not output \"" + fbxPath + "\" to the scene"; }

		//	�C���|�[�^�[�̔j��
		TeardownImporter();

	}
	catch (std::string& error)
	{
		wic::SetColor(Red);
		cout << error << endl;
		wic::SetColor(White);
		cout << "this program exit here!" << endl;
		system("pause");
		exit(NULL);
	}

	//	�O�p��
	if (!Triangulate()) { 
		wic::SetColor(Red);
		cout << "Failed to triangulate this scene." << endl;
		wic::SetColor(White);
		cout << "this program exit here!" << endl;
		system("pause");
		exit(NULL);
	}

	//	���b�V���̕���
	if (!SplitMeshesPerMaterial()) { 
		wic::SetColor(Red);
		cout << "Failed to split mesh" << endl;
		wic::SetColor(White);
		cout << "this program exit here!" << endl;
		system("pause");
		exit(NULL);

	}


	//	���b�V���f�[�^�̏o��
	auto meshCount = (*m_pScene.get())->GetSrcObjectCount<FbxMesh>();
	wic::SetColor(Green);
	cout << "Mesh count:";
	wic::SetColor(White);
	cout << meshCount << endl;
	for (int i = 0; i < meshCount; ++i)
	{
		auto pMesh = (*m_pScene.get())->GetSrcObject<FbxMesh>(i);
		cout << endl;
		wic::SetColor(Green);
		cout << "Mesh name:";
		wic::SetColor(White);
		cout << pMesh->GetName() << endl;

		//	�\���|���S����
		auto polygonCount = pMesh->GetPolygonCount();
		cout << "mesh polygon count = " << polygonCount << endl;

		//	�\�����Ă���|���S�����Ȃ���Ώ������Ȃ�
		if (polygonCount <= 0) { continue; }

		//	�o�̓f�[�^
		Utility::Mesh oMesh;

		//	���b�V�������i�[
		LoadToStore(pMesh, &oMesh);

		//	�O���t�@�C���o��
		auto oName = meshCount == 1 ? outName : outName + "-" + to_string(i);
		Utility::IOMesh::Output(c_OutputDirectory.data(), oName, oMesh);
		//Utility::IOMesh::Output("", oName, oMesh);

		wic::SetColor(Green);
		cout << "CreateMesh success!" << endl;
		wic::SetColor(White);
	}
	cout << "Quit the program." << endl;
}

/*!
	@fn		Triangulate
	@brief	�V�[�����̃��f���̎O�p�|���S�����B
	@note	try catch�����ꂽ�P���ȃ��b�p�[
	@return	true:���� false:���s
*/
bool Converter::FBXConverter::Triangulate()
{
	fbxsdk::FbxGeometryConverter converter(*m_pManager.get());

//	try
//	{
//		if (!converter.Triangulate(*m_pScene.get(), true)) {
//			throw runtime_error("Failed to triangulate this scene.");
//		}
//
//		//	���b�V������s�v�ȃ|���S�����폜����
//		converter.RemoveBadPolygonsFromMeshes(*m_pScene.get());
//	}
//	catch (exception&error)
//	{
//#pragma region ��O����
//		wic::SetColor(Red);
//		std::cout << error.what() << endl;
//		wic::SetColor(White);
//#if defined DEBUG||_DEBUG
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//	}
//
//	return true;
	return converter.Triangulate(*m_pScene.get(), true);
}

/*!
	@fn		SplitMeshesPerMaterial
	@brief	�V�[�����̃��f�����}�e���A���P�ʂɕ���
	@note	try catch�����ꂽ�P���ȃ��b�p�[
	@return	true:���� false:���s
*/
bool Converter::FBXConverter::SplitMeshesPerMaterial()
{
	fbxsdk::FbxGeometryConverter converter(*m_pManager.get());

//	try
//	{
//		if (converter.SplitMeshesPerMaterial(*m_pScene.get(), true)) {
//			throw runtime_error("Failed to split mesh");
//		}
//	}
//	catch (const std::exception& error)
//	{
//#pragma region ��O����
//		wic::SetColor(Red);
//		std::cout << error.what() << endl;
//		wic::SetColor(White);
//#if defined DEBUG||_DEBUG
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//
//	}
//	return true;

	return converter.SplitMeshesPerMaterial(*m_pScene.get(), true);
}

/*!
	@fn			SetupImporter
	@brief		fbx�t�@�C�����C���|�[�^�[�ɓǂݍ��ށB
	@param[in]	�ǂݍ���.fbx�t�@�C���̃p�X
*/
bool Converter::FBXConverter::SetupImporter(std::string fbxPath)
{
//	try
//	{
//		if (!(*m_pImporter.get())->Initialize(
//			fbxPath.c_str(),
//			-1,
//			(*m_pManager.get())->GetIOSettings()
//		)) {
//			throw fbxPath;
//		}
//	}
//	catch (string&str)
//	{
//#pragma region ��O����
//#if defined DEBUG||_DEBUG
//		std::cout << "Could not read \"" << str << "\"" << endl;
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//	}
//	return true;

	return (*m_pImporter.get())->Initialize(
		fbxPath.c_str(),
		-1,
		(*m_pManager.get())->GetIOSettings()
	);
}

/*!
	@fn		TeardownImporter
	@brief	�C���|�[�^�[�̔j�������B
*/
void Converter::FBXConverter::TeardownImporter()
{
	if (m_pImporter)
	{
		(*m_pImporter.get())->Destroy();
		m_pImporter.reset();
	}
}

/*!
	@fn			SetupScene
	@brief		�C���|�[�^�[�ɓǂݍ��񂾃f�[�^���V�[���ɃC���|�[�g�B
	@detail		�ǂݍ��݌�ɃC���|�[�^�[�͔j�����Ă���B
	@param[in]	�ǂݍ���.fbx�t�@�C���̃p�X
*/
bool Converter::FBXConverter::SetupScene(std::string fbxPath)
{
//	try
//	{
//		if (!(*m_pImporter.get())->Import(
//			*m_pScene.get()
//		)) {
//			throw fbxPath;
//		}
//	}
//	catch (string&str)
//	{
//#pragma region ��O����
//#if defined DEBUG||_DEBUG
//		std::cout << "Could not output \"" << str << "\" to the scene" << endl;
//		std::system("pause");
//		std::exit(EXIT_FAILURE);
//#endif
//#pragma endregion
//		return false;
//	}
//	return true;
	return (*m_pImporter.get())->Import(*m_pScene.get());
}

/*!
	@fn			LoadToStore
	@brief		fbx�̃f�[�^���i�[
	@param[in]	���o���̃f�[�^
	@param[in]	�i�[�惁�b�V��
*/
void Converter::FBXConverter::LoadToStore(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	try
	{
		//	���_�C���f�b�N�X
		SetupVertexIndices(from, to);
		if (to->vertexIndices.empty())throw"SetupVertexIndices";

		//	���_
		SetupVertices(from, to);
		if (to->vertices.empty())throw"SetupVertices";

		//	UV
		SetupUV(from, to);
		if (to->uv.empty())throw"SetupUV";

		//	���_�̐�����
		AlignVerticesToUV(to);
	}
	catch (const string&error)
	{
		wic::SetColor(Red);
		cout << "Failed to \"" << error << "\"" << endl;
		wic::SetColor(White);
	}
}

/*!
	@fn			SetupVertexIndices
	@brief		���_�C���f�b�N�X�̃Z�b�g�A�b�v
	@param[in]	�Q�ƌ���Converter���b�V��
	@param[in]	�o�C���h��̃��b�V��
*/
void Converter::FBXConverter::SetupVertexIndices(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	//	�|���S����
	auto polygonCount = from->GetPolygonCount();
	for (int i = 0; i < polygonCount; ++i)
	{
		//	�|���S�����\�����钸�_��(���O�p�����Ă����"3"�ɂȂ�)
		auto polygonVertexCount = from->GetPolygonSize(i);
		for (int j = 0; j < polygonVertexCount; ++j)
		{
			to->vertexIndices.push_back(
				{
					from->GetPolygonVertex(i,j)
				}
			);
		}
	}

	wic::SetColor(Green);
	cout << "Mesh vertex index size:";
	wic::SetColor(White);
	cout << to->vertexIndices.size() << endl;
}

/*!
	@fn			SetupVertices
	@brief		���_�̃Z�b�g�A�b�v
	@param[in]	�Q�ƌ���fbx���b�V��
	@param[in]	�o�C���h��̃��b�V��
*/
void Converter::FBXConverter::SetupVertices(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	//	���_��
	auto vertexCount = from->GetControlPointsCount();

	//	���_�z��̐擪�|�C���^
	FbxVector4* vertices = from->GetControlPoints();

	int index = 0;
	while (index < vertexCount)
	{
		to->vertices.push_back(
			{
				{(float)vertices[index][0]},
				{(float)vertices[index][1]},
				{(float)vertices[index][2]},
				{(float)vertices[index][3]},
			}
		);
		++index;
	}
	wic::SetColor(Green);
	cout << "Mesh vertex size:";
	wic::SetColor(White);
	cout << to->vertices.size() << endl;
}

/*!
	@fn			SetupUV
	@brief		UV�̃Z�b�g�A�b�v
	@detail		���_�C���f�b�N�X�ɑΉ�����UV���i�[�����B
	@param[in]	�Q�ƌ���fbx���b�V��
	@param[in]	�o�C���h��̃��b�V��
*/
void Converter::FBXConverter::SetupUV(fbxsdk::FbxMesh * from, Utility::Mesh * to)
{
	//	UV�Z�b�g��
	FbxStringList uvSetNames;
	from->GetUVSetNames(uvSetNames);

	wic::SetColor(Green);
	cout << "UV set names count:";
	wic::SetColor(White);
	cout << uvSetNames.GetCount() << endl;
	for (int i = 0; i < uvSetNames.GetCount(); ++i)
	{
		wic::SetColor(Cyan);
		cout << "No" << i << ":";
		wic::SetColor(White);
		cout << uvSetNames.GetStringAt(i) << endl;
	}

	auto uvSetCount = uvSetNames.GetCount();
	int index = 0;
	while (index < uvSetCount)
	{
		//	UVSetName��ۑ�
		const string c_UVSetName = uvSetNames.GetStringAt(index);
		to->uvSetNamesList.push_back(c_UVSetName);

		wic::SetColor(Green);
		cout << endl << c_UVSetName << endl;
		wic::SetColor(White);

		FbxArray<FbxVector2>uvsets;
		from->GetPolygonVertexUVs(c_UVSetName.c_str(), uvsets);

		//	UVSetName�ɑΉ�����UV�̊i�[
		for (int i = 0; i < uvsets.Size(); ++i)
		{
			to->uv[c_UVSetName].push_back(
				{
					c_UVSetName,
					{
								(float)uvsets[i][0],
						1.0f  - (float)uvsets[i][1],
					}
				}
			);
		}
		wic::SetColor(Cyan);
		cout << "UV set size:";
		wic::SetColor(White);
		cout << uvsets.Size() << endl;

		wic::SetColor(Cyan);
		cout << "UV count:";
		wic::SetColor(White);
		cout << to->uv[c_UVSetName].size() << endl;

		index++;
	}
}

/*!
	@fn			AlignVerticesToUV
	@brief		���_��UV�ɑ�����B
	@detail		���O�ɑ��̏������낦�Ă����K�v������
	@param[in]	�Ώۂ̃��b�V��
*/
void Converter::FBXConverter::AlignVerticesToUV(Utility::Mesh * mesh)
{
	/*!
		@struct	Vertex
		@brief	���_��UV��R�Â������邽�߂̈ꎞ�\����
	*/
	struct Vertex
	{
		/*!
			@var	v
			@brief	���_
		*/
		Math::FLOAT4 v;

		/*!
			@var	uv
			@brief	UV
		*/
		Math::FLOAT2 uv;

		static inline bool Compare(Vertex other, Vertex self) {
			return self.uv == other.uv&&self.v == other.v;
		}

		/*!
			@var	uvsetName
			@brief	�֘A�t����uvset�̖��O
		*/
		string uvsetName;

	};

	//	�C���f�b�N�X�Ń\�[�g�����d���̂���S�f�[�^
	wic::SetColor(Green);
	cout << "Index sort vertices:";
	vector<Vertex>indexSortVertices;
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < mesh->vertexIndices.size(); ++i)
		{
			indexSortVertices.push_back(
				{
					{mesh->vertices[mesh->vertexIndices[i]]},
					{mesh->uv[hash][i].uv},
					{hash},
				}
			);
		}
	}
	wic::SetColor(White);
	cout << indexSortVertices.size() << endl;

	//	�d���̂Ȃ���ӂȒ��_���(���_ + UV)
	vector<Vertex>uniqueVertices;
	wic::SetColor(Green);
	cout << "Unique vertices:";
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < mesh->vertexIndices.size(); i++)
		{

			//	uvset���͍l�����Ȃ��Ă��悢�̂Ŗ��ݒ�
			Vertex findItem =
			{
				{mesh->vertices[mesh->vertexIndices[i]]},
				{mesh->uv[hash][i].uv},
				{hash}
			};

			//	���_��UV��񂪈�v����C�e���[�^���擾
			auto it = find_if(
				uniqueVertices.begin(),
				uniqueVertices.end(),
				[&](Vertex&v){
				return Vertex::Compare(v,findItem);
			}
			);
			if (it != uniqueVertices.end()) { continue; }

			//	"���_ + UV"�̏���ۑ� 
			uniqueVertices.push_back(findItem);
		}
	}
	wic::SetColor(White);
	cout << uniqueVertices.size() << endl;

	//	���b�V�������č\�����邽�ߏ�����
	mesh->vertexIndices.clear();
	mesh->vertices.clear();
	mesh->uv.clear();

	//	���_���(���W + UV)�i�[
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < uniqueVertices.size(); i++)
		{
			//	i�Ԗڂ̃C���f�b�N�X���Q�Ƃ��钸�_���
			auto vertex = uniqueVertices[i];

			//	���_
			mesh->vertices.push_back(vertex.v);

			//	uvset���ɍ��킹�Đݒ�
			if (hash != vertex.uvsetName) { continue; }

			//	Vertex�^��UV�^�ɃL���X�g���ĉϒ��z��ɋl�߂�
			Utility::Mesh::UV uv=
			{
				{vertex.uvsetName},
				{vertex.uv}
			};
			mesh->uv[hash].push_back(uv);
		}
	}

	//	�C���f�b�N�X�̍č\��
	for (auto hash : mesh->uvSetNamesList)
	{
		for (size_t i = 0; i < indexSortVertices.size(); ++i)
		{
			//	i�Ԗڂ̒��_���
			Vertex findItem = indexSortVertices[i];

			try
			{
				//	�T���Ă��钸�_�̏�񂪊i�[����Ă���C���f�b�N�X���擾
				auto it = find_if(
					uniqueVertices.begin(),
					uniqueVertices.end(),
					[&](Vertex&v) {
					return v.v == findItem.v&&v.uv == findItem.uv;
				}
				);

				if (it == uniqueVertices.end())throw runtime_error("could not find a unique vertex");
				
				//	�C�e���[�^���C���f�b�N�X(�z��̓Y����)�ɕϊ�
				auto index = distance(uniqueVertices.begin(), it);
				mesh->vertexIndices.push_back(index);
			}
			catch (exception&error)
			{
				wic::SetColor(Red);
				cout << "Error" << endl;
				cout << error.what() << endl;
				wic::SetColor(Purple);
				cout << "Failed to reconfigure index." << endl;
				wic::SetColor(Red);
				cout << "uv set name:";
				wic::SetColor(White);
				cout << hash << endl;
				wic::SetColor(Red);
				cout << "index number:";
				wic::SetColor(White);
				cout << i << endl;
				wic::SetColor(Purple);
				cout << "iterator not found";
				wic::SetColor(White);
				continue;
			}
		}
	}

	//	�R���\�[���o��
	cout << endl;
	wic::SetColor(Yellow);
	cout << "mesh vertex size:";
	wic::SetColor(White);
	cout << mesh->vertices.size() << endl;
	wic::SetColor(Yellow);
	cout << "mesh index size:";
	wic::SetColor(White);
	cout << mesh->vertexIndices.size() << endl;
	wic::SetColor(Yellow);
	cout << "mesh uv size" << endl;
	const auto c_UVSetNamesCount = mesh->uvSetNamesList.size();

	for (size_t i = 0; i < c_UVSetNamesCount; i++)
	{
		auto it = mesh->uv[mesh->uvSetNamesList[i]][i];
		wic::SetColor(Yellow);
		cout << it.uvSetName << ":";
		wic::SetColor(White);
		cout << mesh->uv[mesh->uvSetNamesList[i]].size() << endl;
	}
}

