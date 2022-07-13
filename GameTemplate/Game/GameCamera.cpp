#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

namespace
{
	const Vector3 TO_CAMERA_POS = { 0.0f, 0.0f, -1000.0f };   //�����_���王�_�ւ̃x�N�g��
	const float   CAMERA_HEIGHT_POSITION = 52.0f;			  //���_�̍���
	const float   ROTATION_SPEED_X = 2.0f;                    //X���̃J�����̉�]���x
	const float   ROTATION_SPEED_Y = -1.0f;                   //Y���̃J�����̉�]���x
	const float   UP_MAX_POS = 0.3f;                          //������������l
	const float   DOWN_MAX_POS = -0.3;                        //�������������l
}  

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	//�����_���王�_�܂ł̃x�N�g����ݒ�
	m_toCameraPos.Set(TO_CAMERA_POS);

	return true;
}

void GameCamera::Update()
{
	//�����_���v�Z����B
	m_targetPosition = m_player->GetPosition();

	//���_���v�Z����
	m_cameraPosition = m_player->GetPosition();
	m_cameraPosition.y += CAMERA_HEIGHT_POSITION;

	//�ϐ��ɒ����_���王�_�܂ł̃x�N�g������
	Vector3 toCameraPosOld = m_toCameraPos;

	//�p�b�h�̓��͂��g���ăJ�������񂷁B
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y������̉�]
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, ROTATION_SPEED_X * x);
	qRot.Apply(m_toCameraPos);
	//X������̉�]�B
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, ROTATION_SPEED_Y * y);
	qRot.Apply(m_toCameraPos);
	//�J�����̉�]�̏�����`�F�b�N����B
	//�����_���王�_�܂ł̃x�N�g���𐳋K������B
	//���K������ƁA�x�N�g���̑傫�����P�ɂȂ�B
	//�傫�����P�ɂȂ�Ƃ������Ƃ́A�x�N�g�����狭�����Ȃ��Ȃ�A�����݂̂̏��ƂȂ�Ƃ������ƁB
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < DOWN_MAX_POS)
	{
		//�J����������������
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > UP_MAX_POS) 
	{
		//�J���������������
		m_toCameraPos = toCameraPosOld;
	}

	//�����_���v�Z����
	m_targetPosition = m_cameraPosition + m_toCameraPos;
	//���_�̐ݒ�
	g_camera3D->SetPosition(m_cameraPosition);
	//�����_�̐ݒ�
	g_camera3D->SetTarget(m_targetPosition);

	//�J�����̍X�V
	g_camera3D->Update();
}