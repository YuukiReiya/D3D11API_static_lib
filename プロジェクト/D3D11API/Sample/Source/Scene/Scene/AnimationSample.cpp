#include "AnimationSample.h"
#include "../../ResorceLoader.h"
#include <Camera.h>

AnimationSample::AnimationSample()
{
}

AnimationSample::~AnimationSample()
{
}

void AnimationSample::Initialize()
{
	API::Camera::GetInstance().SetViewMatrix({ 0,0,-10 }, { 0,0,0 });
	m_pMesh = ResourceLoader::GetInstance().GetMesh();
}

void AnimationSample::Finalize()
{

}

void AnimationSample::Update()
{

}

void AnimationSample::Render()
{
	m_pMesh->Render();
}