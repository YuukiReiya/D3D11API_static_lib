#include "Common.h"
#include "IOMesh.h"

using namespace std;

void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, Abstract::AbstractMesh * mesh)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	ofstream ofs;
	ofs.open(path, ios::out);

	//	��������

	//���_
	const string c_Space = " ";
	for (auto it : mesh->vertex) 
	{
		ofs << "{" << it.x << c_Space << it.y << c_Space << it.z << "}";// << endl;
	}
	ofs << endl;

	//	���_�C���f�b�N�X
	ofs << "//i" << endl;
	for (auto it : mesh->indexBuffer)
	{
		ofs << it << c_Space;
	}
}
