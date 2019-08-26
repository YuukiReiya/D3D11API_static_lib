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
#include <MyGame.h>

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
	@fn			ConvertRelativePathToFileName
	@brief		���΃p�X���t�@�C�����ɕϊ�
	@param[in]	���΃p�X
	@return		�ϊ���̃t�@�C����
*/
static string ConvertRelativePathToFileName(string relativePath)
{
	string fileName = "";
	auto offset = relativePath.rfind("\\");
	if (offset == string::npos) {
		offset = relativePath.rfind("/");
	}
	fileName = relativePath.substr(offset + 1);
	return fileName;
}

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
	@param[in]	�����o�����b�V���̃^�C�v
	@param[in]	FBX�̃p�X
	@param[in]	�o�͐�̃t�@�C���p�X
*/
void Converter::FBXConverter::Execute(OutputType type, std::string fbxPath, std::string outputName)
{
	//	������
	Setup();

	//	�o�̓J���[
	wic::SetColor(Green);

	try
	{
		//	�C���|�[�^�[
		if (!SetupImporter(fbxPath)) { throw runtime_error("Could not read \"" + fbxPath + "\"."); }
		cout << "SetupImporter success" << endl;

		//	�V�[��
		if (!SetupScene(fbxPath)) { throw runtime_error("Could not output \"" + fbxPath + "\" to the scene."); }
		cout << "SetupScene success" << endl;

		//	�O�p��
		if (!Triangulate()) { throw runtime_error("Failed to triangulate this scene."); }
		cout << "Triangulate success" << endl;

		//	���b�V���̕���
		if (!SplitMeshesPerMaterial()) { throw runtime_error("Failed to split mesh"); }
		cout << "SplitMeshesPerMaterial success" << endl;

		//	���b�V���̑���
		int meshCount = (*m_pScene.get())->GetSrcObjectCount<FbxMesh>();

		//	�����o����
		uint32_t outputCount = 0;

		for (int i = 0; i < meshCount; ++i)
		{
			auto pMesh= (*m_pScene.get())->GetSrcObject<FbxMesh>(i);
		
			//	�X�L�����b�V���ݒ�ŃX�L���������ꍇ�A��
			if (type == OutputType::SKIN&&pMesh->GetDeformerCount(FbxDeformer::EDeformerType::eSkin) <= 0) { continue; }

			//	�����o�����J�E���g
			outputCount++;

			//	�����o����
			std::string outputFullPath = outputName + "/";
			outputFullPath += meshCount == 1 ? outputName : to_string(outputCount) + "/";

			//	���_�C���f�b�N�X
			vector<uint32_t>indices;
			SetupVertexIndices(*pMesh, indices);

			//	UV
			vector<DirectX::XMFLOAT2>uv;
			SetupUV(*pMesh, uv);

			//	���_���W
			vector<D3D11::Graphic::SkinnedVertex>vertices;
			SetupVertices(*pMesh, vertices);

			//	���_�̐���
			AlignVerticesToUV(indices, uv, vertices);

			//	�A�j���[�V����
			vector<API::AnimationClip> clips;
			SetupAnimation(*pMesh,clips);

			//	�����o��
			Utility::IOMesh::OutputSkinMesh(outputFullPath + outputName, indices, vertices);

			//	�X�L���ݒ肶��Ȃ���΃A�j���[�V�����͏����o���Ȃ�
			if (type != OutputType::SKIN) { continue; }

			const bool isMultiAnimation = clips.size() != 1;
			if (isMultiAnimation)
			{
				for (size_t animCount = 0; animCount < clips.size(); ++animCount)
				{
					string fileName = outputFullPath + "anim-" + to_string(animCount);
					Utility::IOMesh::OutputAnimation(fileName, clips[animCount]);
				}

			}
			else {
				Utility::IOMesh::OutputAnimation(outputName + "/" + "anim", clips[0]);
			}
		}
	}
	catch (exception&e)
	{
		wic::SetColor(Red);
		cout << "ERROR" << endl;
		cout << e.what() << endl;
		return;
	}

	wic::SetColor(Green);
	cout << "Completed this program." << endl;
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
	return converter.SplitMeshesPerMaterial(*m_pScene.get(), true);
}

