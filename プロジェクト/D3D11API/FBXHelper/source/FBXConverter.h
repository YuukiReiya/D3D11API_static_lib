/*!
	@file	FBXConverter.h
	@date	2019/04/19
	@author	�ԏ� �G�P
	@brief	FBX_SDK��Manager,Scene�������w���p�[�N���X
	@detail	�V���O���g��
*/
#pragma once
#include "Mesh.h"
#include "OriginalFormatMath.h"
#include <fbxsdk/scene/geometry/fbxmesh.h>
#include <fbxsdk/scene/geometry/fbxlayer.h>
#include <fbxsdk/core/fbxmanager.h>
#include <fbxsdk/fileio/fbximporter.h>
#include <fbxsdk/scene/shading/fbxlayeredtexture.h>
#include <fbxsdk/scene/shading/fbxfiletexture.h>
#include <fbxsdk/scene/geometry/fbxskin.h>
#include <fbxsdk/core/math/fbxmatrix.h>
#include <fbxsdk/core/math/fbxaffinematrix.h>
#include <DirectXMath.h>
#include <AnimationClip.h>
#include <Joint.h>
#include <SkinnedVertex.h>
namespace Converter {
	class FBXConverter
	{
	private:
		/*!
			@brief	�R���X�g���N�^
		*/
		FBXConverter();

		/*!
			@brief	�f�X�g���N�^
		*/
		~FBXConverter();

	public:
		/*!
			@fn		Setup
			@brief	�������B
			@detail	�����o�̃C���X�^���X���Ə�����
		*/
		static void Setup();

		/*!
			@fn		Teardown
			@brief	�j�������B
		*/
		static void Teardown();

		/*!
			@fn			Execute
			@brief		���s����
			@detail		FBX��ǂݍ��݊O���t�@�C���ɏ����o���B
			@param[in]	FBX�̃p�X
			@param[in]	�o�͐�̃t�@�C���p�X
		*/
		static void Execute(std::string fbxPath,std::string outName);

		/*!
			@enum	OutputType
			@brief	�����o�����b�V���̃^�C�v
		*/
		enum OutputType
		{
			/*!
				@var	STATIC
				@brief	�ÓI���b�V��
			*/
			STATIC,

			/*!
				@var	SKIN
				@brief	�X�L�����b�V��
			*/
			SKIN,
		};

		/*!
			@fn			Execute
			@brief		���s����
			@detail		FBX��ǂݍ��݊O���t�@�C���ɏ����o���B
			@param[in]	�����o�����b�V���̃^�C�v
			@param[in]	FBX�̃p�X
			@param[in]	�o�͐�̃t�@�C���p�X
		*/
		static void Execute(OutputType type,std::string fbxPath,std::string outputName);
	private:
		/*!
			@fn		Triangulate
			@brief	�V�[�����̃��f���̎O�p�|���S�����B
			@note	try catch�����ꂽ�P���ȃ��b�p�[
			@return	true:���� false:���s
		*/
		static bool Triangulate();

		/*!
			@fn		SplitMeshesPerMaterial
			@brief	�V�[�����̃��f�����}�e���A���P�ʂɕ���
			@note	try catch�����ꂽ�P���ȃ��b�p�[	
			@return	true:���� false:���s
		*/
		static bool SplitMeshesPerMaterial();

		/*!
			@fn			SetupImporter
			@brief		fbx�t�@�C�����C���|�[�^�[�ɓǂݍ��ށB
			@param[in]	�ǂݍ���.fbx�t�@�C���̃p�X
		*/
		static bool SetupImporter(std::string fbxPath);

		/*!
			@fn		TeardownImporter
			@brief	�C���|�[�^�[�̔j�������B
		*/
		static void TeardownImporter();

		/*!
			@fn			SetupScene
			@brief		�C���|�[�^�[�ɓǂݍ��񂾃f�[�^���V�[���ɃC���|�[�g�B
			@detail		�ǂݍ��݌�ɃC���|�[�^�[�͔j�����Ă���B
			@param[in]	�ǂݍ���.fbx�t�@�C���̃p�X
		*/
		static bool SetupScene(std::string fbxPath);

		/*!
			@fn			LoadToStore
			@brief		fbx�̃f�[�^���i�[
			@param[in]	���o���̃f�[�^
			@param[in]	�i�[�惁�b�V��
		*/
		static void LoadToStore(fbxsdk::FbxMesh*from, Utility::Mesh*to);

