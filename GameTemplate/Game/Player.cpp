#include "stdafx.h"
#include "Player.h"

#include "GameCamera.h"
#include "Bullet.h"
#include "Enemy.h"
#include "graphics/effect/EffectEmitter.h"

namespace
{
	const float MODEL_POSITION_Y = 47.0f;            //モデルの座標
	const float ZERO = 0.0f;                         //0.0f
	const float CHARACON_RADIUS = 20.0f;             //キャラコンの半径
	const float CHARACON_HEIGHT = 50.0f;             //キャラコンの高さ
	const float MOVE_SPEED = 300.0f;                 //移動速度
	const float SHOT_MOVE_SPEED = 65.0f;             //射撃ステートの移動速度
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;    //移動速度の最低値
	const float JUMP_POWER = 200.0f;                 //ジャンプ力
	const float GRAVITY = 500.0f;                    //重力
	const float IDLE_ANIMATION_SPEED = 1.0f;         //待機アニメーションの再生速度
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;   //待機アニメーションの補完時間
	const float RUN_ANIMATION_SPEED = 0.8f;          //走りアニメーションの再生時間
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;    //走りアニメーションの補完時間
	const float SHOT_ANIMATION_SPEED = 2.0f;         //射撃アニメーションの再生時間
	const float SHOT_ANIMATION_INTERPOLATE = 0.1f;   //射撃アニメーションの補完時間
}

Player::Player()
{

}

Player::~Player()
{

}

void Player::InitAnimation()
{
	//アニメーションクリップをロードする
	m_animClips[enAnimClip_RifleIdle].Load("Assets/animData/ar/idle.tka");
	m_animClips[enAnimClip_RifleIdle].SetLoopFlag(true);
	m_animClips[enAnimClip_RifleRun].Load("Assets/animData/ar/walk.tka");
	m_animClips[enAnimClip_RifleRun].SetLoopFlag(true);
}

bool Player::Start()
{
	//アニメーションの初期化
	InitAnimation();
	//カメラのインスタンスを取得
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	m_enemy = FindGO<Enemy>("enemy");
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/gun/ar/ar3.tkm",m_animClips,enAnimClip_Num);
	//座標に原点を代入する
	m_position = Vector3::Zero;
	//エフェクトを読み込む
	EffectEngine::GetInstance()->ResistEffect(1, u"Assets/effect/efk/smoke.efk");
	//座標の設定
	m_modelRender.SetPosition(m_position.x, m_position.y, m_position.z);
	//キャラコンを初期化
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
	//モデルの更新
	m_modelRender.Update();

	return true;
}

void Player::Update()
{
	//HPを表示
	wchar_t wcsbuf[256];
	swprintf_s(wcsbuf, 256, L"HP=%d", int(m_hp));
	m_fontRender.SetText(wcsbuf);
	m_fontRender.SetPosition(-300.0f,0.0f,0.0f);
	m_fontRender.SetScale(2.3f);
	m_fontRender.SetColor(Vector4(1.0f, 1.0f, 0.0f, 1.0f));

	Shot();
	//回転処理
	Rotation();
	//移動処理
	Move();
	Collision();
	//各ステートの遷移処理
	ManageState();
	//アニメーションの再生
	PlayAnimation();

	//モデルの更新
	m_modelRender.Update();
}

void Player::Shot()
{
	if (m_playerState != enPlayerState_Shot)
	{
		return;
	}
	MakeBullet();
	EffectPlay();
}

void Player::EffectPlay()
{
	//攻撃エフェクトを発生させる。
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * 1.5f);
	Vector3 effectPosition = m_position;
	//座標を少し上にする。
	effectPosition = m_forward * 20.0f;
	effectPosition.y += 40.0f;
	effectEmitter->SetPosition(effectPosition);
	//エフェクトを再生する。
	effectEmitter->Play();
}

void Player::Rotation()
{
	//プレイヤーから注視点へのベクトル
	Vector3 targetVector = m_gameCamera->GetTargetPosition() - m_position;
	//正規化
	targetVector.Normalize();
	//注視点へのベクトルからY軸の回転クオータニオンを作成
	m_rotation.SetRotationYFromDirectionXZ(targetVector);
	//視点から注視点へのベクトル
	Vector3 gunTarget = m_gameCamera->GetTargetPosition();
	Vector3 gunCamera = m_gameCamera->GetCameraPosition();
	Vector3 vector = gunTarget - gunCamera;
	//注視点を向く回転角度を取得
	float angle = atan2f(vector.y, vector.Length());
	////X軸に回転角度を加算
	m_rotation.AddRotationX(-angle);
	//回転の設定
	m_modelRender.SetRotation(m_rotation);
}

