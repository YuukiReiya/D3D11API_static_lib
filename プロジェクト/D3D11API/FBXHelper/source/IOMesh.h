/*
	@file	IOMesh.h
	@date	2019/04/27
	@author	�ԏ� �G�P
	@brief	���b�V���f�[�^�̂̓��o�͊֌W
	@detail	�V���O���g��
*/
#pragma once
#include <fstream>
#include <string_view>
#include <iostream>
#include "Mesh.h"
namespace Utility{
	class IOMesh
	{
	public:
		static void Delete(std::string directoryPath, std::string fileName);
		static void Output(std::string directoryPath, std::string fileName, Mesh mesh);
		static void OutputDebug(std::string fileName, Mesh mesh, std::vector<int>uvIndex);
	private:
		IOMesh() = delete;
		~IOMesh() = delete;

		static constexpr std::string_view c_Delimiter = ".";

		/*!
			@brief	y(uuki)f(ormat)m(esh)
		*/
		static constexpr std::string_view c_Extension = "yfm";
	};
}
