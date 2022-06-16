#include "stdafx.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Player.h"

namespace
{
	const float CHARACON_RADIUS = 12.0f;             //�L�����R���̔��a
	const float CHARACON_HEIGHT = 50.0f;             //�L�����R���̍���
	const int   REACT_DAMAGE = 1;                    //�󂯂�_���[�W��
	const int   HP_ZERO = 0;                         //�O
	const float ENEMY_SCALE = 1.0f;
	const float IDLE_ANIMATION_SPEED = 1.0f;         //�ҋ@�A�j���[�V�����̍Đ����x
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //�ҋ@�A�j���[�V�����̕⊮����
	const float RUN_ANIMATION_SPEED = 1.0f;          //����A�j���[�V�����̍Đ����x
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //����A�j���[�V�����̕⊮����
	const float DOWN_ANIMATION_SPEED = 1.0f;         //�_�E���A�j���[�V�����̍Đ�����
	const float DOWN_ANIMATION_INTERPOLATE = 0.1f;   //�_�E���A�j���[�V�����̕⊮����
	const float ATTACK_ANIMATION_SPEED = 1.0f;       //�U���A�j���[�V�����̍Đ�����
	const float ATTACK_ANIMATION_INTERPOLATE = 0.3f; //�U���A�j���[�V�����̕⊮����
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
	m_animClips[enAnimClip_Down].Load("Assets/animData/enemy/down.tka");
	m_animClips[enAnimClip_Down].SetLoopFlag(false);
	m_animClips[enAnimClip_Attack].Load("Assets/animData/enemy/attack.tka");
	m_animClips[enAnimClip_Attack].SetLoopFlag(false);
}

bool Enemy::Start()
{
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");
	//�A�j���[�V�����̏�����
	InitAnimation();
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm",m_animClips,enAnimClip_Num);
	//�L�����R����������
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT,m_position);
	//���f���̍X�V
	m_modelRender.Update();

	return true;
}

void Enemy::Update()
{
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�����蔻�菈��
	Collision();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();

	//���W�̐ݒ�
	m_modelRender.SetPosition(m_position);
	//��]�̐ݒ�
	m_modelRender.SetRotation(m_rotation);
	//�傫���̐ݒ�
	m_modelRender.SetScale(Vector3::One*ENEMY_SCALE);
	//���f���̍X�V
	m_modelRender.Update();
}

void Enemy::Rotation()
{
}

void Enemy::Move()
{
}

void Enemy:: Chase()
{
}

void Enemy::Attack()
{

}

void Enemy::Collision()
{
	//�_�E���X�e�[�g��������
	if (m_enemyState == enEnemyState_Down)
	{
		//�������Ȃ�
		return;
	}

	{
		//�G�̍U���p�̃R���W�����̔z����擾����B
		const auto& collisions = g_collisionObjectManager->FindCollisionObjects("bullet");
		//�z���for���ŉ񂷁B
		for (auto collision : collisions)
		{
			//�R���W�����ƃL�����R�����Փ˂�����B
			if (collision->IsHit(m_charaCon))
			{
				//HP�����炷�B
				m_hp -= REACT_DAMAGE;
				//�R���W�������A�N�e�B�u�ɂ���
				collision->Dead();

				//HP��0�ɂȂ�����B
				if (m_hp <= HP_ZERO)
				{
					//�L�����R�����G���W������폜
					m_charaCon.RemoveRigidBoby();
					//�_�E���X�e�[�g�Ɉڍs����
					m_enemyState = enEnemyState_Down;
				}
				return;
			}
		}
	}
}

void Enemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Enemy::enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//����X�e�[�g�̎�
	case Enemy::enEnemyState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//�_�E���X�e�[�g�̎�
	case Enemy::enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, DOWN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(DOWN_ANIMATION_SPEED);
		break;
		//�U���X�e�[�g�̎�
	case Enemy::enAnimClip_Attack:
		m_modelRender.PlayAnimation(enAnimClip_Attack, ATTACK_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(ATTACK_ANIMATION_SPEED);
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
		//�_�E���X�e�[�g�̎�
	case Enemy::enEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//�U���X�e�[�g�̎�
	case Enemy::enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void Enemy::ProcessCommonStateTransition()
{

}

void Enemy::ProcessIdleStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ�����
	ProcessCommonStateTransition();
}

void Enemy::ProcessRunStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ�����
	ProcessCommonStateTransition();
}

void Enemy::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ�����
		ProcessCommonStateTransition();
	}
}

void Enemy::ProcessDownStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���g���폜����
		DeleteGO(this);
	}
}

void Enemy::Render(RenderContext& rc)
{
	//���f���̍X�V
	m_modelRender.Draw(rc);
}