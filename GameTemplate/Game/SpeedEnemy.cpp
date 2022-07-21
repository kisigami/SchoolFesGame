#include "stdafx.h"
#include "SpeedEnemy.h"

#include "Player.h"
#include "Bullet.h"
#include "SpawnEnemy.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float CHARACON_RADIUS = 22.0f;             //�L�����R���̔��a
	const float CHARACON_HEIGHT = 20.0f;             //�L�����R���̍���
	const int   REACT_DAMAGE_VALUE = 1;              //�󂯂�_���[�W�̒l
	const float MOVE_SPEED = 125.0f;                 //�ړ����x
	const int   HP_DOWN_VALUE = 0;                   //���S����HP�̒l
	const float MODEL_SCALE = 1.4f;                  //�G�l�~�[�̑傫��
	const float ATTACK_COLLISION_SIZE_X = 30.0f;     //�U���̓����蔻���X���傫��
	const float ATTACK_COLLISION_SIZE_Y = 30.0f;	 //�U���̓����蔻���Y���傫��
	const float ATTACK_COLLISION_SIZE_Z = 30.0f;	 //�U���̓����蔻���Z���傫��
	const float IDLE_ANIMATION_SPEED = 1.0f;         //�ҋ@�A�j���[�V�����̍Đ����x
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //�ҋ@�A�j���[�V�����̕⊮����
	const float RUN_ANIMATION_SPEED = 1.8f;          //����A�j���[�V�����̍Đ����x
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //����A�j���[�V�����̕⊮����
	const float DOWN_ANIMATION_SPEED = 1.5f;         //�_�E���A�j���[�V�����̍Đ�����
	const float DOWN_ANIMATION_INTERPOLATE = 0.1f;   //�_�E���A�j���[�V�����̕⊮����
	const float ATTACK_ANIMATION_SPEED = 1.0f;       //�U���A�j���[�V�����̍Đ�����
	const float ATTACK_ANIMATION_INTERPOLATE = 0.3f; //�U���A�j���[�V�����̕⊮����
	const float UNDERGROUND_POSITION = 1000.0f;      //�n���̍��W
	const int   PROBABILITY = 100;                   //�m���̐�
	const int   ATTACK_PROBABILITY = 50;             //�U���ł���m��
	const float ATTACK_DIFF = 48.0f;                 //�U���ł��鋗��
	const float COLLIDER_RADIUS = 3.0f;              //�X�t�B�A�R���C�_�[�̔��a
	const float COLLIDER_HEIGHT = 60.0f;             //�R���C�_�[�̔��ˎ��̍�
	const float LOOKME_ANGLE = 0.3f;                 //�v���C���[��������p�x
}

SpeedEnemy::SpeedEnemy()
{
}

SpeedEnemy::~SpeedEnemy()
{
	DeleteGO(m_walkse);
}

