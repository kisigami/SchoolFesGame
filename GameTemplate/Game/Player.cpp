#include "stdafx.h"
#include "Player.h"

#include "GameCamera.h"
#include "Bullet.h"
#include "graphics/effect/EffectEmitter.h"
#include <math.h>

namespace
{
	const float MODEL_POSITION_Y = 47.0f;                       //モデルの座標
	const float ZERO = 0.0f;								    //0.0f
	const float CHARACON_RADIUS = 25.0f;                        //キャラコンの半径
	const float CHARACON_HEIGHT = 50.0f;                        //キャラコンの高さ
	const float MOVE_SPEED = 200.0f;                            //移動速度
	const float SHOT_MOVE_SPEED = 60.0f;                        //射撃ステートの移動速度
	const float MOVE_SPEED_MINIMUMVALUE = 0.001f;               //移動速度の最低値
	const float JUMP_POWER = 200.0f;                            //ジャンプ力
	const float GRAVITY = 500.0f;                               //重力
	const float IDLE_ANIMATION_SPEED = 1.0f;                    //待機アニメーションの再生速度
	const float IDLE_ANIMATION_INTERPOLATE = 0.3f;              //待機アニメーションの補完時間
	const float RUN_ANIMATION_SPEED = 1.0f;                     //走りアニメーションの再生時間
	const float RUN_ANIMATION_INTERPOLATE = 0.1f;               //走りアニメーションの補完時間
	const float SHOT_ANIMATION_SPEED = 2.0f;                    //射撃アニメーションの再生時間
	const float SHOT_ANIMATION_INTERPOLATE = 0.1f;              //射撃アニメーションの補完時間
	const float INVICNIBILITY_TIME = 1.0f;                      //無敵時間
	const float INVICNIBILITY_MIN_TIME = 0.0f;                  //無敵時間の最低値
	const int   RECEIVE_DAMAGE_VALUE = 20;                      //被ダメージの値
	const int   HP_LOWEST_VALUE = 0;                            //HPの最低値
	const float SMOKE_EFFECT_FORWARD = 20.0f;
	const float SMOKE_EFFECT_Y_POSITION = 40.0f;
}

Player::Player()
{

}

Player::~Player()
{
	DeleteGO(m_walkse);
}

void Player::InitAnimation()
{
	//アニメーションクリップをロードする
	m_animClips[enAnimClip_Idle].Load("Assets/animData/ar/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/ar/walk.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
	m_animClips[enAnimClip_Shot].Load("Assets/animData/ar/shot.tka");
	m_animClips[enAnimClip_Shot].SetLoopFlag(false);
	m_animClips[enAnimClip_Reload].Load("Assets/animData/ar/reload2.tka");
	m_animClips[enAnimClip_Reload].SetLoopFlag(false);
}

bool Player::Start()
{
	g_soundEngine->ResistWaveFileBank(10, "Assets/sound/player/walksound.wav");
	g_soundEngine->ResistWaveFileBank(15, "Assets/sound/player/reload.wav");

	//アニメーションの初期化
	InitAnimation();
	//カメラのインスタンスを取得
	m_gameCamera = FindGO<GameCamera>("gamecamera");
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/gun/ar/ar3.tkm",m_animClips,enAnimClip_Num);
	//座標に原点を代入する
	m_position = Vector3::Zero;
	//アニメーションイベント用の関数を設定する。
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
	});
	//座標の設定
	m_modelRender.SetPosition(m_position);
	//キャラコンを初期化
	m_charaCon.Init(CHARACON_RADIUS, CHARACON_HEIGHT, m_position);
	//モデルの更新
	m_modelRender.Update();

	return true;
}