/*!
	@fn			SetupImporter
	@brief		fbx�t�@�C�����C���|�[�^�[�ɓǂݍ��ށB
	@param[in]	�ǂݍ���.fbx�t�@�C���̃p�X
*/
bool Converter::FBXConverter::SetupImporter(std::string fbxPath)
{
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
	return (*m_pImporter.get())->Import(*m_pScene.get());
}

void Converter::FBXConverter::SetupMaterial(fbxsdk::FbxSurfaceMaterial * material)
{

}

void Converter::FBXConverter::SetupMaterial(fbxsdk::FbxMesh * from)
{
	//	���b�V���̃��[�g�m�[�h�擾
	auto node = from->GetNode();
	if (node == 0) { return; }

	//	�}�e���A����
	auto matCount = node->GetMaterialCount();
	if (matCount == 0) { return; }

	//	�}�e���A�������擾
	for (int i = 0; i < matCount; i++)
	{
		auto material = node->GetMaterial(i);
		if (material == 0) { continue; }

#pragma region //�_�E���L���X�g
		//	�����̐݌v��FBXSDK�ɍ��킹�Ă���

		FbxSurfaceLambert*lambert=nullptr;
		FbxSurfacePhong* phong = nullptr;

		//	lambert
		if (material->GetClassId().Is(FbxSurfaceLambert::ClassId)) {
			lambert = (FbxSurfaceLambert*)material;
		}

		//	phong
		if (material->GetClassId().Is(FbxSurfacePhong::ClassId)) {
			phong = (FbxSurfacePhong*)material;
		}

		auto castMat = material->GetClassId().Is(FbxSurfacePhong::ClassId) ?
			(FbxSurfacePhong*)material :
			material->GetClassId().Is(FbxSurfaceLambert::ClassId) ?
			(FbxSurfaceLambert*)material : nullptr;
		if (castMat == nullptr) {
			cout << "��΂��o�O" << endl;
			system("pause");
			exit(NULL);
		}
#pragma endregion
		////	�A���r�G���g
		//g_pMat->ambient =
		//{
		//	(float)castMat->Ambient.Get()[0],
		//	(float)castMat->Ambient.Get()[1],
		//	(float)castMat->Ambient.Get()[2]
		//};

		////	�f�B�t���[�Y
		//g_pMat->diffuse =
		//{
		//	(float)castMat->Diffuse.Get()[0],
		//	(float)castMat->Diffuse.Get()[1],
		//	(float)castMat->Diffuse.Get()[2],
		//};

		////	�G�~�b�V�u
		//g_pMat->emissive =
		//{
		//	(float)castMat->Emissive.Get()[0],
		//	(float)castMat->Emissive.Get()[1],
		//	(float)castMat->Emissive.Get()[2],
		//};

		////	�o���v�}�b�v
		//g_pMat->bumpMap =
		//{
		//	(float)castMat->Bump.Get()[0],
		//	(float)castMat->Bump.Get()[1],
		//	(float)castMat->Bump.Get()[2],
		//};

		////	���ߓx
		//g_pMat->transparent = static_cast<float>(castMat->TransparencyFactor.Get());

		////	�}�e���A����"Phong"�Ȃ�
		//if (castMat->GetClassId().Is(FbxSurfacePhong::ClassId)) {

		//	//	�X�y�L�����[
		//	Math::FLOAT4 sp =
		//	{
		//		(float)phong->Specular.Get()[0],
		//		(float)phong->Specular.Get()[1],
		//		(float)phong->Specular.Get()[2],
		//	};
		//	g_pMat->specular = { &sp };

		//	//	����
		//	float sh = (float)phong->Shininess.Get();
		//	g_pMat->shiniess = { &sh };

		//	//	����
		//	float refl = (float)phong->ReflectionFactor.Get();
		//	g_pMat->reflection = { &refl };
		//}

	}
}

