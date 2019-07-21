#include "Common.h"
#include <Skeleton.h>
#include <SkeletonMesh.h>
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
	@param[in]	FBX�̃p�X
	@param[in]	�o�͐�̃t�@�C���p�X
*/
void Converter::FBXConverter::Execute(std::string fbxPath, std::string outName)
{

	cout << "----Display dialog----" << endl;

#pragma region �A�j���[�V�������b�V��
	//	�o�̓f�[�^
	//SkeletonMesh*outMesh = new SkeletonMesh;

	SkeletonMesh outMesh;
	AnimationClip animClip;
	try
	{
		Setup();
		//	�F�w��
		wic::SetColor(Green);

		//	�C���|�[�^�[
		if (!SetupImporter(fbxPath)) { throw runtime_error("Could not read \"" + fbxPath + "\"."); }
		cout << "SetupImporter success" << endl;

		//	�V�[��
		if (!SetupScene(fbxPath)) { throw runtime_error("Could not output \"" + fbxPath + "\" to the scene."); }
		cout << "SetupScene success" << endl;

		//	�C���|�[�^�[�̔j��
		TeardownImporter();

		//	�O�p��
		if (!Triangulate()) { throw runtime_error("Failed to triangulate this scene."); }
		cout << "Triangulate success" << endl;

		//	���b�V���̕���
		if (!SplitMeshesPerMaterial()) { throw runtime_error("Failed to split mesh"); }
		cout << "SplitMeshesPerMaterial success" << endl;

		//	���b�V���̑���
		int meshCount = (*m_pScene.get())->GetSrcObjectCount<FbxMesh>();

		//	���b�V�����P�̂��������m�F
		if (meshCount == 1) {
			cout << "���b�V������ł�" << endl;
		}
		else {
			cout << "���b�V���������ł�" << endl;
		}

		//	�P�ꉼ��
		auto pMesh = (*m_pScene.get())->GetSrcObject<fbxsdk::FbxMesh>(0);
		auto pNode = pMesh->GetNode();

		//	�A�j���[�V����
		FbxArray<FbxString*>animNameArray;
		(*m_pScene)->FillAnimStackNameArray(animNameArray);

		//	�A�j���[�V�����̊m�F
		if (animNameArray.GetCount() == 0) {
			wic::SetColor(Red);
			//	���b�Z�[�W��f���ď����𔲂���
			cout << "Failed to read animation!" << endl;
			return;
		}

		cout << "anim list" << endl;
		for (int i = 0; i < animNameArray.GetCount(); ++i) {
			auto it = animNameArray.GetAt(i);
			cout << it << endl;
		}
		cout << "anim list count = " << animNameArray.Size() << endl;

		auto setAnimData = animNameArray[0];
		//	���o�A�j���[�V�������
		auto animStack = (*m_pScene)->FindMember<FbxAnimStack>(setAnimData->Buffer());
		//	�A�j���[�V�����̐ݒ�
		(*m_pScene)->SetCurrentAnimationStack(animStack);

		auto info = (*m_pScene)->GetTakeInfo(*setAnimData);
		auto start = info->mLocalTimeSpan.GetStart();
		auto stop = info->mLocalTimeSpan.GetStop();
		FbxTime frameTime, timeCount;
		frameTime.SetTime(0, 0, 0, 1, 0, (*m_pScene)->GetGlobalSettings().GetTimeMode());
		timeCount = start;

		auto skinDeformer = (FbxSkin*)pMesh->GetDeformer(0, FbxDeformer::eSkin);
		int boneCount = skinDeformer->GetClusterCount();
#pragma region �W���C���g�̃������m��
		//outMesh->skeleton.jointCount = boneCount;
		//outMesh->skeleton.joints = new Joint[outMesh->skeleton.jointCount];

		//outMesh.skeleton.jointCount = boneCount;
		//outMesh.skeleton.joints = new Joint[outMesh.skeleton.jointCount];

		outMesh.skeleton.joints.resize(boneCount);
		animClip.bonesMatrix.resize(boneCount);

#pragma endregion

		wic::SetColor(Purple);
		cout << "BoneNum = " << boneCount << endl;


		//	�A�j���[�V�����̃t���[����
		unsigned int c_Frame = static_cast<unsigned int>(stop.Get() / frameTime.Get());
		//outMesh->frameMatrix.resize(c_Frame);
		animClip.frameCount = c_Frame;
#pragma region ���_
		//	���_��
		auto vertexCount = pMesh->GetControlPointsCount();

		//	���_�z��̐擪�|�C���^
		FbxVector4* vertices = pMesh->GetControlPoints();
		int vertexIndex = 0;
		while (vertexIndex < vertexCount)
		{
			SkinnedVertex sv;
			sv.position = {
				(float)vertices[vertexIndex][0],
				(float)vertices[vertexIndex][1],
				(float)vertices[vertexIndex][2],
			};
			outMesh.vertices.push_back(sv);
			++vertexIndex;
		}
		cout << "���_�� = " << vertexCount << endl;
#pragma endregion

#pragma region ���_�C���f�b�N�X
		//	�|���S����
		auto polygonCount = pMesh->GetPolygonCount();
		for (int i = 0; i < polygonCount; ++i)
		{
			//	�|���S�����\�����钸�_��(���O�p�����Ă����"3"�ɂȂ�)
			auto polygonVertexCount = pMesh->GetPolygonSize(i);
			if (polygonVertexCount != 3) { cout << "ERROR:�O�p���o���Ă��܂���" << endl; }
			for (int j = 0; j < polygonVertexCount; ++j)
			{
				outMesh.indices.push_back(
					{
						static_cast<uint32_t>(pMesh->GetPolygonVertex(i,j))
					}
				);
			}
		}
		cout << "�C���f�b�N�X�� = " << outMesh.indices.size() << endl;
#pragma endregion



		//	�{�[����
		//	�����O�ɒ��_,���_�C���f�b�N�X��ݒ肵�Ă��Ȃ��ƁA�͈͊O�Q�Ƃ��N�����I
		for (int i = 0; i < boneCount; ++i)
		{
			//	�N���X�^�擾
			auto cluster = skinDeformer->GetCluster(i);

			//	�N���X�^���e����^���钸�_�C���f�b�N�X�̐�
			auto indicesCount = cluster->GetControlPointIndicesCount();

			//	����p�̎g���܂킵�^
			FbxAMatrix fbxMat;

#pragma region �����p��
			//	�����p��
			cluster->GetTransformLinkMatrix(fbxMat);
			//fbxMat = fbxMat.Transpose();
			Joint* joint = new Joint;
			joint->invBindPose.m[0][0] = static_cast<float>(fbxMat.Get(0, 0));
			joint->invBindPose.m[0][1] = static_cast<float>(fbxMat.Get(0, 1));
			joint->invBindPose.m[0][2] = static_cast<float>(fbxMat.Get(0, 2));
			joint->invBindPose.m[0][3] = static_cast<float>(fbxMat.Get(0, 3));
			joint->invBindPose.m[1][0] = static_cast<float>(fbxMat.Get(1, 0));
			joint->invBindPose.m[1][1] = static_cast<float>(fbxMat.Get(1, 1));
			joint->invBindPose.m[1][2] = static_cast<float>(fbxMat.Get(1, 2));
			joint->invBindPose.m[1][3] = static_cast<float>(fbxMat.Get(1, 3));
			joint->invBindPose.m[2][0] = static_cast<float>(fbxMat.Get(2, 0));
			joint->invBindPose.m[2][1] = static_cast<float>(fbxMat.Get(2, 1));
			joint->invBindPose.m[2][2] = static_cast<float>(fbxMat.Get(2, 2));
			joint->invBindPose.m[2][3] = static_cast<float>(fbxMat.Get(2, 3));
			joint->invBindPose.m[3][0] = static_cast<float>(fbxMat.Get(3, 0));
			joint->invBindPose.m[3][1] = static_cast<float>(fbxMat.Get(3, 1));
			joint->invBindPose.m[3][2] = static_cast<float>(fbxMat.Get(3, 2));
			joint->invBindPose.m[3][3] = static_cast<float>(fbxMat.Get(3, 3));
			outMesh.skeleton.joints[i] = *joint;
			
			//Joint* joint=new Joint;
			//joint->invBindPose.m[0][0] = static_cast<float>(fbxMat.Get(0, 0));
			//joint->invBindPose.m[1][0] = static_cast<float>(fbxMat.Get(1, 0));
			//joint->invBindPose.m[2][0] = static_cast<float>(fbxMat.Get(2, 0));
			//joint->invBindPose.m[3][0] = static_cast<float>(fbxMat.Get(3, 0));
			//joint->invBindPose.m[0][1] = static_cast<float>(fbxMat.Get(0, 1));
			//joint->invBindPose.m[1][1] = static_cast<float>(fbxMat.Get(1, 1));
			//joint->invBindPose.m[2][1] = static_cast<float>(fbxMat.Get(2, 1));
			//joint->invBindPose.m[3][1] = static_cast<float>(fbxMat.Get(3, 1));
			//joint->invBindPose.m[0][2] = static_cast<float>(fbxMat.Get(0, 2));
			//joint->invBindPose.m[1][2] = static_cast<float>(fbxMat.Get(1, 2));
			//joint->invBindPose.m[2][2] = static_cast<float>(fbxMat.Get(2, 2));
			//joint->invBindPose.m[3][2] = static_cast<float>(fbxMat.Get(3, 2));
			//joint->invBindPose.m[0][3] = static_cast<float>(fbxMat.Get(0, 3));
			//joint->invBindPose.m[1][3] = static_cast<float>(fbxMat.Get(1, 3));
			//joint->invBindPose.m[2][3] = static_cast<float>(fbxMat.Get(2, 3));
			//joint->invBindPose.m[3][3] = static_cast<float>(fbxMat.Get(3, 3));
			//outMesh.skeleton.joints[i] = *joint;

			//Joint joint;
			//joint.invBindPose.m[0][0] = static_cast<float>(fbxMat.Get(0, 0));
			//joint.invBindPose.m[1][0] = static_cast<float>(fbxMat.Get(1, 0));
			//joint.invBindPose.m[2][0] = static_cast<float>(fbxMat.Get(2, 0));
			//joint.invBindPose.m[3][0] = static_cast<float>(fbxMat.Get(3, 0));
			//joint.invBindPose.m[0][1] = static_cast<float>(fbxMat.Get(0, 1));
			//joint.invBindPose.m[1][1] = static_cast<float>(fbxMat.Get(1, 1));
			//joint.invBindPose.m[2][1] = static_cast<float>(fbxMat.Get(2, 1));
			//joint.invBindPose.m[3][1] = static_cast<float>(fbxMat.Get(3, 1));
			//joint.invBindPose.m[0][2] = static_cast<float>(fbxMat.Get(0, 2));
			//joint.invBindPose.m[1][2] = static_cast<float>(fbxMat.Get(1, 2));
			//joint.invBindPose.m[2][2] = static_cast<float>(fbxMat.Get(2, 2));
			//joint.invBindPose.m[3][2] = static_cast<float>(fbxMat.Get(3, 2));
			//joint.invBindPose.m[0][3] = static_cast<float>(fbxMat.Get(0, 3));
			//joint.invBindPose.m[1][3] = static_cast<float>(fbxMat.Get(1, 3));
			//joint.invBindPose.m[2][3] = static_cast<float>(fbxMat.Get(2, 3));
			//joint.invBindPose.m[3][3] = static_cast<float>(fbxMat.Get(3, 3));
			
			//outMesh.skeleton.joints.push_back(joint);
			//outMesh.skeleton.joints[i] = joint;
#pragma endregion

#pragma region �t���[�����p��
			//	�t���[�����p��
			for (size_t f = 0; f < c_Frame; ++f) {
				auto time = start + frameTime * f;
				fbxMat = cluster->GetLink()->EvaluateGlobalTransform(time);
				fbxMat = fbxMat.Transpose();
				DirectX::XMMATRIX m =
				{
					static_cast<float>(fbxMat.Get(0,0)),static_cast<float>(fbxMat.Get(0,1)),static_cast<float>(fbxMat.Get(0,2)),static_cast<float>(fbxMat.Get(0,3)),
					static_cast<float>(fbxMat.Get(1,0)),static_cast<float>(fbxMat.Get(1,1)),static_cast<float>(fbxMat.Get(1,2)),static_cast<float>(fbxMat.Get(1,3)),
					static_cast<float>(fbxMat.Get(2,0)),static_cast<float>(fbxMat.Get(2,1)),static_cast<float>(fbxMat.Get(2,2)),static_cast<float>(fbxMat.Get(2,3)),
					static_cast<float>(fbxMat.Get(3,0)),static_cast<float>(fbxMat.Get(3,1)),static_cast<float>(fbxMat.Get(3,2)),static_cast<float>(fbxMat.Get(3,3)),
				};
				animClip.bonesMatrix[i].push_back(m);
			}
#pragma endregion

#pragma region �d��
			//	�e����^����C���f�b�N�X�̔z��
			auto indices = cluster->GetControlPointIndices();

			//	�e���x(�d��)�̔z��
			auto weights = cluster->GetControlPointWeights();

			//	�N���X�^���e����^���钸�_��
			for (int j = 0; j < indicesCount; ++j)
			{
				//	�Q�Ƃ���C���f�b�N�X
				auto index = indices[j];

				//	�d��
				auto weight = weights[j];

				outMesh.vertices[index].jointsIndex.push_back(i);
				outMesh.vertices[index].jointsWeight.push_back(weight);
			}
#pragma endregion
		}
#pragma region �d�݂̐��K��
		//for (auto& it : outMesh.vertices)
		//{
		//	//	���a
		//	float totalWeight = 0;
		//	for (auto weight : it.jointsWeight) {
		//		totalWeight += weight;
		//	}
		//	if (totalWeight == 1) { continue; }
		//	
		//	//	�W��
		//	float coefficient = 1.0f / (totalWeight);

		//	auto tes = coefficient * totalWeight;
		//	float tm = 0.0f;
		//	for (auto& weight : it.jointsWeight) {
		//		weight *= coefficient;
		//		tm += weight;
		//	}
		//	if (tm != 1.0f) {
		//		cout << "Error:" << static_cast<float>(tm) << endl;
		//	}
		//}

		//for (auto it : outMesh.vertices) {
		//	float totalWeight = 0;
		//	for (auto weight : it.jointsWeight) {
		//		totalWeight += weight;
		//	}
		//	if (totalWeight != 1.0f) {
		//		cout << "���K�����s" << endl;
		//	}
		//}
#pragma endregion

		cout << "�����o��" << endl;

		Utility::IOMesh::Output(outName, outMesh,animClip);

		cout << "start:" << start.Get() << endl;
		cout << "stop:" << stop.Get() << endl;
		cout << "ft:" << frameTime.Get() << endl;

	}
	catch (...) {}

	//#endif
#pragma endregion


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
		if (to->vertexIndices.empty())throw runtime_error("SetupVertexIndices");
		wic::SetColor(Green);
		cout << "SetupVertexIndices success" << endl;

		//	���_
		SetupVertices(from, to);
		if (to->vertices.empty())throw runtime_error("SetupVertices");
		wic::SetColor(Green);
		cout << "SetupVertices success" << endl;

		//	UV
		SetupUV(from, to);
		if (to->uv.empty())throw runtime_error("SetupUV");
		wic::SetColor(Green);
		cout << "SetupUV success" << endl;

		//	���_�̐�����
		AlignVerticesToUV(to);
	}
	catch (exception&error)
	{
		wic::SetColor(Red);
		cout << "Failed to \"" << error.what() << "\"" << endl;
		wic::SetColor(White);
	}
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

	wic::SetColor(Cyan);
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
	wic::SetColor(Cyan);
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

	wic::SetColor(Cyan);
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

		wic::SetColor(Purple);
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
	wic::SetColor(Cyan);
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
	wic::SetColor(Cyan);
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
				cout << "Failed to reconfigure index." << endl;
				cout << "uv set name:";
				wic::SetColor(White);
				cout << hash << endl;
				wic::SetColor(Red);
				cout << "index number:";
				wic::SetColor(White);
				cout << i << endl;
				wic::SetColor(Red);
				cout << "iterator not found";
				wic::SetColor(White);
				continue;
			}
		}
	}

	//	�R���\�[���o��
	cout << endl;
	wic::SetColor(Cyan);
	cout << "mesh vertex size:";
	wic::SetColor(White);
	cout << mesh->vertices.size() << endl;
	wic::SetColor(Cyan);
	cout << "mesh index size:";
	wic::SetColor(White);
	cout << mesh->vertexIndices.size() << endl;
	wic::SetColor(Cyan);
	cout << "mesh uv size" << endl;
	const auto c_UVSetNamesCount = mesh->uvSetNamesList.size();

	for (size_t i = 0; i < c_UVSetNamesCount; i++)
	{
		wic::SetColor(Cyan);
		cout << mesh->uvSetNamesList[i] << ":";
		wic::SetColor(White);
		cout << mesh->uv[mesh->uvSetNamesList[i]].size() << endl;
	}
}

