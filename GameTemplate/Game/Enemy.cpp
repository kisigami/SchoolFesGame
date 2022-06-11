#include "stdafx.h"
#include "Enemy.h"

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
	//�A�j���[�V�����N���b�v�����[�h����
	m_animClips[enAnimClip_Idle].Load("Assets/animData/enemy/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/enemy/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
}


bool Enemy::Start()
{
	//InitAnimation();

	m_modelRender.Init("Assets/modelData/gun/shotgun2.tkm"/*, m_animClips, enAnimClip_Num*/);
	m_position = { 100.0f,0.0f,500.0f };
	m_charaCon.Init(12.0f, 45.0f, m_position);

	m_modelRender.Update();

	return true;
}

void Enemy::Update()
{
	PlayAnimation();
	Collision();

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
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("bullet");
	for (auto collision : collisions)
	{
		//�R���W�����ƃL�����R�����Փ˂�����B
		if (collision->IsHit(m_charaCon))
		{
			//HP�����炷�B
			m_hp -= 1;

			//HP��0�ɂȂ�����B
			if (m_hp <= 0)
			{
				DeleteGO(this);
			}
			return;
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

void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Enemy::enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, 0.3f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//����X�e�[�g�̎�
	case Enemy::enEnemyState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
	}
}

void Enemy::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Enemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//����X�e�[�g�̎�
	case Enemy::enEnemyState_Run:
		ProcessRunStateTransition();
		break;
	}
}

void Enemy::Render(RenderContext& rc)
{
	m_modelRender.Draw(rc);
}