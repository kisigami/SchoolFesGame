#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"

namespace
{
	const float ZERO = 0.0f;                  //0.0f
	const float CHARACON_RADIUS = 20.0f;      //キャラコンの半径
	const float CHARACON_HEIGHT = 50.0f;      //キャラコンの高さ
	const float MOVE_SPEED = 100.0f;          //移動速度
	const float MINIMUMVALUE = 0.00001f;      //移動速度の最低値
}

void Player::InitAnimation()
{
	//アニメーションクリップをロードする
	m_animClips[enAnimClip_Idle].Load("Assets/animData/player/idle.tka");
	m_animClips[enAnimClip_Idle].SetLoopFlag(true);
	m_animClips[enAnimClip_Run].Load("Assets/animData/player/run.tka");
	m_animClips[enAnimClip_Run].SetLoopFlag(true);
}

bool Player::Start()
{
	//アニメーション初期化
	InitAnimation();

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/player/Demo.tkm", m_animClips, enAnimClip_Num);

	//更新
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	//カメラのインスタンスを取得
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	//キャラコンを初期化
	m_charaCon.Init(CHARACON_RADIUS,CHARACON_HEIGHT, m_position);

	return true;
}

void Player::Update()
{
	//回転処理
	Rotation();
	//移動処理
	Move();
	//アニメーションの再生
	PlayAnimation();
	//各ステートの遷移処理
	ManageState();

	//モデルの更新
	m_modelRender.Update();
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
}

void Player::Rotation()
{
	//カメラの注視点へのベクトル
	Vector3 targetVector = m_gameCamera->GetTargetPosition() - m_position;
	//正規化
	targetVector.Normalize();
	//y座標のベクトルを0.0fにする
	targetVector.y = 0.0f;
	//常にカメラの向いている向きを向く
	m_rotation.SetRotationYFromDirectionXZ(targetVector);
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
	//カメラのベクトル×コントローラーの入力値×移動速度
	m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
	m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	//重力
	m_moveSpeed.y -= 500.0f * g_gameTime->GetFrameDeltaTime();
	//キャラコンを使用して、座標を更新する
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//座標の更新
	m_modelRender.SetPosition(m_position);
}

void Player::PlayAnimation()
{
	switch (m_playerState)
	{
		//待機ステートの時
	case Player::enPlayerState_Idle:
		m_modelRender.PlayAnimation(enAnimClip_Idle,0.3f);
		m_modelRender.SetAnimationSpeed(1.0f);
		break;
		//走りステートの時
	case Player::enPlayerState_Run:
		m_modelRender.PlayAnimation(enAnimClip_Run, 0.1f);
		m_modelRender.SetAnimationSpeed(1.0f);
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
	}
}

void Player::ProcessCommonStateTransition()
{
	if (g_pad[0]->IsTrigger(enButtonA))
	{
		m_moveSpeed.y += 250.0f;
	}

	//移動速度があったら
	if (fabsf(m_moveSpeed.x) >= MINIMUMVALUE ||
		fabsf(m_moveSpeed.z) >= MINIMUMVALUE)
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

void Player::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}