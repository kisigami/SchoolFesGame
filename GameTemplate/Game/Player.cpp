#include "stdafx.h"
#include "Player.h"

namespace
{
	const float ZERO = 0.0f;                  //0.0f
	const float CHARACON_RADIUS = 20.0f;      //�L�����R���̔��a
	const float CHARACON_HEIGHT = 50.0f;      //�L�����R���̍���
	const float MOVE_SPEED = 100.0f;          //�ړ����x
	const float MINIMUMVALUE = 0.001f;        //�ړ����x�̍Œ�l
}

bool Player::Start()
{
	//���f���̓ǂݍ���
	m_modelRender.Init("Assets/modelData/Player/Demo.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	m_charaCon.Init(CHARACON_RADIUS,CHARACON_HEIGHT, m_position);

	return true;
}

void Player::Update()
{
	//��]����
	Rotation();
	//�ړ�����
	Move();

	//���f���̍X�V
	m_modelRender.Update();
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
}

void Player::Rotation()
{
	//�R���g���[���[�̓��͂��Ȃ�������
	if (fabsf(m_moveSpeed.x) < MINIMUMVALUE
		&& fabsf(m_moveSpeed.z) < MINIMUMVALUE)
	{
		//�������Ȃ�
		return;
	}
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);
	//��]��ݒ肷��
	m_modelRender.SetRotation(m_rotation);
	//�v���C���[�̐��ʃx�N�g�����v�Z����
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
	m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
	m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	//�L�����R�����g�p���āA���W���X�V����
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//���W�̍X�V
	Vector3 modelPosition = m_position;
	m_modelRender.SetPosition(modelPosition);
}

void Player::Render(RenderContext& rc)
{
	//���f���̕`��
	m_modelRender.Draw(rc);
}