		static void SetupMaterial(fbxsdk::FbxSurfaceMaterial*material);
		static void SetupMaterial(fbxsdk::FbxMesh* from);

		static void SetupTextures(fbxsdk::FbxSurfaceMaterial * material);

		static void SetupLayerTextures(fbxsdk::FbxProperty*prop, fbxsdk::FbxLayeredTexture*layerdTexture);

		static void SetupTexture(fbxsdk::FbxFileTexture*texture);

		/*!
			@fn			SetupVertexIndices
			@brief		���_�C���f�b�N�X�̃Z�b�g�A�b�v
			@param[in]	�Q�ƌ���fbx���b�V��
			@param[in]	�o�C���h��̃��b�V��
		*/
		static void SetupVertexIndices(fbxsdk::FbxMesh* from, Utility::Mesh*to);

		/*!
			@fn			SetupVertexIndices
			@brief		���_�C���f�b�N�X�̃Z�b�g�A�b�v
			@param[in]	FBX���b�V��
			@param[in]	���_�C���f�b�N�X�i�[�p�̉ϒ��z��
		*/
		static void SetupVertexIndices(fbxsdk::FbxMesh& mesh, std::vector<uint32_t>&indices);

		/*!
			@fn			SetupVertices
			@brief		���_�̃Z�b�g�A�b�v
			@param[in]	�Q�ƌ���fbx���b�V��
			@param[in]	�o�C���h��̃��b�V��
		*/
		static void SetupVertices(fbxsdk::FbxMesh*from, Utility::Mesh*to);

		static void SetupVertices(fbxsdk::FbxMesh&mesh, std::vector<D3D11::Graphic::SkinnedVertex>&vertices);

		/*!
			@fn			SetupUV
			@brief		UV�̃Z�b�g�A�b�v
			@detail		���_�C���f�b�N�X�ɑΉ�����UV���i�[�����B
			@param[in]	�Q�ƌ���fbx���b�V��
			@param[in]	�o�C���h��̃��b�V��
		*/
		static void SetupUV(fbxsdk::FbxMesh* from, Utility::Mesh*to);

		/*!
			@fn			SetupUV
			@brief		UV�̃Z�b�g�A�b�v
			@detail		���_�C���f�b�N�X�ɑΉ�����UV���i�[�����B
			@param[in]	FBX���b�V��
			@param[in]	UV�i�[�p�̉ϒ��z��
		*/
		static void SetupUV(fbxsdk::FbxMesh& from, std::vector<DirectX::XMFLOAT2>&uv);

		/*!
			@fn			AlignVerticesToUV
			@brief		���_��UV�ɑ�����B
			@detail		���O�ɑ��̏������낦�Ă����K�v������
			@param[in]	�Ώۂ̃��b�V��
		*/
		static void AlignVerticesToUV(Utility::Mesh*mesh);

		/*!
			@fn		SetupJoint
			@brief	�W���C���Ə��̃Z�b�g�A�b�v
			@detail	�����O��
		*/
		static void SetupJoint(fbxsdk::FbxMesh&mesh, std::vector<D3D11::Graphic::Joint>&joints);
		static void SetupAnimation(fbxsdk::FbxMesh&mesh,std::vector<API::AnimationClip>& clip);
		static void SetupCluster(fbxsdk::FbxSkin&skin, fbxsdk::FbxMatrix evaluateGlobalTimeMatrix, fbxsdk::FbxAMatrix geometryOffset, fbxsdk::FbxTime animTime, API::MatrixPalette& matrixPalette);
		/*!
			@var	m_pManager
			@brief	FbxManager�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxManager*>		m_pManager;

		/*!
			@var	m_pIOsetting
			@brief	FbxIOSettings�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxIOSettings*>	m_pIOsetting;

		/*!
			@var	m_pScene
			@brief	FbxScene�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxScene*>		m_pScene;

		/*!
			@var	m_pImporter
			@brief	FbxImporter�̃|�C���^
			@detail	�X�}�[�g�|�C���^�ŃA�h���X�Ǘ�
		*/
		static std::shared_ptr<fbxsdk::FbxImporter*>	m_pImporter;

		/*!
			@var	c_OutputDirectory
			@brief	�O���t�@�C���̎�͐�f�B���N�g��
		*/
		static constexpr std::string_view c_OutputDirectory = "Output/";

	};

}