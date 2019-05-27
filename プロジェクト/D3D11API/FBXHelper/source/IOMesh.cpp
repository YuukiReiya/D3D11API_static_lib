#include "Common.h"
#include "IOMesh.h"

using namespace std;


void Utility::IOMesh::Delete(std::string directoryPath, std::string fileName)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	ifstream ifs;

	//	�t�@�C�����J���Ă݂�
	ifs.open(path);

	//	�J���̂Ɏ��s
	if (ifs.fail()) {
		cout << "\""<<path<<"\" is open failed!" << endl;
		ifs.close();
		return; 
	}

	//	�t�@�C����Y�ꂸ�ɕ��Ă���
	ifs.close();

	//	�Y���t�@�C���폜
	remove(path.c_str());
	cout << "\"" << path << "\" is deleted!" << endl;
}
void Utility::IOMesh::Output(std::string directoryPath, std::string fileName, Utility::Mesh mesh)
{
	string path = directoryPath + fileName + c_Delimiter.data() + c_Extension.data();
	ofstream ofs;
	ofs.open(path, ios::out);

	//	��������

	//���_
	const string c_Space = " ";
	for (auto it : mesh.vertices)
	{
		ofs << "{" << it.x << c_Space << it.y << c_Space << it.z << "}";// << endl;
	}
	ofs << endl;

	//	���_�C���f�b�N�X
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
