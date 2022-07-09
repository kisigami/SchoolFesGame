#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

namespace
{
	const Vector3 TO_CAMERA_POS = { 0.0f, -5.0f, -1000.0f };  //注視点から視点へのベクトル
	const float   CAMERA_HEIGHT_POSITION = 52.0f;			  //視点の高さ
	const float   CAMERA_FORWAED_POSITION = 4.0f;		      //視点の前座標
	const float   ROTATION_SPEED_X = 1.6f;                    //X軸のカメラの回転速度
	const float   ROTATION_SPEED_Y = -1.6f;                   //Y軸のカメラの回転速度
	const float   UP_MAX_POS = 0.3f;                          //上を向ける上限値
	const float   DOWN_MAX_POS = -0.3;                        //下を向ける上限値
	const float   SPRITE_W = 1920.0f;                         //画像の横幅
	const float   SPRITE_H = 1080.0f;                         //画像の縦幅
}  

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");
	//画像の読み込み
	m_modelRender.Init("Assets/modelData/bullet/bullet.tkm");

	m_spriteRender.Init("Assets/sprite/dot/dot.dds", SPRITE_W, SPRITE_H);
	//注視点から視点までのベクトルを設定
	m_toCameraPos.Set(TO_CAMERA_POS);

	return true;
}

void GameCamera::Update()
{
	//注視点を計算する。
	m_targetPosition = m_player->GetPosition();

	//視点を計算する
	m_cameraPosition = m_player->GetPosition();
	m_cameraPosition.y += CAMERA_HEIGHT_POSITION;

	//変数に注視点から視点までのベクトルを代入
	Vector3 toCameraPosOld = m_toCameraPos;

	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();

	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, ROTATION_SPEED_X * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, ROTATION_SPEED_Y * y);
	qRot.Apply(m_toCameraPos);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < DOWN_MAX_POS)
	{
		//カメラが下向きすぎ
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > UP_MAX_POS) 
	{
		//カメラが上向きすぎ
		m_toCameraPos = toCameraPosOld;
	}

	//注視点を計算する
	m_targetPosition = m_cameraPosition + m_toCameraPos;
	//視点の設定
	g_camera3D->SetPosition(m_cameraPosition);
	//注視点の設定
	g_camera3D->SetTarget(m_targetPosition);

	//カメラの更新
	g_camera3D->Update();
	m_modelRender.SetPosition(m_targetPosition);
	m_modelRender.Update();
}

void GameCamera::Render(RenderContext& rc)
{
	//画像を描画
	m_spriteRender.Draw(rc);
	//m_modelRender.Draw(rc);
}