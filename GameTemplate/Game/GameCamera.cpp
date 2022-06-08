#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

namespace
{
	const Vector3 TO_CAMERA_POS = { 0.0f, 0.0f, -10.0f };  //�����_���王�_�ւ̃x�N�g��
	const float   HEAD_HEIGHT = 65.0f;                      //���_�̍���
	const float   HEAD_FORWARD = 20.0f;                     //���_�̑O���W
	const float   ROTATION_SPEED_X = 1.4f;                  //X���̃J�����̉�]���x
	const float   ROTATION_SPEED_Y = -1.4f;                 //Y���̃J�����̉�]���x
	const float   UP_MAX_POS = 0.95f;                       //������������l
	const float   DOWN_MAX_POS = -0.95;                     //�������������l
}  

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//�����_�̖ڈ�̃��f���̓ǂݍ���
	//m_modelRender.Init("Assets/modelData/bullet/bullet.tkm");
	//�摜�̓ǂݍ���
	m_spriteRender.Init("Assets/sprite/dot.dds", 1920.0f, 1080.0f);

	//�����_���王�_�܂ł̃x�N�g����ݒ�
	m_toCameraPos.Set(TO_CAMERA_POS);
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	return true;
}

void GameCamera::Update()
{
	//�����_���v�Z����B
	m_targetPosition = m_player->GetPosition();
	//���_���v�Z����
	Vector3 cameraPos = m_player->GetPosition();
	//�v���C���[�̓��̈ʒu�ɃJ������u��
	cameraPos.y += HEAD_HEIGHT;
	//�v���C���[�̊�̑O�ɃJ������u��
	cameraPos += g_camera3D->GetForward() * HEAD_FORWARD;

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
	m_targetPosition = cameraPos + m_toCameraPos;

	//���_�̐ݒ�
	g_camera3D->SetPosition(cameraPos);
	//�����_�̐ݒ�
	g_camera3D->SetTarget(m_targetPosition);

	//�J�����̍X�V
	g_camera3D->Update();

	//�����_�̍��W����
	//Vector3 modelPos = m_targetPosition;
	//m_modelRender.SetPosition(modelPos);
	//m_modelRender.SetScale(0.01f, 0.01f, 0.01f);
	//���f���̍X�V
	//m_modelRender.Update();
}

void GameCamera::Render(RenderContext& rc)
{
	//�N���X�w�A��`�悷��
	m_spriteRender.Draw(rc);
	//m_modelRender.Draw(rc);
}