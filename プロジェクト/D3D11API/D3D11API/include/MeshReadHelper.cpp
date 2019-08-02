#include "stdafx.h"
#include "MeshReadHelper.h"
#include "MyGame.h"

/*!
	@brief	using�f�B���N�e�B�u
	@using	D3D11::Helper
*/
using namespace D3D11::Helper;

/*!
	@brief	using�f�B���N�e�B�u
	@using	std
*/
using namespace std;

MeshReadHelper::ReadBuffer D3D11::Helper::MeshReadHelper::Read(std::string path)
{
	ifstream ifs(path);
	if (ifs.fail()) {
		string error = "Failed to read \"" + path + "\" file.";
		ErrorLog(error);
		return ReadBuffer();
	}

	ReadBuffer ret;
	string buf;

	getline(ifs, buf);
	//�C���f�b�N�X
	while (true)
	{
		auto a = buf.find(" ");
		if (a == string::npos) { break; }
		auto b = buf.substr(0, a);

		ret.indices.push_back(stoi(b));

		buf = buf.substr(a + 1);
	}

	string t = "}";
	//uv
	getline(ifs, buf);
	while (true)
	{
		auto a = buf.find(t);
		if (a == string::npos) { break; }

		//	{1 1
		auto b = buf.substr(0, a);

		//	 1 1
		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}

		//	�ۑ��p�̈ꎞ�o�b�t�@
		Graphic::MeshVertex tmp;

		//	x
		auto d = b.find(" ");
		auto e = b.substr(0, d);

		b = b.substr(d + 1);

		//	y
		tmp.uv =
		{
			stof(e),
			stof(b)
		};

		ret.vertices.push_back(tmp);
		//it.uv =
		//{
		//	stof(e),
		//	stof(b)
		//};

		buf = buf.substr(a + 1);
	}

	//	�o�b�t�@�ɕ�����l��
	getline(ifs, buf);


	//	{1 1 1}{2 2 2}

	if (!ret.vertices.empty()) {

		//���_
		int index = 0;
		while (true)
		{
			auto a = buf.find(t);
			if (a == string::npos) { break; }

			//	{1 1 1
			auto b = buf.substr(0, a);

			auto c = b.find("{");
			if (c != string::npos) {
				b = b.substr(c + 1);
			}
			//	1 1 1
			{
				//Graphic::MeshVertex& tmp = ret.vertices[index++];
				//	x
				auto d = b.find(" ");
				auto e = b.substr(0, d);

				//tmp.position.x = stof(e);
				ret.vertices[index].position.x = stof(e);

				b = b.substr(d + 1);

				//	y
				auto f = b.find(" ");
				auto g = b.substr(0, f + 1);

				//tmp.position.y = stof(g);
				ret.vertices[index].position.y = stof(g);

				b = b.substr(f + 1);

				//	z
				//tmp.position.z = stof(b);
				ret.vertices[index].position.z = stof(b);
				//ret.vertices.push_back(tmp);
				index++;
			}

			buf = buf.substr(a + 1);
		}
	}
	else {
		//���_
		while (true)
		{
			auto a = buf.find(t);
			if (a == string::npos) { break; }

			Graphic::MeshVertex tmp;

			//	{1 1 1
			auto b = buf.substr(0, a);

			auto c = b.find("{");
			if (c != string::npos) {
				b = b.substr(c + 1);
			}
			//	1 1 1
			{
				//Graphic::MeshVertex& tmp = ret.vertices[index++];
				//	x
				auto d = b.find(" ");
				auto e = b.substr(0, d);

				tmp.position.x = stof(e);

				b = b.substr(d + 1);

				//	y
				auto f = b.find(" ");
				auto g = b.substr(0, f + 1);

				tmp.position.y = stof(g);
				//ret.vertices[index].position.x = stof(g);

				b = b.substr(f + 1);

				//	z
				tmp.position.z = stof(b);
				//ret.vertices[index].position.x = stof(b);
				ret.vertices.push_back(tmp);
			}

			buf = buf.substr(a + 1);
		}
	}

	return ret;
}

