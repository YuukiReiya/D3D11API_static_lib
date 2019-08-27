#include <iostream>
#include <FBXConverter.h>
#include <Mesh.h>
#include <IOMesh.h>
#include <WinConsoleExpansion.h>
using namespace Utility;
using namespace Converter;

int main()
{
	//	メモリリーク箇所特定
#if defined DEBUG||_DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // DEBUG||_DEBUG

	/////////////////////////////////////////////////
	//	ファイル読み込み
	/////////////////////////////////////////////////
	
	//	読み込む.fbxファイルのパスを指定します。
	std::string fbxPath = "";
	fbxPath = "Spider_3.fbx";

	//	書き出すファイルの名前を指定します。
	std::string yfmPath = "";
	yfmPath = "spider";

	/////////////////////////////////////////////////
	//	コンバート
	/////////////////////////////////////////////////

	//	初期化
	Converter::FBXConverter::Setup();

	//	実行
	FBXConverter::Execute(FBXConverter::OutputType::SKIN, fbxPath, yfmPath);

	//	破棄
	FBXConverter::Teardown();

	/////////////////////////////////////////////////
	//	完了時操作
	/////////////////////////////////////////////////
	system("pause");
	return 0;
}