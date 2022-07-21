#include "stdafx.h"
#include "Bullet.h"
#include "GameCamera.h"
#include "player.h"

#include "collision/CollisionObject.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float FORWARD_POSITION = 20.0f;   //前座標
	const float RIGHT_POSITION = 6.0f;      //右座標
	const float MOVE_SPEED = 2000.0f;       //移動速度
	const float COLISION_SIZE = 5.0f;       //コリジョンの拡大率
	const int   SMOKE_EFFECT_NUMBER = 0;          //煙エフェクトの番号
	const float SMOKE_EFFECT_SCALE = 5.0f;        //煙エフェクトの拡大率
	const int   SHOT_SE_NUMBER = 1;              //銃声SEの番号
	const float SHOT_SE_SCALE = 0.5f;            //銃声SEの大きさ
	const float DELEYE_TIME = 1.5f;         //削除時間
}

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
	//コリジョンを削除する
	DeleteGO(m_collisionObject);
	//SEを削除する
	DeleteGO(m_se);
}

bool Bullet::Start()
{
	//カメラのインスタンスを探す
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");

	//SEを読み込んで登録する
	g_soundEngine->ResistWaveFileBank(SHOT_SE_NUMBER, "Assets/sound/shot.wav");
	//エフェクトを読み込んで登録する
	EffectEngine::GetInstance()->ResistEffect(SMOKE_EFFECT_NUMBER, u"Assets/effect/efk/smoke.efk");

	//注視点の座標を取得する
	Vector3 target = m_gameCamera->GetTargetPosition();
	//注視点に向かうベクトルを計算する
	m_moveSpeed = target - m_position;
	//移動ベクトルの正規化する
	m_moveSpeed.Normalize();
	//座標を少し前にする
	m_position += m_player->GetForward() * FORWARD_POSITION;
	//座標を少し右に寄せる
	m_position += g_camera3D->GetRight() * RIGHT_POSITION;
	//移動速度を設定
	m_moveSpeed *= MOVE_SPEED;

	//煙エフェクトを作成する
	EffectEmitter* effectEmitter = nullptr;
	effectEmitter = NewGO<EffectEmitter>(0);
	//煙エフェクトを初期化する
	effectEmitter->Init(SMOKE_EFFECT_NUMBER);
	//煙エフェクトの大きさを設定
	effectEmitter->SetScale(m_scale * SMOKE_EFFECT_SCALE);
	//煙エフェクトの座標を設定する
	effectEmitter->SetPosition(m_position);
	//煙エフェクトを再生する
	effectEmitter->Play();

	//球状のコリジョンを作成する
	m_collisionObject = NewGO<CollisionObject>(0);
	m_collisionObject->CreateSphere(m_position, Quaternion::Identity, m_scale.z* COLISION_SIZE);
	//名前を設定する
	m_collisionObject->SetName("bullet");
	//自動で削除されないようにする
	m_collisionObject->SetIsEnableAutoDelete(false);

	//銃声SEを作成する
	m_se = NewGO<SoundSource>(0);
	//銃声SEを初期化する
	m_se->Init(SHOT_SE_NUMBER);
	//銃声SEを再生する（ループなし）
	m_se->Play(false);
	//銃声SEの大きさを設定
	m_se->SetVolume(SHOT_SE_SCALE);

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

	//削除タイマーが削除時間を超えたら
	if (m_deleteTimer >= DELEYE_TIME)
	{
		//自身を削除する。
		DeleteGO(this);
	}
}