void SpeedEnemy::InitAnimation()
{
	//�A�j���[�V�����N���b�v�����[�h����
	m_animClips[enAnimClip_Idle].Load("Assets/animData/speedenemy/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/speedenemy/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
	m_animClips[enAnimClip_Attack].Load("Assets/animData/speedenemy/attack.tka");
	m_animClips[enAnimClip_Attack].SetLoopFlag(false);
	m_animClips[enAnimClip_Down].Load("Assets/animData/speedenemy/down.tka");
	m_animClips[enAnimClip_Down].SetLoopFlag(false);

}

bool SpeedEnemy::Start()
{
	g_soundEngine->ResistWaveFileBank(5, "Assets/sound/enemy/enemydownsound.wav");
	g_soundEngine->ResistWaveFileBank(11, "Assets/sound/enemy/walksound2.wav");
	g_soundEngine->ResistWaveFileBank(22, "Assets/sound/enemy/pumch.wav");
	//�G�t�F�N�g��ǂݍ���œo�^����
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/effect/efk/blood.efk");

	//�A�j���[�V�����̏�����
	InitAnimation();
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/speedenemy/speedenemy.tkm", m_animClips, enAnimClip_Num);
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
	//�X�t�B�A�R���C�_�[���������B
	m_sphereCollider.Create(4.0f);
	//�p���`�̃{�[����T��
	m_pumchBoneId = m_modelRender.FindBoneID(L"mixamorig1:LeftHand");
	//�L�����R����������
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
	//�傫����ݒ�
	m_modelRender.SetScale(Vector3::One * MODEL_SCALE);
	//���f���̍X�V
	m_modelRender.Update();
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");
	m_spawnEnemy = FindGO<SpawnEnemy>("spawnenemy");

	m_nvmMesh.Init("Assets/nvm/test.tkn");
	return true;
}

void SpeedEnemy::Update()
{
	if (m_isActive == true)
	{
		PathMove();
		//�v���C���[���猩��ꂢ�邩�H
		CantLookMe();
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
	}

	m_charaCon.SetPosition(m_position);
	m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

	//���W�̐ݒ�
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetScale(Vector3::One * MODEL_SCALE);
	m_modelRender.Update();
}

void SpeedEnemy::PathMove()
{
	//�p�X�ړ��X�e�[�g�ł͂Ȃ�������
	if (m_enemyState != enEnemyState_PathChase)
	{
		//�������Ȃ�
		return;
	}
	//�p�X�����^�C�}�[���J�E���g����
	m_pathTimer += g_gameTime->GetFrameDeltaTime();
	//�ړI�n�̍��W
	Vector3 nextPosition = Vector3::Zero;

	bool isEnd;
	//�p�X�����^�C�}�[���z������
	if (m_pathTimer >= 0.5f)
	{
		// �p�X����
		m_pathFiding.Execute(
			m_path,							// �\�z���ꂽ�p�X�̊i�[��
			m_nvmMesh,						// �i�r���b�V��
			m_position,						// �J�n���W
			m_player->GetPosition(),		// �ړ��ڕW���W
			PhysicsWorld::GetInstance(),	// �����G���W��	
			50.0f,							// AI�G�[�W�F���g�̔��a
			200.0f							// AI�G�[�W�F���g�̍����B
		);
		//�p�X�����^�C�}�[������������
		m_pathTimer = 0.0f;
	}

	//�p�X����ړ�����B
	nextPosition = m_path.Move(
		m_position,                   //���W
		5.0f,                         //�ړ����x
		isEnd                         //�t���O��false�ɂ���
	);

	//�ړI�n�֌������x�N�g�����v�Z����
	Vector3 vector = nextPosition - m_position;
	//���K������
	vector.Normalize();
	//�ړ����x���ړ��x�N�g���~���x
	m_moveSpeed = vector * 125.0f;
	//�L�����R���̍��W��ݒ肷��
	m_charaCon.SetPosition(m_position);
	//�L�����R�����g���č��W���ړ�������
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());

}

void SpeedEnemy::DeActive()
{
	//�A�N�e�B�u�ł͂Ȃ�������
	if (m_isActive == false)
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_enemyState = enEnemyState_Idle;
		//�ҋ@���W�Ɉړ�����
		m_position = m_spawnEnemy->GetStayPoint();
		//�L�����R�����ړ�������
		m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
		//HP�����ɖ߂�
		m_hp = 3;
		m_pathTimer = 1.0f;
	}
}

void SpeedEnemy::Rotation()
{
	//���ꂪ��]�p�x�ɂȂ�B
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	//�N�I�[�^�j�I����ݒ肷��
	m_rotation.SetRotationY(-angle);
	//�v���C���[�̑O�x�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

	//��]�̐ݒ�
	m_modelRender.SetRotation(m_rotation);
}

void SpeedEnemy::Chase()
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
}

void SpeedEnemy::Attack()
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

void SpeedEnemy::MakeAttackCollision()
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
	//�R���W�����ɖ��O
	collisionObject->SetName("enemy_attack");
}

