#pragma once

//クラス宣言
class GameCamera;    //ゲームカメラ
class Player;        //プレイヤー

/// <summary>
/// 弾クラス
/// </summary>
class Bullet:public IGameObject
{
public:

	Bullet();
	~Bullet();
	bool Start();
	void Update();

	/// <summary>
	/// 座標を設定する。
	/// </summary>
	/// <param name="position">座標。</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// 回転を設定する。
	/// </summary>
	/// <param name="rotation">回転。</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// 大きさを設定する。
	/// </summary>
	/// <param name="scale">大きさ。</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

	GameCamera*        m_gameCamera = nullptr;        //ゲームカメラ
	CollisionObject*   m_collisionObject = nullptr;   //コリジョンオブジェクト
	Player*            m_player = nullptr;            //プレイヤー
	float              m_deleteTimer = 0.0f;          //削除タイマー
	Vector3            m_position;                    //座標
	Vector3            m_moveSpeed;                   //移動速度
	Quaternion         m_rotation;                    //回転
	Vector3            m_scale = Vector3::One;        //大きさ
};

