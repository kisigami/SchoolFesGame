#include "stdafx.h"
#include "GameCamera.h"

#include "Player.h"

GameCamera::GameCamera()
{

}

GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 0.0f, -100.0f);

	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	return true;
}

void GameCamera::Update()
{
	//カメラを更新。
	//注視点を計算する。
	Vector3 target = m_player->m_position;
	//視点を計算する
	Vector3 cameraPos = m_player->m_position;
	//プレイヤーの頭の位置にカメラを置く
	cameraPos.y += 65.0f;
	//顔の前にカメラを置く
	cameraPos += g_camera3D->GetForward() * 10.0f;

	Vector3 toCameraPosOld = m_toCameraPos;
	//パッドの入力を使ってカメラを回す。
	float x = g_pad[0]->GetRStickXF();
	float y = g_pad[0]->GetRStickYF();
	//Y軸周りの回転
	Quaternion qRot;
	qRot.SetRotationDeg(Vector3::AxisY, 1.7f * x);
	qRot.Apply(m_toCameraPos);
	//X軸周りの回転。
	Vector3 axisX;
	axisX.Cross(Vector3::AxisY, m_toCameraPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, -1.7f * y);
	qRot.Apply(m_toCameraPos);
	//カメラの回転の上限をチェックする。
	//注視点から視点までのベクトルを正規化する。
	//正規化すると、ベクトルの大きさが１になる。
	//大きさが１になるということは、ベクトルから強さがなくなり、方向のみの情報となるということ。
	Vector3 toPosDir = m_toCameraPos;
	toPosDir.Normalize();
	if (toPosDir.y < -0.5f) {
		//カメラが上向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}
	else if (toPosDir.y > 0.8f) {
		//カメラが下向きすぎ。
		m_toCameraPos = toCameraPosOld;
	}

	//注視点を計算する。
	target= cameraPos + m_toCameraPos;
	//視点の設定
	g_camera3D->SetPosition(cameraPos);
	//注視点の設定
	g_camera3D->SetTarget(target);

	//カメラの更新。
	g_camera3D->Update();
}