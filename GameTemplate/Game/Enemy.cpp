#include "stdafx.h"
#include "Enemy.h"

#include "Bullet.h"
#include "Player.h"
#include "collision/CollisionObject.h"

#include "SpawnEnemy.h"

namespace
{
	const float CHARACON_RADIUS = 12.0f;             //�L�����R���̔��a
	const float CHARACON_HEIGHT = 50.0f;             //�L�����R���̍���
	const int   REACT_DAMAGE = 1;                    //�󂯂�_���[�W��
	const float MOVE_SPEED = 70.0f;                  //�ړ����x
	const int   HP_ZERO = 0;                         //�O
	const float ENEMY_SCALE = 1.0f;                  //�G�l�~�[�̑傫��
	const float ATTACK_COLLISION_SIZE_X = 20.0f;     //�U���̓����蔻���X���傫��
	const float ATTACK_COLLISION_SIZE_Y = 20.0f;	 //�U���̓����蔻���Y���傫��
	const float ATTACK_COLLISION_SIZE_Z = 20.0f;	 //�U���̓����蔻���Z���傫��
	const float IDLE_ANIMATION_SPEED = 1.0f;         //�ҋ@�A�j���[�V�����̍Đ����x
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //�ҋ@�A�j���[�V�����̕⊮����
	const float RUN_ANIMATION_SPEED = 1.0f;          //����A�j���[�V�����̍Đ����x
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //����A�j���[�V�����̕⊮����
	const float DOWN_ANIMATION_SPEED = 1.0f;         //�_�E���A�j���[�V�����̍Đ�����
	const float DOWN_ANIMATION_INTERPOLATE = 0.1f;   //�_�E���A�j���[�V�����̕⊮����
	const float ATTACK_ANIMATION_SPEED = 1.0f;       //�U���A�j���[�V�����̍Đ�����
	const float ATTACK_ANIMATION_INTERPOLATE = 0.3f; //�U���A�j���[�V�����̕⊮����
	const float UNDERGROUND_POSITION = 1000.0f;      //�n���̍��W
	const int   PROBABILITY = 100;                   //�m���̐�
	const int   ATTACK_PROBABILITY = 50;             //�U���ł���m��
	const float ATTACK_DIFF = 50.0f;                 //�U���ł��鋗��
	const float COLLIDER_HEIGHT = 70.0f;             //�R���C�_�[�̍���
	const float LOOKME_ANGLE = 0.5f;                 //�v���C���[��������p�x
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
	//�A�j���[�V�����̏�����
	InitAnimation();
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/enemy/enemy.tkm", m_animClips, enAnimClip_Num);
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(1.0f);
	//�p���`�̃{�[����T��
	m_pumchBoneId = m_modelRender.FindBoneID(L"mixamorig:RightHand");
	//�L�����R����������
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT,m_position);
	//���f���̍X�V
	m_modelRender.Update();
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");
	m_spawnEnemy = FindGO<SpawnEnemy>("spawnenemy");

	return true;
}

void Enemy::Update()
{
	//�v���C���[���猩��ꂢ�邩�H
	//CantLookMe();
	//�U������
	Attack();
	//��]����
	Rotation();
	//�ǐՏ���
	Chase();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�����蔻�菈��
	Collision();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();

	//���W�̐ݒ�
	m_modelRender.SetPosition(m_position);
	//�傫���̐ݒ�
	m_modelRender.SetScale(Vector3::One*ENEMY_SCALE);
	//���f���̍X�V
	m_modelRender.Update();
}

void Enemy::Rotation()
{
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//atan���Ԃ��Ă���p�x�̓��W�A���P�ʂȂ̂�
	//SetRotationDeg�ł͂Ȃ�SetRotation���g�p����B
	m_rotation.SetRotationY(-angle);

	//��]��ݒ肷��B
	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	//��]�̐ݒ�
	m_modelRender.SetRotation(m_rotation);
}

void Enemy::Chase()
{
	//�ǐՃX�e�[�g�ł͂Ȃ�������
	if (m_enemyState != enEnemyState_Chase)
	{
		//�������Ȃ�
		return;
	}
	//�v���C���[�Ɍ������x�N�g��
	Vector3 vector = m_player->GetPosition() - m_position;
	//���K��
	vector.Normalize();
	//�ړ����x���v���C���[�Ɍ������x�N�g���~���x
	m_moveSpeed = vector * MOVE_SPEED;
	m_charaCon.SetPosition(m_position);
	//�L�����R�����g���č��W���ړ�������
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//���W�̐ݒ�
	m_modelRender.SetPosition(m_position);
}

void Enemy::Attack()
{
	//�U���X�e�[�g�ł͂Ȃ�������
	if (m_enemyState != enEnemyState_Attack)
	{
		//�������Ȃ�
		return;
	}
	//�U�����Ȃ�
	if (m_attacking == true)
	{
		//�U���̓����蔻����쐬����
		MakeAttackCollision();
	}
}

