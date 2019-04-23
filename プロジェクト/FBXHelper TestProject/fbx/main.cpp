#include <iostream>
#include <Common.h>
#include <FBXUtility.h>

using namespace FBX;

int main()
{
	FBXUtility::Setup();
	auto ret = FBXUtility::Load("ao_twinte_chan.fbx");

	if (ret) {
		printf("¬Œ÷");
	}
	else {
		printf("¸”s");
	}

	system("pause");
	return 0;
}