float GetMatrixElement(string& str) {
	auto offset = str.find(",");
	auto element = stof(str.substr(0, offset));
	str = str.substr(offset + 1);
	return element;
}

D3D11::Helper::MeshReadHelper::SkeltonMesh D3D11::Helper::MeshReadHelper::ReadAnim(std::string path)
{
	ifstream ifs(path);
	if (ifs.fail()) {
		string error = "Failed to read \"" + path + "\" file.";
		ErrorLog(error);
		return SkeltonMesh();
	}

	SkeltonMesh ret;
	string buf;
	getline(ifs, buf);

	//�C���f�b�N�X
	while (true)
	{
		auto a = buf.find(" ");
		if (a == string::npos) { break; }
		auto b = buf.substr(0, a);

		ret.indices.push_back(stoi(b));

		buf = buf.substr(a + 1);
	}

	//uv
	getline(ifs, buf);
	while (true)
	{
		string t = "}";
		auto a = buf.find(t);
		if (a == string::npos) { break; }

		//	{1 1
		auto b = buf.substr(0, a);

		//	 1 1
		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}

		//	�ۑ��p�̈ꎞ�o�b�t�@
		SkeltonMesh::Vertex tmp;

		//	x
		auto d = b.find(" ");
		auto e = b.substr(0, d);

		b = b.substr(d + 1);

		//	y
		tmp.uv =
		{
			stof(e),
			stof(b)
		};

		ret.vertices.push_back(tmp);
		//it.uv =
		//{
		//	stof(e),
		//	stof(b)
		//};

		buf = buf.substr(a + 1);
	}

	//	�o�b�t�@�ɕ�����l��
	getline(ifs, buf);

	//���_
	int index = 0;
	while (true)
	{
		auto a = buf.find("}");
		if (a == string::npos) { break; }

		//	{1 1 1
		auto b = buf.substr(0, a);

		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}
		//	1 1 1
		{
			//Graphic::MeshVertex& tmp = ret.vertices[index++];
			//	x
			auto d = b.find(" ");
			auto e = b.substr(0, d);

			//tmp.position.x = stof(e);
			ret.vertices[index].position.x = stof(e);

			b = b.substr(d + 1);

			//	y
			auto f = b.find(" ");
			auto g = b.substr(0, f + 1);

			//tmp.position.y = stof(g);
			ret.vertices[index].position.y = stof(g);

			b = b.substr(f + 1);

			//	z
			//tmp.position.z = stof(b);
			ret.vertices[index].position.z = stof(b);
			//ret.vertices.push_back(tmp);
			index++;
		}

		buf = buf.substr(a + 1);
	}

	//getline(ifs, buf);
	//ret.maxBonesElementsCount = stoi(buf);
	//getline(ifs, buf);

	//	�d��
	index = 0;
#if 0
	while (true)
	{
		auto a = buf.find("}");
		if (a == string::npos) { break; }

		//	{ 1:1 2:2... }{ ... }
		auto b = buf.substr(0, a);

		//	���_��
		while (true)
		{
			//	��
			auto c = b.find(" ");
			b = b.substr(c + 1);

			//	�f�[�^����
			if (b.empty()) { break; }

			//	��؂�
			auto d = b.find(":");

			//	�{�[���ԍ�
			auto e = b.substr(0, d);
			ret.vertices[index].indexOfBonesAffested.push_back(stoi(e));

			//	�d��
			b = b.substr(d + 1);
			auto g = b.find(" ");
			auto h = b.substr(0, g);
			ret.vertices[index].weight[stoi(e)] = stof(h);

			//	��
			b = b.substr(g);
		}
		index++;
		buf = buf.substr(a + 1);
	}
#endif // 0

	//	�o�b�t�@�ɕ�����l��
	//getline(ifs, buf);