void Player::Update()
{
	//足音処理
	MoveSound();
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

void Player::EffectPlay()
{
	//攻撃エフェクトを発生させる。
	EffectEmitter* effectEmitter = NewGO<EffectEmitter>(0);
	effectEmitter->Init(1);
	effectEmitter->SetScale(Vector3::One * 1.5f);
	Vector3 effectPosition = m_position;
	//座標を少し上にする。
	effectPosition = m_forward * SMOKE_EFFECT_FORWARD;
	effectPosition.y += SMOKE_EFFECT_Y_POSITION;
	effectEmitter->SetPosition(effectPosition);
	//エフェクトを再生する。
	effectEmitter->Play();
}

void Player::Rotation()
{
	Vector3 m_target = m_gameCamera->GetTargetPosition();
	Vector3 toTarget = m_target - m_position;
	float angle = atan2f(toTarget.x, toTarget.z);
	m_rotation.SetRotationY(angle);

	//視点から注視点へのベクトル
	Vector3 gunTarget = m_gameCamera->GetTargetPosition();
	Vector3 Pos = m_gameCamera->GetCameraPosition();
	Vector3 diff = gunTarget - Pos;
	//注視点を向く回転角度を取得
	float angle2 = atan2f(-diff.y, g_camera3D->GetTargetToPositionLength());
	//X軸に回転角度を加算
	m_rotation.AddRotationX(angle2);

	m_modelRender.SetRotation(m_rotation);

	//プレイヤーの正面ベクトルを計算する。
	m_forward = Vector3::AxisZ;
	m_rotation.Apply(m_forward);
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
	cameraRight.y =  ZERO;
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
	if (m_position.y < 1.0f)
	{
		m_position.y = 1.0f;
	}
	//モデルをカメラに合わせて動かす
	Vector3 a = m_gameCamera->GetCameraPosition();
	a.y -= 4.0f;
	m_modelRender.SetPosition(a); 
}

void Player::Invincibility()
{
	if (m_invincibilityFlag == true)
	{
		//無敵タイマーを減少させる
		m_receiveDamageTimer -= g_gameTime->GetFrameDeltaTime();
		//無敵タイマーが0.0f以下だったら
		if (m_receiveDamageTimer <= INVICNIBILITY_MIN_TIME)
		{
			m_invincibilityFlag = false;
			//無敵タイマーをリセットする
			m_receiveDamageTimer = INVICNIBILITY_TIME;
		}
	}
}

void Player::Collision()
{
	//被ダメージステートかダウンステートだったら
	if (m_invincibilityFlag == true ||
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
			m_hp -= RECEIVE_DAMAGE_VALUE;
			//コリジョンを殺す
			collision->Dead();
			//コリジョンを非アクティブにする
			collision->Deactivate();

			//HPが0より小さかったら
			if (m_hp <= HP_LOWEST_VALUE)
			{
				//ダウンステートに移行する
				m_playerState = enPlayerState_Down;
				return;
			}
			//HPが0より大きかったら
			else
			{
				m_invincibilityFlag = true;
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
	position.y -= 3.0f;
	position += m_forward * 30.0f;
	//座標を設定する
	bullet->SetPosition(position);
	bullet->SetRotation(m_rotation);
	m_bulletNum--;
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle,IDLE_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(IDLE_ANIMATION_SPEED);
		break;
		//走りステートの時
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);
		break;
		//射撃ステートの時
	case Player::enPlayerState_Shot:
		m_modelRender.PlayAnimation(enAnimClip_Shot, SHOT_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(SHOT_ANIMATION_SPEED);
		break;
		//射撃ステートの時
	case Player::enPlayerState_Reload:
		m_modelRender.PlayAnimation(enAnimClip_Reload, SHOT_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(2.0f);
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
		//リロードステートの時
	case Player::enPlayerState_Reload:
		ProcessReloadStateTransition();
		break;
		//ダウンステートの時
	case Player::enPlayerState_Down:
		ProcessDownStateTransition();
		break;
	}
}

void Player::ProcessCommonStateTransition()
{
	//RB2ボタンが押されたら
	if (g_pad[0]->IsPress(enButtonRB2))
	{
		//残弾が0だったら
		if (m_bulletNum == 0)
		{
			//リロード音を再生する
			ReloadSound();
			//リロードステートに移行する
			m_playerState = enPlayerState_Reload;
		}
		else
		{
			//射撃ステートに移行する
			m_playerState = enPlayerState_Shot;
		}
		return;
	}
	//Xボタンが押される＆残弾が最大ではなかったら
	if (g_pad[0]->IsPress(enButtonX) && m_bulletNum < 30)
	{
		//リロード音を再生する
		ReloadSound();
		//リロードステートに移行する
		m_playerState = enPlayerState_Reload;
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
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステートに移行する
		m_playerState = enPlayerState_Idle;
	}
}

void Player::ProcessReloadStateTransition()
{
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//残弾を最大にする
		m_bulletNum = 30;
		//共通のステートの遷移処理
		ProcessCommonStateTransition();
	}
}

void Player::ProcessDownStateTransition()
{
	//共通のステートの遷移処理
	ProcessCommonStateTransition();
}

void Player::MoveSound()
{
	//seが再生中だったら
	if (m_soundFlag == true)
	{
		//移動速度が
		if (fabsf(m_moveSpeed.x) < 0.001f &&
			fabsf(m_moveSpeed.z) < 0.001f)
		{
			m_walkse->Stop();
			m_soundFlag = false;
		}
	}

	if (m_soundFlag == false)
	{
		if (fabsf(m_moveSpeed.x) > 0.001f &&
			fabsf(m_moveSpeed.z) > 0.001f)
		{
			m_walkse = NewGO<SoundSource>(0);
			m_walkse->Init(10);
			m_walkse->SetVolume(0.7f);
			m_walkse->Play(true);
			m_soundFlag = true;
		}
	}
}

void Player::ReloadSound()
{
	SoundSource* se;
	se = NewGO<SoundSource>(0);
	se->Init(15);
	se->SetVolume(0.5f);
	se->Play(false);
	if (m_hp <= 0)
	{
		se->Stop();
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が「attack_start」なら
	if (wcscmp(eventName, L"shot") == 0)
	{
		MakeBullet();
	}
}

void Player::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}