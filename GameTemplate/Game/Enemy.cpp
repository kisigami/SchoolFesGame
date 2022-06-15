#include "stdafx.h"
#include "Enemy.h"
#include "Bullet.h"

namespace
{

}

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::InitAnimation()
{
	//アニメーションクリップをロードする
	m_animClips[enAnimClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
	m_animClips[enAnimClip_Down].Load("Assets/animData/enemy/down.tka");
	m_animClips[enAnimClip_Down].SetLoopFlag(false);
}

bool Enemy::Start()
{
	InitAnimation();

	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm",m_animClips,enAnimClip_Num);

	m_modelRender.Update();


	return true;
}

void Enemy::Update()
{
	PlayAnimation();
	Collision();
	ManageState();

	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();
}

void Enemy::Rotation()
{

}

void Enemy::Move()
{

}

void Enemy::Collision()
{
	if (m_enemyState == enEnemyState_Down)
	{
		return;
	}
	{
		//敵の攻撃用のコリジョンの配列を取得する。
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("bullet1");
		//配列をfor文で回す。
		for (auto collision : collisions)
		{
			//コリジョンとキャラコンが衝突したら。
			if (collision->IsHit(m_charaCon))
			{
				//HPを減らす。
				m_hp -= 1;
				//コリジョンを非アクティブにする
				collision->Dead();

				//HPが0になったら。
				if (m_hp <= 0)
				{
					m_charaCon.RemoveRigidBoby();
					m_enemyState = enEnemyState_Down;
				}
				return;
			}
		}
	}
}

void Enemy::ProcessCommonStateTransition()
{

}

void Enemy::ProcessIdleStateTransition()
{
	ProcessCommonStateTransition();
}

void Enemy::ProcessRunStateTransition()
{
	ProcessCommonStateTransition();
}

void Enemy::ProcessDownStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
	}
}

void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case Enemy::enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//走りステートの時
	case Enemy::enEnemyState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
	case Enemy::enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//待機ステートの時
	case Enemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//走りステートの時
	case Enemy::enEnemyState_Run:
		ProcessRunStateTransition();
		break;
	case Enemy::enEnemyState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}