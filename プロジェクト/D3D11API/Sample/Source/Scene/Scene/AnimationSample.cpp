#include "AnimationSample.h"
#include "../../ResorceLoader.h"
AnimationSample::AnimationSample()
{
}

AnimationSample::~AnimationSample()
{
}

void AnimationSample::Initialize()
{
	m_pMesh = ResourceLoader::GetInstance().GetMesh();
	m_pMesh->SetupMaterial(m_pMaterial.get());
	m_pMesh->SetupShader(m_pShader.get());
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