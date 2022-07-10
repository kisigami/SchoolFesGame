#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float MOVE_SPEED = 2000.0f;    //�ړ����x
	const float COLISION_SIZE = 5.0f;    //�R���W�����̑傫��
	const float DELEYE_TIME = 1.5f;      //�폜����
}

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
	DeleteGO(m_collisionObject);
	DeleteGO(m_se);
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
	m_position += g_camera3D->GetRight() * 6.0f;
	//���K��
	m_moveSpeed.Normalize();
	m_moveSpeed *= MOVE_SPEED;
	
	EffectEngine::GetInstance()->ResistEffect(11, u"Assets/effect/efk/smoke.efk");
	EffectEmitter* effectEmitter = nullptr;
	effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(11);
	effectEmitter->SetScale(m_scale * 5.0f);
	//Vector3 EffectPosition = m_position;
	//EffectPosition += m_player->GetForward() * 10.0f;
	//EffectPosition += g_camera3D->GetRight() * 2.0f;
	effectEmitter->SetPosition(m_position);
	effectEmitter->Play();

	//����̃R���W�������쐬����
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, m_scale.z* COLISION_SIZE);
	//���O��ݒ肷��
	m_collisionObject->SetName("bullet");
	//�����ō폜����Ȃ��悤��
	m_collisionObject->SetIsEnableAutoDelete(false);

	g_soundEngine->ResistWaveFileBank(1, "Assets/sound/shot.wav");
	m_se = NewGO<SoundSource>(0);
	m_se->Init(1);
	m_se->Play(false);
	m_se->SetVolume(0.5f);

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