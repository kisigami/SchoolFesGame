#pragma once

class Player:public IGameObject
{
public:
	bool Start();
	void Update();
	void Render(RenderContext& rc);

private:
	/// <summary>
	/// 回転処理
	/// </summary>
	void Rotation();
	/// <summary>
	/// 移動処理
	/// </summary>
	void Move();

	ModelRender          m_modelRender;                 //モデルレンダー
	CharacterController  m_charaCon;                    //キャラクターコントローラー
	Vector3              m_position;                    //座標
	Vector3              m_scale;                       //大きさ
	Vector3              m_moveSpeed;                   //移動速度
	Vector3              m_forward = Vector3::AxisZ;    //プレイヤーの前ベクトル
	Quaternion           m_rotation;                    //回転

};

