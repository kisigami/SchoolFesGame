#include "stdafx.h"
#include "Player.h"

#include "GameCamera.h"
#include "Bullet.h"
#include "Enemy.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float MODEL_POSITION_Y = 47.0f;            //���f���̍��W
	const float ZERO = 0.0f;                         //0.0f
	const float CHARACON_RADIUS = 20.0f;             //�L�����R���̔��a
	const float CHARACON_HEIGHT = 50.0f;             //�L�����R���̍���
	const float MOVE_SPEED = 300.0f;                 //�ړ����x
	const float SHOT_MOVE_SPEED = 65.0f;             //�ˌ��X�e�[�g�̈ړ����x
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;    //�ړ����x�̍Œ�l
	const float JUMP_POWER = 200.0f;                 //�W�����v��
	const float GRAVITY = 500.0f;                    //�d��
	const float IDLE_ANIMATION_SPEED = 1.0f;         //�ҋ@�A�j���[�V�����̍Đ����x
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //�ҋ@�A�j���[�V�����̕⊮����
	const float RUN_ANIMATION_SPEED = 0.8f;          //����A�j���[�V�����̍Đ�����
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //����A�j���[�V�����̕⊮����
	const float SHOT_ANIMATION_SPEED = 2.0f;         //�ˌ��A�j���[�V�����̍Đ�����
	const float SHOT_ANIMATION_INTERPOLATE = 0.1f;   //�ˌ��A�j���[�V�����̕⊮����
}

Player::Player()
{

}

Player::~Player()
{

}

void Player::InitAnimation()
{
	//�A�j���[�V�����N���b�v�����[�h����
	m_animClips[enAnimClip_RifleIdle].Load("Assets/animData/ar/idle.tka");
	m_animClips[enAnimClip_RifleIdle].SetLoopFlag(true);
	m_animClips[enAnimClip_RifleRun].Load("Assets/animData/ar/walk.tka");
	m_animClips[enAnimClip_RifleRun].SetLoopFlag(true);
}

bool Player::Start()
{
	//�A�j���[�V�����̏�����
	InitAnimation();
	//�J�����̃C���X�^���X���擾
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	m_enemy = FindGO<Enemy>("enemy");
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/gun/ar/ar3.tkm",m_animClips,enAnimClip_Num);
	//���W�Ɍ��_��������
	m_position = Vector3::Zero;
	//�G�t�F�N�g��ǂݍ���
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/smoke.efk");
	//���W�̐ݒ�
	m_modelRender.SetPosition(m_position.x, m_position.y, m_position.z);
	//�L�����R����������
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
	//���f���̍X�V
	m_modelRender.Update();

	return true;
}

void Player::Update()
{
	//HP��\��
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"HP=%d", int(m_hp));
	m_fontRender.SetText(wcsbuf);
	m_fontRender.SetPosition(-300.0f,0.0f,0.0f);
	m_fontRender.SetScale(2.3f);
	m_fontRender.SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));

	Shot();
	//��]����
	Rotation();
	//�ړ�����
	Move();
	Collision();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();

	//���f���̍X�V
	m_modelRender.Update();
}

void Player::Shot()
{
	if (m_playerState != enPlayerState_Shot)
	{
		return;
	}
	MakeBullet();
	EffectPlay();
}

void Player::EffectPlay()
{
	//�U���G�t�F�N�g�𔭐�������B
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * 1.5f);
	Vector3 effectPosition = m_position;
	//���W��������ɂ���B
	effectPosition = m_forward * 20.0f;
	effectPosition.y += 40.0f;
	effectEmitter->SetPosition(effectPosition);
	//�G�t�F�N�g���Đ�����B
	effectEmitter->Play();
}

void Player::Rotation()
{
	//�v���C���[���璍���_�ւ̃x�N�g��
	Vector3 targetVector = m_gameCamera->GetTargetPosition() - m_position;
	//���K��
	targetVector.Normalize();
	//�����_�ւ̃x�N�g������Y���̉�]�N�I�[�^�j�I�����쐬
	m_rotation.SetRotationYFromDirectionXZ(targetVector);
	//���_���璍���_�ւ̃x�N�g��
	Vector3 gunTarget = m_gameCamera->GetTargetPosition();
	Vector3 gunCamera = m_gameCamera->GetCameraPosition();
	Vector3 vector = gunTarget - gunCamera;
	//�����_��������]�p�x���擾
	float angle = atan2f(vector.y, vector.Length());
	////X���ɉ�]�p�x�����Z
	m_rotation.AddRotationX(-angle);
	//��]�̐ݒ�
	m_modelRender.SetRotation(m_rotation);
}