void Player::Move()
{
	//移動速度
	m_moveSpeed.x = ZERO;
	m_moveSpeed.z = ZERO;
	//ステックの入力量を取得
	float lStick_x = g_pad[0]->GetLStickXF();
	float lStick_y = g_pad[0]->GetLStickYF();
	//cameraの前方向と右方向を取得
	Vector3 cameraForward = g_camera3D->GetForward();
	Vector3 cameraRight = g_camera3D->GetRight();
	//XZ平面での前方方向、右方向に変換する
	cameraForward.y = ZERO;
	cameraForward.Normalize();
	cameraRight.y = ZERO;
	cameraRight.Normalize();
	//射撃ステートだったら
	if (m_playerState == enPlayerState_Shot)
	{
		//射撃ステートの移動速度＝カメラのベクトル×コントローラーの入力値×速度
		m_moveSpeed += cameraForward * lStick_y * SHOT_MOVE_SPEED;
		m_moveSpeed += cameraRight * lStick_x * SHOT_MOVE_SPEED;
	}
	//それ以外のステートだったら
	else
	{
		//移動速度＝カメラのベクトル×コントローラーの入力値×速度
		m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
		m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	}
	//重力
	m_moveSpeed.y -= 500.0f * g_gameTime->GetFrameDeltaTime();
	//キャラコンの座標をプレイヤーの座標に代入
	m_position = m_charaCon.Execute( m_moveSpeed,g_gameTime->GetFrameDeltaTime());
	//モデルをカメラに合わせて動かす
	m_modelRender.SetPosition(
		m_gameCamera->GetCameraPosition().x,
		m_gameCamera->GetCameraPosition().y - 5.0f,
		m_gameCamera->GetCameraPosition().z
	);
}

void Player::Collision()
{
	//被ダメージステートかダウンステートだったら
	if (m_playerState == enPlayerState_ReceiveDamage ||
		m_playerState == enPlayerState_Down)
	{
		//何もしない
		return;
	}
	//敵の攻撃の当たり判定
	const auto& collisions = g_collisionObjectManager->FindCollisionObjects("enemy_attack");
	for (auto collision : collisions)
	{
		//コリジョンがキャラコンと衝突したら
		if (collision->IsHit(m_charaCon))
		{
			//ダメージを受ける
			m_hp -= 20;
			//コリジョンを殺す
			collision->Dead();
			//コリジョンを非アクティブにする
			collision->Deactivate();
			//エネミーを攻撃中ではなくする
			m_enemy->m_attacking = false;
			//HPが0より小さかったら
			if (m_hp <= 0)
			{
				//ダウンステートに移行する
				m_playerState = enPlayerState_Down;
				return;
			}
			//HPが0より大きかったら
			else
			{
				//被ダメージステートに移行する
				m_playerState = enPlayerState_ReceiveDamage;
			}
			return;
		}
	}
}

void Player::MakeBullet()
{
	//弾オブジェクトを作成する
	Bullet* bullet = NewGO<Bullet>(0);
	Vector3 position = m_gameCamera->GetCameraPosition();
	//座標を視点の下にする
	position.y -= 5.0f;
	//座標を設定する
	bullet->SetPosition(position);
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_RifleIdle,IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//走りステートの時
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_RifleRun, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//射撃ステートの時
	case Player::enPlayerState_Shot:
		break;
	}
}

void Player::ManageState()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case Player::enPlayerState_Idle:
		ProcessIdleStateTransition();
		break;
		//走りステートの時
	case Player::enPlayerState_Run:
		ProcessRunStateTransition();
		break;
		//射撃ステートの時
	case Player::enPlayerState_Shot:
		ProcessShotStateTransition();
		break;
		//被ダメージステートの時
	case Player::enPlayerState_ReceiveDamage:
		ProcessReceiveDamageStateTransition();
		break;
		//ダウンステートの時
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_playerState = enPlayerState_Shot;
		return;
	}

	//移動速度があったら
	if (fabsf(m_moveSpeed.x) >= MOVE_SPEED_MINIMUMVALUE ||
		fabsf(m_moveSpeed.z) >= MOVE_SPEED_MINIMUMVALUE)
	{
		//走りステートに移行する
		m_playerState = enPlayerState_Run;
		return;
	}

	//移動速度がなかったら
	else
	{
		//待機ステートに移行する
		m_playerState = enPlayerState_Idle;
	}
}

void Player::ProcessIdleStateTransition()
{
	//共通のステートの遷移処理
	ProcessCommonStateTransition();
}

void Player::ProcessRunStateTransition()
{
	//共通のステートの遷移処理
	ProcessCommonStateTransition();
}

void Player::ProcessShotStateTransition()
{
	//待機ステートに移行する
	m_playerState = enPlayerState_Idle;
}

void Player::ProcessReceiveDamageStateTransition()
{
	//無敵タイマーを減少させる
	m_receiveDamageTimer -= g_gameTime->GetFrameDeltaTime();
	//無敵タイマーが0.0f以下だったら
	if (m_receiveDamageTimer <= 0.0f)
	{
		//共通のステートの遷移処理
		ProcessCommonStateTransition();
		//無敵タイマーをリセットする
		m_receiveDamageTimer = 1.0f;
	}
}

void Player::ProcessDownStateTransition()
{
	//共通のステートの遷移処理
	ProcessCommonStateTransition();
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{

}

void Player::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
	//フォントの描画
	m_fontRender.Draw(rc);
}