void Enemy::MakeAttackCollision()
{
	//�U�������蔻��p�̃R���W�����I�u�W�F�N�g���쐬����B
	auto collisionObject = NewGO<CollisionObject>(0);
	//���̃{�[���̃��[���h�s����擾����B
	Matrix matrix = m_modelRender.GetBone(m_pumchBoneId)->GetWorldMatrix();
	//�{�b�N�X��̃R���W�������쐬����B
	collisionObject->CreateBox(m_position, 
		Quaternion::Identity, 
		Vector3(ATTACK_COLLISION_SIZE_X,
			ATTACK_COLLISION_SIZE_Y, 
			ATTACK_COLLISION_SIZE_Z));
	collisionObject->SetWorldMatrix(matrix);
	collisionObject->SetName("enemy_attack");
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
				collision->Deactivate();
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
		//�ǐՃX�e�[�g�̎�
	case Enemy::enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
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
	case Enemy::enEnemyState_Attack:
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
	case Enemy::enEnemyState_Chase:
		ProcessChaseStateTransition();
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
	//�v���C���[���������Ȃ�
	if (m_mitukatta == true)
	{
		//�v���C���[���U���ł���Ȃ�
		if (CanAttack() == true)
		{
			//�m���ōU���X�e�[�g���ҋ@�X�e�[�g�Ɉڍs����
			int randm = rand() % PROBABILITY;
			if (randm >= ATTACK_PROBABILITY)
			{
				//�U���X�e�[�g�Ɉڍs����
				m_enemyState = enEnemyState_Attack;
			}
			else
			{
				//�ҋ@�X�e�[�g�Ɉڍs����
				m_enemyState = enEnemyState_Idle;
			}
			return;
		}
		//�U���ł��Ȃ��Ȃ�
		else
		{
			//�ǐՃX�e�[�g�ڍs����
			m_enemyState = enEnemyState_Chase;
			return;
		}
	}
	//�v���C���[�������ĂȂ�������
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_enemyState = enEnemyState_Idle;
	}
}

void Enemy::ProcessIdleStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ�����
	ProcessCommonStateTransition();
}

void Enemy::ProcessChaseStateTransition()
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
		m_isActive = false;
		m_hp = 6;
		m_position = m_spawnEnemy->GetStayPoint();
		m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
		m_enemyState = enEnemyState_Idle;
	}
}

//�Փ˂����Ƃ��ɌĂ΂��֐��I�u�W�F�N�g(�Ǘp)
struct SweepResultWall :public btCollisionWorld::ConvexResultCallback
{
	bool isHit = false;						//�Փ˃t���O�B

	virtual	btScalar	addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		//�ǂƂԂ����ĂȂ�������B
		if (convexResult.m_hitCollisionObject->getUserIndex() != enCollisionAttr_Wall) {
			//�Փ˂����͕̂ǂł͂Ȃ��B
			return 0.0f;
		}

		//�ǂƂԂ�������B
		//�t���O��true�ɁB
		isHit = true;
		return 0.0f;
	}
};

void Enemy::CantLookMe()
{
	m_mitukatta = false;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;

	if (diff.LengthSq() >= 500.0f * 500.0f)
	{
		return;
	}
	diff.Normalize();
	float angle = acosf(diff.Dot(m_forward));

	//�v���C���[�����E���ɋ��Ȃ�������B
	if (Math::PI * LOOKME_ANGLE <= fabsf(angle))
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return;
	}

	btTransform start, end;
	start.setIdentity();
	end.setIdentity();
	//�n�_�̓G�l�~�[�̍��W�B
	start.setOrigin(btVector3(m_position.x, m_position.y + COLLIDER_HEIGHT, m_position.z));
	//�I�_�̓v���C���[�̍��W�B
	end.setOrigin(btVector3(playerPosition.x, playerPosition.y + COLLIDER_HEIGHT, playerPosition.z));

	SweepResultWall callback;
	//�R���C�_�[���n�_����I�_�܂œ������āB
	//�Փ˂��邩�ǂ����𒲂ׂ�B
	PhysicsWorld::GetInstance()->ConvexSweepTest((const btConvexShape*)m_sphereCollider.GetBody(), start, end, callback);
	//�ǂƏՓ˂����I
	if (callback.isHit == true)
	{
		//�v���C���[�͌������Ă��Ȃ��B
		return;
	}

	//�ǂƏՓ˂��ĂȂ��I�I
	//�v���C���[�������t���O��true�ɁB
	m_mitukatta = true;
}

const bool Enemy::CanAttack() const
{
	//�v���C���[���������Ȃ�
	if (m_mitukatta == true)
	{
		//�v���C���[�Ƃ̋������v�Z����
		Vector3 diff = m_player->GetPosition() - m_position;
		//������x�߂�������
		if (diff.LengthSq() <= ATTACK_DIFF * ATTACK_DIFF)
		{
			//�U���ł���
			return true;
		}
	}
	//�U���ł��Ȃ�
	return false;
}

void Enemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�Ȃ�
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//�U����
		m_attacking = true;
	}
	//�L�[�̖��O���uattack_end�v�Ȃ�
	else if (wcscmp(eventName, L"attack_end") == 0) 
	{
		//�U��������Ȃ�
		m_attacking = false;
	}
}

void Enemy::Render(RenderContext& rc)
{
	if (m_isActive == true)
	{
		//���f���̍X�V
		m_modelRender.Draw(rc);
	}
}