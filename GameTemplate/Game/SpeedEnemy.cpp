#include "stdafx.h"
#include "SpeedEnemy.h"

SpeedEnemy::SpeedEnemy()
{
}

SpeedEnemy::~SpeedEnemy()
{
}

bool SpeedEnemy::Start()
{
	m_modelRender.Init("Assets/modelData/speedenemy/speedenemy.tkm");
	m_modelRender.SetScale(Vector3::One * 1.2);
	m_modelRender.Update();
	return true;
}

void SpeedEnemy::Update()
{
	m_modelRender.Update();
}

void SpeedEnemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}
