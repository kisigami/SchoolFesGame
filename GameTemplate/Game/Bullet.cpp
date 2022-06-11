#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "collision/CollisionObject.h"
#include "Enemy.h"

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
	DeleteGO(m_collisionObject);
}

bool Bullet::Start()
{
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	m_enemy = FindGO<Enemy>("enemy");

	m_moveSpeed = m_gameCamera->GetTargetPosition() - m_position;
	m_moveSpeed.Normalize();
	m_position += m_moveSpeed * 100.0f;
	m_moveSpeed *= 2000.0f;

	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, 10.0f * m_scale.z);
	m_collisionObject->SetName("bullet");
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void Bullet::Update()
{	
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();

	//タイマーを加算する。
	m_timer += g_gameTime->GetFrameDeltaTime();
	m_collisionObject->SetPosition(m_position);
	//タイマーが0.7f以上だったら。
	if (m_timer >= 3.0f)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}