void SpeedEnemy::Collision()
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
				//�v���C���[�𔭌�����
				m_mitukatta = true;
				//HP�����炷�B
				m_hp -= REACT_DAMAGE_VALUE;

				//�R���W�������A�N�e�B�u�ɂ���
				collision->Dead();
				collision->Deactivate();
				//HP��0�ɂȂ�����B
				if (m_hp <= HP_DOWN_VALUE)
				{
					//�_�E���X�e�[�g�Ɉڍs����
					m_enemyState = enEnemyState_Down;

					//�������쐬����
					m_se = NewGO<SoundSource>(0);
					//����������������
					m_se->Init(5);
					//�����̑傫����ݒ�
					m_se->SetVolume(0.3f);
					//�������Đ�����i���[�v�Ȃ��j
					m_se->Play(false);
					//�L�����R�����폜����
					m_charaCon.RemoveRigidBoby();
					//�J�E���g��i�߂�
					int counter = m_player->GetKillEnemyCount();
					counter += 1;
					m_player->SetKillEnemyCount(counter);
				}
				return;
			}
		}
	}
}

void SpeedEnemy::PlayAnimation()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle, IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//�v���C���[��������ǐՃX�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Chase:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//�v���C���[�������Ȃ��ǐՃX�e�[�g�̎�
	case SpeedEnemy::enEnemyState_PathChase:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//����X�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//�_�E���X�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Down:
		m_modelRender.PlayAnimation(enAnimClip_Down, DOWN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(DOWN_ANIMATION_SPEED);
		break;
		//�U���X�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Attack:
		m_modelRender.PlayAnimation(enAnimClip_Attack, ATTACK_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(ATTACK_ANIMATION_SPEED);
		break;
	}
}

void SpeedEnemy::ManageState()
{
	switch (m_enemyState)
	{
		//�ҋ@�X�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Idle:
		ProcessIdleStateTransition();
		break;
		//�v���C���[��������ǐՃX�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Chase:
		ProcessChaseStateTransition();
		break;
		//�v���C���[�������Ȃ��ǐՃX�e�[�g�̎�
	case SpeedEnemy::enEnemyState_PathChase:
		ProcessChaseStateTransition();
		break;
		//�_�E���X�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Down:
		ProcessDownStateTransition();
		break;
		//�U���X�e�[�g�̎�
	case SpeedEnemy::enEnemyState_Attack:
		ProcessAttackStateTransition();
		break;
	}
}

void SpeedEnemy::ProcessCommonStateTransition()
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
		m_enemyState = enEnemyState_PathChase;
	}
}

void SpeedEnemy::ProcessIdleStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ�����
	ProcessCommonStateTransition();
}

void SpeedEnemy::ProcessChaseStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ�����
	ProcessCommonStateTransition();
}

void SpeedEnemy::ProcessAttackStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ�����
		ProcessCommonStateTransition();
	}
}

void SpeedEnemy::ProcessDownStateTransition()
{
	//�A�j���[�V�����̍Đ����I�������
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//��A�N�e�B�u�ɂ���
		m_isActive = false;
		//�X�e�[�^�X������������
		DeActive();
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

void SpeedEnemy::CantLookMe()
{
	m_mitukatta = false;

	Vector3 playerPosition = m_player->GetPosition();
	Vector3 diff = playerPosition - m_position;

	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);

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

const bool SpeedEnemy::CanAttack() const
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

void SpeedEnemy::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�Ȃ�
	if (wcscmp(eventName, L"attack_start") == 0)
	{
		//�U�������Đ�����
		SoundSource* se;
		se = NewGO<SoundSource>(0);
		se->Init(22);
		se->SetVolume(0.4f);
		se->Play(false);
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

void SpeedEnemy::Render(RenderContext& rc)
{
	//�A�N�e�B�u��������
	if (m_isActive == true)
	{
		//���f����`�悷��
		m_modelRender.Draw(rc);
	}
}
