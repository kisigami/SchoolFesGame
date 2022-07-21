#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float FORWARD_POSITION = 20.0f;   //�O���W
	const float RIGHT_POSITION = 6.0f;      //�E���W
	const float MOVE_SPEED = 2000.0f;       //�ړ����x
	const float COLISION_SIZE = 5.0f;       //�R���W�����̊g�嗦
	const int   SMOKE_EFFECT_NUMBER = 0;          //���G�t�F�N�g�̔ԍ�
	const float SMOKE_EFFECT_SCALE = 5.0f;        //���G�t�F�N�g�̊g�嗦
	const int   SHOT_SE_NUMBER = 1;              //�e��SE�̔ԍ�
	const float SHOT_SE_SCALE = 0.5f;            //�e��SE�̑傫��
	const float DELEYE_TIME = 1.5f;         //�폜����
}

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
	//�R���W�������폜����
	DeleteGO(m_collisionObject);
	//SE���폜����
	DeleteGO(m_se);
}

bool Bullet::Start()
{
	//�J�����̃C���X�^���X��T��
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	//SE��ǂݍ���œo�^����
	g_soundEngine->ResistWaveFileBank(SHOT_SE_NUMBER, "Assets/sound/shot.wav");
	//�G�t�F�N�g��ǂݍ���œo�^����
	EffectEngine::GetInstance()->ResistEffect(SMOKE_EFFECT_NUMBER, u"Assets/effect/efk/smoke.efk");

	//�����_�̍��W���擾����
	Vector3 target = m_gameCamera->GetTargetPosition();
	//�����_�Ɍ������x�N�g�����v�Z����
	m_moveSpeed = target - m_position;
	//�ړ��x�N�g���̐��K������
	m_moveSpeed.Normalize();
	//���W�������O�ɂ���
	m_position += m_player->GetForward() * FORWARD_POSITION;
	//���W�������E�Ɋ񂹂�
	m_position += g_camera3D->GetRight() * RIGHT_POSITION;
	//�ړ����x��ݒ�
	m_moveSpeed *= MOVE_SPEED;

	//���G�t�F�N�g���쐬����
	EffectEmitter* effectEmitter = nullptr;
	effectEmitter = NewGO<EffectEmitter>(0);
	//���G�t�F�N�g������������
	effectEmitter->Init(SMOKE_EFFECT_NUMBER);
	//���G�t�F�N�g�̑傫����ݒ�
	effectEmitter->SetScale(m_scale * SMOKE_EFFECT_SCALE);
	//���G�t�F�N�g�̍��W��ݒ肷��
	effectEmitter->SetPosition(m_position);
	//���G�t�F�N�g���Đ�����
	effectEmitter->Play();

	//����̃R���W�������쐬����
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, m_scale.z* COLISION_SIZE);
	//���O��ݒ肷��
	m_collisionObject->SetName("bullet");
	//�����ō폜����Ȃ��悤�ɂ���
	m_collisionObject->SetIsEnableAutoDelete(false);

	//�e��SE���쐬����
	m_se = NewGO<SoundSource>(0);
	//�e��SE������������
	m_se->Init(SHOT_SE_NUMBER);
	//�e��SE���Đ�����i���[�v�Ȃ��j
	m_se->Play(false);
	//�e��SE�̑傫����ݒ�
	m_se->SetVolume(SHOT_SE_SCALE);

	return true;
}

void Bullet::Update()
{	
	//�ړ����x�ō��W���ړ�������
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//�폜�^�C�}�[�����Z����B
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	//���W��ݒ肷��
	m_collisionObject->SetPosition(m_position);

	//�폜�^�C�}�[���폜���Ԃ𒴂�����
	if (m_deleteTimer >= DELEYE_TIME)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}