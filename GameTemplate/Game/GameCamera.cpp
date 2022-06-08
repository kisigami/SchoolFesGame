#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

namespace
{
	const Vector3 TO_CAMERA_POS = { 0.0f, 0.0f, -10.0f };  //注視点から視点へのベクトル
	const float   HEAD_HEIGHT = 65.0f;                      //視点の高さ
	const float   HEAD_FORWARD = 20.0f;                     //視点の前座標
	const float   ROTATION_SPEED_X = 1.4f;                  //X軸のカメラの回転速度
	const float   ROTATION_SPEED_Y = -1.4f;                 //Y軸のカメラの回転速度
	const float   UP_MAX_POS = 0.95f;                       //上を向ける上限値
	const float   DOWN_MAX_POS = -0.95;                     //下を向ける上限値
}  

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//注視点の目印のモデルの読み込み
	//m_modelRender.Init("Assets/modelData/bullet/bullet.tkm");
	//画像の読み込み
	m_spriteRender.Init("Assets/sprite/dot.dds", 1920.0f, 1080.0f);

	//注視点から視点までのベクトルを設定
	m_toCameraPos.Set(TO_CAMERA_POS);
	//プレイヤーのインスタンスを探す
	m_player = FindGO<Player>("player");

	return true;
}

void GameCamera::Update()
{
	//注視点を計算する。
	m_targetPosition = m_player->GetPosition();
	//視点を計算する
	Vector3 cameraPos = m_player->GetPosition();
	//プレイヤーの頭の位置にカメラを置く
	cameraPos.y += HEAD_HEIGHT;
	//プレイヤーの顔の前にカメラを置く
	cameraPos += g_camera3D->GetForward() * HEAD_FORWARD;

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
	m_targetPosition = cameraPos + m_toCameraPos;

	//視点の設定
	g_camera3D->SetPosition(cameraPos);
	//注視点の設定
	g_camera3D->SetTarget(m_targetPosition);

	//カメラの更新
	g_camera3D->Update();

	//注視点の座標を代入
	//Vector3 modelPos = m_targetPosition;
	//m_modelRender.SetPosition(modelPos);
	//m_modelRender.SetScale(0.01f, 0.01f, 0.01f);
	//モデルの更新
	//m_modelRender.Update();
}

void GameCamera::Render(RenderContext& rc)
{
	//クロスヘアを描画する
	m_spriteRender.Draw(rc);
	//m_modelRender.Draw(rc);
}