/*!
	@note	���̃}�e���A���Ɏg�p���Ă���e�N�X�`���f�[�^�̃Z�b�g�A�b�v
	
*/
void Converter::FBXConverter::SetupTextures(fbxsdk::FbxSurfaceMaterial * material)
{
	//	�f�B�t���[�Y
	auto prop = material->FindProperty(FbxSurfaceMaterial::sDiffuse);

	//	�����̃e�N�X�`�����u�����h��������(���C���[�e�N�X�`��)
	auto layerTexCount = prop.GetSrcObjectCount<FbxLayeredTexture>();
	for (int i = 0; i < layerTexCount; ++i)
	{
		auto layeredTexture = prop.GetSrcObject<FbxLayeredTexture>(i);
		wic::SetColor(Purple);
		SetupLayerTextures(&prop, layeredTexture);
	}

	//	�P��̂���
	auto texCount = prop.GetSrcObjectCount<FbxFileTexture>();
	for (int i = 0; i < texCount; i++)
	{
		auto tex = prop.GetSrcObject<FbxFileTexture>(i);
		wic::SetColor(Purple);
		if (!tex) { continue; }
		cout << "Texture " << i << ":";
		SetupTexture(tex);
	}

	//TODO: ���ꂪ���藧�̂�"layerTexCount = texCount = 0"�̎��̂�
	//		������ɂ����̂�"layerTexCount == 0 && texCount == 0"����������...
	//NOTE:	�����ɓ���̂�".tga"�Ȃǃe�N�X�`���ǂݍ��݂Ɏ��s��������
	if (layerTexCount == texCount)
	{
		
	}
	
}

void Converter::FBXConverter::SetupLayerTextures(fbxsdk::FbxProperty * prop, fbxsdk::FbxLayeredTexture * layerdTexture)
{
	auto texCount = layerdTexture->GetSrcObjectCount<FbxFileTexture>();
	for (int i = 0; i < texCount; ++i)
	{
		auto texture = prop->GetSrcObject<FbxFileTexture>(i);
		wic::SetColor(Purple);
		if (!texture) { continue; }
		cout << "Layered Texture " << i << ":";
		SetupTexture(texture);
	}
}

void Converter::FBXConverter::SetupTexture(fbxsdk::FbxFileTexture * texture)
{
	//TODO:�}�e���A�����̃e�N�X�`�����擾����

	//FbxMaterialConverter
	//	�t�@�C�����̕\��
	wic::SetColor(White);
	cout << ConvertRelativePathToFileName(texture->GetRelativeFileName()) << endl;
}

/*!
	@fn			SetupVertexIndices
	@brief		���_�C���f�b�N�X�̃Z�b�g�A�b�v
	@param[in]	FBX���b�V��
	@param[in]	���_�C���f�b�N�X�i�[�p�̉ϒ��z��
*/
void Converter::FBXConverter::SetupVertexIndices(fbxsdk::FbxMesh & mesh, std::vector<uint32_t>& indices)
{
	auto polygonCount = mesh.GetPolygonCount();
	for (int i = 0; i < polygonCount; ++i)
	{
		//	�|���S�����\�����钸�_��(�O�p�����Ă���ΕK��"3")
		auto polygonVertexCount = mesh.GetPolygonSize(i);
		
		//	���_��"3"�R�ɂȂ��Ă��Ȃ���Έȍ~�̓���͖��ۏ�Ȃ̂ŃG���[������d����ǂ�
		constexpr uint32_t c_PolygonVertexCount = 3;
		if (polygonVertexCount != c_PolygonVertexCount)
		{
			throw runtime_error("���_���O�p���o���Ă��܂���B");
		}
		for (int j = 0; j < polygonVertexCount; ++j)
		{
			indices.push_back(
				{
					static_cast<uint32_t>(mesh.GetPolygonVertex(i,j))
				}
			);
		}
	}
}

