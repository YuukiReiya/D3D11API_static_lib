#include <iostream>
#include <FBXConverter.h>
#include <Mesh.h>
#include <IOMesh.h>
#include <WinConsoleExpansion.h>
#include <string>
using namespace Utility;
using namespace Converter;
using namespace std;
using namespace ConsoleExpansion;

using wic = ConsoleExpansion::WinConsoleExpansion;

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
	string fbxPath = "";
	do
	{
		wic::SetColor(Cyan);
		cout << "FBXファイルのパスを指定してください。" << endl;
		wic::SetColor(White);
		getline(cin, fbxPath);
		wic::SetColor(Cyan);

		//	FBXファイルが指定されているか確認
		try
		{
			auto delimiterOffset = fbxPath.rfind(".");
			if (delimiterOffset == string::npos) { throw -1; }
			auto extension = fbxPath.substr(delimiterOffset);
			if (extension != ".fbx") { throw -1; }
		}
		//	FBXファイルが指定されていないので、再入力を促す。
		catch (...)
		{
			wic::SetColor(Red);
			cout << ".fbxファイルが指定されていません!" << endl
				<< "再入力してください。" << endl;
			continue;
		}

		//	入力の確定確認
		string ret;
		cout << "\"" << fbxPath << "\"" << "を読み込みます。" << endl
			<< "よろしいですか?<y/n>" << endl;
		getline(cin, ret);

		//	書き出しファイルの指定に移る
		if (ret == "y") 
		{ 
			break;
		}
		//	アプリケーション終了
		else if(ret == "n")
		{
			wic::SetColor(Purple);
			cout << "コンバーターを終了します。" << endl;
			wic::SetColor(White);
			system("pause");
			return -1;
		}

		//	コンソールを綺麗にする
		system("cls");
	} while (true);

	//	書き出すファイルの名前を指定します。
	std::string yfmPath = "";
	do
	{
		wic::SetColor(Cyan);
		cout << "書き出し先のパス及びフォルダ名を指定してください。" << endl;
		wic::SetColor(White);
		getline(cin, yfmPath);
		wic::SetColor(Cyan);

		string ret;
		cout << "\"" << yfmPath << "\"" << "に書き出します。" << endl
			<< "よろしいですか?<y/n>" << endl;
		getline(cin, ret);

		//	入力の確定確認
		if (ret == "y")
		{
			break;
		}
		//	アプリケーションの終了
		else if (ret == "n")
		{
			wic::SetColor(Purple);
			cout << "コンバーターを終了します。" << endl;
			wic::SetColor(White);
			system("pause");
			return -1;
		}

		//	コンソールをクリア
		system("cls");
	} while (true);


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