#if 0
	//	�����p��
	while (true)
	{
		auto a = buf.find("}");
		if (a == string::npos) { break; }

		DirectX::XMFLOAT4X4 m;

		auto b = buf.substr(1, a);

		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}


		////	v[0]
		//m._11 = GetMatrixElement(b);
		//m._12 = GetMatrixElement(b);
		//m._13 = GetMatrixElement(b);
		//m._14 = GetMatrixElement(b);
		////	v[1]
		//m._21 = GetMatrixElement(b);
		//m._22 = GetMatrixElement(b);
		//m._23 = GetMatrixElement(b);
		//m._24 = GetMatrixElement(b);
		////	v[2]
		//m._31 = GetMatrixElement(b);
		//m._32 = GetMatrixElement(b);
		//m._33 = GetMatrixElement(b);
		//m._34 = GetMatrixElement(b);
		////	v[3]
		////m._41 = GetMatrixElement(b);
		////m._42 = GetMatrixElement(b);
		////m._43 = GetMatrixElement(b);
		////m._44 = GetMatrixElement(b);
		//m._41 = 0;
		//m._42 = 0;
		//m._43 = 0;
		//m._44 = 1;

		///////////////////////////////////
		//	v[0]
		m._11 = GetMatrixElement(b);
		m._12 = GetMatrixElement(b);
		m._13 = GetMatrixElement(b);
		m._14 = 0;
		//	v[1]
		m._21 = GetMatrixElement(b);
		m._22 = GetMatrixElement(b);
		m._23 = GetMatrixElement(b);
		m._24 = 0;
		//	v[2]
		m._31 = GetMatrixElement(b);
		m._32 = GetMatrixElement(b);
		m._33 = GetMatrixElement(b);
		m._34 = 0;
		//	v[3]
		m._41 = GetMatrixElement(b);
		m._42 = GetMatrixElement(b);
		m._43 = GetMatrixElement(b);
		m._44 = 1;

		auto tmp = DirectX::XMLoadFloat4x4(&m);
		tmp = DirectX::XMMatrixTranspose(tmp);
		DirectX::XMStoreFloat4x4(&m, tmp);
		//	�i�[
		ret.initialMatrix.push_back(m);

		//	������X�V
		buf = buf.substr(a + 1);
	}
#endif // 0

	//getline(ifs, buf);

	//	�A�j���[�V�����t���[��
	const unsigned int c_frame = stoi(buf);
	ret.frame = c_frame;

	getline(ifs, buf);

	const unsigned int c_boneCount = ret.initialMatrix.size();

	//	�t���[���p��
	for (size_t i = 0; i < c_frame; i++)
	{
		vector<DirectX::XMFLOAT4X4>tmp;
		for (size_t j = 0; j < c_boneCount; j++)
		{
			auto a = buf.find("}");
			if (a == string::npos) { break; }

			DirectX::XMFLOAT4X4 m;

			auto b = buf.substr(1, a);

			auto c = b.find("{");
			if (c != string::npos) {
				b = b.substr(c + 1);
			}
			


			//	v[0]
			m._11 = GetMatrixElement(b);
			m._12 = GetMatrixElement(b);
			m._13 = GetMatrixElement(b);
			m._14 = GetMatrixElement(b);
			//	v[1]
			m._21 = GetMatrixElement(b);
			m._22 = GetMatrixElement(b);
			m._23 = GetMatrixElement(b);
			m._24 = GetMatrixElement(b);
			//	v[2]
			m._31 = GetMatrixElement(b);
			m._32 = GetMatrixElement(b);
			m._33 = GetMatrixElement(b);
			m._34 = GetMatrixElement(b);
			//	v[3]
			m._41 = GetMatrixElement(b);
			m._42 = GetMatrixElement(b);
			m._43 = GetMatrixElement(b);
			m._44 = GetMatrixElement(b);

			//	�i�[
			auto xmmat = DirectX::XMLoadFloat4x4(&m);
			xmmat = DirectX::XMMatrixTranspose(xmmat);
			DirectX::XMStoreFloat4x4(&m, xmmat);

			tmp.push_back(m);

			//	������X�V
			buf = buf.substr(a + 1);
		}
		ret.frameMatrix.push_back(tmp);
	}

	return ret;
}