/*!
	@fn			SetupVertices
	@brief		���_�̃Z�b�g�A�b�v
	@param[in]	�Q�ƌ���fbx���b�V��
	@param[in]	���_���i�[�p�̉ϒ��z��
*/
void Converter::FBXConverter::SetupVertices(fbxsdk::FbxMesh & mesh, std::vector<D3D11::Graphic::SkinnedVertex>& vertices)
{
	auto vertexCount = mesh.GetControlPointsCount();
	FbxVector4*controlPoints = mesh.GetControlPoints();
	//	���W
	for (int i = 0; i < vertexCount; ++i)
	{
		D3D11::Graphic::SkinnedVertex v;
		v.position = {
			static_cast<float>(controlPoints[i][0]),
			static_cast<float>(controlPoints[i][1]),
			static_cast<float>(controlPoints[i][2]), 
		};
		v.uv = { 0,0 };
		vertices.push_back(v);
	}
	//	UV


	//	�W���C���g���(�d��)
	auto skinDeformerCount = mesh.GetDeformerCount(FbxDeformer::EDeformerType::eSkin);
	//cout << "Skin Deformer Count = " << skinDeformerCount << endl;
	if (skinDeformerCount != 1) { throw runtime_error("Skin Deformer Count != 1"); }
	for (int skinDeformerIndex = 0; skinDeformerIndex < skinDeformerCount; ++skinDeformerIndex)
	{
		auto skinDeformer = static_cast<FbxSkin*>(mesh.GetDeformer(skinDeformerIndex, FbxDeformer::EDeformerType::eSkin));
		const int clusterCount = skinDeformer->GetClusterCount();
		for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
		{
			auto cluster = skinDeformer->GetCluster(clusterIndex);
			auto indicesCount = cluster->GetControlPointIndicesCount();
			for (int i = 0; i < indicesCount; ++i)
			{
				auto index = cluster->GetControlPointIndices()[i];
				auto weight = cluster->GetControlPointWeights()[i];
				vertices[index].joints.push_back({ static_cast<uint32_t>(clusterIndex), static_cast<float>(weight) });
			}
		}
	}
}

/*!
	@fn			SetupUV
	@brief		UV�̃Z�b�g�A�b�v
	@detail		���_�C���f�b�N�X�ɑΉ�����UV���i�[�����B
	@param[in]	FBX���b�V��
	@param[in]	UV�i�[�p�̉ϒ��z��
*/
void Converter::FBXConverter::SetupUV(fbxsdk::FbxMesh & mesh, std::vector<DirectX::XMFLOAT2>& uv)
{
	//	UV�Z�b�g��
	FbxStringList uvSetNameList;
	mesh.GetUVSetNames(uvSetNameList);
	const int c_UVSetCount = uvSetNameList.GetCount();
	if (c_UVSetCount < 0 || c_UVSetCount>1) {
		wic::SetColor(Yellow);
		cout << "Warning" << endl << "�}�e���A����������肭�s���Ă��܂���B" << endl;
		cout << "���Ӑ}���Ȃ�UV���W�ɂȂ�ꍇ������܂��B" << endl;
	}
	FbxArray<FbxVector2>positions;
	const string c_UVSetName = uvSetNameList.GetStringAt(0);
	mesh.GetPolygonVertexUVs(c_UVSetName.c_str(), positions);
	for (int j = 0; j < positions.Size(); ++j)
	{
		uv.push_back(
			{
				static_cast<float>(positions[j][0]),
				1.0f - static_cast<float>(positions[j][1]),
			}
		);
	}
}

