#include "stdafx.h"
#include "Player.h"

#include "GameCamera.h"
#include "Bullet.h"

namespace
{
	const float ZERO = 0.0f;                  //0.0f
	const float CHARACON_RADIUS = 20.0f;      //�L�����R���̔��a
	const float CHARACON_HEIGHT = 50.0f;      //�L�����R���̍���
	const float MOVE_SPEED = 100.0f;          //�ړ����x
	const float MINIMUMVALUE = 0.001f;        //�ړ����x�̍Œ�l
	const float JUMP_SPEED = 250.0f;          //�W�����v��
	const float GRAVITY = 500.0f;             //�d��
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
	m_animClips[enAnimClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/player/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
}

bool Player::Start()
{
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/gun/shotgun.tkm");

	m_position = { 0.0f,0.0f,0.0f };

	//�X�V
	m_modelRender.SetPosition(m_position.x,55.0f, m_position.z);
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
	//�e�X�e�[�g�̑J�ڏ���
	ManageState();

	//���f���̍X�V
	m_modelRender.Update();
	//���W�̍X�V
	m_modelRender.SetPosition(m_position.x, 55.0f, m_position.z);
	//��]�̍X�V
	m_modelRender.SetRotation(m_rotation);
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
	Vector3 vector = m_gameCamera->GetTargetPosition() - m_gameCamera->GetCameraPosition();
	//�����_��������]�p�x���擾
	float angle = atan2f(vector.y, vector.Length());
	//X���ɉ�]�p�x�����Z
	m_rotation.AddRotationX(-angle);
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
	m_moveSpeed.y = 0;
	//�L�����R�����g�p���āA���W���X�V����
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
}

void Player::MakeBullet()
{
	//�e���쐬����
	Bullet* bullet = NewGO<Bullet>(0);
	Vector3 bulletPosition = m_position;
	bulletPosition.y += 60.0f;
	bulletPosition += g_camera3D->GetRight() * 10.0f;
	bullet->SetPosition(bulletPosition);
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
	//A�{�^���������ꂽ��
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		//�W�����v����
		MakeBullet();
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