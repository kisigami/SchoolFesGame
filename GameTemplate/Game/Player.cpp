#include "stdafx.h"
#include "Player.h"

namespace
{
	const float ZERO = 0.0f;                  //0.0f
	const float CHARACON_RADIUS = 20.0f;      //キャラコンの半径
	const float CHARACON_HEIGHT = 50.0f;      //キャラコンの高さ
	const float MOVE_SPEED = 100.0f;          //移動速度
	const float MINIMUMVALUE = 0.001f;        //移動速度の最低値
}

bool Player::Start()
{
	//モデルの読み込み
	m_modelRender.Init("Assets/modelData/Player/Demo.tkm");
	m_modelRender.SetPosition(m_position);
	m_modelRender.Update();

	m_charaCon.Init(CHARACON_RADIUS,CHARACON_HEIGHT, m_position);

	return true;
}

void Player::Update()
{
	//回転処理
	Rotation();
	//移動処理
	Move();

	//モデルの更新
	m_modelRender.Update();
	m_modelRender.SetPosition(m_position);
	m_modelRender.SetRotation(m_rotation);
}

void Player::Rotation()
{
	//コントローラーの入力がなかったら
	if (fabsf(m_moveSpeed.x) < MINIMUMVALUE
		&& fabsf(m_moveSpeed.z) < MINIMUMVALUE)
	{
		//何もしない
		return;
	}
	float angle = atan2(-m_moveSpeed.x, m_moveSpeed.z);
	m_rotation.SetRotationY(-angle);
	//回転を設定する
	m_modelRender.SetRotation(m_rotation);
	//プレイヤーの正面ベクトルを計算する
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
	cameraRight.y = ZERO;
	cameraRight.Normalize();
	m_moveSpeed += cameraForward * lStick_y * MOVE_SPEED;
	m_moveSpeed += cameraRight * lStick_x * MOVE_SPEED;
	//キャラコンを使用して、座標を更新する
	m_position = m_charaCon.Execute(m_moveSpeed, g_gameTime->GetFrameDeltaTime());
	//座標の更新
	Vector3 modelPosition = m_position;
	m_modelRender.SetPosition(modelPosition);
}

void Player::Render(RenderContext& rc)
{
	//モデルの描画
	m_modelRender.Draw(rc);
}