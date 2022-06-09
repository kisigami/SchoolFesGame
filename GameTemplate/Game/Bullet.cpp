#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "Player.h"
#include "collision/CollisionObject.h"

bool Bullet::Start()
{
	m_modelRender.Init("Assets/modelData/bullet/bullet.tkm");
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	m_player = FindGO<Player>("player");

	m_position = m_player->GetPosition();
	m_position.y += 50.0f;
	m_position += g_camera3D->GetRight() * 30.0f;

	m_modelRender.SetPosition(m_position);

	m_moveSpeed = m_gameCamera->GetTargetPosition() - m_position;
	m_moveSpeed.Normalize();
	m_position += m_moveSpeed * 50.0f;
	m_moveSpeed *= 1200.0f;

	return true;
}

void Bullet::Update()
{	
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}


void Bullet::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}