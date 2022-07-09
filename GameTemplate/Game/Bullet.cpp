#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float MOVE_SPEED = 2000.0f;    //移動速度
	const float COLISION_SIZE = 3.0f;    //コリジョンの大きさ
	const float DELEYE_TIME = 1.5f;      //削除時間
}

Bullet::Bullet()
{

}

Bullet::~Bullet()
{
	//エフェクトの再生を停止する。
	m_effectEmitter->Stop();
	DeleteGO(m_effectEmitter);
	DeleteGO(m_collisionObject);
}

bool Bullet::Start()
{
	//カメラのインスタンスを探す
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");

	//注視点へのベクトル
	Vector3 target = m_gameCamera->GetTargetPosition();
	m_moveSpeed = target - m_position;
	m_position += m_player->GetForward() * 20.0f;
	//正規化
	m_moveSpeed.Normalize();
	m_moveSpeed *= MOVE_SPEED;

	EffectEngine::GetInstance()->ResistEffect(10, u"Assets/effect/efk/bullet.efk");
	m_effectEmitter = NewGO<EffectEmitter>(0);
	m_effectEmitter->Init(10);
	m_effectEmitter->SetScale(m_scale * 2.0f);
	m_effectEmitter->SetRotation(m_rotation);
	m_effectEmitter->Play();

	//球状のコリジョンを作成する
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, m_scale.z* COLISION_SIZE);
	//名前を設定する
	m_collisionObject->SetName("bullet");
	//自動で削除されないように
	m_collisionObject->SetIsEnableAutoDelete(false);

	return true;
}

void Bullet::Update()
{	
	//移動速度で座標を移動させる
	m_position += m_moveSpeed * g_gameTime->GetFrameDeltaTime();
	//削除タイマーを加算する。
	m_deleteTimer += g_gameTime->GetFrameDeltaTime();
	//座標を設定する
	m_collisionObject->SetPosition(m_position);
	m_effectEmitter->SetPosition(m_position);
	//削除タイマーが削除時間を超えたら
	if (m_deleteTimer >= DELEYE_TIME)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}