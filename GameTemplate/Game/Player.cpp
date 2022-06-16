#include "stdafx.h"
#include "Player.h"

#include "GameCamera.h"
#include "Bullet.h"

namespace
{
	const float MODEL_POSITION_Y = 47.0f;            //モデルの座標
	const float ZERO = 0.0f;                         //0.0f
	const float CHARACON_RADIUS = 20.0f;             //キャラコンの半径
	const float CHARACON_HEIGHT = 50.0f;             //キャラコンの高さ
	const float MOVE_SPEED = 100.0f;                 //移動速度
	const float SHOT_MOVE_SPEED = 65.0f;             //射撃ステートの移動速度
	const float MINIMUMVALUE = 0.001f;               //移動速度の最低値
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
	/*m_animClips[enAnimClip_RifleRun].Load("Assets/animData/run.tka");
	m_animClips[enAnimClip_RifleRun].SetLoopFlag(true);*/
	m_animClips[enAnimClip_RifleShot].Load("Assets/animData/ar/shot.tka");
	m_animClips[enAnimClip_RifleShot].SetLoopFlag(false);

}

bool Player::Start()
{
	//アニメーションの初期化
	InitAnimation();

	//カメラのインスタンスを取得
	m_gameCamera = FindGO<GameCamera>("gamecamera");

	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/gun/ar/ar.tkm", m_animClips, enAnimClip_Num);
	
	m_modelRender.AddAnimationEvent([&](const wchar_t* clipName, const wchar_t* eventName) {
		OnAnimationEvent(clipName, eventName);
		});

	m_position = { 0.0f,0.0f,0.0f };

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
	//フォント
	wchar_t text[256];
	swprintf_s(text, 256, L"%d",m_bulletNum);
	m_fontRender.SetText(text);
	m_fontRender.SetPosition(Vector3::Zero);
	m_fontRender.SetScale(2.0f);

	//フォント
	wchar_t text2[256];
	swprintf_s(text2, 256, L"p%f", m_position.y);
	font.SetText(text2);
	font.SetPosition(0.0f,-100.0f,0.0f);
	font.SetScale(2.0f);

	//回転処理
	Rotation();
	//移動処理
	Move();
	//各ステートの遷移処理
	ManageState();
	//アニメーションの再生
	PlayAnimation();

	//モデルの更新
	m_modelRender.Update();
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
	Vector3 vector = m_gameCamera->GetTargetPosition() - m_gameCamera->GetCameraPosition();
	//注視点を向く回転角度を取得
	float angle = atan2f(vector.y, vector.Length());
	//X軸に回転角度を加算
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
	//Aボタンが押されたら&足がついていたら
	if (g_pad[0]->IsTrigger(enButtonA) &&
		m_charaCon.IsOnGround() == true)
	{
		//ジャンプする
		m_moveSpeed.y = JUMP_POWER;
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

void Player::MakeBullet()
{
	//弾を作成する
	Bullet* bullet = NewGO<Bullet>(0);
	//プレイヤーの座標を取得
	Vector3 bulletPosition = m_position;
	//座標を上げる
	bulletPosition.y += 47.0f;
	//座標を設定する
	bullet->SetPosition(bulletPosition);
	//弾数を減らす
	m_bulletNum -= 1;
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
		/*m_modelRender.PlayAnimation(enAnimClip_RifleRun, RUN_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(RUN_ANIMATION_SPEED);*/
		break;
		//射撃ステートの時
	case Player::enPlayerState_Shot:
		m_modelRender.PlayAnimation(enAnimClip_RifleShot, SHOT_ANIMATION_INTERPOLATE);
		m_modelRender.SetAnimationSpeed(SHOT_ANIMATION_SPEED);
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
	}
}

void Player::ProcessCommonStateTransition()
{
	//残りの弾数が０以上だったら
	if (m_bulletNum > 0)
	{
		//Aボタンが押されたら
		if (g_pad[0]->IsPress(enButtonRB2))
		{
			//射撃ステートに移行する
			m_playerState = enPlayerState_Shot;
			return;
		}
	}

	if (g_pad[0]->IsPress(enButtonX))
	{
		m_bulletNum = 30;
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

void Player::ProcessShotStateTransition()
{
	//アニメーションの再生が終わっていたら
	if (m_modelRender.IsPlayingAnimation() == false)
	{
		//待機ステートに移行する
		m_playerState = enPlayerState_Idle;
	}
}

void Player::OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName)
{
	(void)clipName;
	//キーの名前が「shot_attack」の時
	if (wcscmp(eventName, L"shot_attack") == 0) 
	{
		//弾を作成する
		MakeBullet();
	}
}

void Player::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
	//フォントの描画
	m_fontRender.Draw(rc);
	font.Draw(rc);
}