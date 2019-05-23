#include "Common.h"
#include "IOMesh.h"

using namespace std;

void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, Abstract::AbstractMesh * mesh)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	ofstream ofs;
	ofs.open(path, ios::out);

	//	書き込み

	//頂点
	const string c_Space = " ";
	for (auto it : mesh->vertex) 
	{
		ofs << "{" << it.x << c_Space << it.y << c_Space << it.z << "}";// << endl;
	}
	ofs << endl;

	//	頂点インデックス
	//ofs << "//i" << endl;
	for (auto it : mesh->indexBuffer)
	{
		ofs << it << c_Space;
	}
	ofs << endl;

	//	uv
	ofs << "//uv" << endl;
	for (auto it : mesh->uv)
	{
		ofs << "{" << it.u << c_Space << it.v << "}";// << endl;
	}
}

void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, Utility::OutputMesh mesh)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	ofstream ofs;
	ofs.open(path, ios::out);

	//	書き込み

	//頂点
	const string c_Space = " ";
	for (auto it : mesh.vertexPos)
	{
		ofs << "{" << it.x << c_Space << it.y << c_Space << it.z << "}";// << endl;
	}
	ofs << endl;

	//	頂点インデックス
	//ofs << "//i" << endl;
	for (auto it : mesh.index)
	{
		ofs << it << c_Space;
	}
	ofs << endl;

	//	uv
	//ofs << "//uv" << endl;
	for (auto it : mesh.uv)
	{
		ofs << "{" << it.x << c_Space << it.y << "}";// << endl;
	}

}