D3D11::Helper::MeshReadHelper::SkinMesh D3D11::Helper::MeshReadHelper::ReadSkin(std::string path)
{
	ifstream ifs(path);
	if (ifs.fail()) {
		string error = "Failed to read \"" + path + "\" file.";
		ErrorLog(error);
		return SkinMesh();
	}

	SkinMesh ret;
	string buf;
	getline(ifs, buf);

	//�C���f�b�N�X
	while (true)
	{
		auto a = buf.find(",");
		if (a == string::npos) { break; }
		auto b = buf.substr(0, a);

		ret.indices.push_back(stoi(b));

		buf = buf.substr(a + 1);
	}

	//uv
	getline(ifs, buf);
#pragma region UV
#if 0
	while (true)
	{
		string t = "}";
		auto a = buf.find(t);
		if (a == string::npos) { break; }

		//	{1 1
		auto b = buf.substr(0, a);

		//	 1 1
		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}

		//	�ۑ��p�̈ꎞ�o�b�t�@
		SkeltonMesh::Vertex tmp;

		//	x
		auto d = b.find(" ");
		auto e = b.substr(0, d);

		b = b.substr(d + 1);

		//	y
		tmp.uv =
		{
			stof(e),
			stof(b)
		};

		buf = buf.substr(a + 1);
	}
#endif // 0
#pragma endregion

	//	�o�b�t�@�ɕ�����l��
	getline(ifs, buf);
	
	//���_
	int index = 0;
	ret.vertices.resize(1605);
	string c_Delimiter = ",";
	while (true)
	{
		auto a = buf.find("}");
		if (a == string::npos) { break; }

		//	{1 1 1
		auto b = buf.substr(0, a);

		auto c = b.find("{");
		if (c != string::npos) {
			b = b.substr(c + 1);
		}
		//	1 1 1
		{
			//Graphic::MeshVertex& tmp = ret.vertices[index++];
			//	x
			auto d = b.find(c_Delimiter);
			auto e = b.substr(0, d);

			//tmp.position.x = stof(e);
			ret.vertices[index].position.x = stof(e);

			b = b.substr(d + 1);

			//	y
			auto f = b.find(c_Delimiter);
			auto g = b.substr(0, f + 1);

			//tmp.position.y = stof(g);
			ret.vertices[index].position.y = stof(g);

			b = b.substr(f + 1);

			//	z
			//tmp.position.z = stof(b);
			ret.vertices[index].position.z = stof(b);
			//ret.vertices.push_back(tmp);
			index++;
		}

		buf = buf.substr(a + 1);
	}

	//	�A�j���[�V�����t���[��
	getline(ifs, buf);
	const unsigned int c_frame = stoi(buf);

	const int c_vertexCount = ret.vertices.size();
	c_Delimiter = " ";
	//	�����s��
	for (size_t i = 0; i < c_frame; i++)
	{
		if (buf.empty()) { break; }
		for (size_t j = 0; j < c_vertexCount; j++)
		{
			auto a = buf.find("}");
			if (a == string::npos) { break; }

			DirectX::XMFLOAT4X4 m;

			auto b = buf.substr(1, a);

			auto c = b.find("{");
			if (c != string::npos) {
				b = b.substr(c + 1);
			}

			//	v[0]
			auto offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			auto str = b.substr(0, offset);
			m._11 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._12 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._13 = stof(str);
			b = b.substr(offset + 1);

			m._14 = 0;

			//	v[1]
			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._21 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._22 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._23 = stof(str);
			b = b.substr(offset + 1);

			m._24 = 0;

			//	v[2]
			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._31 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._32 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._33 = stof(str);
			b = b.substr(offset + 1);

			m._34 = 0;

			//	v[3]
			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._41 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._42 = stof(str);
			b = b.substr(offset + 1);

			offset = b.find(c_Delimiter);//�G���[�������Ȃ�
			str = b.substr(0, offset);
			m._43 = stof(str);
			b = b.substr(offset + 1);

			m._44 = 1;

			ret.vertices[j].compMat.push_back(m);
			buf = buf.substr(a + 1);
		}
		getline(ifs, buf);
	}

	return ret;
}