/*!
	@fn			AlignVerticesToUV
	@brief		���_��UV�ɑ�����B
	@detail		���O�ɑ��̏������낦�Ă����K�v������
	@param[in]	�Ώۂ̃��b�V��
*/
void Converter::FBXConverter::AlignVerticesToUV(std::vector<uint32_t>& indices, std::vector<DirectX::XMFLOAT2>& uv, std::vector<D3D11::Graphic::SkinnedVertex>& vertices)
{
	//	���_���Ɋi�[
	vector<D3D11::Graphic::SkinnedVertex>skinVertices;
	for (size_t i = 0; i < indices.size(); ++i)
	{
		D3D11::Graphic::SkinnedVertex val = vertices[indices[i]];
		val.uv = uv[i];
		skinVertices.push_back(val);
	}

	//	�C���f�b�N�X��Cpy�o�b�t�@
	auto indicesCpy = indices;

	//	�����N���A
	indices.clear();
	vertices.clear();

	//	��ӂȒ��_
	vector<D3D11::Graphic::SkinnedVertex>uniqueSkinVertices;
	for (size_t i = 0; i < indicesCpy.size(); ++i)
	{
		auto v = skinVertices[i];
		auto itr = find(uniqueSkinVertices.begin(), uniqueSkinVertices.end(), v);
		if (uniqueSkinVertices.end() != itr)
		{
			indices.push_back(std::distance(uniqueSkinVertices.begin(), itr));
			continue;
		}
		uniqueSkinVertices.push_back(v);
		indices.push_back(uniqueSkinVertices.size() - 1);
	}

	//	�R�s�[
	vertices = uniqueSkinVertices;
}

/*!
	@fn			SetupAnimation
	@brief		�A�j���[�V�������̃Z�b�g�A�b�v
	@param[in]	FBX���b�V��
	@param[in]	�o�C���h����A�j���[�V�����N���b�v
*/
void Converter::FBXConverter::SetupAnimation(FbxMesh&mesh, std::vector<API::AnimationClip>& clips)
{
	auto animStackCount = (*m_pScene.get())->GetSrcObjectCount<FbxAnimStack>();
	auto pNode = mesh.GetNode();
	for (int animIndex = 0; animIndex < animStackCount; ++animIndex)
	{
		auto pAnimStack = FbxCast<FbxAnimStack>((*m_pScene.get())->GetSrcObject<FbxAnimStack>(animIndex));
		(*m_pScene)->SetCurrentAnimationStack(pAnimStack);

		//	�A�j���[�V�������
		auto info = (*m_pImporter)->GetTakeInfo(animIndex);
		
		//	���[�J������
		//auto offset = info->mImportOffset;
		auto localStartTime = info->mLocalTimeSpan.GetStart();
		auto localStopTime = info->mLocalTimeSpan.GetStop();
		
		FbxTime timeSpan;
		timeSpan.SetTime(0, 0, 0, 1, 0,(*m_pScene)->GetGlobalSettings().GetTimeMode());
		
		//	60fps
		//float startTime = (offset.Get() + localStartTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::EMode::eFrames60);
		//float stopTime = (offset.Get() + localStopTime.Get()) / FbxTime::GetOneFrameValue(FbxTime::EMode::eFrames60);

		//	�A�j���[�V�����t���[��
		const size_t c_Frame = static_cast<size_t>((localStopTime.Get() - localStartTime.Get()) / timeSpan.Get());

		//	�s��p���b�g[�t���[��][�W���C���g]
		API::AnimationClip buf;
		buf.matrixPalette.resize(c_Frame);
		for (size_t frame = 0; frame < c_Frame; frame++)
		{
			auto skinDeformerCount = mesh.GetDeformerCount(FbxDeformer::EDeformerType::eSkin);
			//cout << "Skin Deformer Count = " << skinDeformerCount << endl;
			if (skinDeformerCount != 1) { throw runtime_error("Skin Deformer Count != 1"); }
			for (int skinDeformerIndex = 0; skinDeformerIndex < skinDeformerCount; ++skinDeformerIndex)
			{
				auto skinDeformer = static_cast<FbxSkin*>(mesh.GetDeformer(skinDeformerIndex, FbxDeformer::EDeformerType::eSkin));

				//
				SetupCluster(
					*skinDeformer,
					pNode->EvaluateGlobalTransform(frame*timeSpan.Get()),
					FbxAMatrix(
						pNode->GetGeometricTranslation(FbxNode::EPivotSet::eSourcePivot),
						pNode->GetGeometricRotation(FbxNode::EPivotSet::eSourcePivot),
						pNode->GetGeometricScaling(FbxNode::EPivotSet::eSourcePivot)
					),
					frame * timeSpan.Get(),
					buf.matrixPalette[frame]
				);

				
			}
		}

		//	�W���C���g��
		const size_t c_JointCount = buf.matrixPalette[0].size();

		//	�s��p���b�g[�W���C���g][�t���[��]�ɕϊ�
		API::AnimationClip clip;
		clip.matrixPalette.resize(c_JointCount);
		for (auto& it : clip.matrixPalette) { it.resize(c_Frame); }
		for (size_t frame = 0; frame < c_Frame; frame++)
		{
			for (size_t jointIndex = 0; jointIndex < c_JointCount; jointIndex++)
			{
				clip.matrixPalette[jointIndex][frame] = buf.matrixPalette[frame][jointIndex];
			}
		}

		clips.push_back(clip);
	}
}

