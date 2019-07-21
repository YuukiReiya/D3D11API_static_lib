#pragma once
#include <vector>
#include <DirectXMath.h>
#include <memory>
#include "Skeleton.h"

//NOTE:�����o�������Ȃ��Ȃ�ʖ���`�ł���
//using AnimationJoints = std::vector<DirectX::XMMATRIX>;
//struct FrameMatrix
//{
//	std::vector<DirectX::XMMATRIX>FrameMatrix;
//
//};
using FrameMatrix = std::vector<DirectX::XMMATRIX>;

struct AnimationClip
{
	Skeleton*pSkeleton;//�֘A�X�P���g��
	uint32_t frameCount;//	�t���[����
	//std::vector<DirectX::XMMATRIX>jointsMatrix;//�t���[���s��
	std::vector<FrameMatrix>bonesMatrix;//�{�[�����̃t���[���s�񂪓����Ă���

};