void Player::Move()
{
	//�ړ����x
	m_moveSpeed.x = ZERO;
	m_moveSpeed.z = ZERO;
	//�X�e�b�N�̓��͗ʂ��擾
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//camera�̑O�����ƉE�������擾
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ���ʂł̑O�������A�E�����ɕϊ�����
	cameraForward.y = ZERO;
	cameraForward.Normalize();
	cameraRight.y = ZERO;
	cameraRight.Normalize();
	//�ˌ��X�e�[�g��������
	if (m_playerState == enPlayerState_Shot)
	{
		//�ˌ��X�e�[�g�̈ړ����x���J�����̃x�N�g���~�R���g���[���[�̓��͒l�~���x
		m_moveSpeed += cameraForward * lStick_y * SHOT_MOVE_SPEED;
		m_moveSpeed += cameraRight * lStick_x * SHOT_MOVE_SPEED;
	}
	//����ȊO�̃X�e�[�g��������
	else
	{
		//�ړ����x���J�����̃x�N�g���~�R���g���[���[�̓��͒l�~���x
		m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
		m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	}
	//�d��
	m_moveSpeed.y -= 500.0f * g_gameTime->GetFrameDeltaTime();
	//�L�����R���̍��W���v���C���[�̍��W�ɑ��
	m_position = m_charaCon.Execute( m_moveSpeed,g_gameTime->GetFrameDeltaTime());
	//���f�����J�����ɍ��킹�ē�����
	m_modelRender.SetPosition(
		m_gameCamera->GetCameraPosition().x,
		m_gameCamera->GetCameraPosition().y - 5.0f,
		m_gameCamera->GetCameraPosition().z
	);
}

void Player::Collision()
{
	//��_���[�W�X�e�[�g���_�E���X�e�[�g��������
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_Down)
	{
		//�������Ȃ�
		return;
	}
	//�G�̍U���̓����蔻��
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		//�R���W�������L�����R���ƏՓ˂�����
		if (collision->IsHit(m_charaCon))
		{
			//�_���[�W���󂯂�
			m_hp -= 20;
			//�R���W�������E��
			collision->Dead();
			//�R���W�������A�N�e�B�u�ɂ���
			collision->Deactivate();
			//�G�l�~�[���U�����ł͂Ȃ�����
			m_enemy->m_attacking = false;
			//HP��0��菬����������
			if (m_hp <= 0)
			{
				//�_�E���X�e�[�g�Ɉڍs����
				m_playerState = enPlayerState_Down;
				return;
			}
			//HP��0���傫��������
			else
			{
				//��_���[�W�X�e�[�g�Ɉڍs����
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}
}

void Player::MakeBullet()
{
	//�e�I�u�W�F�N�g���쐬����
	Bullet* bullet = NewGO<Bullet>(0);
	Vector3 position = m_gameCamera->GetCameraPosition();
	//���W�����_�̉��ɂ���
	position.y -= 5.0f;
	//���W��ݒ肷��
	bullet->SetPosition(position);
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_RifleIdle,IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_RifleRun, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//�ˌ��X�e�[�g�̎�
	case Player::enPlayerState_Shot:
		break;
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		ProcessRunStateTransition();
		break;
		//�ˌ��X�e�[�g�̎�
	case Player::enPlayerState_Shot:
		ProcessShotStateTransition();
		break;
		//��_���[�W�X�e�[�g�̎�
	case Player::enPlayerState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//�_�E���X�e�[�g�̎�
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_playerState = enPlayerState_Shot;
		return;
	}

	//�ړ����x����������
	if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE ||
		fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{
		//����X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_Run;
		return;
	}

	//�ړ����x���Ȃ�������
	else
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_Idle;
	}
}

void Player::ProcessIdleStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}

void Player::ProcessShotStateTransition()
{
	//�ҋ@�X�e�[�g�Ɉڍs����
	m_playerState = enPlayerState_Idle;
}

void Player::ProcessReceiveDamageStateTransition()
{
	//���G�^�C�}�[������������
	m_receiveDamageTimer -= g_gameTime->GetFrameDeltaTime();
	//���G�^�C�}�[��0.0f�ȉ���������
	if (m_receiveDamageTimer <= 0.0f)
	{
		//���ʂ̃X�e�[�g�̑J�ڏ���
		ProcessCommonStateTransition();
		//���G�^�C�}�[�����Z�b�g����
		m_receiveDamageTimer = 1.0f;
	}
}

void Player::ProcessDownStateTransition()
{
	//���ʂ̃X�e�[�g�̑J�ڏ���
	ProcessCommonStateTransition();
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

void Player::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
	//�t�H���g�̕`��
	m_fontRender.Draw(rc);
}