/*!
	@fn			SetupCluster
	@brief		�N���X�^(�W���C���g)�̃Z�b�g�A�b�v
	@param[in]	���b�V���̃X�L��
	@param[in]	�w�莞�Ԃ̃O���[�o���I�t�Z�b�g�s��
	@param[in]	���b�V���̃I�t�Z�b�g(TRS)
	@param[in]	�A�j���[�V�����̎���
	@param[in]	�X�L���s����i�[����s��p���b�g
*/
void Converter::FBXConverter::SetupCluster(fbxsdk::FbxSkin & skin, FbxMatrix evaluateGlobalTimeMatrix, FbxAMatrix geometryOffset, FbxTime animTime, API::MatrixPalette& matrixPalette)
{
	//	�N���X�^��
	auto clusterCount = skin.GetClusterCount();
	for (int i = 0; i < clusterCount; ++i)
	{
		auto cluster = skin.GetCluster(i);

		//	�X�L�j���O�s��
		FbxMatrix skinningMatrix;

		//
		FbxMatrix clusterGlobalCurrentPosition, clusterRelativeInitPosition, clusterRelativeCurrentPositionInverse;
		FbxAMatrix referenceGlobalInitPosition, clusterGlobalInitPosition;

		cluster->GetTransformMatrix(referenceGlobalInitPosition);
		referenceGlobalInitPosition *= geometryOffset;
		cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
		clusterGlobalCurrentPosition = cluster->GetLink()->EvaluateGlobalTransform(animTime);
		clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse()*referenceGlobalInitPosition;
		clusterRelativeCurrentPositionInverse = evaluateGlobalTimeMatrix.Inverse()*clusterGlobalCurrentPosition;

		skinningMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
		DirectX::XMMATRIX mat = {
			static_cast<float>(skinningMatrix.Get(0,0)),static_cast<float>(skinningMatrix.Get(0,1)),static_cast<float>(skinningMatrix.Get(0,2)),static_cast<float>(skinningMatrix.Get(0,3)),
			static_cast<float>(skinningMatrix.Get(1,0)),static_cast<float>(skinningMatrix.Get(1,1)),static_cast<float>(skinningMatrix.Get(1,2)),static_cast<float>(skinningMatrix.Get(1,3)),
			static_cast<float>(skinningMatrix.Get(2,0)),static_cast<float>(skinningMatrix.Get(2,1)),static_cast<float>(skinningMatrix.Get(2,2)),static_cast<float>(skinningMatrix.Get(2,3)),
			static_cast<float>(skinningMatrix.Get(3,0)),static_cast<float>(skinningMatrix.Get(3,1)),static_cast<float>(skinningMatrix.Get(3,2)),static_cast<float>(skinningMatrix.Get(3,3)),
		};

		matrixPalette.push_back(mat);
	}
}