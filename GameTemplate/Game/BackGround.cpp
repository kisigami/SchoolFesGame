#include "stdafx.h"
#include "BackGround.h"

bool BackGround::Start()
{
	m_modelRender.Init("Assets/modelData/background/background.tkm");

	return true;
}

void BackGround::Update()
{
	m_modelRender.Update();
}

void BackGround::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}