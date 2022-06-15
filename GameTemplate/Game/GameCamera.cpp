#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

namespace
{
	const Vector3 TO_CAMERA_POS = { 0.0f, 0.0f, -10000.0f };  //�����_���王�_�ւ̃x�N�g��
	const float   HEAD_HEIGHT = 52.0f;					      //���_�̍���
	const float   ROTATION_SPEED_X = 1.4f;                    //X���̃J�����̉�]���x
	const float   ROTATION_SPEED_Y = -1.4f;                   //Y���̃J�����̉�]���x
	const float   UP_MAX_POS = 0.8f;                          //������������l
	const float   DOWN_MAX_POS = -0.7;                        //�������������l
}  

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�摜�̓ǂݍ���
	m_spriteRender.Init("Assets/sprite/dot.dds", 1920.0f, 1080.0f);

	//�����_���王�_�܂ł̃x�N�g����ݒ�
	m_toCameraPos.Set(TO_CAMERA_POS);
	
	g_camera3D->SetFar(10000.0f);
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	return true;
}

void GameCamera::Update()
{
	//�����_���v�Z����B
	m_targetPosition = m_player->GetPosition();
	m_targetPosition.y += HEAD_HEIGHT;

	//���_���v�Z����
	m_cameraPosition = m_player->GetPosition();
	m_cameraPosition.y += HEAD_HEIGHT;
	m_cameraPosition += g_camera3D->GetForward() * 4.0f;

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

void GameCamera::Render(RenderContext& rc)
{
	//�N���X�w�A��`�悷��
	m_spriteRender.Draw(rc);
}