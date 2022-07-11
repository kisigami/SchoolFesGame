#include "stdafx.h"
#include "Player.h"

#include "GameCamera.h"
#include "Bullet.h"
#include "Enemy.h"
#include "graphics/effect/EffectEmitter.h"
#include <math.h>

namespace
{
	const float MODEL_POSITION_Y = 47.0f;                       //���f���̍��W
	const float ZERO = 0.0f;								    //0.0f
	const float CHARACON_RADIUS = 20.0f;                        //�L�����R���̔��a
	const float CHARACON_HEIGHT = 45.0f;                        //�L�����R���̍���
	const float MOVE_SPEED = 170.0f;                            //�ړ����x
	const float SHOT_MOVE_SPEED = 65.0f;                        //�ˌ��X�e�[�g�̈ړ����x
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;               //�ړ����x�̍Œ�l
	const float JUMP_POWER = 200.0f;                            //�W�����v��
	const float GRAVITY = 500.0f;                               //�d��
	const float IDLE_ANIMATION_SPEED = 1.0f;                    //�ҋ@�A�j���[�V�����̍Đ����x
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;              //�ҋ@�A�j���[�V�����̕⊮����
	const float RUN_ANIMATION_SPEED = 1.0f;                     //����A�j���[�V�����̍Đ�����
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;               //����A�j���[�V�����̕⊮����
	const float SHOT_ANIMATION_SPEED = 2.0f;                    //�ˌ��A�j���[�V�����̍Đ�����
	const float SHOT_ANIMATION_INTERPOLATE = 0.1f;              //�ˌ��A�j���[�V�����̕⊮����
	const Vector3 tocamerapos = { 0.0f,0.0f,-1000.0f };
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
	m_animClips[enAnimClip_Idle].Load("Assets/animData/ar/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/ar/walk.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
	m_animClips[enAnimClip_Shot].Load("Assets/animData/ar/shot.tka");
	m_animClips[enAnimClip_Shot].SetLoopFlag(false);
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
	//�A�j���[�V�����C�x���g�p�̊֐���ݒ肷��B
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
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
	Vector3 m_target = m_gameCamera->GetTargetPosition();
	Vector3 toTarget = m_target - m_position;
	float angle = atan2f(toTarget.x, toTarget.z);
	m_rotation.SetRotationY(angle);

	//���_���璍���_�ւ̃x�N�g��
	Vector3 gunTarget = m_gameCamera->GetTargetPosition();
	Vector3 Pos = m_gameCamera->GetCameraPosition();
	Vector3 diff = gunTarget - Pos;
	//�����_��������]�p�x���擾
	float angle2 = atan2f(-diff.y, g_camera3D->GetTargetToPositionLength());
	//X���ɉ�]�p�x�����Z
	m_rotation.AddRotationX(angle2);

	m_modelRender.SetRotation(m_rotation);

	//�v���C���[�̐��ʃx�N�g�����v�Z����B
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
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
	m_position.y = 0.0f;

	//���f�����J�����ɍ��킹�ē�����
	m_modelRender.SetPosition(
		m_gameCamera->GetCameraPosition().x,
		48.0f,
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
	position.y -= 3.0f;
	position += m_forward * 30.0f;
	//���W��ݒ肷��
	bullet->SetPosition(position);
	bullet->SetRotation(m_rotation);
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle,IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//�ˌ��X�e�[�g�̎�
	case Player::enPlayerState_Shot:
		m_modelRender.PlayAnimation(enAnimClip_Shot, SHOT_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(SHOT_ANIMATION_SPEED);
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
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsPress(enButtonA))
	{
		//�ˌ��X�e�[�g�Ɉڍs����
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
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//�ҋ@�X�e�[�g�Ɉڍs����
		m_playerState = enPlayerState_Idle;
	}
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
	(void)clipName;
	//�L�[�̖��O���uattack_start�v�Ȃ�
	if (wcscmp(eventName, L"shot") == 0)
	{
		MakeBullet();
	}
}

void Player::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
	//�t�H���g�̕`��
	//m_fontRender.Draw(rc);
}