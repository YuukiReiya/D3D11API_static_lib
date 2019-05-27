/*
	@file	IOMesh.h
	@date	2019/04/27
	@author	番場 宥輝
	@brief	メッシュデータのの入出力関係
	@detail	シングルトン
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
