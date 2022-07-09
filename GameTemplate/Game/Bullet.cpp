#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float MOVE_SPEED = 2000.0f;    //�ړ����x
	const float COLISION_SIZE = 3.0f;    //�R���W�����̑傫��
	const float DELEYE_TIME = 1.5f;      //�폜����
}

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
	//�G�t�F�N�g�̍Đ����~����B
	m_effectEmitter->Stop();
	DeleteGO(m_effectEmitter);
	DeleteGO(m_collisionObject);
}

bool Bullet::Start()
{
	//�J�����̃C���X�^���X��T��
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	//�v���C���[�̃C���X�^���X��T��
	m_player = FindGO<Player>("player");

	//�����_�ւ̃x�N�g��
	Vector3 target = m_gameCamera->GetTargetPosition();
	m_moveSpeed = target - m_position;
	m_position += m_player->GetForward() * 20.0f;
	//���K��
	m_moveSpeed.Normalize();
	m_moveSpeed *= MOVE_SPEED;

	EffectEngine::GetInstance()->ResistEffect(10, u"Assets/effect/efk/bullet.efk");
	m_effectEmitter = NewGO<EffectEmitter>(0);
	m_effectEmitter->Init(10);
	m_effectEmitter->SetScale(m_scale * 2.0f);
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//����̃R���W�������쐬����
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, m_scale.z* COLISION_SIZE);
	//���O��ݒ肷��
	m_collisionObject->SetName("bullet");
	//�����ō폜����Ȃ��悤��
	m_collisionObject->SetIsEnableAutoDelete(false);

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
	m_effectEmitter->SetPosition(m_position);
	//�폜�^�C�}�[���폜���Ԃ𒴂�����
	if (m_deleteTimer >= DELEYE_TIME)
	{
		//���g���폜����B
		DeleteGO(this);
	}
}