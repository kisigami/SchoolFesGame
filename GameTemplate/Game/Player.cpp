#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

namespace
{
	const float ZERO = 0.0f;                  //0.0f
	const float CHARACON_RADIUS = 20.0f;      //�L�����R���̔��a
	const float CHARACON_HEIGHT = 50.0f;      //�L�����R���̍���
	const float MOVE_SPEED = 100.0f;          //�ړ����x
	const float MINIMUMVALUE = 0.00001f;      //�ړ����x�̍Œ�l
}

void Player::InitAnimation()
{
	//�A�j���[�V�����N���b�v�����[�h����
	m_animClips[enAnimClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/player/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
}

bool Player::Start()
{
	//�A�j���[�V����������
	InitAnimation();

	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/player/Demo.tkm", m_animClips, enAnimClip_Num);

	//�X�V
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//�J�����̃C���X�^���X���擾
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	//�L�����R����������
	m_charaCon.Init(CHARACON_RADIUS,CHARACON_HEIGHT, m_position);

	return true;
}

void Player::Update()
{
	//��]����
	Rotation();
	//�ړ�����
	Move();
	//�A�j���[�V�����̍Đ�
	PlayAnimation();
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();

	//���f���̍X�V
	m_modelRender.Update();
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
}

void Player::Rotation()
{
	//�J�����̒����_�ւ̃x�N�g��
	Vector3 targetVector = m_gameCamera->GetTargetPosition() - m_position;
	//���K��
	targetVector.Normalize();
	//y���W�̃x�N�g����0.0f�ɂ���
	targetVector.y = 0.0f;
	//��ɃJ�����̌����Ă������������
	m_rotation.SetRotationYFromDirectionXZ(targetVector);
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
	//�J�����̃x�N�g���~�R���g���[���[�̓��͒l�~�ړ����x
	m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
	m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	//�d��
	m_moveSpeed.y -= 500.0f * g_gameTime->GetFrameDeltaTime();
	//�L�����R�����g�p���āA���W���X�V����
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//���W�̍X�V
	m_modelRender.SetPosition(m_position);
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//�ҋ@�X�e�[�g�̎�
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle,0.3f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//����X�e�[�g�̎�
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
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
	}
}

void Player::ProcessCommonStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_moveSpeed.y += 250.0f;
	}

	//�ړ����x����������
	if (fabsf(m_moveSpeed.x) >= MINIMUMVALUE ||
		fabsf(m_moveSpeed.z) >= MINIMUMVALUE)
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

void Player::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
}