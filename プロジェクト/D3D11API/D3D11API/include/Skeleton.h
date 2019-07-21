/*
	@file	Skeleton.h
	@date	2019/07/20
	@author	番場 宥輝
	@brief	関節とスケルトンの構造体定義

	@note	ゲームエンジンアーキテクチャ参考
*/
#pragma once
#include <string>
#include <DirectXMath.h>
//#include <memory>
#include <vector>
/*!
	@struct	Joint
	@brief	関節
*/
struct Joint
{
	Joint() :invBindPose(
		{
			0,0,0,
			0,0,0,
			0,0,0,
			0,0,0,
		}
	) ,
	name("")
	{}
	~Joint(){}
	DirectX::XMFLOAT4X3 invBindPose;
	std::string name;
	uint8_t parent;
};

/*!
	@struct	Skeleton
	@brief	スケルトン

	@note	メッシュに対応した関節を保持する構造体
*/
struct Skeleton
{
	//Skeleton() :joints(nullptr) {}
	//uint32_t jointCount;
	////std::shared_ptr<Joint>joints;
	//Joint* joints;

	std::vector<Joint>joints;
};